#ifndef QIRCUSER_H
#define QIRCUSER_H

#include <QString>

class QIrcUser
{

public:
    QIrcUser();
    QIrcUser(QString, QString, int = 0);

    void setUsername(QString);
    void setPassword(QString);
    void setRole(int);

    QString getUsername();
    QString getPassword();
    int getRole();

    bool isValid();
private:
    QString username;
    QString password;
    int role;

};

#endif // QIRCUSER_H
