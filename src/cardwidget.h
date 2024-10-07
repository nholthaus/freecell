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

#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QFrame>
#include <QLabel>
#include "card.h"

class CardWidget : public QFrame
{
	Q_OBJECT
public:

	constexpr static int WIDTH		   = 150;
	constexpr static int HEIGHT		   = 210;
	constexpr static int BORDER_RADIUS = 15;
	constexpr static int MARGIN		   = 5;

public:

	explicit CardWidget(QWidget* parent = nullptr);
	void setCard(Card::Value value, Card::Suit suit);
	void setSelected(bool selected);

	[[nodiscard]] bool		  selected() const;
	[[nodiscard]] Card::Value value() const noexcept;
	[[nodiscard]] Card::Suit  suit() const noexcept;

protected:

	void paintEvent(QPaintEvent* event) override;

protected:

	Card::Value m_value;
	Card::Suit	m_suit;
	QPixmap		m_backgroundImage;
	bool		m_selected = false;
};

#endif // CARDWIDGET_H