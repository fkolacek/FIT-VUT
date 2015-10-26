#ifndef QIRCCONNECTION_H
#define QIRCCONNECTION_H

#include <QString>
#include <QDebug>
#include "qircbuffer.h"

enum QIrcConnectionStatus{
    CONNECTED,
    ZOMBIE,

    PHASE_NICK,
    PHASE_USER,
    PHASE_AUTHORIZED,
    DISCONNECTED = 666,
};

class QIrcConnection
{

public:
    QIrcConnection();
    QIrcConnection(QString, QString);

    void setNick(QString);
    void setUser(QString);
    void setAddress(QString);
    void setRealName(QString);
    void setStatus(QIrcConnectionStatus);
    void setTickResponse(QString);
    void setTickRequest(QString);
    void setLogin(QString);

    QString getId();
    QString getNick();
    QString getUser();
    QString getAddress();
    QString getRealName();
    QIrcConnectionStatus getStatus();
    QString getTickResponse();
    QString getTickRequest();
    QString getLogin();
    QIrcBuffer& getBuffer();

private:
    QString id;
    QString nick;
    QString user;
    QString address;
    QString realName;
    QIrcConnectionStatus status;
    QString tickResponse;
    QString tickRequest;

    QString login;

    //Input buffer - because of Windows
    QIrcBuffer buffer;

};

#endif // QIRCCONNECTION_H
