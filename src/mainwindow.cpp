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
	gameMenu->addAction(QIcon(":/icons/freecell_ico_white"), "New Game", Qt::Key_F2, m_board, &Board::newGame);
	gameMenu->addAction("Select Game...", Qt::Key_F4, m_board, &Board::selectGame);
	gameMenu->addAction("Restart Game", Qt::Key_F5, m_board, &Board::restartGame);
	gameMenu->addSeparator();
	gameMenu->addAction(QIcon(":/icons/undo"), "Undo Last Move", QKeySequence(QKeySequence::Undo), m_board, &Board::onUndo, Qt::QueuedConnection);
	gameMenu->addAction(QIcon(":/icons/redo"),"Redo Last Move", QKeySequence(QKeySequence::Redo), m_board, &Board::onRedo, Qt::QueuedConnection);
	gameMenu->addSeparator();
	auto* relaxedAction = gameMenu->addAction("Relaxed Mode", QKeySequence(Qt::ALT | Qt::Key_R), this, [this](bool value) { m_board->setRelaxed(value); });
	relaxedAction->setCheckable(true);
	gameMenu->addSeparator();
	gameMenu->addAction("Exit", QKeySequence(QKeySequence(Qt::ALT | Qt::Key_X)), QGuiApplication::instance(), SLOT(quit()));

	auto* viewMenu		   = new QMenu("View");
	auto* fullscreenAction = viewMenu->addAction("Fullscreen", QKeySequence(QKeySequence::FullScreen), this,
												 [this] { this->isFullScreen() ? this->showNormal() : this->showFullScreen(); });
	fullscreenAction->setCheckable(true);

	menuBar()->addMenu(gameMenu);
	menuBar()->addMenu(viewMenu);

	m_board->newGame();
}