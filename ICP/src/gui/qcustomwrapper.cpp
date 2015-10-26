/**
  * @file qcustomwrapper.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QCustomWrapper
  */

#include "qcustomwrapper.h"

QCustomWrapper::QCustomWrapper(QWidget *parent) : QWidget(parent){
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(Qt::white));
    this->desk = NULL;
    this->p = parent;

    QFont font;
    font.setFamily("Waree");
    font.setPointSize(13);
    font.setBold(true);
    font.setWeight(70);

    this->mainLabel = new QLabel(this);
    this->mainLabel->setGeometry(10, 10, 620, 400);
    this->mainLabel->setText("Please connect to remote server first.");
    this->mainLabel->setFont(font);
    this->mainLabel->setAlignment(Qt::AlignCenter);
    this->mainLabel->show();

}

QDesk* QCustomWrapper::getDesk(){
    return this->desk;
}

void QCustomWrapper::setDesk(int deskSize, Game* game, Client* client){
    this->clearDesk();
    this->mainLabel->hide();
    this->desk = new QDesk(deskSize, game, client, this);
    this->resize(this->desk->size().width(), this->desk->size().height());
    this->setMaximumSize(this->desk->size().width(), this->desk->size().height());
    this->setMinimumSize(this->desk->size().width(), this->desk->size().height());
    this->desk->show();

    connect(this->desk, SIGNAL(fireMove(int,int,int)), this->p, SLOT(handleMove(int, int, int)));
    connect(this->desk, SIGNAL(firePut(int,int,stoneAngle)), this->p, SLOT(handlePut(int, int, stoneAngle)));
}

void QCustomWrapper::clearDesk(){
    if(this->desk){
        this->desk->hide();
        delete this->desk;
    }

    this->desk = NULL;

    this->mainLabel->show();
}

void QCustomWrapper::showLabel(bool show){
    if(show)
        this->mainLabel->show();
    else
        this->mainLabel->hide();
}
