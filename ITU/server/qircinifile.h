#ifndef QIRCINIFILE_H
#define QIRCINIFILE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QHash>
#include <QRegExp>

#include "qircexception.h"

class QIrcIniFile : public QObject
{
    Q_OBJECT
public:
    QIrcIniFile(QObject *parent = 0);
    QIrcIniFile(QString fileName = "config.ini", QObject *parent = 0);

    void clear();
    void loadFromFile(QString fileName = "config.ini");

    //setters
    void setSection(QString sectionName);
    void setValue(QString sectionName, QString key, QString val);

    //getters
    bool isSection(QString sectionName);
    bool isValue(QString sectionName, QString key);

    QHash<QString, QString> getSection(QString sectionName);
    QString getValue(QString sectionName, QString key);

private:
    QHash<QString, QHash<QString, QString> > values;

};

#endif // QIRCINIFILE_H
