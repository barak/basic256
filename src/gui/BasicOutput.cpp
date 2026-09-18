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

#include <iostream>

#include <QPainter>
#include <QTextCursor>
#include <QTextBlock>
#include <QList>
#include <QPair>
#include <QMutex>
#include <QClipboard>
#include <QMimeData>
#include <QRegularExpression>

#include <QtGui/QAction>
#include <QtWidgets/QToolBar>
#include <QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#ifdef BASIC256_ENABLE_PRINTER
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#endif

#include "Settings.h"
#include "BasicOutput.h"
#include "EditorTheme.h"
#include "BasicKeyboard.h"

extern QMutex *mymutex;
extern BasicKeyboard *basicKeyboard;

BasicOutput::BasicOutput( ) : QTextEdit () {
    inputText.clear();
    setReadOnly(true);
	setInputMethodHints(Qt::ImhNoPredictiveText);
	setFocusPolicy(Qt::StrongFocus);
	setAcceptRichText(false);
	setUndoRedoEnabled(false);
	// Paint the output pane from EditorTheme rather than leaving it to the OS
	// colour scheme. Text is emitted with an explicit colour and no background
	// is ever set, so on Qt 6.5+ under a dark desktop theme the widget's Base
	// role turns near-black and the text disappears into it.
	setStyleSheet(EditorTheme::current().paneStyleSheet("QTextEdit"));
	gettingInput = false;
	outFormatColored = false;
	resetOutputFormat();
	saveLastPosition();

}

BasicOutput::~BasicOutput( ) {
    // destructor for basic output
}

void BasicOutput::getInput() {
	// move cursor to the end of the existing text and start input
	inputText.clear();
	gettingInput = true;
	setFocus();
	emit(mainWindowsVisible(2,true));
	restoreLastPosition();
	inputPosition = lastPosition;
	setCurrentCharFormat(normalFormat());
	setReadOnly(false);
	updatePasteButton();
}

void BasicOutput::stopInput() {
	gettingInput = false;
	setReadOnly(true);
    updatePasteButton();
}


void BasicOutput::keyPressEvent(QKeyEvent *e) {
    e->accept();
    if (!gettingInput) {
        mymutex->lock();
        basicKeyboard->keyPressed(e);
        QTextEdit::keyPressEvent(e);
		mymutex->unlock();
    } else {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            saveLastPosition();
            QTextCursor t(textCursor());
            t.setPosition(inputPosition);
            t.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
            t.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, lastPosition);
            inputText=t.selectedText();
            restoreLastPosition();
            insertPlainText("\n");
            saveLastPosition();
            stopInput();
            emit(inputEntered(inputText)); // send the string back to the interperter and run controller

       } else if (e->key() == Qt::Key_Backspace) {
            QTextCursor t(textCursor());
            t.movePosition(QTextCursor::PreviousCharacter);
            if (t.position() >= inputPosition)
                QTextEdit::keyPressEvent(e);
                saveLastPosition();
        } else {
            QTextEdit::keyPressEvent(e);
        }
    }
}


void BasicOutput::keyReleaseEvent(QKeyEvent *e) {
	e->accept();
	if (!gettingInput) {
        mymutex->lock();
        basicKeyboard->keyReleased(e);
        QTextEdit::keyReleaseEvent(e);
		mymutex->unlock();
	}
}

void BasicOutput::focusOutEvent(QFocusEvent* ){
    //clear pressed keys list when lose focus to avoid detecting still pressed keys
    basicKeyboard->reset();
}

bool BasicOutput::initActions(QMenu * vMenu, QToolBar * vToolBar) {
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

    vToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	vToolBar->addAction(copyAct);
	vToolBar->addAction(pasteAct);
    vToolBar->addAction(printAct);
    vToolBar->addAction(clearAct);

	QObject::connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));
	QObject::connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
	QObject::connect(printAct, SIGNAL(triggered()), this, SLOT(slotPrint()));
    QObject::connect(this, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    QObject::connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(updatePasteButton()));
    QObject::connect(clearAct, SIGNAL(triggered()), this, SLOT(slotClear()));

	m_usesToolBar = true;
	m_usesMenu = true;

	return true;
}

void BasicOutput::slotPrint() {
#if !defined(BASIC256_ENABLE_PRINTER)
    QMessageBox::warning(this, QObject::tr("Print Error"), QObject::tr("Printing is not supported in this platform at this time."));
#else
    QTextDocument *document = this->document();
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(QObject::tr("Print Text Output"));

    if (dialog->exec() == QDialog::Accepted) {
        if ((printer.printerState() != QPrinter::Error) && (printer.printerState() != QPrinter::Aborted))	{
            document->print(&printer);
        } else {
            QMessageBox::warning(this, QObject::tr("Print Error"), QObject::tr("Unable to carry out printing.\nPlease check your printer settings."));
        }
    }
#endif
}

void BasicOutput::paintEvent(QPaintEvent* event) {
	// paint a visible cursor at the text cursor
	QTextEdit::paintEvent(event);
	QRect cursor = cursorRect();
	cursor.setWidth(2);
	QPainter p(viewport());
	p.fillRect(cursor, Qt::SolidPattern);
}

// Ensure that drag and drop is allowed only in permitted area when BASIC-256 wait for input
void BasicOutput::dragEnterEvent(QDragEnterEvent *e){
	if (e->mimeData()->hasFormat("text/plain") && gettingInput && !isReadOnly()  )
		e->acceptProposedAction();
}

void BasicOutput::dragMoveEvent (QDragMoveEvent *event){
	QTextCursor t = cursorForPosition(event->pos());
	if (t.position() >= inputPosition){
		event->acceptProposedAction();
		QDragMoveEvent move(event->pos(),event->dropAction(), event->mimeData(), event->mouseButtons(),
			event->keyboardModifiers(), event->type());
		QTextEdit::dragMoveEvent(&move); // Call the parent function (show cursor and keep selection)
	} else {
		event->ignore();
	}
}


//Ensure that drang and drop operation or paste operation will add only first line of the copied text.
void BasicOutput::insertFromMimeData(const QMimeData* source)
{
	if (source->hasText()) {
		QString s = source->text();
		QStringList l = s.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
		textCursor().insertText(l.at(0));
		setFocus();
	}
}

void BasicOutput::updatePasteButton(){
     pasteAct->setEnabled(this->canPaste());
}

void BasicOutput::slotClear(){
     clearAct->setEnabled(false);
     clear();
     lastPosition = 0;
}

void BasicOutput::slotWrap(bool checked) {
	if (checked) {
		setLineWrapMode(QTextEdit::WidgetWidth);
	} else {
		setLineWrapMode(QTextEdit::NoWrap);
	}
}

void BasicOutput::applyTheme() {
	setStyleSheet(EditorTheme::current().paneStyleSheet("QTextEdit"));

	// Text already on screen keeps the colour it was written with, so output
	// from before the switch would stay dark on a dark page. Repaint only the
	// runs written in the other theme's normal colour; error lines have their
	// own colour and are left as they are. Ranges are collected first because
	// merging a format invalidates the fragment iterators.
	const QColor normal = EditorTheme::current().outputText;
	QList<QPair<int,int> > ranges;
	for (QTextBlock b = document()->begin(); b.isValid(); b = b.next()) {
		for (QTextBlock::iterator it = b.begin(); !it.atEnd(); ++it) {
			const QTextFragment f = it.fragment();
			if (!f.isValid()) continue;
			const QColor c = f.charFormat().foreground().color();
			if (c != EditorTheme::light().outputText && c != EditorTheme::dark().outputText) continue;
			if (c == normal) continue;
			ranges.append(qMakePair(f.position(), f.length()));
		}
	}

	QTextCharFormat fmt;
	fmt.setForeground(normal);
	for (int i = 0; i < ranges.size(); i++) {
		QTextCursor cur(document());
		cur.setPosition(ranges.at(i).first);
		cur.setPosition(ranges.at(i).first + ranges.at(i).second, QTextCursor::KeepAnchor);
		cur.mergeCharFormat(fmt);
	}

	// Anything printed from here on uses the new normal colour, unless the
	// program picked one of its own with TEXTCOLOR.
	setCurrentCharFormat(normalFormat());
}

int BasicOutput::getCurrentPosition() {
		QTextCursor t(textCursor());
		return t.position();
}

void BasicOutput::saveLastPosition() {
	lastPosition = getCurrentPosition();
}

void BasicOutput::restoreLastPosition() {
	moveToPosition(lastPosition);
}

void BasicOutput::moveToPosition(int pos) {
	// move to an absolute character number (position) in the document
	QTextCursor t(textCursor());
	t.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	t.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pos);
	setTextCursor(t);
}

// Program output is written in outFormat, which TEXTCOLOR and TEXTFONT change
// and resetOutputFormat() puts back. A program that never touches either gets
// the pane font and the theme's normal output colour, exactly as before.

QTextCharFormat BasicOutput::normalFormat() {
	QTextCharFormat fmt = outFormat;
	if (!outFormatColored) {
		fmt.setForeground(EditorTheme::current().outputText);
	}
	return fmt;
}

void BasicOutput::resetOutputFormat() {
	outFormat = QTextCharFormat();
	outFormatColored = false;
	setCurrentCharFormat(normalFormat());
}

void BasicOutput::setOutputColor(QColor fg, QColor bg) {
	outFormat.setForeground(fg);
	// A fully transparent background means "no background", which is how the
	// one argument form of TEXTCOLOR clears one that was set earlier.
	if (bg.alpha() == 0) {
		outFormat.clearBackground();
	} else {
		outFormat.setBackground(bg);
	}
	outFormatColored = true;
	setCurrentCharFormat(outFormat);
}

void BasicOutput::setOutputFont(QString family, int size, int weight, bool italic) {
	// Same argument list as the graphics FONT statement: an empty family, or a
	// size of -1, falls back to the font the pane was given in preferences.
	const QFont base = font();
	family = family.trimmed();
	outFormat.setFont(QFont(family.isEmpty() ? base.family() : family,
							size > 0 ? size : base.pointSize(),
							weight, italic));
	setCurrentCharFormat(outFormat);
}

void BasicOutput::outputText(QString text) {
	writeText(text, normalFormat());
}

void BasicOutput::outputText(QString text, QColor color) {
	// An explicit colour - error text - overrides whatever TEXTCOLOR set.
	QTextCharFormat fmt = outFormat;
	fmt.setForeground(color);
	writeText(text, fmt);
}

void BasicOutput::writeText(const QString &text, const QTextCharFormat &fmt) {
	restoreLastPosition();
	writeTerminalText(text, fmt);
	ensureCursorVisible();
	saveLastPosition();
}

// Write text the way a terminal does: characters replace whatever they land on
// up to the end of the row, and a newline moves to the start of the next row
// instead of splitting the current one. With the cursor at the end of the
// document - where PRINT leaves it - there is nothing to replace and no next
// row, so this is identical to inserting. Only LOCATE can move the cursor
// somewhere that makes the difference visible.
//
// A row is a block (a real newline), not a wrapped visual line, so row numbers
// mean the same thing whether or not word wrap is turned on.
void BasicOutput::writeTerminalText(const QString &text, const QTextCharFormat &fmt) {
	QTextCursor t(textCursor());
	int from = 0;

	while (true) {
		const int nl = text.indexOf(QChar::LineFeed, from);
		const QString seg = (nl < 0 ? text.mid(from) : text.mid(from, nl - from));

		if (!seg.isEmpty()) {
			const int start = t.position();
			t.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
			const int room = t.position() - start;
			t.setPosition(start);
			if (room > 0) {
				t.setPosition(start + qMin(room, (int) seg.length()), QTextCursor::KeepAnchor);
			}
			t.insertText(seg, fmt);
		}

		if (nl < 0) {
			break;
		}

		if (t.blockNumber() + 1 < document()->blockCount()) {
			t.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
		} else {
			t.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
			t.insertText(QString(QChar::LineFeed), fmt);
		}
		from = nl + 1;
	}

	setTextCursor(t);
}

// Move the cursor to a column and row, both zero based. Rows past the end of
// the document and columns past the end of a row are filled in with blanks, so
// LOCATE always lands where it was asked to.
void BasicOutput::moveCursorToColRow(int col, int row) {
	if (col < 0) col = 0;
	if (row < 0) row = 0;

	QTextCursor t(textCursor());

	while (document()->blockCount() <= row) {
		t.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
		t.insertText(QString(QChar::LineFeed), outFormat);
	}

	const QTextBlock block = document()->findBlockByNumber(row);
	const int length = block.length() - 1;		// less the block separator
	t.setPosition(block.position());
	if (length < col) {
		t.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
		t.insertText(QString(col - length, ' '), outFormat);
	} else {
		t.setPosition(block.position() + col);
	}

	setTextCursor(t);
	saveLastPosition();
}

void BasicOutput::locateCursor(int col, int row) {
	moveCursorToColRow(col, row);
	ensureCursorVisible();
}

// Report the column and row the next PRINT will use. That is the saved output
// position, not the live text cursor, which the user can move by clicking in
// the pane without affecting where the program prints.
void BasicOutput::cursorColRow(int *col, int *row) {
	QTextCursor t(document());
	t.setPosition(qBound(0, lastPosition, document()->characterCount() - 1));
	*col = t.positionInBlock();
	*row = t.blockNumber();
}
