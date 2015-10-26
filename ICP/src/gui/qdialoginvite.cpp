/**
  * @file QDialogInvite.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogInvite
  */

#include "qdialoginvite.h"
#include "ui_qdialoginvite.h"

QDialogInvite::QDialogInvite(vector<Client> clients, Client* client, QWidget *parent) : QDialog(parent), ui(new Ui::QDialogInvite){
    ui->setupUi(this);
    this->setWindowTitle("Labyrinth2015 - Invite players");
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);
    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());

    if(client){
        for(vector<Client>::iterator p = clients.begin(); p != clients.end(); p++){
            if((*p).getUID() != client->getUID()){
                int uid = (*p).getUID();
                QString name = QString::fromStdString((*p).getName());

                ui->listUsers->addItem(QString(QString::number(uid)+"\t"+name));
            }
        }
    }
}

QDialogInvite::~QDialogInvite(){
    delete ui;
}

QStringList QDialogInvite::getPlayers(){
    QStringList players;

    QList<QListWidgetItem *> items = ui->listUsers->selectedItems();
    for(QList<QListWidgetItem *>::iterator p = items.begin(); p != items.end(); p++){
        QString text = (*p)->text();
        QStringList parts = text.split('\t');
        players.push_back(parts.at(0));
    }

    return players;
}

