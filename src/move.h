// ---------------------------------------------------------------------------------------------------------------------
//
/// @file       move.h
/// @author     Nic Holthaus
/// @date       10/8/2024
/// @copyright  (c) 2024 STR. The use of this software is subject to the terms and conditions outlined
///             in the LICENSE file. By using this software, the user agrees to be bound by the terms and
///             conditions set forth in the LICENSE file.
//
// ---------------------------------------------------------------------------------------------------------------------
//
/// @brief      Definition of the `Move` Class.
/// @details
//
// ---------------------------------------------------------------------------------------------------------------------

#ifndef FREECELL_MOVE_H
#define FREECELL_MOVE_H

//----------------------------
//  INCLUDES
//----------------------------

class Card;
class AbstractCardHolder;

//----------------------------------------------------------------------------------------------------------------------
//      CLASS: Move
//----------------------------------------------------------------------------------------------------------------------
/// @brief
//----------------------------------------------------------------------------------------------------------------------
class Move
{
public:

	Move(Card* card, AbstractCardHolder* parent, AbstractCardHolder* previousParent);

	[[nodiscard]] Card*				  card() const;
	[[nodiscard]] AbstractCardHolder* parent() const;
	[[nodiscard]] AbstractCardHolder* previousParent() const;

private:

	Card*				m_card			 = nullptr;
	AbstractCardHolder* m_parent		 = nullptr;
	AbstractCardHolder* m_previousParent = nullptr;
};

#endif // FREECELL_MOVE_H
