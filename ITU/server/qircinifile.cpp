#include "qircinifile.h"

QIrcIniFile::QIrcIniFile(QObject *parent): QObject(parent){
    this->clear();
}

QIrcIniFile::QIrcIniFile(QString fileName, QObject *parent): QObject(parent){
    this->loadFromFile(fileName);
}

void QIrcIniFile::clear(){
    this->values.clear();
}

void QIrcIniFile::loadFromFile(QString fileName){
    this->clear();

    QFile fileHandler(fileName);

    if(!fileHandler.open(QFile::Text | QFile::ReadOnly))
        throw new QIrcException(INI_FILE_CANNOT_OPEN_FILE);

    QRegExp regSection("^\\[([A-z\\-\\_]+)\\]$");
    QRegExp regComment("^\\#(.*)$");
    QRegExp regKeyVal("^([A-z0-9\\-\\_]+) ?\\= ?(.+)$");

    QString line;
    QString sectionName = "General";

    while(!fileHandler.atEnd()){
        line = fileHandler.readLine().trimmed();

        if(line.isEmpty())
            continue;

        if(regSection.indexIn(line) != -1)
            sectionName = regSection.cap(1);
        else if(regKeyVal.indexIn(line) != -1)
            this->setValue(sectionName, regKeyVal.cap(1), regKeyVal.cap(2));
        else if(regComment.indexIn(line) != -1)
            continue;
        else{
            fileHandler.close();
            throw new QIrcException(INI_FILE_SYNTAX_ERROR);
        }
    }

    fileHandler.close();
}

void QIrcIniFile::setSection(QString sectionName){
    sectionName = sectionName.toLower();

    if(this->isSection(sectionName))
        return;

    this->values.insert(sectionName, QHash<QString, QString>());
}

void QIrcIniFile::setValue(QString sectionName, QString key, QString val){
    sectionName = sectionName.toLower();
    key = key.toLower();

    this->setSection(sectionName);

    this->values[sectionName].insert(key, val);
}

bool QIrcIniFile::isSection(QString sectionName){
    sectionName = sectionName.toLower();

    return this->values.contains(sectionName);
}

bool QIrcIniFile::isValue(QString sectionName, QString key){
    sectionName = sectionName.toLower();
    key = key.toLower();

    if(!this->values.contains(sectionName))
        return false;

    return this->values.value(sectionName).contains(key);
}

QHash<QString, QString> QIrcIniFile::getSection(QString sectionName){
    sectionName = sectionName.toLower();

    if(isSection(sectionName))
        return this->values.value(sectionName);

    return QHash<QString, QString>();
}

QString QIrcIniFile::getValue(QString sectionName, QString key){
    sectionName = sectionName.toLower();
    key = key.toLower();

    if(isValue(sectionName, key))
        return this->values.value(sectionName).value(key);

    return QString();
}
