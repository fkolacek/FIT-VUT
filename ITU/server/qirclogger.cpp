#include "qirclogger.h"

QIrcLogger::QIrcLogger(QString logName, bool verbose, QObject *parent): QObject(parent){
    this->verbose = verbose;

    if(this->verbose)
        this->outputHandler = new QTextStream(stdout, QIODevice::WriteOnly);
    else
        this->outputHandler = NULL;

    this->logHandler = new QFile(logName);

    if(!this->logHandler->open(QFile::Text | QFile::WriteOnly))
        throw new QIrcException(LOGGER_CANNOT_OPEN_FILE);
}

QIrcLogger::~QIrcLogger(){
    if(this->logHandler->isOpen())
        this->logHandler->close();

    delete this->logHandler;

    this->logHandler = NULL;

    if(this->outputHandler != NULL){
       delete this->outputHandler;

       this->outputHandler = NULL;
    }
}

void QIrcLogger::receiveMessage(QString message){
    QTextStream stream(this->logHandler);

    stream << message << "\n";

    if(this->verbose){
        *(this->outputHandler) << message << "\n";
        this->outputHandler->flush();
    }
}
