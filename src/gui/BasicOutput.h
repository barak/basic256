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


#ifndef __BASICOUTPUT_H
#define __BASICOUTPUT_H

#include <QKeyEvent>
#include <QTextCharFormat>
#include <QPaintEvent>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>

#include <qglobal.h>

#include "ViewWidgetIFace.h"

class BasicOutput : public QTextEdit, public ViewWidgetIFace
{
  Q_OBJECT
	public:
		BasicOutput();
		~BasicOutput();
		
		char *inputString;
		int currentKey;			// store the last key pressed for key function
		void inputStart();
		void outputText(QString);
		void outputText(QString, QColor);
		void locateCursor(int, int);
		void cursorColRow(int *, int *);
		void setOutputColor(QColor, QColor);
		void setOutputFont(QString, int, int, bool);
		void resetOutputFormat();
		QAction *copyAct;
		QAction *pasteAct;
		QAction *printAct;
		QAction *clearAct;

	virtual bool initActions(QMenu *, QToolBar *);

	public slots:
		void getInput();
		void stopInput();
		void slotPrint();					// sent output to printer
		void paintEvent(QPaintEvent*);		// display cursor on redraw
		void updatePasteButton();
		void slotClear();
		void slotWrap(bool);
		// Repaint the pane and any text already in it from EditorTheme.
		void applyTheme();

			
	signals:
		void inputEntered(QString);
		void mainWindowsVisible(int, bool);
	  
	protected:
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
		void dragEnterEvent(QDragEnterEvent *e);
		void dragMoveEvent(QDragMoveEvent *e);
		void insertFromMimeData(const QMimeData *source);
		void focusOutEvent(QFocusEvent* );

	private:
		// cursor Management
		int lastPosition;
		int getCurrentPosition();
		void saveLastPosition();
		void restoreLastPosition();
		void moveToPosition(int);

		int inputPosition;
		bool gettingInput;
		QString inputText;


		// Format program output is written in - the state behind TEXTCOLOR and
		// TEXTFONT. outFormatColored says whether a program set the foreground,
		// in which case it wins over the theme's normal output colour.
		QTextCharFormat outFormat;
		bool outFormatColored;
		QTextCharFormat normalFormat();
		void writeText(const QString &, const QTextCharFormat &);
		void writeTerminalText(const QString &, const QTextCharFormat &);
		void moveCursorToColRow(int, int);
};


#endif
