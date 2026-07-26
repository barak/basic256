/** Copyright (C) 2011, J.M.Reneau.
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

#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtGui/QAction>
#include <QComboBox>

#include "BasicEdit.h"

#ifndef REPLACEWINH
#define REPLACEWINH

class ReplaceWin : public QDialog
{
  Q_OBJECT

public:
	ReplaceWin();
	void closeEvent(QCloseEvent *);
	void setReplaceMode(bool);
    void findAgain();
    QLineEdit *findText;
    QComboBox *findTextCombo;

private slots:
    void changeFindText(QString);
    void clickFindButton();
    void clickReplaceButton();
    void clickReplaceAllButton();
    void switchToFind();
    void switchToReplace();

private:
    QLabel *findLabel;
    QLabel *replaceLabel;
    QLineEdit *replaceText;
    QComboBox *replaceTextCombo;
    QCheckBox *backCheckbox;
    QCheckBox *caseCheckbox;
    QCheckBox *wordsCheckbox;
    QPushButton *cancelButton;
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    bool replaceMode;
    void saveHistory();
};

#endif
