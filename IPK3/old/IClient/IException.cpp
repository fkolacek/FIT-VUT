/**
 * @file	IException.cpp
 * @brief	Projekt IPK 2 - Client
 *
 * @date	2013/03/02
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IException.h"

const string IErrorTexts[] = {
    "Invalid source port",
    "Invalid destination port",
    "Invalid arguments",

    "Cannot connect to remote host (invalid socket state)",
    "Cannot create socket",
    "Cannot resolve address to IP address",
    "Cannot connect to remote host",
    "Cannot read from socket",
    "Cannot write to socket",
    "Cannot bind socket",

    "Invalid response",

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
