#include "qircuserdb.h"

QIrcUserDB::QIrcUserDB(QString fileName){
    this->fileName = fileName;
    this->users.clear();
}

void QIrcUserDB::setFileName(QString fileName){
    this->fileName = fileName;
}

void QIrcUserDB::load(){
    this->users.clear();

    if(!QFile::exists(this->fileName))
        return;

    QFile fileHandler(this->fileName);

    if(!fileHandler.open(QFile::Text | QFile::ReadOnly))
        throw new QIrcException(USERDB_CANNOT_OPEN_FILE);

    QString line;
    QRegExp regexp("^([A-z0-9\\_]+)\\:([A-z0-9\\_]+)\\:([0-9])$");
    while(!fileHandler.atEnd()){
        line = fileHandler.readLine().trimmed();

        if(regexp.indexIn(line) != -1){
            QString username, password;
            int role;

            username = regexp.cap(1);
            password = regexp.cap(2);
            role = regexp.cap(3).toInt();

            QIrcUser user(username, password, role);

            this->users.insert(username, user);
        }
        else{
            fileHandler.close();
            throw new QIrcException(USERDB_SYNTAX_ERROR);
        }
    }

    fileHandler.close();
}

void QIrcUserDB::save(){
    QFile fileHandler(this->fileName);

    if(!fileHandler.open(QFile::Text | QFile::WriteOnly))
        throw new QIrcException(USERDB_CANNOT_OPEN_FILE);

    QString line;
    for(QHash<QString, QIrcUser>::Iterator p = this->users.begin(); p != this->users.end(); p++){
        line = (*p).getUsername() + ":" + (*p).getPassword() + ":" + QString::number((*p).getRole()) + "\n";

        fileHandler.write(line.toAscii());
    }

    fileHandler.close();
}

bool QIrcUserDB::addUser(QString username, QString password, int role){
    if(this->isUser(username))
        return false;

    username = username.toLower();

    //QString hash = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Sha1);

    QIrcUser user(username, password, role);

    this->users.insert(username, user);
    this->save();
    return true;
}

bool QIrcUserDB::isUser(QString username){
    username = username.toLower();

    return this->users.contains(username);
}

bool QIrcUserDB::delUser(QString username){
    username = username.toLower();

    if(!this->isUser(username))
        return false;

    this->users.erase(this->users.find(username));
    this->save();
    return true;
}

bool QIrcUserDB::isValid(QString username, QString password){
    if(!this->isUser(username))
        return false;

    username = username.toLower();

    //QString hash = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Sha1);

    return (this->user(username).getPassword() == password);
}

QIrcUser QIrcUserDB::user(QString username){
    username = username.toLower();

    return this->users[username];
}

int QIrcUserDB::count(){
    return this->users.count();
}
