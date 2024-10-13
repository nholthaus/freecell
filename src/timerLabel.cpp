//
// Created by NHolt on 10/12/2024.
//

#include "timerLabel.h"

#include <QTime>

TimerLabel::TimerLabel(QWidget* parent)
	: Label(parent)
{
	this->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
}

void TimerLabel::setTime(const int seconds)
{
	QString prefix("Time: ");
	QString time = QTime(0,0).addSecs(seconds).toString("mm:ss");
	this->setText(prefix.append(time));
}
