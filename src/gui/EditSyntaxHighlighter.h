/**
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
 
#ifndef __EDITSYNTAXHIGHLIGHTER_H
#define __EDITSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

class EditSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    EditSyntaxHighlighter(QTextDocument *parent = 0);

    // Rebuild every rule from the current EditorTheme and repaint.
    void applyTheme();

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct HighlightRule
    {
      QRegularExpression pattern;
      QTextCharFormat format;
    };
    
    typedef QVector< HighlightRule > VecHighlightRules;
    
    VecHighlightRules m_standardRules;
    
    void initKeywords();
    void initConstants();
    void initQuotes();
    void initLabels();
    void initNumbers();
    void initComments();
    
    QTextCharFormat m_keywordFmt;
    QTextCharFormat m_constantFmt;
    QTextCharFormat m_quoteFmt;
    QTextCharFormat m_labelFmt;
    QTextCharFormat m_numberFmt;
    QTextCharFormat m_commentFmt;
};

#endif	// __EDITSYNTAXHIGHLIGHTER_H
