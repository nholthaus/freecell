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

#include "freecell.h"
#include "board.h"
#include "cardwidget.h"
#include "cardspotproxy.h"

#include <QFrame>

/*!
 * \brief Constructor
 * \param board
 */
Freecell::Freecell(Board* board) : CardSpot(board)
{
    mProxy = new CardSpotProxy(this);
    mProxy->setData(0, QVariant("freecell"));

    auto* widget = new QFrame();
    widget->resize(CardWidget::WIDTH, CardWidget::HEIGHT);
	widget->setStyleSheet(QString("background-color:transparent; border: 6px solid darkGreen; border-radius: %1px;").arg(CardWidget::BORDER_RADIUS));
    mProxy->setWidget(widget);
    mBoard->addItem(mProxy);
}

bool Freecell::isStackable()
{
	return false;
}

bool Freecell::canStackCard(Card* card)
{
	return isEmpty() && card->isMovable() && !card->getChild() && card->getValue() != Card::Value::ACE;
}