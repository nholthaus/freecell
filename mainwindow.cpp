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
#include <QApplication>
#include <QMenuBar>
#include "board.h"

/*!
 * \brief Constructor
 * \param parent The parent widget
 */
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	auto* gameMenu = new QMenu("Game");
	gameMenu->addAction("New Game", Qt::Key_F2, this, SLOT(newGame()));
	gameMenu->addSeparator();
	auto* relaxedAction = gameMenu->addAction("Relaxed", this, [this](bool value) { m_board->setRelaxed(value); });
	relaxedAction->setCheckable(true);
	gameMenu->addSeparator();
	gameMenu->addAction("E&xit", qApp, SLOT(quit()));
	menuBar()->addMenu(gameMenu);

	setWindowTitle("Freecell");
	setWindowIcon(QIcon(":/icons/freecell"));

	m_board = new Board();
	setCentralWidget(m_board->getBoardWidget());

	newGame();
}

/*!
 * \brief Start a new game
 */
void MainWindow::newGame()
{
	endGame();
	m_board->dealCards();
}

/*!
 * \brief Triggers the end of a game
 */
void MainWindow::endGame()
{
	m_board->collectCards();
}