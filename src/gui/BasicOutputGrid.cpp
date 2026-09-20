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

#include <QFontMetricsF>
#include <QMutex>
#include <QPainter>
#include <QRegularExpression>

#include "BasicOutputGrid.h"
#include "BasicKeyboard.h"
#include "EditorTheme.h"

extern QMutex *mymutex;
extern BasicKeyboard *basicKeyboard;

// The screen a bare TEXTSCREEN gives, and the limits. 40x25 is the C64's screen
// and the BBC Micro's MODE 7.
#define GRID_DEFAULT_COLS	40
#define GRID_DEFAULT_ROWS	25
#define GRID_MAX_COLS		512
#define GRID_MAX_ROWS		256

BasicOutputGrid::BasicOutputGrid() : QWidget() {
	m_cols = GRID_DEFAULT_COLS;
	m_rows = GRID_DEFAULT_ROWS;
	m_square = false;
	m_col = m_row = 0;
	m_fg = m_bg = 0;
	m_colored = false;
	m_weight = QFont::Normal;
	m_italic = false;
	m_cellW = m_cellH = 1.0;
	m_originX = m_originY = 0.0;
	m_gettingInput = false;
	m_inputCol = m_inputRow = 0;
	m_inputCaret = 0;
	m_selActive = m_selDragging = m_selBlock = false;
	m_selAnchorCol = m_selAnchorRow = m_selCol = m_selRow = 0;

	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setCursor(Qt::IBeamCursor);
	m_cells.resize(m_cols * m_rows);
	recomputeCellMetrics();
}

BasicOutputGrid::~BasicOutputGrid() {
}

// Tab belongs to the program, not to the focus chain: a form built out of
// LOCATE and KEY steps between its fields with it.
bool BasicOutputGrid::focusNextPrevChild(bool) {
	return false;
}

QColor BasicOutputGrid::normalFg() const {
	// A program that never called TEXTCOLOR follows the theme, exactly as the
	// flowing pane does.
	if (!m_colored) return EditorTheme::current().outputText;
	return QColor::fromRgba(m_fg);
}

QColor BasicOutputGrid::paneColor() const {
	if (m_paneBackground.isValid()) return m_paneBackground;
	return EditorTheme::current().background;
}

void BasicOutputGrid::setScreenSize(int cols, int rows, bool square) {
	m_square = square;
	if (cols <= 0) cols = GRID_DEFAULT_COLS;
	if (rows <= 0) rows = GRID_DEFAULT_ROWS;
	if (cols > GRID_MAX_COLS) cols = GRID_MAX_COLS;
	if (rows > GRID_MAX_ROWS) rows = GRID_MAX_ROWS;

	m_cols = cols;
	m_rows = rows;
	m_cells.clear();
	m_cells.resize(m_cols * m_rows);
	m_col = m_row = 0;
	clearSelection();
	recomputeCellMetrics();
	update();
}

// The shape of a cell: the family's own width to height ratio, measured big so
// that hinting at small sizes cannot skew it. A cell that is not this shape is
// a character stretched out of shape, which is what turns a circle into an egg.
qreal BasicOutputGrid::cellAspect(const QFont &family) const {
	QFont f = family;
	f.setStretch(QFont::AnyStretch);
	f.setPixelSize(100);
	const QFontMetricsF fm(f);
	const qreal h = fm.height();
	if (h <= 0.0) return 0.6;
	// 'W' is the widest letter, so in a monospaced family it is the cell and in
	// a proportional one it is the character that has to fit. Bounded because a
	// decorative face can report an advance that would leave the grid a row of
	// slivers or a handful of slabs.
	const qreal w = fm.horizontalAdvance(QLatin1Char('W'));
	if (w <= 0.0) return 0.6;
	return qBound(0.25, w / h, 1.5);
}

// The grid is fitted to the pane, so the cell size comes from the window and
// the font is fitted into it. TEXTFONT therefore chooses the family, weight and
// slant here and its size argument is ignored -- there is nowhere for a fixed
// point size to go when the screen has to be cols x rows whatever the dock.
void BasicOutputGrid::recomputeCellMetrics() {
	QFont f = m_family.isEmpty() ? m_paneFont : QFont(m_family);
	f.setWeight((QFont::Weight) m_weight);
	f.setItalic(m_italic);
	f.setStretch(QFont::AnyStretch);

	// Fit cols x rows cells of the font's own shape inside the pane and centre
	// what is left over, the way a screen sits in its bezel. Taking the cell as
	// width/cols by height/rows instead only draws true when the dock happens
	// to be exactly as square as the screen asked for.
	// A square screen is the one the eighties machines actually had: their
	// cell was 8 by 8 and a character drawn in it was as far from the one
	// below as from the one beside it. The font is not stretched to fill it --
	// that would undo the whole point of keeping a glyph its own shape -- so
	// the character is simply centred in a wider cell.
	const qreal aspect = m_square ? 1.0 : cellAspect(f);
	const qreal availW = qMax(1.0, (qreal) width());
	const qreal availH = qMax(1.0, (qreal) height());
	m_cellH = qMax(1.0, qMin(availH / (qreal) m_rows, availW / (qreal) m_cols / aspect));
	m_cellW = qMax(1.0, m_cellH * aspect);
	m_originX = qMax(0.0, (availW - m_cellW * m_cols) / 2.0);
	m_originY = qMax(0.0, (availH - m_cellH * m_rows) / 2.0);

	// Size by the cell height, which is what makes the text as large as the
	// screen allows.
	int px = qMax(1, (int) m_cellH);
	while (px > 1) {
		f.setPixelSize(px);
		if (QFontMetricsF(f).height() <= m_cellH) break;
		px--;
	}
	f.setPixelSize(px);

	// Then nudge the glyphs to the cell width. The cell was cut to the font's
	// own ratio, so this is the rounding of a whole pixel size and no more --
	// but without it the last pixel of a box drawing character does not meet
	// the next one and the joins in the art come apart.
	const qreal advance = QFontMetricsF(f).horizontalAdvance(QLatin1Char('W'));
	if (advance > 0.0) {
		int stretch = (int) qRound(m_cellW / advance * 100.0);
		// A square cell is wider than the font asks for, and the glyph is left
		// its own shape and centred in it rather than pulled out to the edges.
		// It is still condensed when the family is wider than the cell, or a
		// broad face would spill into the square beside it.
		if (m_square) stretch = qMin(stretch, 100);
		f.setStretch(qBound(25, stretch, 400));
	}
	m_cellFont = f;
}

void BasicOutputGrid::resizeEvent(QResizeEvent *e) {
	QWidget::resizeEvent(e);
	recomputeCellMetrics();
}

void BasicOutputGrid::setPaneFont(const QFont &f) {
	m_paneFont = f;
	recomputeCellMetrics();
	update();
}

void BasicOutputGrid::applyTheme() {
	update();
}

void BasicOutputGrid::resetOutputFormat() {
	m_fg = m_bg = 0;
	m_colored = false;
	m_paneBackground = QColor();
	m_family.clear();
	m_weight = QFont::Normal;
	m_italic = false;
	recomputeCellMetrics();
	update();
}

void BasicOutputGrid::setOutputColor(QColor fg, QColor bg) {
	// A transparent foreground is TEXTCOLOR with no arguments: the theme
	// decides the colour again, which is what m_colored being false means.
	if (fg.alpha() == 0) {
		m_fg = 0;
		m_colored = false;
	} else {
		m_fg = fg.rgba();
		m_colored = true;
	}
	// Alpha 0 is the one argument form of TEXTCOLOR clearing the background.
	m_bg = (bg.alpha() == 0 ? 0 : bg.rgba());
	// The cursor is drawn in the text colour, so it changes with it.
	update();
}

void BasicOutputGrid::setOutputBackground(QColor bg) {
	m_paneBackground = (bg.alpha() == 0 ? QColor() : bg);
	update();
}

void BasicOutputGrid::setOutputFont(QString family, int, int weight, bool italic) {
	// The size argument is deliberately dropped: see recomputeCellMetrics.
	m_family = family.trimmed();
	m_weight = weight > 0 ? weight : QFont::Normal;
	m_italic = italic;
	recomputeCellMetrics();
	update();
}

// CLS. Colours and font survive, the way a console keeps them; the reset back
// to the theme happens when a program starts, not here.
void BasicOutputGrid::clearScreen() {
	m_cells.fill(Cell());
	m_col = m_row = 0;
	clearSelection();
	update();
}

QString BasicOutputGrid::charAt(int col, int row) const {
	if (!inRange(col, row)) return QString();
	return QString(m_cells.at(index(col, row)).ch);
}

void BasicOutputGrid::scrollUp() {
	// No scrollback: the top row is gone, exactly as it was on the machines
	// this imitates.
	for (int r = 1; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			m_cells[index(c, r - 1)] = m_cells.at(index(c, r));
		}
	}
	for (int c = 0; c < m_cols; c++) {
		m_cells[index(c, m_rows - 1)] = Cell();
	}
	if (m_inputRow > 0) m_inputRow--;
	clearSelection();
}

// The wrap the last column leaves owing, paid the moment another character
// arrives. Nothing happens if the program stops there, so a PRINT that fills
// the screen exactly leaves the whole screen on show.
void BasicOutputGrid::applyPendingWrap() {
	if (m_col < m_cols) return;
	m_col = 0;
	m_row++;
	if (m_row >= m_rows) {
		m_row = m_rows - 1;
		scrollUp();
	}
}

void BasicOutputGrid::advanceCursor() {
	// Past the last column the cursor rests rather than wraps: see the header.
	m_col++;
}

void BasicOutputGrid::newLine() {
	// A line ending settles the owed wrap rather than adding to it, so the row
	// a full line ends on is not followed by a blank one.
	m_col = 0;
	m_row++;
	if (m_row >= m_rows) {
		m_row = m_rows - 1;
		scrollUp();
	}
}

void BasicOutputGrid::putChar(QChar ch, QRgb fg, QRgb bg) {
	applyPendingWrap();
	if (!inRange(m_col, m_row)) return;
	Cell &cell = m_cells[index(m_col, m_row)];
	cell.ch = ch;
	cell.fg = fg;
	cell.bg = bg;
	advanceCursor();
}

void BasicOutputGrid::outputText(QString text) {
	outputText(text, normalFg());
}

void BasicOutputGrid::outputText(QString text, QColor color) {
	const QRgb fg = color.rgba();
	for (int i = 0; i < text.length(); i++) {
		const QChar ch = text.at(i);
		if (ch == QChar::LineFeed) {
			newLine();
		} else if (ch == QChar::CarriageReturn) {
			m_col = 0;
		} else if (ch == QLatin1Char('\t')) {
			// Tab stops every 8 columns, the console convention. A tab that is
			// owed a wrap takes it first, so it counts from the new row's own
			// stops rather than from the end of the row before.
			applyPendingWrap();
			const int next = ((m_col / 8) + 1) * 8;
			while (m_col < next && m_col < m_cols) putChar(QLatin1Char(' '), fg, m_bg);
		} else {
			putChar(ch, fg, m_bg);
		}
	}
	update();
}

void BasicOutputGrid::locateCursor(int col, int row) {
	// Off screen is clamped rather than grown: the screen is a fixed size.
	m_col = qBound(0, col, m_cols - 1);
	m_row = qBound(0, row, m_rows - 1);
	update();
}

void BasicOutputGrid::cursorColRow(int *col, int *row) {
	// TEXTCOL never reports a column that is not on the screen, so a cursor
	// resting past the last one reads as the last one.
	*col = qMin(m_col, m_cols - 1);
	*row = m_row;
}

// ---------------------------------------------------------------- painting

void BasicOutputGrid::paintEvent(QPaintEvent *) {
	QPainter p(this);
	const QColor pane = paneColor();
	p.fillRect(rect(), pane);
	p.setFont(m_cellFont);

	const EditorTheme &theme = EditorTheme::current();
	const QColor selBg = theme.selectionBackground;
	const QColor selFg = theme.selectionForeground;
	const QColor fallbackFg = theme.outputText;

	for (int r = 0; r < m_rows; r++) {
		const qreal y = m_originY + r * m_cellH;
		for (int c = 0; c < m_cols; c++) {
			const Cell &cell = m_cells.at(index(c, r));
			const QRectF box(m_originX + c * m_cellW, y, m_cellW, m_cellH);
			const bool sel = cellSelected(c, r);

			if (sel) {
				p.fillRect(box, selBg);
			} else if (qAlpha(cell.bg) != 0) {
				p.fillRect(box, QColor::fromRgba(cell.bg));
			}

			if (cell.ch.isNull() || cell.ch == QLatin1Char(' ')) continue;

			if (sel) {
				p.setPen(selFg);
			} else {
				p.setPen(qAlpha(cell.fg) != 0 ? QColor::fromRgba(cell.fg) : fallbackFg);
			}
			// Centred in its own cell rather than laid out by the font's own
			// advance, so a proportional family still lands on the grid.
			p.drawText(box, Qt::AlignCenter, QString(cell.ch));
		}
	}

	// The cursor, drawn the way the flowing pane draws it: a bar in the text
	// colour, so it stays visible on a dark TEXTBACKGROUND.
	// A cursor resting past the last column is drawn against the right hand
	// edge of the screen, which reads as "this row is full" rather than
	// sitting on top of the character that filled it.
	if (m_row >= 0 && m_row < m_rows) {
		const bool resting = m_col >= m_cols;
		const int caretCol = resting ? m_cols - 1 : m_col;
		const qreal x = m_originX + caretCol * m_cellW + (resting ? m_cellW - 2.0 : 0.0);
		p.fillRect(QRectF(x, m_originY + m_row * m_cellH, 2.0, m_cellH), normalFg());
	}
}

// --------------------------------------------------------------- selection

void BasicOutputGrid::cellAt(const QPoint &pt, int *col, int *row) const {
	*col = qBound(0, (int) ((pt.x() - m_originX) / m_cellW), m_cols - 1);
	*row = qBound(0, (int) ((pt.y() - m_originY) / m_cellH), m_rows - 1);
}

bool BasicOutputGrid::cellSelected(int col, int row) const {
	if (!m_selActive) return false;

	if (m_selBlock) {
		const int c0 = qMin(m_selAnchorCol, m_selCol), c1 = qMax(m_selAnchorCol, m_selCol);
		const int r0 = qMin(m_selAnchorRow, m_selRow), r1 = qMax(m_selAnchorRow, m_selRow);
		return col >= c0 && col <= c1 && row >= r0 && row <= r1;
	}

	// Flowing selection, in reading order.
	int c0 = m_selAnchorCol, r0 = m_selAnchorRow, c1 = m_selCol, r1 = m_selRow;
	if (r1 < r0 || (r1 == r0 && c1 < c0)) {
		qSwap(c0, c1);
		qSwap(r0, r1);
	}
	if (row < r0 || row > r1) return false;
	if (row == r0 && col < c0) return false;
	if (row == r1 && col > c1) return false;
	return true;
}

void BasicOutputGrid::clearSelection() {
	if (m_selActive) {
		m_selActive = false;
		emit(selectionChanged(false));
		update();
	}
}

QString BasicOutputGrid::selectedText() const {
	if (!m_selActive) return QString();
	QString out;
	bool first = true;
	for (int r = 0; r < m_rows; r++) {
		QString line;
		bool any = false;
		for (int c = 0; c < m_cols; c++) {
			if (!cellSelected(c, r)) continue;
			any = true;
			line.append(m_cells.at(index(c, r)).ch);
		}
		if (!any) continue;
		// Trailing blanks on a grid are padding, not content.
		while (line.endsWith(QLatin1Char(' '))) line.chop(1);
		if (!first) out.append(QLatin1Char('\n'));
		out.append(line);
		first = false;
	}
	return out;
}

QString BasicOutputGrid::screenText() const {
	QString out;
	for (int r = 0; r < m_rows; r++) {
		QString line;
		for (int c = 0; c < m_cols; c++) line.append(m_cells.at(index(c, r)).ch);
		while (line.endsWith(QLatin1Char(' '))) line.chop(1);
		if (r) out.append(QLatin1Char('\n'));
		out.append(line);
	}
	// Blank rows at the foot of the screen are not worth printing.
	while (out.endsWith(QLatin1Char('\n'))) out.chop(1);
	return out;
}

void BasicOutputGrid::mousePressEvent(QMouseEvent *e) {
	if (e->button() != Qt::LeftButton) {
		QWidget::mousePressEvent(e);
		return;
	}
	setFocus();
	// Alt drags a rectangle, which is what box art and aligned columns want.
	m_selBlock = (e->modifiers() & Qt::AltModifier) != 0;
	cellAt(e->pos(), &m_selAnchorCol, &m_selAnchorRow);
	m_selCol = m_selAnchorCol;
	m_selRow = m_selAnchorRow;
	m_selDragging = true;
	const bool was = m_selActive;
	m_selActive = false;
	if (was) emit(selectionChanged(false));
	update();
}

void BasicOutputGrid::mouseMoveEvent(QMouseEvent *e) {
	if (!m_selDragging) {
		QWidget::mouseMoveEvent(e);
		return;
	}
	int c, r;
	cellAt(e->pos(), &c, &r);
	if (c == m_selCol && r == m_selRow && m_selActive) return;
	m_selCol = c;
	m_selRow = r;
	const bool active = !(c == m_selAnchorCol && r == m_selAnchorRow);
	if (active != m_selActive) {
		m_selActive = active;
		emit(selectionChanged(active));
	}
	update();
}

void BasicOutputGrid::mouseReleaseEvent(QMouseEvent *e) {
	if (e->button() == Qt::LeftButton) m_selDragging = false;
	QWidget::mouseReleaseEvent(e);
}

// ------------------------------------------------------------------- input

void BasicOutputGrid::startInput() {
	m_gettingInput = true;
	m_inputText.clear();
	m_inputCaret = 0;
	// The typed line starts on the screen, never on the resting place past the
	// last column, or redrawing it would count from a row it is not on.
	applyPendingWrap();
	m_inputCol = m_col;
	m_inputRow = m_row;
	setFocus();
	update();
}

void BasicOutputGrid::endInput() {
	m_gettingInput = false;
	update();
}

// Repaint the line being typed. One blank past the end covers the character a
// backspace has just removed.
void BasicOutputGrid::redrawInput() {
	const QRgb fg = normalFg().rgba();

	m_col = m_inputCol;
	m_row = m_inputRow;
	for (int i = 0; i < m_inputText.length(); i++) putChar(m_inputText.at(i), fg, m_bg);
	putChar(QLatin1Char(' '), fg, m_bg);

	// The cursor sits at the caret, not at the end of what was typed.
	const int flat = m_inputCol + m_inputCaret;
	m_col = qBound(0, flat % m_cols, m_cols - 1);
	m_row = qBound(0, m_inputRow + (flat / m_cols), m_rows - 1);
	update();
}

void BasicOutputGrid::commitInput() {
	// The typed line ends where it ends; the cursor drops to the row below.
	const int flat = m_inputCol + m_inputText.length();
	m_col = 0;
	m_row = m_inputRow + (flat / m_cols);
	newLine();

	const QString text = m_inputText;
	m_inputText.clear();
	m_inputCaret = 0;
	m_gettingInput = false;
	update();
	emit(inputEntered(text));
}

void BasicOutputGrid::insertInputText(const QString &text) {
	if (!m_gettingInput) return;
	QString s = text;
	// Only ever the first line, matching the flowing pane's paste.
	const int nl = s.indexOf(QRegularExpression(QStringLiteral("[\r\n]")));
	if (nl >= 0) s = s.left(nl);
	if (s.isEmpty()) return;
	m_inputText.insert(m_inputCaret, s);
	m_inputCaret += s.length();
	redrawInput();
}

void BasicOutputGrid::keyPressEvent(QKeyEvent *e) {
	e->accept();

	if (!m_gettingInput) {
		// Everything, Tab and the arrows included, is offered to the program
		// through KEY and KEYPRESSED.
		mymutex->lock();
		basicKeyboard->keyPressed(e);
		mymutex->unlock();
		return;
	}

	switch (e->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			commitInput();
			return;
		case Qt::Key_Backspace:
			if (m_inputCaret > 0) {
				m_inputText.remove(m_inputCaret - 1, 1);
				m_inputCaret--;
				redrawInput();
			}
			return;
		case Qt::Key_Delete:
			if (m_inputCaret < m_inputText.length()) {
				m_inputText.remove(m_inputCaret, 1);
				redrawInput();
			}
			return;
		case Qt::Key_Left:
			if (m_inputCaret > 0) { m_inputCaret--; redrawInput(); }
			return;
		case Qt::Key_Right:
			if (m_inputCaret < m_inputText.length()) { m_inputCaret++; redrawInput(); }
			return;
		case Qt::Key_Home:
			m_inputCaret = 0;
			redrawInput();
			return;
		case Qt::Key_End:
			m_inputCaret = m_inputText.length();
			redrawInput();
			return;
		case Qt::Key_Up:
		case Qt::Key_Down:
			// INPUT is one field. Roaming between rows would leave nothing
			// sensible to return, so the row keys do nothing while it runs.
			return;
		default:
			break;
	}

	const QString t = e->text();
	if (!t.isEmpty() && t.at(0).isPrint()) {
		m_inputText.insert(m_inputCaret, t);
		m_inputCaret += t.length();
		redrawInput();
	}
}

void BasicOutputGrid::keyReleaseEvent(QKeyEvent *e) {
	e->accept();
	if (!m_gettingInput) {
		mymutex->lock();
		basicKeyboard->keyReleased(e);
		mymutex->unlock();
	}
}

void BasicOutputGrid::focusOutEvent(QFocusEvent *) {
	basicKeyboard->reset();
}
