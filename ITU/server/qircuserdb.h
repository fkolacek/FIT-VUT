#ifndef QIRCUSERDB_H
#define QIRCUSERDB_H

#include <QString>
#include <QHash>
#include <QFile>
#include <QRegExp>
#include <QCryptographicHash>

#include "qircexception.h"
#include "qircuser.h"

class QIrcUserDB
{

public:
    QIrcUserDB(QString fileName = "users.db");

    void setFileName(QString);
    void load();
    void save();

    bool addUser(QString, QString, int = 0);
    bool isUser(QString);
    bool delUser(QString);
    QIrcUser user(QString);

    bool isValid(QString, QString);

    int count();
private:
    QString fileName;
    QHash<QString, QIrcUser> users;

};

#endif // QIRCUSERDB_H
