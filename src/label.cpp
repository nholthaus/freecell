//
// Created by NHolt on 10/12/2024.
//

#include "label.h"
#include "cardwidget.h"

Label::Label(QWidget* parent)
	: QLabel(parent)
{
	setStyleSheet("Label"
			  "{"
			  "	 color: rgba(255, 255, 255, 64);"
			  "	 background-color: rgba(0, 100, 0, 25);"
			  "	 font: 'Bookman Old Style' bold;"
			  "	 font-size: 18px;"
			  "	 border: 6px solid rgba(0, 100, 0, 255);"
			  "	 border-radius: 15px"
			  "}");
	setFixedWidth(CardWidget::WIDTH);
	setFixedHeight(46);
	setAlignment(Qt::AlignCenter);
}
