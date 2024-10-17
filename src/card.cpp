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

#include "card.h"
#include "board.h"
#include "cardproxy.h"
#include "cardwidget.h"

#include <QPropertyAnimation>

#include <QMetaEnum>
#include <iostream>
#include <sstream>

/*!
 * \brief Constructor
 * \param suit The suit of the card
 * \param value The value of the card
 * \param board The board of the game
 */
Card::Card(Suit suit, Value value, Board* board)
	: AbstractCardHolder()
{
	m_child		  = 0;
	m_suit		  = suit;
	m_value		  = value;
	m_board		  = board;
	m_isOnAceSpot = false;

	m_widget = new CardWidget();
	m_widget->setCard(value, suit);
	setSelected(false);

	m_proxy = new CardProxy(this);
	m_proxy->setWidget(m_widget);
	m_board->addItem(m_proxy);
}

Card::~Card()
{
	delete m_widget;
}

/*!
 * \brief Get a string for displaying the card's identity
 * \return QString
 */
QString Card::getLabel()
{
	return getValueName();
}

/*!
 * \brief Change the parent of the card and update the position accordingly
 * \param parent  The new parent
 * \param animate If set to true, the position will change through an animation
 */
void Card::setParent(AbstractCardHolder* parent, bool animate)
{
	auto newParent = parent;
	auto oldParent = m_parent;

	if (m_parent)
	{
		m_parent->setChild(nullptr);
	}

	m_parent = parent;

	if (m_parent)
	{
		m_parent->setChild(this);
		updatePosition(animate);
		m_board->unselectCard();
	}

	if(oldParent != newParent)
	{
		emit this->moved({this, newParent, oldParent});
	}
}

/*!
 * \brief Get the number of cards stacked over this one
 * \return int
 */
int Card::countChildren()
{
	if (!m_child)
	{
		return 0;
	}
	return m_child->countChildren() + 1;
}

/*!
 * \brief Check if a card can be stacked over this one
 * \param card The card to check
 * \return boolean
 */
bool Card::canStackCard(Card* card)
{
	return isStackable() && getChild() == nullptr && card->isMovable() && isValidParentOf(card);
}

/*!
 * \brief Check if the card can receive other cards over itself
 * \return boolean
 */
bool Card::isStackable()
{
	if(!m_parent)
		return false;

	return m_parent->isStackable();
}

/*!
 * \brief Check if the card can be moved from its current spot
 * \return boolean
 */
bool Card::isMovable()
{
	if (m_isOnAceSpot || m_isScattered)
	{
		return false;
	}
	if (!m_child)
	{
		return true;
	}
	return isValidParentOf(m_child) && m_child->isMovable() && m_board->hasEnoughFreecells(countChildren() + 1);
}

/*!
 * \brief Check if a given card's color and value allows it to be stacked over this one
 * \param card The card to check
 * \return
 */
bool Card::isValidParentOf(Card* card)
{
	if (!card)
	{
		return true;
	}
	if (m_isOnAceSpot)
	{
		return getValue() - card->getValue() == -1 && card->getSuit() == getSuit();
	}
	return this->getValue() - card->getValue() == 1 && card->getBlackRedColor() != this->getBlackRedColor();
}

bool Card::isValidParentOfAllChildren()
{
	if(!m_child)
		return true;

	return isValidParentOf(m_child) && m_child->isValidParentOfAllChildren();
}

/*!
 * \brief The the "ace spot" flag of the card
 * The flag is used for the stacking behaviour, as ace spot only receive card of the same colour
 * in ascendant order
 * \param on The new value
 */
void Card::setOnAceSpot(bool on)
{
	m_isOnAceSpot = on;
}

/*!
 * \brief Getter for the "ace spot" flag
 * \return boolean
 */
bool Card::isOnAceSpot()
{
	return m_isOnAceSpot;
}

/*!
 * \brief Get the value of this card as a string
 * \return QString
 */
QString Card::getValueName()
{
	QString cardValue = "";

	switch (m_value)
	{
	case ACE:
		cardValue = "ACE";
		break;
	case JACK:
		cardValue = "JACK";
		break;
	case QUEEN:
		cardValue = "QUEEN";
		break;
	case KING:
		cardValue = "KING";
		break;
	default:
		std::stringstream ss;
		ss << m_value;
		cardValue = ss.str().c_str();
		break;
	}

	return cardValue;
}

/*!
 * \brief Get the color of this card as a string
 * \return QString
 */
QString Card::getSuitName()
{
	return QMetaEnum::fromType<Card::Suit>().valueToKey((int)m_suit);
}

Card::Value Card::getValue()
{
	return m_value;
}

Card::Suit Card::getSuit()
{
	return m_suit;
}

/*!
 * \brief Convert the color of the card to the "real" (red or black) color
 * \return 1 or 2
 */
char Card::getBlackRedColor()
{
	if (m_suit == Card::Suit::HEARTS || m_suit == Card::Suit:: DIAMONDS)
	{
		return 1;
	}
	return 2;
}

QPoint Card::getChildPosition()
{
	QPoint pos = getPosition();
	int	   x   = pos.x();
	int	   y   = pos.y();
	if (!m_isOnAceSpot)
	{
		y += +CardWidget::HEIGHT / 6;
	}

	return {x, y};
}

/*!
 * \brief Get the position of the card in board coordinates
 * \return QPoint
 */
QPoint Card::getPosition()
{
	return m_position;
}

void Card::animatePosition(QPoint pos)
{
	m_position = pos;
	setZIndex(100);

	QPropertyAnimation* animation = new QPropertyAnimation(m_widget, "pos");
	animation->setDuration(100);
	animation->setStartValue(m_widget->pos());
	animation->setEndValue(m_position);
	animation->start(QAbstractAnimation::DeleteWhenStopped);

	QObject::connect(animation, SIGNAL(finished()), this, SLOT(resetZIndex()), Qt::QueuedConnection);

	if (m_child)
	{
		m_child->updatePosition(true);
	}
}

void Card::animateRotation(int angle)
{
	QPropertyAnimation* animation = new QPropertyAnimation(m_proxy, "rotation");
	animation->setDuration(100);
	animation->setStartValue(0);
	animation->setEndValue(angle);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/*!
 * \brief Change the position of the card and its children to pos
 * \param pos The new position
 */
void Card::setPosition(QPoint pos)
{
	m_position = pos;
	m_widget->move(m_position);
	if (m_child)
	{
		m_child->updatePosition();
	}
}

/*!
 * \brief Replace the card to its expected position
 * \param animate Flag for animating the position update
 */
void Card::updatePosition(bool animate)
{
	if(!m_parent)
		return;

	if (animate)
	{
		animatePosition(m_parent->getChildPosition());
		setZIndex(100);
	}
	else
	{
		setPosition(m_parent->getChildPosition());
		setZIndex(m_parent->getZIndex() + 1);
	}
}

/*!
 * \brief Get the minimum zindex for the card to be visible over all its children
 * \return int
 */
int Card::getTopZIndex()
{
	if (m_child)
	{
		return m_child->getTopZIndex();
	}
	return getZIndex() + 1;
}

/*!
 * \brief Getter for the zindex of the card
 * \return int
 */
int Card::getZIndex()
{
	return m_proxy->zValue();
}

/*!
 * \brief Set the zindex of the card
 * \param index   The new value
 * \param cascade If set to true, the children are also updated
 */
void Card::setZIndex(int index, bool cascade)
{
	if (m_child && cascade)
	{
		m_child->setZIndex(index + 1);
	}
	m_proxy->setZValue(index);
}

void Card::resetZIndex()
{
	int parentZIndex = 0;
	if(m_parent)
		parentZIndex = m_parent->getZIndex();

	setZIndex(parentZIndex + 1);
}

/*!
 * \brief Display the card
 */
void Card::show()
{
	m_widget->show();
}

/*!
 * \brief Hide the card
 */
void Card::hide()
{
	m_widget->hide();
}

void Card::select()
{
	if(m_board->hasEnoughFreecells(this->countChildren() + 1))
		m_board->selectCard(this);
}

/*!
 * \brief Update the card design so it is selected or not
 * \param selected The new sselected status
 */
void Card::setSelected(bool selected)
{
	m_widget->setSelected(selected);
}

/*!
 * \brief Check if the card is selected
 * \return boolean
 */
bool Card::isSelected()
{
	return m_widget->selected();
}

/*!
 * \brief Attempt automatic move with this card
 * \see Board::automaticMove()
 */
void Card::automaticMove()
{
	m_board->automaticMove(this);
}

CardProxy* Card::proxy()
{
	return m_proxy;
}

void Card::scatter(QPoint point, int angle)
{
	blockSignals(true);
	setOnAceSpot(false);
	setParent(nullptr);
	animatePosition(point);
	animateRotation(angle);
	m_isScattered = true;
	blockSignals(false);
}

void Card::setScattered(bool scattered)
{
	m_isScattered = scattered;
}