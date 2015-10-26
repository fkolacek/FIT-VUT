#include "qircexception.h"

char* QIrcExceptionMessages[] = {
    [LOGGER_CANNOT_OPEN_FILE] = "[Logger] Cannot open file",
    [INI_FILE_CANNOT_OPEN_FILE] = "[Config] Cannot open file",
    [INI_FILE_SYNTAX_ERROR] = "[Config] Syntax error",
    [SERVER_INVALID_DIRECTIVE_LISTEN] = "[Server] Invalid value in directive listen",
    [SERVER_INVALID_DIRECTIVE_PORT] = "[Server] Invalid value in directive port",
    [SERVER_INVALID_DIRECTIVE_CONNECTIONS] = "[Server] Invalid value in directive connections",
    [USERDB_CANNOT_OPEN_FILE] = "[UserDB] Cannot open file",
    [USERDB_SYNTAX_ERROR] = "[UserDB] Syntax error",

    [SERVER_RUNTIME_CANNOT_BIND] = "[Server] Cannot bind to address",
};

QIrcException::QIrcException(QIrcExceptionEnum type, QString file, int line){
    this->message = QString(QIrcExceptionMessages[type]);
    this->code = type;
    this->file = file;
    this->line = line;
}

QIrcException::QIrcException(QString message, int code, QString file, int line){
    this->message = message;
    this->code = code;
    this->file = file;
    this->line = line;
}

QString QIrcException::getMessage(){
    return this->message;
}
