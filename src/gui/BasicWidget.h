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


#ifndef __BASICWIDGET_H
#define __BASICWIDGET_H

#include <qglobal.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QScrollArea>

class QMenu;
class ToolBar;
class ViewWidgetIFace;
	
class BasicWidget : public QWidget
{
    Q_OBJECT
public:
    BasicWidget(const QString & title = QString(), const QString & name = QString(), QWidget * viewWidget = 0, QScrollArea * scrollArea = 0);
    ~BasicWidget();
	
	bool setViewWidget(QWidget * view);
	bool usesToolBar();
	bool isVisibleToolBar();
	bool usesMenu();
	QMenu *getMenu() { return m_menu; }

public slots:
	void slotShowToolBar(const bool);

private:
	ViewWidgetIFace *m_viewWidget;
	QToolBar *m_toolBar;
	QMenu *m_menu;
	QVBoxLayout *m_layout;
    QScrollArea *m_scrollArea;
};

#endif
