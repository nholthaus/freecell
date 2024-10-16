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

#include "deck.h"
#include "card.h"

#include <algorithm>
#include <random>

#include "board.h"

/*!
 * \brief Constructor
 * \param board The board
 */
Deck::Deck(Board* board)
{
	build(board);
}

/*!
 * \brief Shuffle the cards in this deck
 *
 * The cards are randomly reordered in the deck. Any number of cards can be
 * present in the deck.
 */
void Deck::shuffle(unsigned int seed)
{
	std::mt19937 generator{seed};
	std::ranges::shuffle(mCards, generator);
}

/*!
 * \brief Build the deck
 *
 * The function creates the cards ordered by color and vale
 * and adds them to the deck successively.
 *
 * \param board The board
 */
void Deck::build(Board* board)
{
	for (auto* card : mCards)
	{
		if (card)
			card->deleteLater();
	}
	mCards.clear();

	for (auto i = static_cast<Card::Suit>(1); i < Card::Suit::LASTSUIT; ++i)
	{
		for (Card::Value j = Card::Value::ACE; j < Card::Value::LASTVALUE; ++j)
		{
			auto card = new Card(i, j, board);
			pushCard(card);
		}
	}
}

/*!
 * \brief Draw the last card from the deck
 * \return A card
 */
Card* Deck::drawCard()
{
	Card* card = mCards.back();
	mCards.pop_back();

	return card;
}

/*!
 * \brief Push a card at the end of the deck
 * \param card The card to add
 */
void Deck::pushCard(Card* card)
{
	card->setScattered(false);
	mCards.push_back(card);
}

/*!
 * \brief Get the number of cards in the deck
 * \return The size (int)
 */
int Deck::getSize() const
{
	return mCards.size();
}

/*!
 * \brief Whether the deck is empty of cards
 * \return true if empty
 */
bool Deck::empty() const noexcept
{
	return mCards.empty();
}