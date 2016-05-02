/**
 * @file	IException.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/17
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IException.h"
/* Nefunguje na Eve.. zajimave
const string IErrorTexts[] = {
    [SOCK_INVALID_SOCKET_STATE] = "Cannot connect to remote host (invalid socket state)",
    [SOCK_CANNOT_CREATE] = "Cannot create socket",
    [SOCK_CANNOT_RESOLVE_ADDRESS] = "Cannot resolve address to IP address",
    [SOCK_CANNOT_CONNECT_TO_REMOTE_HOST] = "Cannot connect to remote host",
    [SOCK_CANNOT_READ] = "Cannot read from socket",
    [SOCK_CANNOT_WRITE] = "Cannot write to socket",

    [REQUEST_INVALID_URI] = "Invalid requested URI",
    [RESPONSE_INVALID_HEADER] = "Invalid header in response",
    [CLIENT_CANNOT_SEND_DATA] = "Cannot send data from client",
    [CLIENT_INVALID_RESPONSE] = "Client received invalid response",
    [CLIENT_MAX_REDIRECT_COUNT_EXCEEDED] = "Max redirect count exceeded",

    [SYSTEM_CANNOT_COMPILE_REGEX] = "Cannot compile regexp",
    [SYSTEM_CANNOT_OPEN_FILE] = "Cannot open output file",
    [SYSTEM_UNDEFINED_EXCEPTION] = "System cautched undefined exception"
};
*/

const string IErrorTexts[] = {
    "Cannot connect to remote host (invalid socket state)",
    "Cannot create socket",
    "Cannot resolve address to IP address",
    "Cannot connect to remote host",
    "Cannot read from socket",
    "Cannot write to socket",

    "Invalid requested URI",
    "Invalid header in response",
    "Cannot send data from client",
    "Client received invalid response",
    "Max redirect count exceeded",

    "Cannot compile regexp",
    "Cannot open output file",
    "System cautched undefined exception"
};

IException::IException(int code, string file, int line){
    this->message = string(IErrorTexts[code]);
    this->code = code;
    this->file = file;
    this->line = line;
}

IException::IException(string message, int code, string file, int line){
    this->message = message;
    this->code = code;
    this->file = file;
    this->line = line;
}


IException::~IException(){
    //dtor
}

string IException::getMessage(){
    return this->message;
}

int IException::getCode(){
    return this->code;
}

string IException::getFile(){
    return this->file;
}

int IException::getLine(){
    return this->line;
}
