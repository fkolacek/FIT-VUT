#include "qircuser.h"

QIrcUser::QIrcUser(){
    this->username = "";
    this->password = "x";
    this->role = 0;
}

QIrcUser::QIrcUser(QString username, QString password, int role){
    this->username = username;
    this->password = password;
    this->role = role;
}

void QIrcUser::setUsername(QString username){
    this->username = username;
}

void QIrcUser::setPassword(QString password){
    this->password = password;
}

void QIrcUser::setRole(int role){
    this->role = role;
}

QString QIrcUser::getUsername(){
    return this->username;
}

QString QIrcUser::getPassword(){
    return this->password;
}

int QIrcUser::getRole(){
    return this->role;
}

bool QIrcUser::isValid(){
    return !(this->username.isEmpty() && this->password == "x");
}
