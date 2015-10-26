#ifndef QIRCSERVER_H
#define QIRCSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QStringList>
#include <QHash>
#include <QRegExp>
#include <QTimer>
#include <iostream>

#include "qircexception.h"
#include "qirclogger.h"
#include "qircinifile.h"
#include "qircuserdb.h"
#include "qircroom.h"
#include "qircconnection.h"

class QIrcServer : public QObject
{
    Q_OBJECT
public:
    explicit QIrcServer(QObject *parent = 0);
    virtual ~QIrcServer();
    void run();

private:
    QIrcLogger* logger;
    QIrcIniFile* config;
    QTcpServer* server;
    QHostAddress serverAddress;
    int serverPort;

    QTimer* serverTimer;

    QHash<QTcpSocket*, QIrcConnection>* connections;
    //QStringList nicks;
    QHash<QString, QIrcRoom> rooms;

    QIrcUserDB* users;

    void initLogger();
    void initConfig();
    void initUsers();
    void initServer();
    void initTimer();

    void validateConfig();

    //Helpers
    void socketWrite(QTcpSocket*, QString);
    QTcpSocket* getSocket(QString);
    QIrcConnection& getConnection(QTcpSocket*);

    // Phases
    void phaseAuthorized(QTcpSocket*);
    void phaseJoin(QTcpSocket*, QString);
    void phasePart(QTcpSocket*, QString);
    void phaseWho(QTcpSocket*, QString);
    void phaseWhois(QTcpSocket*, QString);
    void phaseNames(QTcpSocket*, QString);
    void phasePrivmsg(QTcpSocket*, QString, QString);
    void phaseQuit(QTcpSocket*, QString);
    void phaseNick(QTcpSocket*, QString);
    void phaseAuth(QTcpSocket*, QString, QString);
    void phaseReg(QTcpSocket*, QString, QString);

private slots:
    void handleNewConnection();
    void serverReply();
    void clientDisconnect();
    void timerTick();
    
signals:
    void sendMessage(QString);
    
};

#endif // QIRCSERVER_H
