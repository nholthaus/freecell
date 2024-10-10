//
// Created by NHolt on 10/10/2024.
//

#include "Button.h"

#include <QIcon>

Button::Button(QWidget* parent)
	: QPushButton(parent)
{
	setStyleSheet("Button"
				  "{"
				  "	 color: rgba(255, 255, 255, 64);"
				  "	 background-color: rgba(0, 100, 0, 25);"
				  "	 font: 'Bookman Old Style' bold;"
				  "	 font-size: 24px;"
				  "	 border: 6px solid rgba(0, 100, 0, 255);"
				  "	 border-radius: 15px"
				  "}"
				  "Button:hover"
				  "{"
				  "  background-color: rgba(0, 100, 0, 128);"
				  "}");
	setFixedWidth(CardWidget::WIDTH);
	setFixedHeight(46);
	setIconSize({32, 32});
}
