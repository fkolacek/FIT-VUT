/**
 * @file	IException.cpp
 * @brief	Projekt IPK 2 - Server
 *
 * @date	2013/03/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IException.h"

const string IErrorTexts[] = {
    "Cannot create socket",
    "Cannot bind to address",
    "Cannot listen on address",
    "Cannot accept incoming connection",
    "Cannot select socket",
    "Cannot read from socket",
    "Cannot write to socket",

    "Invalid format of users file",

    "Cannot compile regexp",
    "Cannot open output file",
    "Cannot fork new process",
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
