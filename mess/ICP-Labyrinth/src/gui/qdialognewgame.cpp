/**
  * @file qdialognewgame.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogNewGame
  */

#include "qdialognewgame.h"
#include "ui_qdialognewgame.h"

QDialogNewGame::QDialogNewGame(QWidget *parent) : QDialog(parent), ui(new Ui::QDialogNewGame){
    ui->setupUi(this);

    this->setWindowTitle("Labyrinth2015 - New Game");
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);
    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());
}

QDialogNewGame::~QDialogNewGame(){
    delete ui;
}

int QDialogNewGame::getPlayers(){
    return ui->spinPlayers->value();
}

int QDialogNewGame::getCards(){
    return ui->spinCards->value();
}

int QDialogNewGame::getSize(){
    return ui->spinSize->value();
}
