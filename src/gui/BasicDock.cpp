/** Copyright (C) 2016, J.M.Reneau
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

#include <qglobal.h>

#include <QtWidgets/QDockWidget>
#include <QtGui/QAction>
#include <QCloseEvent>
#include "BasicDock.h"


BasicDock::BasicDock() {
	visible_action = NULL;
}
	
void BasicDock::setActionCheck(QAction *vischeck) {
	visible_action = vischeck;
}
	
void BasicDock::closeEvent(QCloseEvent *event) {
	// on close of a BasicDock - see if a checkable menu action
	// needs to be unchecked
	if (visible_action) visible_action->setChecked(false);
	event->accept();
}
