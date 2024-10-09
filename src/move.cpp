// ---------------------------------------------------------------------------------------------------------------------
//
/// @file       move.cpp
/// @author     Nic Holthaus
/// @date       10/8/2024
/// @copyright  (c) 2024 STR. The use of this software is subject to the terms and conditions outlined
///             in the LICENSE file. By using this software, the user agrees to be bound by the terms and
///             conditions set forth in the LICENSE file.
//
// ---------------------------------------------------------------------------------------------------------------------
//
/// @details    Implementation file for `move.h`.
//
// ---------------------------------------------------------------------------------------------------------------------

//----------------------------
//  INCLUDES
//----------------------------

#include "move.h"

//======================================================================================================================
//      MEMBER FUNCTIONS
//======================================================================================================================

Card* Move::card() const
{
	return m_card;
}

AbstractCardHolder* Move::parent() const
{
	return m_parent;
}

AbstractCardHolder* Move::previousParent() const
{
	return m_previousParent;
}