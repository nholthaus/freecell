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

#include "acespot.h"
#include "board.h"
#include "cardspotproxy.h"
#include "cardwidget.h"

#include <QLabel>
#include <QMetaEnum>

/*!
 * \brief Constructor
 * \param board The board
 */
AceSpot::AceSpot(Board* board, Card::Suit suit)
	: CardSpot(board)
	, m_suit(suit)
{
	mProxy = new CardSpotProxy(this);
	mProxy->setData(0, QVariant("acespot"));
	mProxy->setData(1, QVariant(m_suit));

	auto backgroundImage = QPixmap(QString(":/suits/%1").arg(Card::suitName(suit)));
	backgroundImage = backgroundImage.scaled(backgroundImage.width()/2.0, backgroundImage.height()/2.0);

	auto* widget = new QLabel();
	widget->resize(CardWidget::WIDTH, CardWidget::HEIGHT);
	widget->setStyleSheet(QString("background-color:transparent;"
								  "border: 6px solid darkGreen;"
								  "border-radius: %1px;")
							  .arg(CardWidget::BORDER_RADIUS));
	widget->setPixmap(backgroundImage);
	widget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	mProxy->setWidget(widget);
	mBoard->addItem(mProxy);
}

/*!
 * \brief Check if a card can be stacked on this spot
 * \param card The card to check
 * \return boolean
 */
bool AceSpot::canStackCard(Card* card)
{
	if (isEmpty() && card->isMovable() && card->getValue() == Card::Value::ACE && card->getSuit() == m_suit)
	{
		return true;
	}
	return false;
}

/*!
 * \brief Check if the cards can be stacked one after another on this spot
 * \return boolean
 */
bool AceSpot::isStackable()
{
	return true;
}

/*!
 * \brief Set a new card as child of this spot
 * \param card The card
 */
void AceSpot::setChild(Card* card)
{
	CardSpot::setChild(card);
	if (card)
	{
		card->setOnAceSpot(true);
	}
}