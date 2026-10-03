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


#ifndef __BASICOUTPUTPANE_H
#define __BASICOUTPUTPANE_H

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>

#include <qglobal.h>

#include "ViewWidgetIFace.h"
#include "BasicOutput.h"
#include "BasicOutputGrid.h"

// The Text Output dock's contents: a stack holding one pane per output mode,
// which forwards the statements the interpreter issues - PRINT, LOCATE,
// TEXTCOLOR and the rest - to whichever pane is on top, so that the rest of the
// program never has to know which mode is active.
//
// Page 1 is BasicOutput, the flowing QTextEdit pane every program gets. Page 2
// is BasicOutputGrid, the fixed character screen TEXTSCREEN switches to. The
// dock's menu, toolbar and four actions live here rather than on a pane so that
// they survive the switch.
class BasicOutputPane : public QStackedWidget, public ViewWidgetIFace
{
  Q_OBJECT
	public:
		BasicOutputPane();
		~BasicOutputPane();

		// The statements the interpreter issues, forwarded to the active pane.
		void outputText(QString);
		void outputText(QString, QColor);
		void locateCursor(int, int);
		void cursorColRow(int *, int *);
		void setOutputColor(QColor, QColor);
		void setOutputFont(QString, int, int, bool);
		void setOutputBackground(QColor);
		void resetOutputFormat();

		// TEXTSCREEN: cols or rows of zero leaves grid mode and hands the dock
		// back to the flowing pane.
		void setScreenSize(int cols, int rows, bool square);
		// TEXTCHAR: the character at a cell. Empty off screen, and always empty
		// in the flowing pane, which has no cells to read.
		QString charAt(int col, int row);
		bool isGridMode() const { return m_gridMode; }

		// Pane management, issued by MainWindow and RunController.
		void clearOutput();
		bool isEmpty();
		void applyTheme();
		void setPaneFont(const QFont &);

		QAction *copyAct;
		QAction *pasteAct;
		QAction *printAct;
		QAction *clearAct;

	virtual bool initActions(QMenu *, QToolBar *);

	public slots:
		void getInput();
		void stopInput();
		void slotWrap(bool);
		void slotPrint();
		void slotClear();
		void slotCopy();
		void slotPaste();
		void updatePasteButton();
		void slotGridSelectionChanged(bool);

	signals:
		// Relayed from the active pane so that RunController and MainWindow
		// connect to this host once and never to an individual pane.
		void inputEntered(QString);
		void mainWindowsVisible(int, bool);

	private:
		BasicOutput *m_text;
		BasicOutputGrid *m_grid;
		bool m_gridMode;
		bool m_wrap;
		void setGridMode(bool);
		void printText(const QString &, const QFont &);
};


#endif
