/********************************************************************************
** Form generated from reading UI file 'qdialoginvite.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGINVITE_H
#define UI_QDIALOGINVITE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QDialogInvite
{
public:
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QListWidget *listUsers;
    QLabel *label;

    void setupUi(QDialog *QDialogInvite)
    {
        if (QDialogInvite->objectName().isEmpty())
            QDialogInvite->setObjectName(QString::fromUtf8("QDialogInvite"));
        QDialogInvite->resize(200, 280);
        btnOk = new QPushButton(QDialogInvite);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setGeometry(QRect(110, 250, 80, 22));
        btnCancel = new QPushButton(QDialogInvite);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(10, 250, 80, 22));
        listUsers = new QListWidget(QDialogInvite);
        listUsers->setObjectName(QString::fromUtf8("listUsers"));
        listUsers->setGeometry(QRect(10, 40, 180, 200));
        listUsers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listUsers->setSelectionMode(QAbstractItemView::MultiSelection);
        listUsers->setSelectionBehavior(QAbstractItemView::SelectItems);
        label = new QLabel(QDialogInvite);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 180, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("Waree"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        retranslateUi(QDialogInvite);
        QObject::connect(btnCancel, SIGNAL(clicked()), QDialogInvite, SLOT(reject()));
        QObject::connect(btnOk, SIGNAL(clicked()), QDialogInvite, SLOT(accept()));

        QMetaObject::connectSlotsByName(QDialogInvite);
    } // setupUi

    void retranslateUi(QDialog *QDialogInvite)
    {
        QDialogInvite->setWindowTitle(QApplication::translate("QDialogInvite", "Dialog", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("QDialogInvite", "Invite", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("QDialogInvite", "Cancel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QDialogInvite", "Invite players", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDialogInvite: public Ui_QDialogInvite {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGINVITE_H
