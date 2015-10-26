#ifndef QIRCBUFFER_H
#define QIRCBUFFER_H

#include <QByteArray>
#include <QString>

class QIrcBuffer
{

public:
    QIrcBuffer(QString = "\r\n");

    void append(QByteArray);
    void clear();
    QByteArray flush();
    bool isReady();

private:
    QByteArray buffer;
    QString delimiter;
};

#endif // QIRCBUFFER_H
