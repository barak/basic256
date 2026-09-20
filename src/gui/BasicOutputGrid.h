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


#ifndef __BASICOUTPUTGRID_H
#define __BASICOUTPUTGRID_H

#include <QColor>
#include <QFont>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QVector>
#include <QtWidgets/QWidget>

#include <qglobal.h>

// The retro screen TEXTSCREEN switches the Text Output dock to: a fixed grid of
// cols x rows character cells, painted a cell at a time.
//
// A column here really is a column. The flowing pane cannot promise that -- it
// is a rich text document, so a column is a character and only lines up when
// the font happens to be monospaced -- which is the whole reason this exists.
// Each character is drawn centred in its own cell rather than laid out by the
// font's own advance, so the grid stays square even in a proportional family.
//
// There is no scrollback, which is not an omission: on the machines this
// imitates -- Spectrum, C64, BBC Micro, CPC, MSX -- the screen *was* the
// buffer, and rows that scrolled off the top were gone. TEXTCHAR reads a
// character back off the screen the way SCREEN$ and PEEKing screen RAM did.
class BasicOutputGrid : public QWidget
{
  Q_OBJECT
	public:
		BasicOutputGrid();
		~BasicOutputGrid();

		// One character cell. A background with alpha 0 means "no background
		// of its own", and the pane colour shows through.
		struct Cell {
			QChar ch;
			QRgb fg;
			QRgb bg;
			Cell() : ch(QLatin1Char(' ')), fg(0), bg(0) {}
		};

		// square: cells as tall as they are wide, the way an eighties screen
		// was drawn, rather than cells the shape of the font's own line box.
		void setScreenSize(int cols, int rows, bool square);
		int screenCols() const { return m_cols; }
		int screenRows() const { return m_rows; }

		// The interpreter's statements, the same set the flowing pane answers.
		void outputText(QString);
		void outputText(QString, QColor);
		void locateCursor(int, int);
		void cursorColRow(int *, int *);
		void setOutputColor(QColor, QColor);
		void setOutputFont(QString, int, int, bool);
		void setOutputBackground(QColor);
		void resetOutputFormat();
		void clearScreen();
		void applyTheme();
		void setPaneFont(const QFont &);

		// TEXTCHAR: the character at a cell, or "" when off screen.
		QString charAt(int col, int row) const;

		// Copy and Print, which the host's actions dispatch to.
		bool hasSelection() const { return m_selActive; }
		QString selectedText() const;
		QString screenText() const;
		void clearSelection();

		void startInput();
		void endInput();
		bool isGettingInput() const { return m_gettingInput; }
		void insertInputText(const QString &);

	signals:
		void inputEntered(QString);
		void selectionChanged(bool);

	protected:
		void paintEvent(QPaintEvent *);
		void resizeEvent(QResizeEvent *);
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void mouseReleaseEvent(QMouseEvent *);
		void focusOutEvent(QFocusEvent *);
		// Tab has to be a key like any other so that a program polling KEY can
		// use it to step between fields in a form. Without this QWidget::event
		// eats it for the focus chain and keyPressEvent never sees it.
		bool focusNextPrevChild(bool);

	private:
		int m_cols;
		int m_rows;
		bool m_square;				// TEXTSCREEN's third argument
		QVector<Cell> m_cells;

		int m_col;					// the cursor: where the next PRINT lands
		int m_row;
		// The character that fills the last column does not scroll the screen
		// by itself: the cursor rests past the end of the row and the wrap
		// happens when there is another character to put down. Wrapping at
		// once would throw the top row away the moment a program filled the
		// bottom one, which is why terminals and the 8 bit machines both
		// deferred it. m_col == m_cols is that resting place.

		QRgb m_fg;					// TEXTCOLOR
		QRgb m_bg;
		bool m_colored;				// has a program set a colour of its own?
		QColor m_paneBackground;	// TEXTBACKGROUND
		QString m_family;
		int m_weight;
		bool m_italic;
		QFont m_paneFont;			// what Preferences chose, for the family

		// Cell geometry, recomputed on every resize: the grid is fitted to the
		// pane, so the font size follows the window rather than the statement.
		// The cell keeps the font's own width to height ratio and the grid is
		// centred in whatever is left over, so a character stays the shape the
		// designer drew it however the dock is dragged. Filling the pane in
		// both directions instead would stretch every glyph with the window.
		qreal m_cellW;
		qreal m_cellH;
		qreal m_originX;			// the grid's top left corner within the pane
		qreal m_originY;
		QFont m_cellFont;
		void recomputeCellMetrics();
		qreal cellAspect(const QFont &) const;

		QColor normalFg() const;
		QColor paneColor() const;
		int index(int col, int row) const { return row * m_cols + col; }
		bool inRange(int col, int row) const {
			return col >= 0 && col < m_cols && row >= 0 && row < m_rows;
		}
		void putChar(QChar, QRgb fg, QRgb bg);
		void applyPendingWrap();
		void newLine();
		void scrollUp();
		void advanceCursor();

		// input
		bool m_gettingInput;
		int m_inputCol;				// where the typed text starts
		int m_inputRow;
		QString m_inputText;
		int m_inputCaret;			// caret offset within m_inputText
		void redrawInput();
		void commitInput();

		// selection
		bool m_selActive;
		bool m_selDragging;
		bool m_selBlock;			// Alt held: rectangular rather than flowing
		int m_selAnchorCol, m_selAnchorRow;
		int m_selCol, m_selRow;
		bool cellSelected(int col, int row) const;
		void cellAt(const QPoint &, int *col, int *row) const;
};


#endif
