#include "qircmessage.h"

QIrcMessage::QIrcMessage(QString user, QString message){
    this->user = user;
    this->message = message;
    this->date = QDateTime::currentDateTime();
}

QString QIrcMessage::getUser(){
    return this->user;
}

QString QIrcMessage::getMessage(){
    return this->message;
}

QDateTime QIrcMessage::getDate(){
    return this->date;
}
