/**
  * @file qcli.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QCLI
  */

#ifndef QCLI_H
#define QCLI_H

#include <QThread>
#include <QMessageBox>
#include "../cli/cli.h"

/**
  * @brief Class QCLI used for storing Game state in separated thread
  */
class QCLI : public QThread{
    Q_OBJECT

public:
    QCLI(QString, int, QWidget* = 0);
    void run();
    CLI* getClient();
signals:
    void sendMessage(QString);
public slots:
    void receiveMessage(QString);

private:
    QWidget* mother;
    CLI* client;
    QString hostname;
    int port;


};

#endif // QCLI_H
