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

#include "cardwidget.h"
#include <QVBoxLayout>

CardWidget::CardWidget(QWidget *parent) :
    QFrame(parent)
{
    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);

    mColorLabel = new QLabel(this);
    mColorLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    hLayout->addWidget(mColorLabel);

    mLabel = new QLabel(this);
    hLayout->addWidget(mLabel);

    auto *vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(10, 10, 10, 10);

    mBigColorLabel = new QLabel(this);
    mBigColorLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    vLayout->addWidget(mBigColorLabel, 1);

    setLayout(vLayout);
    resize(WIDTH, HEIGHT);
}

void CardWidget::setText(const QString& text)
{
    mLabel->setText(text);
    mLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mLabel->setStyleSheet("color:black;font-family: 'Times New Roman'; font-size: 16px;");
    mLabel->setWordWrap(true);
}

void CardWidget::setColor(CardColor color)
{
    QString resource;
    switch (color) {
    case HEARTS:
        resource = "hearts";
        break;
    case DIAMONDS:
        resource = "diamonds";
        break;
    case SPADES:
        resource = "spades";
        break;
    case CLUBS:
        resource = "clubs";
        break;
    case LASTCOLOR:
      break;
    }

    QPixmap pix = QPixmap(":/icons/" + resource).scaledToWidth(WIDTH / 8, Qt::SmoothTransformation);
    mColorLabel->setPixmap(pix);
    pix = QPixmap(":/icons/" + resource).scaledToWidth(WIDTH * 3 / 4, Qt::SmoothTransformation);
    mBigColorLabel->setPixmap(pix);

}