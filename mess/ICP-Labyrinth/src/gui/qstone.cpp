/**
  * @file qstone.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QStone
  */

#include "qstone.h"

QString FileMaps[STONE_TYPE_END][STONE_ANGLE_END] = {
    [STONE_L] = {
        [STONE_UP]      = QString(":gfx/stone_l_up.png"),
        [STONE_LEFT]    = QString(":gfx/stone_l_left.png"),
        [STONE_RIGHT]   = QString(":gfx/stone_l_right.png"),
        [STONE_DOWN]    = QString(":gfx/stone_l_down.png")
    },
    [STONE_T] = {
        [STONE_UP]      = QString(":gfx/stone_t_up.png"),
        [STONE_LEFT]    = QString(":gfx/stone_t_left.png"),
        [STONE_RIGHT]   = QString(":gfx/stone_t_right.png"),
        [STONE_DOWN]    = QString(":gfx/stone_t_down.png")
    },
    [STONE_I] = {
        [STONE_UP]      = QString(":gfx/stone_i_up.png"),
        [STONE_LEFT]    = QString(":gfx/stone_i_left.png"),
        [STONE_RIGHT]   = QString(":gfx/stone_i_right.png"),
        [STONE_DOWN]    = QString(":gfx/stone_i_down.png")
    },
    [STONE_NONE] = {
        [STONE_UP]      = QString(":gfx/stone_none.png"),
        [STONE_LEFT]    = QString(":gfx/stone_none.png"),
        [STONE_RIGHT]   = QString(":gfx/stone_none.png"),
        [STONE_DOWN]    = QString(":gfx/stone_none.png")
    }
};

QStone::QStone(int x, int y, stoneType type, stoneAngle angle, stoneItem item, QWidget *parent) : QPushButton(parent){
    this->x = x;
    this->y = y;

    this->resize(50, 50);
    this->setAutoFillBackground(true);
    this->setMaximumSize(50, 50);
    this->setMinimumSize(50, 50);

    this->refresh(type, angle, item);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(handleClicked(bool)));
}

void QStone::refresh(stoneType type, stoneAngle angle, stoneItem item){
    //QPixmap pix(FileMaps[type][angle]);
    //QIcon icon(pix);
    //this->setIcon(icon);
    //this->setIconSize(pix.size());

    QString fileName = FileMaps[type][angle];

    this->setStyleSheet("background-image: url("+fileName+");"
                        "background-repeat: no-repeat;"
                        "background-position: center center;"
                        "border: 0;"
                        "color: #FFFFFF;"
                        "font-weight: bold;"
                        "font-size: 20px;");

    if(item != STONE_ITEM_NONE){
        this->setText(QString((int)item + 'A'));
    }
    else
        this->setText("");
}

void QStone::handleClicked(bool){
    emit opMove(this->x, this->y);
}
