#include "qircroom.h"

QIrcRoom::QIrcRoom(QString name, QString owner, QString topic){
    this->name = name;
    this->owner = owner;
    this->topic = topic;
}

void QIrcRoom::addUser(QString user){
    this->users.append(user);
}

void QIrcRoom::removeUser(QString user){
    for(QStringList::Iterator p = this->users.begin(); p != this->users.end(); p++){
        if(*p == user){
            this->users.erase(p);
            break;
        }
    }
}

void QIrcRoom::addMessage(QString user, QString message){
    QIrcMessage msg(user, message);

    this->messages.append(msg);
}

void QIrcRoom::setTopic(QString topic){
    this->topic = topic;
}

QList<QIrcMessage> QIrcRoom::getMessages(){
    return this->messages;
}

QStringList QIrcRoom::getUsers(){
    return this->users;
}

int QIrcRoom::getUsersCount(){
    return this->users.size();
}

QString QIrcRoom::getName(){
    return this->name;
}

QString QIrcRoom::getOwner(){
    return this->owner;
}

QString QIrcRoom::getTopic(){
    return this->topic;
}
