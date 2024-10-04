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

#ifndef CARD_H
#define CARD_H

#include <QPoint>

#include "abstractcardholder.h"

class CardWidget;
class CardProxy;
class Board;

/*!
 * \brief A Card
 */
class Card : public AbstractCardHolder
{
	Q_OBJECT
public:

	enum Value : int
	{
		ACE	  = 1,
		JACK  = 11,
		QUEEN = 12,
		KING  = 13,
		LASTVALUE
	};
	Q_ENUM(Value)

	enum Suit : int
	{
		HEARTS	 = 1,
		DIAMONDS = 2,
		SPADES	 = 3,
		CLUBS	 = 4,
		LASTSUIT
	};
	Q_ENUM(Suit)

public:

	Card(Suit suit, Value value, Board* board);

	void setParent(AbstractCardHolder*, bool = false);
	int	 countChildren();

	bool canStackCard(Card*) override;
	bool isStackable() override;
	bool isMovable();
	bool isValidParentOf(Card*);
	void setOnAceSpot(bool);
	bool isOnAceSpot();

	QString getValueName();
	QString getSuitName();
	QString getLabel();

	Value getValue();
	Suit  getSuit();
	char  getBlackRedColor();

	QPoint getPosition();
	QPoint getChildPosition() override;
	void   animatePosition(QPoint pos);
	void   setPosition(QPoint pos);
	void   updatePosition(bool animate = false);
	int	   getTopZIndex();
	int	   getZIndex() override;
	void   setZIndex(int index, bool cascade = true);
	void   hide();
	void   show();

	void select() override;
	bool isSelected();
	void setSelected(bool selected);
	void automaticMove();

public slots:
	void resetZIndex();

protected:

	Suit  m_suit;
	Value m_value;

	Board*		m_board;
	CardWidget* m_widget;
	CardProxy*	m_proxy;

	QPoint m_position;
	bool   m_selected;

	bool m_isOnAceSpot;
};

inline Card::Value& operator++(Card::Value& value)
{
	if (value < Card::Value::LASTVALUE)
	{
		value = static_cast<Card::Value>(static_cast<int>(value) + 1);
	}
	return value;
}

inline Card::Suit& operator++(Card::Suit& color)
{
	if (color < Card::Suit::LASTSUIT)
	{
		color = static_cast<Card::Suit>(static_cast<int>(color) + 1);
	}
	return color;
}
#endif // CARD_H