#include "MainForm.h"

MainForm::MainForm(QWidget * parent, Qt::WindowFlags f) : QWidget ( parent,f ) {
  ui.setupUi(this);

  connect(ui.but_calc, SIGNAL(pressed()), this, SLOT(buttonPressed()));
  connect(this, SIGNAL(haveResult(QString)), ui.txt_result, SLOT(setText(QString)));
  connect(this, SIGNAL(setState(bool)), ui.widget, SLOT(newState(bool)));

  this->validator = new QIntValidator(this);

  ui.txt_op1->setValidator(this->validator);
  ui.txt_op3->setValidator(this->validator);

  ui.txt_result->setReadOnly(true);
}

MainForm::~MainForm(){
    delete this->validator;
}

void MainForm::buttonPressed(){
    int op1 = ui.txt_op1->text().toInt();
    QString op2 = ui.txt_op2->text();
    int op3 = ui.txt_op3->text().toInt();

    emit setState(true);

    if(op2 == "+"){
        emit haveResult(QString::number(op1 + op3));
    }
    else if(op2 == "-"){
        emit haveResult(QString::number(op1 - op3));
    }
    else if(op2 == "*"){
        emit haveResult(QString::number(op1 * op3));
    }
    else if(op2 == "/"){
        if(op3 == 0){
            emit haveResult("Division by zero");
            emit setState(false);
        }
        else
            emit haveResult(QString::number(op1 / op3));
    }
    else{
        emit haveResult("Invalid operation");
        emit setState(false);
    }
}
