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

#include "abstractcardholder.h"

/*!
 * \brief Construtor
 */
AbstractCardHolder::AbstractCardHolder()
{
	m_parent = nullptr;
	m_child	 = nullptr;
}

AbstractCardHolder* AbstractCardHolder::getParent()
{
    return m_parent;
}

void AbstractCardHolder::setChild(Card* card)
{
	m_child = card;
}

Card* AbstractCardHolder::getChild()
{
    return m_child;
}

int AbstractCardHolder::getZIndex()
{
    return 0;
}