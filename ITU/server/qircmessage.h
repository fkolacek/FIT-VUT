#ifndef QIRCMESSAGE_H
#define QIRCMESSAGE_H

#include <QString>
#include <QDateTime>

class QIrcMessage
{

public:
    QIrcMessage(QString, QString);

    QString getUser();
    QString getMessage();
    QDateTime getDate();
private:
    QString user;
    QString message;
    QDateTime date;
};

#endif // QIRCMESSAGE_H
