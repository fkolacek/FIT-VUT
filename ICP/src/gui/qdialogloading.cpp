/**
  * @file qdialogloading.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDialogLoading
  */

#include "qdialogloading.h"
#include "ui_qdialogloading.h"

QDialogLoading::QDialogLoading(QWidget *parent) : QDialog(parent), ui(new Ui::QDialogLoading){
    ui->setupUi(this);

    this->move(parent->window()->frameGeometry().topLeft() + parent->window()->rect().center() - rect().center());
}

QDialogLoading::~QDialogLoading(){
    delete ui;
}
