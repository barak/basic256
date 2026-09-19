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

#include <QClipboard>
#include <QIcon>
#include <QTextDocument>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#ifdef BASIC256_ENABLE_PRINTER
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#endif

#include "BasicOutputPane.h"

BasicOutputPane::BasicOutputPane() : QStackedWidget() {
	m_gridMode = false;
	m_wrap = true;
	// Null until initActions runs: the mode can be switched before the dock
	// has built its toolbar.
	copyAct = pasteAct = printAct = clearAct = NULL;

	m_text = new BasicOutput();
	m_grid = new BasicOutputGrid();
	addWidget(m_text);
	addWidget(m_grid);
	setCurrentWidget(m_text);

	// Relayed rather than connected pane by pane: everything outside this
	// class talks to the host and keeps working when the active pane changes.
	QObject::connect(m_text, SIGNAL(inputEntered(QString)), this, SIGNAL(inputEntered(QString)));
	QObject::connect(m_text, SIGNAL(mainWindowsVisible(int, bool)), this, SIGNAL(mainWindowsVisible(int, bool)));
	QObject::connect(m_grid, SIGNAL(inputEntered(QString)), this, SIGNAL(inputEntered(QString)));
	QObject::connect(m_grid, SIGNAL(selectionChanged(bool)), this, SLOT(slotGridSelectionChanged(bool)));

	setFocusProxy(m_text);
}

BasicOutputPane::~BasicOutputPane() {
	// the panes are children of the stack and are deleted with it
}

// TEXTSCREEN. Entering grid mode gives a blank screen; leaving it hands the
// dock back to the flowing pane with its document untouched, so the output from
// before the switch is still there.
void BasicOutputPane::setGridMode(bool on) {
	if (on == m_gridMode) return;
	m_gridMode = on;
	setCurrentWidget(on ? (QWidget *) m_grid : (QWidget *) m_text);
	setFocusProxy(on ? (QWidget *) m_grid : (QWidget *) m_text);
	currentWidget()->setFocus();

	// Copy follows whichever pane is showing, and neither starts with a
	// selection to copy.
	if (copyAct) copyAct->setEnabled(false);
	updatePasteButton();
}

void BasicOutputPane::setScreenSize(int cols, int rows) {
	if (cols <= 0 || rows <= 0) {
		setGridMode(false);
		return;
	}
	m_grid->setScreenSize(cols, rows);
	setGridMode(true);
}

QString BasicOutputPane::charAt(int col, int row) {
	if (!m_gridMode) return QString();
	return m_grid->charAt(col, row);
}

// The interpreter's statements. Each one goes to the pane that is on top.

void BasicOutputPane::outputText(QString text) {
	if (m_gridMode) m_grid->outputText(text); else m_text->outputText(text);
}

void BasicOutputPane::outputText(QString text, QColor color) {
	if (m_gridMode) m_grid->outputText(text, color); else m_text->outputText(text, color);
}

void BasicOutputPane::locateCursor(int col, int row) {
	if (m_gridMode) m_grid->locateCursor(col, row); else m_text->locateCursor(col, row);
}

void BasicOutputPane::cursorColRow(int *col, int *row) {
	if (m_gridMode) m_grid->cursorColRow(col, row); else m_text->cursorColRow(col, row);
}

void BasicOutputPane::setOutputColor(QColor fg, QColor bg) {
	// Both panes are kept in step so that the colours survive a mode switch.
	m_text->setOutputColor(fg, bg);
	m_grid->setOutputColor(fg, bg);
}

void BasicOutputPane::setOutputFont(QString family, int size, int weight, bool italic) {
	m_text->setOutputFont(family, size, weight, italic);
	m_grid->setOutputFont(family, size, weight, italic);
}

void BasicOutputPane::setOutputBackground(QColor bg) {
	m_text->setOutputBackground(bg);
	m_grid->setOutputBackground(bg);
}

// Called when a program starts, not by CLS. As well as putting the colours
// back, this drops out of grid mode: a program must never be able to hand the
// dock on to the next one stuck in a screen it did not ask for.
void BasicOutputPane::resetOutputFormat() {
	setGridMode(false);
	m_text->resetOutputFormat();
	m_grid->resetOutputFormat();
}

void BasicOutputPane::clearOutput() {
	if (m_gridMode) m_grid->clearScreen(); else m_text->slotClear();
}

bool BasicOutputPane::isEmpty() {
	if (m_gridMode) return m_grid->screenText().isEmpty();
	return m_text->toPlainText().isEmpty();
}

void BasicOutputPane::applyTheme() {
	m_text->applyTheme();
	m_grid->applyTheme();
}

// The pane font comes from the editor preferences. Grid mode takes only the
// family from it and sizes the cells from the window instead.
void BasicOutputPane::setPaneFont(const QFont &f) {
	m_text->setFont(f);
	m_grid->setPaneFont(f);
}

void BasicOutputPane::getInput() {
	if (m_gridMode) {
		m_grid->startInput();
		emit(mainWindowsVisible(2, true));
	} else {
		m_text->getInput();
	}
	updatePasteButton();
}

void BasicOutputPane::stopInput() {
	m_grid->endInput();
	m_text->stopInput();
	updatePasteButton();
}

// Word wrap is a property of a flowing document. A grid wraps at its own last
// column and has nothing to toggle, so the setting is remembered and reapplied
// when the flowing pane comes back.
void BasicOutputPane::slotWrap(bool checked) {
	m_wrap = checked;
	m_text->slotWrap(checked);
}

void BasicOutputPane::slotClear() {
	if (clearAct) clearAct->setEnabled(false);
	clearOutput();
}

void BasicOutputPane::slotGridSelectionChanged(bool has) {
	if (m_gridMode && copyAct) copyAct->setEnabled(has);
}

void BasicOutputPane::slotCopy() {
	if (m_gridMode) {
		const QString sel = m_grid->selectedText();
		if (!sel.isEmpty()) QApplication::clipboard()->setText(sel);
	} else {
		m_text->copy();
	}
}

void BasicOutputPane::slotPaste() {
	if (m_gridMode) {
		m_grid->insertInputText(QApplication::clipboard()->text());
	} else {
		m_text->paste();
	}
}

void BasicOutputPane::updatePasteButton() {
	if (!pasteAct) return;
	if (m_gridMode) {
		// Grid mode has nowhere to paste except into a line being typed.
		pasteAct->setEnabled(m_grid->isGettingInput() && !QApplication::clipboard()->text().isEmpty());
	} else {
		pasteAct->setEnabled(m_text->canPaste());
	}
}

// Printing the grid goes through a throwaway document rather than the painter,
// so that it pages and scales exactly the way printing the flowing pane does.
// A fixed pitch font keeps the columns that grid mode exists to line up.
void BasicOutputPane::printText(const QString &text, const QFont &f) {
#if !defined(BASIC256_ENABLE_PRINTER)
	Q_UNUSED(text);
	Q_UNUSED(f);
	QMessageBox::warning(this, QObject::tr("Print Error"), QObject::tr("Printing is not supported in this platform at this time."));
#else
	QTextDocument document;
	document.setDefaultFont(f);
	document.setPlainText(text);

	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(QObject::tr("Print Text Output"));

	if (dialog->exec() == QDialog::Accepted) {
		if ((printer.printerState() != QPrinter::Error) && (printer.printerState() != QPrinter::Aborted)) {
			document.print(&printer);
		} else {
			QMessageBox::warning(this, QObject::tr("Print Error"), QObject::tr("Unable to carry out printing.\nPlease check your printer settings."));
		}
	}
#endif
}

void BasicOutputPane::slotPrint() {
	if (m_gridMode) {
		QFont f(QStringLiteral("Courier New"));
		f.setStyleHint(QFont::Monospace);
		f.setFixedPitch(true);
		f.setPointSize(10);
		printText(m_grid->screenText(), f);
	} else {
		m_text->slotPrint();
	}
}

// The toolbar and menu belong to the dock, not to a pane, so that they survive
// a switch between modes. The actions dispatch through the host slots above.
bool BasicOutputPane::initActions(QMenu * vMenu, QToolBar * vToolBar) {
	if ((NULL == vMenu) || (NULL == vToolBar)) {
		return false;
	}

	vToolBar->setObjectName("outtoolbar");

	QIcon copyIcon, pasteIcon, printIcon, clearIcon;
	copyIcon.addFile(":icons/16x16/copy.png",  QSize(16, 16));
	copyIcon.addFile(":icons/22x22/copy.png",  QSize(22, 22));
	pasteIcon.addFile(":icons/16x16/paste.png", QSize(16, 16));
	pasteIcon.addFile(":icons/22x22/paste.png", QSize(22, 22));
	printIcon.addFile(":icons/16x16/print.png", QSize(16, 16));
	printIcon.addFile(":icons/22x22/print.png", QSize(22, 22));
	clearIcon.addFile(":icons/16x16/clear.png", QSize(16, 16));
	clearIcon.addFile(":icons/24x24/clear.png", QSize(24, 24));

	copyAct = vMenu->addAction(copyIcon, QObject::tr("Copy"));
	copyAct->setShortcutContext(Qt::WidgetShortcut);
	copyAct->setShortcuts(QKeySequence::keyBindings(QKeySequence::Copy));
	copyAct->setEnabled(false);
	pasteAct = vMenu->addAction(pasteIcon, QObject::tr("Paste"));
	pasteAct->setShortcutContext(Qt::WidgetShortcut);
	pasteAct->setShortcuts(QKeySequence::keyBindings(QKeySequence::Paste));
	pasteAct->setEnabled(false);
	printAct = vMenu->addAction(printIcon, QObject::tr("Print"));
	printAct->setShortcutContext(Qt::WidgetShortcut);
	printAct->setShortcuts(QKeySequence::keyBindings(QKeySequence::Print));
	clearAct = vMenu->addAction(clearIcon, QObject::tr("Clear"));
	clearAct->setEnabled(false);

	// The grid is not a QTextEdit and has no key handling of its own for these,
	// so Ctrl+C and Ctrl+V have to reach it through the actions. The flowing
	// pane answers them itself, as it always did.
	m_grid->addAction(copyAct);
	m_grid->addAction(pasteAct);

	vToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	vToolBar->addAction(copyAct);
	vToolBar->addAction(pasteAct);
	vToolBar->addAction(printAct);
	vToolBar->addAction(clearAct);

	QObject::connect(copyAct, SIGNAL(triggered()), this, SLOT(slotCopy()));
	QObject::connect(pasteAct, SIGNAL(triggered()), this, SLOT(slotPaste()));
	QObject::connect(printAct, SIGNAL(triggered()), this, SLOT(slotPrint()));
	QObject::connect(clearAct, SIGNAL(triggered()), this, SLOT(slotClear()));
	QObject::connect(m_text, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
	QObject::connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(updatePasteButton()));

	m_usesToolBar = true;
	m_usesMenu = true;

	return true;
}
