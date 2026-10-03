/** Copyright (C) 2006, Ian Paul Larsen.
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#include "FileSecurity.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include "Error.h"
#include "ErrorCodes.h"
#include "Settings.h"


FileSecurity::FileSecurity(Asker asker) : ask(asker) {
	settingsAllowFile = SETTINGSALLOWNO;
	allowFileThisRun = false;
}

// The root is anchored here, once per run, not on the live working directory:
// CHANGEDIR moves the cwd, and a cwd-relative jail would walk itself open in
// one line.  RunController has already set the cwd to the program's own folder.
void
FileSecurity::startRun(const QString &folder, int setting) {
	settingsAllowFile = setting;
	programRoot = QDir(folder).canonicalPath();
	if (programRoot.isEmpty()) programRoot = folder;
	allowFileThisRun = false;
	userChosenPaths.clear();
}

// A path the user picked in a file dialog: consent by selection.
void
FileSecurity::rememberChosen(const QString &path) {
	userChosenPaths.insert(resolvePath(path));
}

// Turn whatever a program wrote into one canonical absolute path.  The target
// often does not exist yet and canonicalFilePath() returns nothing in that
// case, so canonicalise the nearest ancestor that does exist and re-attach the
// rest -- without that a program escapes with "../.." or a symlinked folder.
QString
FileSecurity::resolvePath(const QString &path) {
	QString wanted = QDir::cleanPath(QFileInfo(path).absoluteFilePath());
	QString tail;
	QDir probe(wanted);
	while (!probe.exists() && !probe.isRoot()) {
		tail = probe.dirName() + (tail.isEmpty() ? QString() : "/" + tail);
		if (!probe.cdUp()) break;
	}
	QString resolved = probe.canonicalPath();
	if (resolved.isEmpty()) resolved = probe.absolutePath();
	if (!tail.isEmpty()) resolved += "/" + tail;
	return resolved;
}


// --- A very small SQL scanner.  It reads only far enough to find the leading
// keyword and any file name that follows it, which is all that is needed to
// stop SQLite reaching outside the program's folder.  Qt's SQLite driver
// prepares exactly one statement per exec(), so there is no "; DROP ..."
// chaining to unpick here.

static void sqlSkipNoise(const QString &s, int &i) {
	while (i < s.length()) {
		if (s.at(i).isSpace()) {
			i++;
		} else if (s.at(i) == '-' && i + 1 < s.length() && s.at(i + 1) == '-') {
			while (i < s.length() && s.at(i) != '\n') i++;
		} else if (s.at(i) == '/' && i + 1 < s.length() && s.at(i + 1) == '*') {
			i += 2;
			while (i + 1 < s.length() && !(s.at(i) == '*' && s.at(i + 1) == '/')) i++;
			i = qMin(i + 2, s.length());
		} else {
			break;
		}
	}
}

static QString sqlWord(const QString &s, int &i) {
	sqlSkipNoise(s, i);
	int start = i;
	while (i < s.length() && (s.at(i).isLetter() || s.at(i) == '_')) i++;
	return s.mid(start, i - start);
}

// Read one single quoted literal, '' meaning an embedded quote.  False when
// the next token is not a literal at all - an expression, a bound parameter -
// in which case the target cannot be known before SQLite resolves it.
static bool sqlLiteral(const QString &s, int &i, QString &out) {
	sqlSkipNoise(s, i);
	if (i >= s.length() || s.at(i) != '\'') return false;
	i++;
	out.clear();
	while (i < s.length()) {
		if (s.at(i) == '\'') {
			if (i + 1 < s.length() && s.at(i + 1) == '\'') {
				out += '\'';
				i += 2;
				continue;
			}
			i++;
			return true;
		}
		out += s.at(i++);
	}
	return false;			// unterminated
}

// A database file named by a statement, held to the same rule as any other path.
bool
FileSecurity::allowDbTarget(const QString &target) {
	if (target.isEmpty()) return true;			// private temporary file, removed on detach
	if (target.compare(":memory:", Qt::CaseInsensitive) == 0) return true;
	if (target.startsWith("file:", Qt::CaseInsensitive)) {
		// The URI form carries its own mode=rwc and can name a vfs, so the
		// plain path rules do not describe what it would do.
		error->q(ERROR_PERMISSION, target);
		return false;
	}
	// the "Interpreter" context keeps this with the other prompt texts, which
	// the opcodes pass in with tr()
	return allowPath(target, QCoreApplication::translate("Interpreter", "open the database"));
}

// DBOPEN is gated, but that only covers the first file: ATTACH and VACUUM INTO
// name further files from an already open connection, which is how a program
// would otherwise write anywhere on the disk through SQLite.  Everything else
// is ordinary SQL and passes through untouched.
bool
FileSecurity::allowSql(const QString &stmt) {
	int i = 0;
	QString kw = sqlWord(stmt, i).toUpper();

	if (kw == QLatin1String("ATTACH")) {
		int save = i;
		if (sqlWord(stmt, i).toUpper() != QLatin1String("DATABASE")) i = save;	// DATABASE is optional
		QString target;
		if (!sqlLiteral(stmt, i, target)) {
			error->q(ERROR_PERMISSION, stmt.trimmed());
			return false;
		}
		return allowDbTarget(target);
	}

	if (kw == QLatin1String("VACUUM")) {
		// Only the INTO form writes a new file; a plain VACUUM rewrites the
		// database that is already open.
		while (true) {
			QString w = sqlWord(stmt, i).toUpper();
			if (w.isEmpty()) return true;
			if (w == QLatin1String("INTO")) break;
		}
		QString target;
		if (!sqlLiteral(stmt, i, target)) {
			error->q(ERROR_PERMISSION, stmt.trimmed());
			return false;
		}
		return allowDbTarget(target);
	}

	return true;
}


// Gate for every path a program names.  Anything at or below the folder the
// program was loaded from is its own business and passes silently; anything
// outside is a decision for the person at the keyboard.  "what" is a short
// verb phrase naming the operation, shown in the prompt.
bool
FileSecurity::allowPath(const QString &path, const QString &what) {
#ifdef Q_OS_WASM
	// The browser filesystem is already sandboxed to this origin and a modal
	// cannot block the WASM main thread, so there is nothing to gate and no
	// way to ask.
	(void) path; (void) what;
	return true;
#else
	if (path.isEmpty()) return true;			// the caller reports its own error
	if (path.compare("STDOUT", Qt::CaseInsensitive) == 0) return true;

	QString resolved = resolvePath(path);

	// Windows and macOS compare paths without regard to case
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
	const Qt::CaseSensitivity cs = Qt::CaseInsensitive;
#else
	const Qt::CaseSensitivity cs = Qt::CaseSensitive;
#endif
	if (!programRoot.isEmpty() &&
		(resolved.compare(programRoot, cs) == 0 ||
		 resolved.startsWith(programRoot + "/", cs))) return true;

	// The user picked this one in a file dialog, which is consent enough
	if (userChosenPaths.contains(resolved)) return true;

	if (allowFileThisRun) return true;

	int doit = settingsAllowFile;
	if (doit == SETTINGSALLOWASK) {
		doit = ask(what, resolved);
	}

	if (doit == SETTINGSALLOWRUN) {
		// remembered for this run only, never written to settings
		allowFileThisRun = true;
		return true;
	}
	if (doit == SETTINGSALLOWYES) return true;

	error->q(ERROR_PERMISSION, resolved);
	return false;
#endif
}
