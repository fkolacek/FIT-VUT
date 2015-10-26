#include "ISegment.h"

ISegment::ISegment(unsigned int seq, string data){
    this->seq = seq;
    this->win = SEGMENT_WIN;
    this->setTimer();
    this->data = data;
}

ISegment::~ISegment(){
    //dtor
}

void ISegment::setWin(unsigned int win){
    this->win = win;
}

void ISegment::setTimer(){
    gettimeofday(&(this->timer), NULL);
}

unsigned int ISegment::getSeq(){
    return this->seq;
}

unsigned int ISegment::getWin(){
    return this->win;
}

long int ISegment::getTimer(){
    long int seconds, useconds;
    struct timeval now;

    gettimeofday(&now, NULL);

    seconds = this->timer.tv_sec;
    useconds = this->timer.tv_usec;

    return (seconds * 1000 + useconds / 1000);
}

string ISegment::getData(){
    return this->data;
}

string ISegment::getSegment(){
    stringstream stream;

    stream << "<rdt-segment id=\"xkolac12\">";
    stream << "<header sn=\"" << this->seq << "\" win=\"" << this->win << "\" tack=\"" << this->getTimer() << "\">";
    stream << this->seq << "</header>";
    stream << "<data>" << this->data << "</data>";
    stream << "</rdt-segment>";

    return stream.str();
}

string ISegment::getAck(){
    stringstream stream;

    stream << "<rdt-segment id=\"xkolac12\">";
    stream << "<header ack=\"" << this->seq << "\" tack=\"" << this->getTimer() << "\">";
    stream << this->seq << "</header>";
    stream << "<data>ACK</data>";
    stream << "</rdt-segment>";

    return stream.str();
}
