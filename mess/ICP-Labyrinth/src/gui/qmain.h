/**
  * @file qmain.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for class QMain
  */

#ifndef QMAIN_H
#define QMAIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include "qdebuglog.h"
#include "qcustomwrapper.h"
#include "qdialogconnect.h"
#include "qdialogloading.h"
#include "qdialoghelp.h"
#include "qdialoginvite.h"
#include "qdialognewgame.h"
#include "qdialoggames.h"
#include "qcli.h"


namespace Ui {
class QMain;
}

/**
  * @brief Class QMain - MainWindow
  */
class QMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QMain(QWidget *parent = 0);
    ~QMain();

    void initTimer(int);

public slots:
    void handleShowHelp();
    void handleShowConnect();
    void handleDisconnect();
    void handleShowInvite();
    void handleShowNewGame();
    void handleShowLoadGame();
    void handleShowSaveGame();

    void handleWorkerMessage(QString);
    void handleWorkerDied();
    void debugToggle();
    void timerTick();

    void handleMove(int, int, int);
    void handlePut(int, int, stoneAngle);
signals:
    void sendMessage(QString);
    void logMessage(QString, int=0);

private:
    QCLI* worker;
    QString hostname;
    int port;

    QDebugLog* log;
    QTimer* timer;
    QDialogLoading* timerDialog;
    QCustomWrapper* desk;
    Ui::QMain *ui;
};

#endif // QMAIN_H
