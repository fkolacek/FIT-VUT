#include "qircbuffer.h"

QIrcBuffer::QIrcBuffer(QString delimiter){
    this->delimiter = delimiter;
    this->buffer.clear();
}

void QIrcBuffer::append(QByteArray data){
    this->buffer.append(data);
}

void QIrcBuffer::clear(){
    this->buffer.clear();
}

QByteArray QIrcBuffer::flush(){
    QByteArray data;

    if(!this->isReady())
        return data;

    int pos = this->buffer.indexOf(this->delimiter);

    data = this->buffer.mid(0, pos + this->delimiter.size());

    this->buffer.remove(0, pos + this->delimiter.size());

    return data;
}

bool QIrcBuffer::isReady(){
    return this->buffer.contains(this->delimiter.toAscii());
}
