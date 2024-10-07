/*
 * This file is part of Freecell.
 *
 * Freecell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Freecell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Freecell.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cardwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QMetaEnum>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QStyleOption>
#include <QVBoxLayout>

CardWidget::CardWidget(QWidget* parent)
	: QFrame(parent)
{
	// Enable clipping for the frame (ensures that content outside the rounded border is clipped)
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setAttribute(Qt::WA_OpaquePaintEvent, true);

	resize(WIDTH, HEIGHT);
}

void CardWidget::setCard(Card::Value value, Card::Suit suit)
{
	QString valueName = QMetaEnum::fromType<Card::Value>().valueToKey(value);
	if (valueName.isEmpty())
		valueName = QString::number(value);

	QString resource = QString(":/cards/%1/%2").arg(QMetaEnum::fromType<Card::Suit>().valueToKey(suit)).arg(valueName);
	this->setStyleSheet(QString("background-color: transparent; border-image: url(%1); margin: %2px;").arg(resource).arg(MARGIN));
}

Card::Value CardWidget::value() const noexcept
{
	return m_value;
}

Card::Suit CardWidget::suit() const noexcept
{
	return m_suit;
}

void CardWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);  // Enable antialiasing

	// Create a path for the rounded rectangle
	QPainterPath path;
	int radius = BORDER_RADIUS;  // Adjust the radius as needed
	QRectF rect = this->rect().adjusted(0.5, 0.5, -0.5, -0.5);  // Adjust rect for the 1px border
	path.addRoundedRect(rect, radius, radius);

	// Clip the content outside the rounded corners
	painter.setClipPath(path);

	// Load the background image
	QPixmap backgroundImage(":/images/card"); // Replace with your image path

	// Check if the image was loaded successfully
	if (backgroundImage.isNull())
	{
		qWarning("Failed to load background image.");
	}

	// Create a QBrush with the background image
	QBrush backgroundBrush(backgroundImage);
	backgroundBrush.setStyle(Qt::TexturePattern); // Set the brush to tile the image

	// Step 1: Fill the background with white
	painter.fillPath(path, backgroundBrush);

	// Step 2: Draw the border image (assuming 9-slice scaling for borders)
	QStyleOptionFrame opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Frame, &opt, &painter, this);

	// Step 4: Draw a 1px black border
	if(!m_selected)
	{
		QPen pen(Qt::darkGray, 1); // 1px black border
		painter.setPen(pen);
	}
	else
	{
		QPen pen(Qt::darkGray, 1); // change this color if you want to debug selections. It looks bad in prod.
		painter.setPen(pen);
	}
	painter.drawPath(path);

	// Call the base class implementation to draw child widgets
	QFrame::paintEvent(event);
}

bool CardWidget::selected() const
{
	return m_selected;
}

void CardWidget::setSelected(bool selected)
{
	m_selected = selected;
}