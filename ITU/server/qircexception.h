#ifndef QIRCEXCEPTION_H
#define QIRCEXCEPTION_H

#include <QDebug>
#include <QString>

enum QIrcExceptionEnum{
    LOGGER_CANNOT_OPEN_FILE,
    INI_FILE_CANNOT_OPEN_FILE,
    INI_FILE_SYNTAX_ERROR,

    SERVER_INVALID_DIRECTIVE_LISTEN,
    SERVER_INVALID_DIRECTIVE_PORT,
    SERVER_INVALID_DIRECTIVE_CONNECTIONS,

    USERDB_CANNOT_OPEN_FILE,
    USERDB_SYNTAX_ERROR,

    SERVER_RUNTIME_CANNOT_BIND,
};

extern char* QIrcExceptionMessages[];

class QIrcException
{

public:
    QIrcException(QIrcExceptionEnum, QString = "", int = 0);
    QIrcException(QString, int = 0, QString = "", int = 0);

    QString getMessage();

private:
    QString message;
    int code;
    QString file;
    int line;
};

#endif // QIRCEXCEPTION_H
