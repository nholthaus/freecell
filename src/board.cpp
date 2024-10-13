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

#include "board.h"
#include "acespot.h"
#include "boardscene.h"
#include "cardproxy.h"
#include "cardwidget.h"
#include "columnspot.h"
#include "freecell.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QInputDialog>
#include <QPointF>
#include <QTimer>

#include <random>
#include <thread>

#include "Button.h"
#include "timerLabel.h"

using namespace std::chrono_literals;

Board::Board()
	: QObject()
{
	int i;

	for (i = 0; i < NB_COLUMNS; i++)
	{
		mLeafColumns[i] = nullptr;
	}

	mSelectedCard = nullptr;

	mBoardWidget = new QGraphicsView();

	// Load the background image
	QPixmap backgroundImage(":/images/background"); // Replace with your image path

	// Check if the image was loaded successfully
	if (backgroundImage.isNull())
	{
		qWarning("Failed to load background image.");
	}

	// Create a QBrush with the background image
	QBrush backgroundBrush(backgroundImage);
	backgroundBrush.setStyle(Qt::TexturePattern); // Set the brush to tile the image

	// Set the background brush of the view
	mBoardWidget->setBackgroundBrush(backgroundBrush);

	// Optional: Disable the default scrollbars
	mBoardWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mBoardWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	qreal minWidth	= 9 * CardWidget::WIDTH + 12 * SPACING;
	qreal minHeight = 4 * CardWidget::HEIGHT + 4 * SPACING;

	mScene = new BoardScene(QRectF(QPointF(0, 0), QPointF(minWidth, minHeight)), mBoardWidget);
	mBoardWidget->setAlignment(Qt::AlignTop | Qt::AlignCenter);
	mBoardWidget->setMinimumSize(minWidth, minHeight);
	mBoardWidget->setScene(mScene);

	QObject::connect(mScene, SIGNAL(rightClick()), this, SLOT(tryAutomaticAceMove()));

	Freecell* freecell;
	for (i = 0; i < 4; i++)
	{
		freecell = new Freecell(this);
		freecell->setPosition(QPointF(i * (CardWidget::WIDTH + SPACING) + 2 * SPACING, SPACING));
		mFreeCells.push_back(freecell);
	}

	AceSpot* aceSpot;
	for (i = 0; i < 4; i++)
	{
		aceSpot = new AceSpot(this, static_cast<Card::Suit>(i + 1));
		aceSpot->setPosition(QPointF((5 + i) * (CardWidget::WIDTH + SPACING) + 2 * SPACING, SPACING));
		mAceSpots.push_back(aceSpot);
	}

	ColumnSpot* columnSpot;
	for (i = 0; i < NB_COLUMNS; i++)
	{
		columnSpot = new ColumnSpot(this);
		columnSpot->setPosition(QPointF((0.5 + i) * (CardWidget::WIDTH + SPACING) + 2 * SPACING, 2 * SPACING + CardWidget::HEIGHT));
		mColumns[i] = columnSpot;
	}

	mDeck = new Deck(this);

	auto* newGameButton = new Button();
	newGameButton->setText("New Game");
	connect(newGameButton, &Button::clicked, this, &Board::newGame);

	mNewGameProxy = mScene->addWidget(newGameButton);
	mNewGameProxy->setPos(QPointF(2.5 * SPACING + CardWidget::WIDTH / 2, mScene->height() - newGameButton->height() - SPACING));

	auto* restartButton = new Button();
	restartButton->setText("Restart");
	connect(restartButton, &Button::clicked, this, &Board::restartGame);

	mRestartProxy = mScene->addWidget(restartButton);
	mRestartProxy->setPos(QPointF(3.5 * SPACING + 1.5 * CardWidget::WIDTH, mScene->height() - restartButton->height() - SPACING));

	mGameTimer = new QTimer;
	mGameTimer->setInterval(1000);

	auto* timerLabel = new TimerLabel();
	timerLabel->setTime(0);
	connect(mGameTimer, &QTimer::timeout, this, [=] { timerLabel->setTime(++mGameTime); });

	mTimerProxy = mScene->addWidget(timerLabel);
	mTimerProxy->setPos(QPointF(4.5 * SPACING + 2.5 * CardWidget::WIDTH, mScene->height() - restartButton->height() - SPACING));

	auto* gameNumberLabel = new Label();
	gameNumberLabel->setText("Game #: 0");
	gameNumberLabel->setFixedWidth(2 * CardWidget::WIDTH + SPACING);

	mGameNumberProxy = mScene->addWidget(gameNumberLabel);
	mGameNumberProxy->setPos(QPointF(mScene->width() / 2 - gameNumberLabel->width() / 2, mScene->height() - gameNumberLabel->height() - SPACING));

	auto* undoButton = new Button();
	undoButton->setIcon(QIcon(":/icons/undo"));
	undoButton->setText("UNDO");
	connect(undoButton, &Button::clicked, this, &Board::onUndo);

	mUndoProxy = mScene->addWidget(undoButton);
	mUndoProxy->setPos(QPointF(mScene->width() - undoButton->width() - CardWidget::WIDTH / 2 - 2 * SPACING, mScene->height() - undoButton->height() - SPACING));
}

QWidget* Board::getBoardWidget()
{
	return mBoardWidget;
}

void Board::addItem(QGraphicsProxyWidget* proxy)
{
	mScene->addItem(proxy);
}

void Board::dealCards(unsigned int gameNumber)
{
	Card* card;
	int	  i = 0, col = 0;

	mDeck->build(this); // NMH: TODO don't rebuild the deck unless restarting the same game
	mDeck->shuffle(gameNumber);

	mUndoMoves.clear();
	mRedoMoves.clear();

	if (auto* label = dynamic_cast<QLabel*>(mGameNumberProxy->widget()); label)
		label->setText(QString("Game #: %1").arg(gameNumber));

	while (!mDeck->empty())
	{
		card = mDeck->drawCard();
		QPoint pos((i % NB_COLUMNS) * CardWidget::WIDTH, (i / NB_COLUMNS) * CardWidget::HEIGHT / 8);
		card->setPosition(pos);
		card->setParent(mLeafColumns[col]);
		card->setOnAceSpot(false);
		mLeafColumns[col] = card;

		// first line
		if (i / NB_COLUMNS == 0)
		{
			card->setParent(mColumns[i]);
		}
		col = ++i % NB_COLUMNS;

		mCards.push_back(card);
		card->show();

		connect(card, &Card::moved, this, &Board::onCardMoved);
	}
}

void Board::collectCards()
{
	Card* card;

	for (auto& mLeafColumn : mLeafColumns)
	{
		mLeafColumn = nullptr;
	}

	while (!mCards.empty())
	{
		card = mCards.back();
		card->hide();
		card->setParent(nullptr);
		mDeck->pushCard(card);

		mCards.pop_back();
	}
}

int Board::countFreeCells()
{
	int count = 0;
	for (auto freecell : mFreeCells)
	{
		if (freecell->isEmpty())
		{
			count++;
		}
	}

	return count;
}

int Board::countEmptyColumns()
{
	int count = 0;
	for (auto column : mColumns)
	{
		if (column->isEmpty())
		{
			count++;
		}
	}

	return count;
}

bool Board::hasEnoughFreecells(int cardsToMove)
{
	if (mRelaxed)
		return true;

	return cardsToMove <= (countFreeCells() + 1) * (int)pow(2, countEmptyColumns());
}

void Board::automaticMove(Card* card)
{
	// See if it's an ACE
	if (tryAutomaticAceMove(card))
	{
		return;
	}

	// if not, try to move its stack to the bottom of a column
	AbstractCardHolder* bottomSpot;
	for (auto& mColumn : mColumns)
	{
		bottomSpot = mColumn;
		while (bottomSpot->getChild())
		{
			// get the bottom card of the column
			bottomSpot = bottomSpot->getChild();
		}
		if (bottomSpot->canStackCard(card) && hasEnoughFreecells(card->countChildren()))
		{
			card->setParent(bottomSpot, true);
			return;
		}
	}

	// lastly, move it to a free cell IF it doesn't have children
	if (!card->getChild())
	{
		std::vector<Freecell*>::iterator itFreecell;
		for (itFreecell = mFreeCells.begin(); itFreecell < mFreeCells.end(); itFreecell++)
		{
			if ((*itFreecell)->isEmpty())
			{
				card->setParent(*itFreecell, true);
				return;
			}
		}
	}
}

bool Board::tryAutomaticAceMove(Card* card)
{
	if (card)
	{
		std::vector<AceSpot*>::iterator itAce;
		AbstractCardHolder*				holder = nullptr;
		for (itAce = mAceSpots.begin(); itAce < mAceSpots.end(); itAce++)
		{
			holder = *itAce;
			while (holder->getChild())
			{
				holder = holder->getChild();
			}
			if (holder->canStackCard(card))
			{
				unselectCard();
				selectCard(card);
				holder->select();
				return true;
			}
		}
	}
	else
	{
		for (auto& mColumn : mColumns)
		{
			if (mColumn->getChild())
			{
				card = mColumn->getChild();
				while (card->getChild())
				{
					// get the bottom card of the column
					card = card->getChild();
				}
				if (tryAutomaticAceMove(card))
				{
					// search for another card to move
					tryAutomaticAceMove();
				}
			}
		}
	}
	return false;
}

void Board::onCardMoved(Move move)
{
	mUndoMoves.push_back(move);
	while (tryAutomaticAceMove(nullptr))
		;
	if (!mGameTimer->isActive())
		mGameTimer->start();
}

void Board::onUndo()
{
	if (!mUndoMoves.empty())
	{
		auto move = mUndoMoves.back();
		mUndoMoves.pop_back();

		if (auto card = move.card(); card)
		{
			card->blockSignals(true);
			card->setOnAceSpot(false);
			card->setParent(move.previousParent(), true);
			card->blockSignals(false);

			mRedoMoves.push_back(move);
		}
	}
}

void Board::onRedo()
{
	if (!mRedoMoves.empty())
	{
		auto move = mRedoMoves.back();
		mRedoMoves.pop_back();

		if (auto card = move.card(); card)
		{
			card->blockSignals(true);
			card->setOnAceSpot(false);
			card->setParent(move.parent(), true);
			card->blockSignals(false);

			mUndoMoves.push_back(move);
		}
	}
}

void Board::unselectCard()
{
	if (mSelectedCard)
	{
		mSelectedCard->setSelected(false);
		mSelectedCard = nullptr;
	}
}

void Board::selectCard(Card* card)
{
	if (mSelectedCard == card)
	{
		unselectCard();
	}
	else if (mSelectedCard && card->canStackCard(mSelectedCard))
	{
		if (card->isOnAceSpot())
		{
			mSelectedCard->setOnAceSpot(true);
		}
		mSelectedCard->setParent(card, true);
	}
	else
	{
		if (mSelectedCard)
			mSelectedCard->updatePosition(true);
		setSelectedCard(card);
	}
}

void Board::setSelectedCard(Card* card)
{
	if (mSelectedCard)
	{
		mSelectedCard->setSelected(false);
	}
	mSelectedCard = card;
	mSelectedCard->setSelected(true);
}

Card* Board::getSelectedCard()
{
	return mSelectedCard;
}

void Board::setRelaxed(bool value)
{
	mRelaxed = value;
}

bool Board::isRelaxed() const noexcept
{
	return mRelaxed;
}

/*!
 * \brief Start a new game
 */
void Board::newGame()
{
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);

	// finish any ongoing game
	endGame();

	// generate random game number
	std::random_device							randomDevice;
	std::uniform_int_distribution<unsigned int> gameNumberDistribution(1'000'000, 9'999'999);
	mGameNumber = gameNumberDistribution(randomDevice);

	// deal the cards
	this->dealCards(mGameNumber);

	QGuiApplication::restoreOverrideCursor();
}

/**
 * Select a specific game number to play
 */
void Board::selectGame()
{
	bool ok		= false;
	mGameNumber = QInputDialog::getInt(mBoardWidget, "Select Game #", "Game #:", mGameNumber, 1'000'000, 9'999'999, 1, &ok);
	if (ok)
	{
		QGuiApplication::setOverrideCursor(Qt::WaitCursor);
		endGame();
		this->dealCards(mGameNumber);
		QGuiApplication::restoreOverrideCursor();
	}
}

/// @brief restart the current game
void Board::restartGame()
{
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	endGame();
	this->dealCards(mGameNumber);
	QGuiApplication::restoreOverrideCursor();
}

/*!
 * \brief Triggers the end of a game
 */
void Board::endGame()
{
	this->collectCards();
	resetGameTime();
}

void Board::resetGameTime()
{
	mGameTimer->stop();
	mGameTime = 0;
	if (mTimerProxy)
		if (auto* timerLabel = dynamic_cast<TimerLabel*>(mTimerProxy->widget()); timerLabel)
			timerLabel->setTime(0);
}
