/** Copyright (C) 2016, J.M.Reneau.
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


#ifndef __BASICDOCK_H
#define __BASICDOCK_H

#include <qglobal.h>

#include <QtWidgets/QDockWidget>
#include <QtGui/QAction>
#include <QCloseEvent>


class BasicDock : public QDockWidget
{
    Q_OBJECT
public:
	BasicDock();
	
	void setActionCheck(QAction *vischeck);
	void closeEvent(QCloseEvent *);
private:
	QAction *visible_action;
};

#endif
