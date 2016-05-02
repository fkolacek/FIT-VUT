/********************************************************************************
** Form generated from reading UI file 'qdialoghelp.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGHELP_H
#define UI_QDIALOGHELP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QDialogHelp
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QDialog *QDialogHelp)
    {
        if (QDialogHelp->objectName().isEmpty())
            QDialogHelp->setObjectName(QString::fromUtf8("QDialogHelp"));
        QDialogHelp->resize(383, 213);
        label = new QLabel(QDialogHelp);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 350, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Waree"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(QDialogHelp);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 350, 51));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_2->setWordWrap(true);
        label_3 = new QLabel(QDialogHelp);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 120, 350, 51));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_3->setWordWrap(true);
        pushButton = new QPushButton(QDialogHelp);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(290, 180, 81, 22));

        retranslateUi(QDialogHelp);
        QObject::connect(pushButton, SIGNAL(pressed()), QDialogHelp, SLOT(accept()));

        QMetaObject::connectSlotsByName(QDialogHelp);
    } // setupUi

    void retranslateUi(QDialog *QDialogHelp)
    {
        QDialogHelp->setWindowTitle(QApplication::translate("QDialogHelp", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QDialogHelp", "Labyrinth2015 v0.91", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QDialogHelp", "This is the implementation of desk game Labyrinth. This program is developed in C++ programing language using Qt toolkit for creating GUI.", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QDialogHelp", "Author: Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz> Date: 7.5.2015", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("QDialogHelp", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDialogHelp: public Ui_QDialogHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGHELP_H
