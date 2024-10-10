//
// Created by NHolt on 10/10/2024.
//

#ifndef BUTTON_H
#define BUTTON_H

#include "cardwidget.h"
#include <QPushButton>


class Button : public QPushButton
{
	Q_OBJECT

public:
	explicit Button(QWidget *parent = nullptr);
};

#endif // BUTTON_H
