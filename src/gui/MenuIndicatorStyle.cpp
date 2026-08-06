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

#include "MenuIndicatorStyle.h"

#include <QPainter>
#include <QPaintDevice>
#include <QPixmap>
#include <QPixmapCache>
#include <QStyleOption>

void MenuIndicatorStyle::drawControl(ControlElement element, const QStyleOption *option,
									 QPainter *painter, const QWidget *widget) const {

	if (element == CE_MenuItem) {
		const QStyleOptionMenuItem *item = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
		if (item && item->menuItemType == QStyleOptionMenuItem::Normal
				 && item->checkType != QStyleOptionMenuItem::NotCheckable) {

			qreal dpr = 1.0;
			if (painter && painter->device()) dpr = painter->device()->devicePixelRatioF();

			QStyleOptionMenuItem indicated(*item);
			indicated.icon = indicatorIcon(*item, widget, dpr);
			// The pixmap now carries the state, so hand the base style what
			// looks like an ordinary item: styles that tick a checked item, or
			// that draw a box of their own for an icon-less one, would
			// otherwise paint a second indicator over ours.
			indicated.checkType = QStyleOptionMenuItem::NotCheckable;
			indicated.checked = false;

			QProxyStyle::drawControl(element, &indicated, painter, widget);
			return;
		}
	}

	QProxyStyle::drawControl(element, option, painter, widget);
}

QIcon MenuIndicatorStyle::indicatorIcon(const QStyleOptionMenuItem &item, const QWidget *widget,
										qreal devicePixelRatio) const {

	const bool exclusive = (item.checkType == QStyleOptionMenuItem::Exclusive);

	// Never larger than an icon slot: the base style scales an oversized icon
	// down to PM_SmallIconSize, and a smooth-scaled box looks blurred next to
	// the crisp ones the same style paints in dialogs.
	const int iconSize = proxy()->pixelMetric(PM_SmallIconSize, &item, widget);
	int size = proxy()->pixelMetric(exclusive ? PM_ExclusiveIndicatorWidth : PM_IndicatorWidth,
									&item, widget);
	if (size <= 0) size = iconSize;
	if (iconSize > 0 && size > iconSize) size = iconSize;
	if (size <= 0) return QIcon();

	// The palette key covers a switch between the light and dark colour scheme,
	// which repaints the menus with different indicator colours.
	const QString key = QStringLiteral("basic256-menuindicator-")
			+ QString::number(size) + QLatin1Char('-')
			+ QString::number(qRound(devicePixelRatio * 100)) + QLatin1Char('-')
			+ QLatin1Char(exclusive ? 'r' : 'c')
			+ QLatin1Char(item.checked ? '1' : '0') + QLatin1Char('-')
			+ QString::number(item.palette.cacheKey());

	QPixmap pixmap;
	if (!QPixmapCache::find(key, &pixmap)) {
		pixmap = QPixmap(QSize(size, size) * devicePixelRatio);
		pixmap.setDevicePixelRatio(devicePixelRatio);
		pixmap.fill(Qt::transparent);

		QPainter pixmapPainter(&pixmap);
		QStyleOptionButton indicator;
		indicator.rect = QRect(0, 0, size, size);
		indicator.palette = item.palette;
		indicator.fontMetrics = item.fontMetrics;
		// Always drawn as enabled. A disabled item is dimmed by the base style
		// when it renders the icon (QIcon::Disabled); dimming it twice would
		// leave the box nearly invisible.
		indicator.state = State_Enabled | (item.checked ? State_On : State_Off);
		// No widget is passed: this is a check box being drawn, not the menu,
		// and a style that reads hover or animation state off the widget would
		// be reading it from the wrong one.
		proxy()->drawPrimitive(exclusive ? PE_IndicatorRadioButton : PE_IndicatorCheckBox,
							   &indicator, &pixmapPainter, nullptr);
		pixmapPainter.end();

		QPixmapCache::insert(key, pixmap);
	}

	return QIcon(pixmap);
}
