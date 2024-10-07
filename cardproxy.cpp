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

#include "cardproxy.h"
#include "card.h"
#include "cardspotproxy.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>

/*!
 * \brief Constructor
 * \param card The card linked to this proxy
 */
CardProxy::CardProxy(Card* card)
	: QGraphicsProxyWidget()
{
	mCard = card;
	setData(0, QVariant("card"));
}

void CardProxy::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	mCard->automaticMove();
}

/*!
 * \brief Handles mouse press events
 * \param event The mouse event
 */
void CardProxy::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!mCard->isSelected() && mCard->isMovable() && mCard->isValidParentOfAllChildren())
		{
			mCard->select();
			mCard->setZIndex(100);
			event->setAccepted(true);
			return;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		mCard->setZIndex(mCard->getTopZIndex(), false);
		event->setAccepted(true);
		return;
	}
	event->setAccepted(false);
}

/*!
 * \brief Handles mouse release events
 * \param event The mouse event
 */
void CardProxy::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		int index = 1;
		if (mCard->getParent())
		{
			index = mCard->getParent()->getZIndex() + 1;
		}
		mCard->setZIndex(index, false);
	}
	else if (event->button() == Qt::LeftButton)
	{
		QList<QGraphicsItem*> items		= this->scene()->items(event->scenePos());
		bool				  selection = false;

		// if the card is not moved enough, replace it
		if ((event->buttonDownScenePos(Qt::LeftButton) - event->scenePos()).manhattanLength() < 10)
		{
			mCard->updatePosition(true);
			return;
		}

		// browse the items underneath
		for (auto& item : items)
		{
			if (item != this)
			{
				if (item->data(0) == QVariant("card"))
				{
					auto* proxy = dynamic_cast<CardProxy*>(item);
					if (proxy->mCard->getChild() == nullptr)
					{
						proxy->mCard->select();
						selection = true;
					}
					break;
				}
				if (item->data(0) == QVariant("acespot") || item->data(0) == QVariant("freecell") || item->data(0) == QVariant("columnspot"))
				{
					auto* proxy = dynamic_cast<CardSpotProxy*>(item);
					proxy->select();
					selection = true;
					break;
				}
			}
		}
		// if not item matches, replace the card at its original position
		if (!selection)
		{
			mCard->updatePosition(true);
		}
	}
}

/*!
 * \brief Handles mouse mouse events
 * \param event The mouse event
 */
void CardProxy::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		mCard->setZIndex(100);
		mCard->setPosition(event->scenePos().toPoint() - event->buttonDownPos(Qt::LeftButton).toPoint());
	}
}