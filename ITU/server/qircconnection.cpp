#include "qircconnection.h"

QIrcConnection::QIrcConnection(){
    this->status = ZOMBIE;
    this->tickResponse = "null";
    this->tickRequest = "null";
    this->login = "";
    this->buffer.clear();
}

QIrcConnection::QIrcConnection(QString id, QString address){
    this->id = id;
    this->status = CONNECTED;
    this->tickResponse = "null";
    this->tickRequest = "null";
    this->login = "";
    this->buffer.clear();
    this->address = address;
}

void QIrcConnection::setNick(QString nick){
    this->nick = nick;
}

void QIrcConnection::setUser(QString user){
    this->user = user;
}

void QIrcConnection::setAddress(QString address){
    this->address = address;
}

void QIrcConnection::setRealName(QString realName){
    this->realName = realName;
}

void QIrcConnection::setStatus(QIrcConnectionStatus status){
    this->status = status;
}

void QIrcConnection::setTickResponse(QString tickResponse){
    this->tickResponse = tickResponse;
}

void QIrcConnection::setTickRequest(QString tickRequest){
    this->tickRequest = tickRequest;
}

void QIrcConnection::setLogin(QString login){
    this->login = login.toLower();
}

QString QIrcConnection::getId(){
    return this->id;
}

QString QIrcConnection::getNick(){
    return this->nick;
}

QString QIrcConnection::getUser(){
    return this->user;
}

QString QIrcConnection::getAddress(){
    return this->address;
}

QString QIrcConnection::getRealName(){
    return this->realName;
}

QIrcConnectionStatus QIrcConnection::getStatus(){
    return this->status;
}

QString QIrcConnection::getTickResponse(){
    return this->tickResponse;
}

QString QIrcConnection::getTickRequest(){
    return this->tickRequest;
}

QString QIrcConnection::getLogin(){
    return this->login;
}

QIrcBuffer& QIrcConnection::getBuffer(){
    return this->buffer;
}


