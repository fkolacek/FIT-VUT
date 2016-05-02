/**
 * @file	ISegment.cpp
 * @brief	Projekt IPK 3 - Client
 *
 * @date	2013/03/02
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */


#include "ISegment.h"

ISegment::ISegment(){
    this->sequenceNumber = 0;
    this->windowSize = 0;
    this->timer = 0;
    this->data = "";
}

ISegment::ISegment(unsigned int sequenceNumber, string data){
    this->sequenceNumber = sequenceNumber;
    this->windowSize = 0;
    this->timer = 0;
    this->data = data;
}

ISegment::~ISegment(){
    //dtor
}

void ISegment::setWindowSize(unsigned int windowSize){
    this->windowSize = windowSize;
}

void ISegment::setTimer(unsigned int timer){
    this->timer = timer;
}

unsigned int ISegment::getSequenceNumber(){
    return this->sequenceNumber;
}

unsigned int ISegment::getWindowSize(){
    return this->windowSize;
}

unsigned int ISegment::getTimer(){
    return this->timer;
}

string ISegment::getData(){
    return this->data;
}

string ISegment::getSegment(){
    stringstream stream;

    stream << "<rdt-segment id=\"xkolac12\">";
    stream << "<header sn=\"" << this->sequenceNumber << "\" ack=\""<< this->sequenceNumber << "\" win=\"" << this->windowSize << "\" tack=\"" << this->timer << "\"></header>";
    stream << "<data>" << this->data << "</data>";
    stream << "</rdt-segment>";

    return stream.str();
}

string ISegment::getAck(){
    stringstream stream;

    stream << "<rdt-segment id=\"xkolac12\">";
    stream << "<header sn=\"" << this->sequenceNumber << "\" ack=\""<< this->sequenceNumber << "\" win=\"" << this->windowSize << "\" tack=\"" << this->timer << "\"></header>";
    stream << "<data>ACK</data>";
    stream << "</rdt-segment>";

    return stream.str();
}
