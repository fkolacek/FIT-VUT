/********************************************************************************
** Form generated from reading UI file 'qdialogconnect.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGCONNECT_H
#define UI_QDIALOGCONNECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dialogConnect
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *txtHostname;
    QLineEdit *txtPort;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *dialogConnect)
    {
        if (dialogConnect->objectName().isEmpty())
            dialogConnect->setObjectName(QString::fromUtf8("dialogConnect"));
        dialogConnect->resize(240, 116);
        label = new QLabel(dialogConnect);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 80, 22));
        label_2 = new QLabel(dialogConnect);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 80, 22));
        txtHostname = new QLineEdit(dialogConnect);
        txtHostname->setObjectName(QString::fromUtf8("txtHostname"));
        txtHostname->setGeometry(QRect(90, 10, 140, 22));
        txtPort = new QLineEdit(dialogConnect);
        txtPort->setObjectName(QString::fromUtf8("txtPort"));
        txtPort->setGeometry(QRect(90, 40, 140, 22));
        pushButton = new QPushButton(dialogConnect);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(60, 80, 80, 22));
        pushButton_2 = new QPushButton(dialogConnect);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(150, 80, 80, 22));
        QWidget::setTabOrder(pushButton_2, pushButton);
        QWidget::setTabOrder(pushButton, txtHostname);
        QWidget::setTabOrder(txtHostname, txtPort);

        retranslateUi(dialogConnect);
        QObject::connect(pushButton_2, SIGNAL(clicked()), dialogConnect, SLOT(accept()));
        QObject::connect(pushButton, SIGNAL(clicked()), dialogConnect, SLOT(reject()));

        QMetaObject::connectSlotsByName(dialogConnect);
    } // setupUi

    void retranslateUi(QDialog *dialogConnect)
    {
        dialogConnect->setWindowTitle(QApplication::translate("dialogConnect", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dialogConnect", "Hostname", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("dialogConnect", "Port", 0, QApplication::UnicodeUTF8));
        txtHostname->setText(QString());
        pushButton->setText(QApplication::translate("dialogConnect", "Storno", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("dialogConnect", "Connect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dialogConnect: public Ui_dialogConnect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGCONNECT_H
