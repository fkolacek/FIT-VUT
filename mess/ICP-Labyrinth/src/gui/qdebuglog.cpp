/**
  * @file qdebuglog.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDebugLog
  */

#include "qdebuglog.h"
#include "ui_qdebuglog.h"

QDebugLog::QDebugLog(QWidget *parent) : QDialog(parent),ui(new Ui::QDebugLog){
    ui->setupUi(this);

    this->setAutoFillBackground(true);
    this->setPalette(QPalette(Qt::white));
    this->setWindowTitle("Labyrinth2015 - Log");
    this->resize(600, 350);
    this->setMaximumSize(600, 350);
    this->setMinimumSize(600,350);
}

QDebugLog::~QDebugLog(){
    delete ui;
}

void QDebugLog::receiveMessage(QString message){
    ui->log->append(message);
}
