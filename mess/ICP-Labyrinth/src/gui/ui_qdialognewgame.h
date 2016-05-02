/********************************************************************************
** Form generated from reading UI file 'qdialognewgame.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGNEWGAME_H
#define UI_QDIALOGNEWGAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_QDialogNewGame
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *spinPlayers;
    QSpinBox *spinCards;
    QSpinBox *spinSize;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *QDialogNewGame)
    {
        if (QDialogNewGame->objectName().isEmpty())
            QDialogNewGame->setObjectName(QString::fromUtf8("QDialogNewGame"));
        QDialogNewGame->resize(190, 142);
        QDialogNewGame->setMinimumSize(QSize(190, 142));
        QDialogNewGame->setMaximumSize(QSize(190, 142));
        label = new QLabel(QDialogNewGame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 80, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("Waree"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(QDialogNewGame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 80, 20));
        label_2->setFont(font);
        label_3 = new QLabel(QDialogNewGame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 70, 80, 20));
        label_3->setFont(font);
        spinPlayers = new QSpinBox(QDialogNewGame);
        spinPlayers->setObjectName(QString::fromUtf8("spinPlayers"));
        spinPlayers->setGeometry(QRect(120, 10, 60, 23));
        spinPlayers->setMinimum(2);
        spinPlayers->setMaximum(4);
        spinPlayers->setValue(2);
        spinCards = new QSpinBox(QDialogNewGame);
        spinCards->setObjectName(QString::fromUtf8("spinCards"));
        spinCards->setGeometry(QRect(120, 40, 60, 23));
        spinCards->setReadOnly(false);
        spinCards->setMinimum(12);
        spinCards->setMaximum(24);
        spinCards->setSingleStep(12);
        spinCards->setValue(24);
        spinSize = new QSpinBox(QDialogNewGame);
        spinSize->setObjectName(QString::fromUtf8("spinSize"));
        spinSize->setGeometry(QRect(120, 70, 60, 23));
        spinSize->setMinimum(5);
        spinSize->setMaximum(11);
        spinSize->setSingleStep(2);
        spinSize->setValue(7);
        pushButton = new QPushButton(QDialogNewGame);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(100, 110, 81, 22));
        pushButton_2 = new QPushButton(QDialogNewGame);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 110, 81, 22));
        QWidget::setTabOrder(pushButton, pushButton_2);
        QWidget::setTabOrder(pushButton_2, spinPlayers);
        QWidget::setTabOrder(spinPlayers, spinCards);
        QWidget::setTabOrder(spinCards, spinSize);

        retranslateUi(QDialogNewGame);
        QObject::connect(pushButton_2, SIGNAL(clicked()), QDialogNewGame, SLOT(reject()));
        QObject::connect(pushButton, SIGNAL(clicked()), QDialogNewGame, SLOT(accept()));

        QMetaObject::connectSlotsByName(QDialogNewGame);
    } // setupUi

    void retranslateUi(QDialog *QDialogNewGame)
    {
        QDialogNewGame->setWindowTitle(QApplication::translate("QDialogNewGame", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QDialogNewGame", "Players", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QDialogNewGame", "Cards", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QDialogNewGame", "Size", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("QDialogNewGame", "Create", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("QDialogNewGame", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QDialogNewGame: public Ui_QDialogNewGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGNEWGAME_H
