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

#include <QMetaEnum>
#include <QPoint>

#include "abstractcardholder.h"
#include "move.h"

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
		CLUBS	 = 1,
		DIAMONDS = 2,
		HEARTS	 = 3,
		SPADES	 = 4,
		LASTSUIT
	};
	Q_ENUM(Suit)

	static QString suitName(Card::Suit suit) noexcept
	{
		return QMetaEnum::fromType<Card::Suit>().valueToKey(suit);
	}

public:

	Card(Suit suit, Value value, Board* board);
	~Card() override;

	void setParent(AbstractCardHolder*, bool = false);
	int	 countChildren();

	bool canStackCard(Card*) override;
	bool isStackable() override;
	bool isMovable();
	bool isValidParentOf(Card*);
	bool isValidParentOfAllChildren();
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
	void   animateRotation(int angle);
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
	void setScattered(bool scattered);
	void automaticMove();

	CardProxy* proxy();

signals:
	void moved(Move move);

public slots:
	void resetZIndex();
	void scatter(QPoint point, int angle);

protected:

	Suit  m_suit;
	Value m_value;

	QPoint m_position;

	Board*		m_board = nullptr;
	CardWidget* m_widget = nullptr;
	CardProxy*	m_proxy = nullptr;

	bool m_isOnAceSpot = false;
	bool m_isScattered = false;
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