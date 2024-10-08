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

#include "mainwindow.h"
#include "board.h"

#include <QApplication>
#include <QInputDialog>
#include <QMenuBar>
#include <QStyle>
#include <QToolTip>
#include <random>

/*!
 * \brief Constructor
 * \param parent The parent widget
 */
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{

	setWindowTitle("Freecell");
	setWindowIcon(QIcon(":/icons/freecell"));

	m_board = new Board();
	setCentralWidget(m_board->getBoardWidget());

	auto* gameMenu = new QMenu("Game");
	gameMenu->addAction(QIcon(":/icons/freecell_ico_white"), "New Game", Qt::Key_F2, this, SLOT(newGame()));
	gameMenu->addAction("Select Game...", Qt::Key_F4, this, SLOT(selectGame()));
	gameMenu->addAction("Restart Game", Qt::Key_F5, this, SLOT(restartGame()));
	gameMenu->addSeparator();
	gameMenu->addAction(QIcon(":/icons/undo"), "Undo Last Move", QKeySequence(QKeySequence::Undo), m_board, &Board::onUndo, Qt::QueuedConnection);
	gameMenu->addAction(QIcon(":/icons/redo"),"Redo Last Move", QKeySequence(QKeySequence::Redo), m_board, &Board::onRedo, Qt::QueuedConnection);
	gameMenu->addSeparator();
	auto* relaxedAction = gameMenu->addAction("Relaxed Mode", QKeySequence(Qt::ALT | Qt::Key_R), this, [this](bool value) { m_board->setRelaxed(value); });
	relaxedAction->setCheckable(true);
	gameMenu->addSeparator();
	gameMenu->addAction("Exit", QKeySequence(QKeySequence(Qt::ALT | Qt::Key_X)), qApp, SLOT(quit()));

	auto* viewMenu		   = new QMenu("View");
	auto* fullscreenAction = viewMenu->addAction("Fullscreen", QKeySequence(QKeySequence::FullScreen), this,
												 [this] { this->isFullScreen() ? this->showNormal() : this->showFullScreen(); });
	fullscreenAction->setCheckable(true);

	menuBar()->addMenu(gameMenu);
	menuBar()->addMenu(viewMenu);

	newGame();
}

/*!
 * \brief Start a new game
 */
void MainWindow::newGame()
{
	// finish any ongoing game
	endGame();

	// generate random game number
	std::random_device							randomDevice;
	std::uniform_int_distribution<unsigned int> gameNumberDistribution(1'000'000, 9'999'999);
	m_gameNumber = gameNumberDistribution(randomDevice);

	// deal the cards
	m_board->dealCards(m_gameNumber);
}

void MainWindow::selectGame()
{
	bool ok		 = false;
	m_gameNumber = QInputDialog::getInt(this, "Select Game #", "Game #:", m_gameNumber, 1'000'000, 9'999'999, 1, &ok);
	if (ok)
	{
		endGame();
		m_board->dealCards(m_gameNumber);
	}
}

/// @brief restart the current game
void MainWindow::restartGame()
{
	endGame();
	m_board->dealCards(m_gameNumber);
}

/*!
 * \brief Triggers the end of a game
 */
void MainWindow::endGame()
{
	m_board->collectCards();
}
