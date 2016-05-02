/********************************************************************************
** Form generated from reading UI file 'qdebuglog.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDEBUGLOG_H
#define UI_QDEBUGLOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_QDebugLog
{
public:
    QTextBrowser *log;

    void setupUi(QDialog *QDebugLog)
    {
        if (QDebugLog->objectName().isEmpty())
            QDebugLog->setObjectName(QString::fromUtf8("QDebugLog"));
        QDebugLog->resize(600, 350);
        log = new QTextBrowser(QDebugLog);
        log->setObjectName(QString::fromUtf8("log"));
        log->setGeometry(QRect(10, 10, 580, 330));
        log->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        retranslateUi(QDebugLog);

        QMetaObject::connectSlotsByName(QDebugLog);
    } // setupUi

    void retranslateUi(QDialog *QDebugLog)
    {
        QDebugLog->setWindowTitle(QApplication::translate("QDebugLog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDebugLog: public Ui_QDebugLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDEBUGLOG_H
