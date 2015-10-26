/**
  * @file qdialogconnect.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogConnect
  */

#include "qdialogconnect.h"
#include "ui_qdialogconnect.h"

dialogConnect::dialogConnect(QWidget *parent) : QDialog(parent), ui(new Ui::dialogConnect){
    ui->setupUi(this);

    this->setWindowTitle("Labyrinth2015 - Connect to remote server");
    ui->txtHostname->setText("localhost");
    ui->txtPort->setText("32890");
    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(Qt::white));
}

dialogConnect::~dialogConnect()
{
    delete ui;
}

QString dialogConnect::getHostname(){
    return ui->txtHostname->text();
}

int dialogConnect::getPort(){
    return ui->txtPort->text().toInt();
}
