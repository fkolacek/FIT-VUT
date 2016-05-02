/********************************************************************************
** Form generated from reading UI file 'untitled.ui'
**
** Created: Thu Oct 3 10:07:33 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNTITLED_H
#define UI_UNTITLED_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "mywidget.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLineEdit *txt_op1;
    QLineEdit *txt_op2;
    QLineEdit *txt_op3;
    QLineEdit *txt_result;
    QPushButton *but_calc;
    MyWidget *widget;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(218, 249);
        txt_op1 = new QLineEdit(Form);
        txt_op1->setObjectName(QString::fromUtf8("txt_op1"));
        txt_op1->setGeometry(QRect(10, 10, 201, 25));
        txt_op2 = new QLineEdit(Form);
        txt_op2->setObjectName(QString::fromUtf8("txt_op2"));
        txt_op2->setGeometry(QRect(10, 40, 201, 25));
        txt_op3 = new QLineEdit(Form);
        txt_op3->setObjectName(QString::fromUtf8("txt_op3"));
        txt_op3->setGeometry(QRect(10, 70, 201, 25));
        txt_result = new QLineEdit(Form);
        txt_result->setObjectName(QString::fromUtf8("txt_result"));
        txt_result->setGeometry(QRect(10, 100, 201, 25));
        but_calc = new QPushButton(Form);
        but_calc->setObjectName(QString::fromUtf8("but_calc"));
        but_calc->setGeometry(QRect(10, 130, 201, 27));
        widget = new MyWidget(Form);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 160, 201, 80));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        but_calc->setText(QApplication::translate("Form", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNTITLED_H
