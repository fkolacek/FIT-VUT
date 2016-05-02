/********************************************************************************
** Form generated from reading UI file 'qdesk.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDESK_H
#define UI_QDESK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QDesk
{
public:

    void setupUi(QWidget *QDesk)
    {
        if (QDesk->objectName().isEmpty())
            QDesk->setObjectName(QString::fromUtf8("QDesk"));
        QDesk->resize(643, 554);
        QDesk->setAutoFillBackground(true);

        retranslateUi(QDesk);

        QMetaObject::connectSlotsByName(QDesk);
    } // setupUi

    void retranslateUi(QWidget *QDesk)
    {
        QDesk->setWindowTitle(QApplication::translate("QDesk", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDesk: public Ui_QDesk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDESK_H
