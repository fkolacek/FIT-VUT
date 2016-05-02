/**
  * @file qstone.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QStone
  */

#ifndef QSTONE_H
#define QSTONE_H

#include <QPushButton>
#include <QPixmap>
#include <QIcon>
#include "../cli/game.h"

/**
  * @brief Maps for drawing stones in GUI
  */
extern QString FileMaps[STONE_TYPE_END][STONE_ANGLE_END];

/**
  * @brief Class represents Stone
  */
class QStone : public QPushButton{
    Q_OBJECT

    public:
        QStone(int, int, stoneType = STONE_NONE, stoneAngle = STONE_UP, stoneItem = STONE_ITEM_NONE, QWidget *parent = 0);
        void refresh(stoneType, stoneAngle, stoneItem);

    public slots:
        void handleClicked(bool);
    signals:
        void opMove(int, int);

    private:
        int x;
        int y;
};

#endif // QSTONE_H
