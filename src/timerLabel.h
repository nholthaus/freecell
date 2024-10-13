//
// Created by NHolt on 10/12/2024.
//

#ifndef TIMERLABEL_H
#define TIMERLABEL_H

#include "label.h"

class TimerLabel : public Label
{
	Q_OBJECT
public:

	explicit TimerLabel(QWidget* parent = nullptr);
	void	 setTime(int seconds);
};

#endif // TIMERLABEL_H
