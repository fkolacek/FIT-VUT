/**
  * @file qdialoggames.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogGames
  */

#include "qdialoggames.h"
#include "ui_qdialoggames.h"

QDialogGames::QDialogGames(vector<string> games, QWidget *parent) : QDialog(parent), ui(new Ui::QDialogGames){
    ui->setupUi(this);

    this->setWindowTitle("Labyrinth2015 - Load game");
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);
    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());

    for(vector<string>::iterator p = games.begin(); p != games.end(); p++){
        QString name = QString::fromStdString((*p));
        ui->listGames->addItem(name);
    }
}

QDialogGames::~QDialogGames(){
    delete ui;
}

QList<QListWidgetItem*> QDialogGames::getGames(){
    return ui->listGames->selectedItems();

}
