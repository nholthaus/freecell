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

#ifndef ACESPOT_H
#define ACESPOT_H

#include "cardspot.h"
#include "card.h"

class Board;

/*!
 * \brief Represents ace spot objects
 *
 * The ace spot is the base of the card stack for a given color, going from ace to king.
 * This class handles the behaviour of the spot in order to allow only aces to
 * be stacked directly on it.
 */
class AceSpot : public CardSpot
{
public:

	explicit AceSpot(Board* board, Card::Suit suit);

	bool canStackCard(Card* card) override;
	bool isStackable() override;
	void setChild(Card* card) override;

private:

	Card::Suit m_suit;
};

#endif // ACESPOT_H