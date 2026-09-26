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

#ifndef FILESECURITY_H
#define FILESECURITY_H

#include <functional>
#include <QSet>
#include <QString>

/*
 * FileSecurity - which files and folders a running program may touch.
 *
 * The rule
 * --------
 * A program may use anything at or below the folder it was loaded from, the
 * program root, without a word.  Anything outside it is decided by the
 * "Allow files outside the program's folder" preference: Do not allow, Ask
 * confirmation from user, or Allow.  When the answer is to ask, the person at
 * the keyboard gets Don't allow / Allow once / Allow for this run.  A path the
 * user picked in a file dialog is consent already given.  SQL that names a
 * file of its own - ATTACH DATABASE and VACUUM INTO - is held to the same rule.
 * The browser build lets everything through: its filesystem is sandboxed by
 * the browser already and a modal cannot block its main thread.
 *
 * What the interpreter asks
 * -------------------------
 *   startRun(folder, setting)  once per run, before the first opcode: fixes
 *                              the program root (canonicalised) and the
 *                              preference, and forgets the previous run's
 *                              answers
 *   allowPath(path, what)      before any statement touches a named path;
 *                              "what" is a short, translated verb phrase for
 *                              the prompt, such as "delete the file"
 *   allowSql(statement)        before DBEXECUTE and DBOPENSET run a statement
 *   rememberChosen(path)       when a file dialog hands back a path
 *
 * allowPath and allowSql return true when the operation may go ahead.  When
 * they return false they have already queued ERROR_PERMISSION naming the
 * resolved path, so the caller simply stops - the "if (!allowPath(...)) break;"
 * every opcode uses.
 *
 * What it needs from the interpreter
 * ----------------------------------
 * One thing only: a way to ask.  The asker is handed in at construction and
 * called on the interpreter thread with the verb phrase and the resolved path;
 * it returns a SETTINGSALLOW* answer.  Blocking on the GUI thread, and failing
 * closed under --silent where there is nobody to ask, are the asker's business,
 * because the interpreter is the one place that talks to the GUI.  Apart from
 * that the class knows nothing about the interpreter: no stack, no variables,
 * no signals, so it can be exercised on its own.
 *
 * The one other dependency is the global error queue (Error.h), which every
 * part of the interpreter already reports through.
 *
 * Threading
 * ---------
 * Everything runs on the interpreter thread.  The state is per run and is
 * only ever touched from there.
 *
 * Deliberately not here
 * ---------------------
 * The dialog itself (RunController::dialogAllowFile), reading the preference
 * (run() reads every setting in one place), and the other permission gates -
 * SYSTEM, SETSETTING, port I/O, NETLISTEN - which are separate preferences
 * with their own prompts.  They could join this class later; this is the file
 * rule on its own.
 */
class FileSecurity {
	public:
		// Returns SETTINGSALLOWNO, SETTINGSALLOWYES or SETTINGSALLOWRUN.
		typedef std::function<int(const QString &what, const QString &resolved)> Asker;

		explicit FileSecurity(Asker asker);

		void startRun(const QString &folder, int setting);
		void rememberChosen(const QString &path);
		bool allowPath(const QString &path, const QString &what);
		bool allowSql(const QString &stmt);

		// One canonical absolute path for whatever a program wrote.
		static QString resolvePath(const QString &path);

	private:
		bool allowDbTarget(const QString &target);

		Asker ask;
		QString programRoot;			// canonical folder the running program was loaded from
		int settingsAllowFile;			// access outside programRoot: NO / ASK / YES
		bool allowFileThisRun;			// the "yes, for the rest of this run" answer to the file gate
		QSet<QString> userChosenPaths;	// paths the user picked in a file dialog - consent by selection
};

#endif
