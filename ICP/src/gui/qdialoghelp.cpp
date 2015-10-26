/**
  * @file qdialoghelp.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogHelp
  */

#include "qdialoghelp.h"
#include "ui_qdialoghelp.h"

QDialogHelp::QDialogHelp(QWidget *parent) : QDialog(parent), ui(new Ui::QDialogHelp){
    ui->setupUi(this);

    this->setWindowTitle("Labyrinth2015 - About");
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);
    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());
}

QDialogHelp::~QDialogHelp(){
    delete ui;
}
