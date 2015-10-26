#ifndef QIRCLOGGER_H
#define QIRCLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "qircexception.h"

class QIrcLogger : public QObject
{
    Q_OBJECT
public:
    explicit QIrcLogger(QString logName = "server.log", bool verbose = true, QObject *parent = 0);
    virtual ~QIrcLogger();

private:
    QFile* logHandler;
    QTextStream* outputHandler;
    bool verbose;

public slots:
    void receiveMessage(QString);

};

#endif // QIRCLOGGER_H
