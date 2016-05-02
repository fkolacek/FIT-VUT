/********************************************************************************
** Form generated from reading UI file 'qdialoggames.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGGAMES_H
#define UI_QDIALOGGAMES_H

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

class Ui_QDialogGames
{
public:
    QListWidget *listGames;
    QLabel *label;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *QDialogGames)
    {
        if (QDialogGames->objectName().isEmpty())
            QDialogGames->setObjectName(QString::fromUtf8("QDialogGames"));
        QDialogGames->resize(200, 280);
        listGames = new QListWidget(QDialogGames);
        listGames->setObjectName(QString::fromUtf8("listGames"));
        listGames->setGeometry(QRect(10, 40, 180, 200));
        listGames->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        label = new QLabel(QDialogGames);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 180, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("Waree"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        btnOk = new QPushButton(QDialogGames);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setGeometry(QRect(110, 250, 80, 22));
        btnCancel = new QPushButton(QDialogGames);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(10, 250, 80, 22));
        QWidget::setTabOrder(btnOk, btnCancel);
        QWidget::setTabOrder(btnCancel, listGames);

        retranslateUi(QDialogGames);
        QObject::connect(btnCancel, SIGNAL(clicked()), QDialogGames, SLOT(reject()));
        QObject::connect(btnOk, SIGNAL(clicked()), QDialogGames, SLOT(accept()));

        QMetaObject::connectSlotsByName(QDialogGames);
    } // setupUi

    void retranslateUi(QDialog *QDialogGames)
    {
        QDialogGames->setWindowTitle(QApplication::translate("QDialogGames", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QDialogGames", "Select game", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("QDialogGames", "Load", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("QDialogGames", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDialogGames: public Ui_QDialogGames {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGGAMES_H
