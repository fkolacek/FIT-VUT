/**
  * @file qcustombutton.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QCustomButton
  */

#include "qcustombutton.h"

QCustomButton::QCustomButton(int x, int y, QWidget* parent) : QPushButton(parent){
    this->x = x;
    this->y = y;

    this->resize(50, 50);
    this->setAutoFillBackground(true);
    this->setMaximumSize(50, 50);
    this->setMinimumSize(50, 50);
    //this->setPalette(QPalette(Qt::black));

    QPixmap pix(":/gfx/cross.png");
    QIcon icon(pix);
    this->setIcon(icon);
    this->setIconSize(pix.size());

    connect(this, SIGNAL(clicked(bool)), this, SLOT(handleClicked(bool)));
}

void QCustomButton::handleClicked(bool checked){
    emit opPut(this->x, this->y);
}

int QCustomButton::getX(){
    return this->x;
}

int QCustomButton::getY(){
    return this->y;
}

