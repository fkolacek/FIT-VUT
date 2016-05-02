/********************************************************************************
** Form generated from reading UI file 'qmain.ui'
**
** Created: Sun May 10 17:58:13 2015
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMAIN_H
#define UI_QMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMain
{
public:
    QAction *action_About;
    QAction *actionInvite_players;
    QAction *actionNew_game;
    QAction *actionLoad_game;
    QAction *actionSave_game;
    QAction *actionExit;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionDebug;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_Menu;
    QMenu *menu_Help;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QMain)
    {
        if (QMain->objectName().isEmpty())
            QMain->setObjectName(QString::fromUtf8("QMain"));
        QMain->resize(640, 480);
        QMain->setMinimumSize(QSize(640, 480));
        QMain->setMaximumSize(QSize(640, 480));
        action_About = new QAction(QMain);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/gfx/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_About->setIcon(icon);
        actionInvite_players = new QAction(QMain);
        actionInvite_players->setObjectName(QString::fromUtf8("actionInvite_players"));
        actionInvite_players->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/gfx/invite.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInvite_players->setIcon(icon1);
        actionNew_game = new QAction(QMain);
        actionNew_game->setObjectName(QString::fromUtf8("actionNew_game"));
        actionNew_game->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/gfx/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_game->setIcon(icon2);
        actionLoad_game = new QAction(QMain);
        actionLoad_game->setObjectName(QString::fromUtf8("actionLoad_game"));
        actionLoad_game->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/gfx/load.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_game->setIcon(icon3);
        actionSave_game = new QAction(QMain);
        actionSave_game->setObjectName(QString::fromUtf8("actionSave_game"));
        actionSave_game->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/gfx/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_game->setIcon(icon4);
        actionExit = new QAction(QMain);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionConnect = new QAction(QMain);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/gfx/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon5);
        actionDisconnect = new QAction(QMain);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionDisconnect->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/gfx/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon6);
        actionDebug = new QAction(QMain);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        centralWidget = new QWidget(QMain);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QMain->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QMain);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 19));
        menu_Menu = new QMenu(menuBar);
        menu_Menu->setObjectName(QString::fromUtf8("menu_Menu"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        QMain->setMenuBar(menuBar);
        statusBar = new QStatusBar(QMain);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QMain->setStatusBar(statusBar);
        toolBar = new QToolBar(QMain);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        QMain->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menu_Menu->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_Menu->addAction(actionConnect);
        menu_Menu->addAction(actionDisconnect);
        menu_Menu->addSeparator();
        menu_Menu->addAction(actionInvite_players);
        menu_Menu->addAction(actionNew_game);
        menu_Menu->addAction(actionLoad_game);
        menu_Menu->addAction(actionSave_game);
        menu_Menu->addSeparator();
        menu_Menu->addAction(actionExit);
        menu_Help->addAction(action_About);
        menu_Help->addSeparator();
        menu_Help->addAction(actionDebug);
        toolBar->addAction(actionConnect);
        toolBar->addAction(actionDisconnect);
        toolBar->addSeparator();
        toolBar->addAction(actionInvite_players);
        toolBar->addSeparator();
        toolBar->addAction(actionNew_game);
        toolBar->addAction(actionLoad_game);
        toolBar->addAction(actionSave_game);
        toolBar->addSeparator();
        toolBar->addAction(action_About);
        toolBar->addAction(actionExit);

        retranslateUi(QMain);
        QObject::connect(actionExit, SIGNAL(triggered()), QMain, SLOT(close()));
        QObject::connect(action_About, SIGNAL(triggered()), QMain, SLOT(handleShowHelp()));
        QObject::connect(actionInvite_players, SIGNAL(triggered()), QMain, SLOT(handleShowInvite()));
        QObject::connect(actionLoad_game, SIGNAL(triggered()), QMain, SLOT(handleShowLoadGame()));
        QObject::connect(actionSave_game, SIGNAL(triggered()), QMain, SLOT(handleShowSaveGame()));
        QObject::connect(actionNew_game, SIGNAL(triggered()), QMain, SLOT(handleShowNewGame()));
        QObject::connect(actionConnect, SIGNAL(triggered()), QMain, SLOT(handleShowConnect()));
        QObject::connect(actionDisconnect, SIGNAL(triggered()), QMain, SLOT(handleDisconnect()));
        QObject::connect(actionDebug, SIGNAL(triggered()), QMain, SLOT(debugToggle()));

        QMetaObject::connectSlotsByName(QMain);
    } // setupUi

    void retranslateUi(QMainWindow *QMain)
    {
        QMain->setWindowTitle(QApplication::translate("QMain", "Labyrinth2015", 0, QApplication::UnicodeUTF8));
        action_About->setText(QApplication::translate("QMain", "&About", 0, QApplication::UnicodeUTF8));
        actionInvite_players->setText(QApplication::translate("QMain", "Invite players", 0, QApplication::UnicodeUTF8));
        actionNew_game->setText(QApplication::translate("QMain", "New game", 0, QApplication::UnicodeUTF8));
        actionLoad_game->setText(QApplication::translate("QMain", "Load game", 0, QApplication::UnicodeUTF8));
        actionSave_game->setText(QApplication::translate("QMain", "Save game", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("QMain", "&Exit", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("QMain", "&Connect", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("QMain", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionDebug->setText(QApplication::translate("QMain", "Debug", 0, QApplication::UnicodeUTF8));
        menu_Menu->setTitle(QApplication::translate("QMain", "&Menu", 0, QApplication::UnicodeUTF8));
        menu_Help->setTitle(QApplication::translate("QMain", "&Help", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("QMain", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QMain: public Ui_QMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMAIN_H
