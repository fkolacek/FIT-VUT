#ifndef QIRCROOM_H
#define QIRCROOM_H

#include <QString>
#include <QList>
#include <QStringList>

#include "qircmessage.h"


class QIrcRoom
{

public:
    QIrcRoom(QString = "System", QString = "", QString = "");


    void addUser(QString);
    void removeUser(QString);

    void addMessage(QString, QString);

    void setTopic(QString);

    QList<QIrcMessage> getMessages();
    QStringList getUsers();
    int getUsersCount();
    QString getName();
    QString getOwner();
    QString getTopic();

private:
    QString name;
    QString owner;
    QString topic;
    QList<QIrcMessage> messages;
    QStringList users;
};

#endif // QIRCROOM_H
