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

#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <deque>
#include <vector>

#include "card.h"
#include "deck.h"

class QGraphicsProxyWidget;
class QGraphicsView;
class QWidget;

class AceSpot;
class ColumnSpot;
class Freecell;
class BoardScene;

class Board : public QObject
{
	Q_OBJECT
public:

	static constexpr int SPACING = 15;

public:

	Board();

	void dealCards(unsigned int gameNumber);
	void collectCards();

	int	 countFreeCells();
	int	 countEmptyColumns();
	bool hasEnoughFreecells(int cardsToMove);

	void automaticMove(Card*);
	void unselectCard();
	void selectCard(Card*);

	void  setSelectedCard(Card*);
	Card* getSelectedCard();
	void  addItem(QGraphicsProxyWidget*);

	void setRelaxed(bool value);
	bool isRelaxed() const noexcept;

	QWidget* getBoardWidget();

public slots:

	void newGame();
	void selectGame();
	void restartGame();
	void endGame();

	bool tryAutomaticAceMove(Card* card = nullptr);
	void onCardMoved(Move move);
	void onUndo();
	void onRedo();

protected:

	std::vector<AceSpot*>  mAceSpots;
	std::vector<Freecell*> mFreeCells;

	const static int NB_COLUMNS = 8;
	Card*			 mLeafColumns[NB_COLUMNS];
	ColumnSpot*		 mColumns[NB_COLUMNS];

	Deck*		   mDeck;
	QGraphicsView* mBoardWidget;
	BoardScene*	   mScene;

	Card*			   mSelectedCard;
	std::vector<Card*> mCards;

	std::deque<Move> mUndoMoves;
	std::deque<Move> mRedoMoves;

	QGraphicsProxyWidget* mNewGameProxy = nullptr;
	QGraphicsProxyWidget* mRestartProxy = nullptr;
	QGraphicsProxyWidget* mGameNumberProxy = nullptr;
	QGraphicsProxyWidget* mUndoProxy = nullptr;

	bool		 mRelaxed	 = false;
	unsigned int mGameNumber = 0;
};

#endif // BOARD_H