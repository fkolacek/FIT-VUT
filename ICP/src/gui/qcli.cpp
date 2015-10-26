/**
  * @file qcli.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QCli
  */

#include "qcli.h"

QCLI::QCLI(QString hostname, int port, QWidget* mother) : QThread(){
    this->mother = mother;
    this->client = NULL;
    this->hostname = hostname;
    this->port = port;
}

void QCLI::run(){
    try{
        this->client = new CLI(false, 2048);
        this->client->connectToHost(hostname.toStdString(), port);

    }
    catch(Exception* e){
        this->client = NULL;
        emit sendMessage(QString::fromStdString(e->getMessage()));
        delete e;
    }
}

CLI* QCLI::getClient(){
    return this->client;
}

void QCLI::receiveMessage(QString message){
    if(this->client){
        this->client->addMessage(message.toStdString());
    }
}
