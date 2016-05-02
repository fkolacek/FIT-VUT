#include "udt.h"

UDT::UDT(){
    SN = 0;
    W = 0;
    T = 0;
    D = "";
}

UDT::UDT(unsigned int a, string b){
    SN = a;
    W = 0;
    T = 0;
    D = b;
}

void UDT::setW(int a){
    W = a;
}

void UDT::setT(unsigned int a){
    T = a;
}

unsigned int UDT::getSN(){
    return SN;
}

unsigned int UDT::getT(){
    return T;
}

string UDT::getD(){
    return D;
}

string UDT::toString(int a){
	stringstream s;
	string b;
	s << a;
	s >> b;
	
	return b;
}

string UDT::toS(){
    string out = "";

    out += string("<rdt-segment id=\"xmatya03\">");
    out += string("<header sn=\"" + toString(SN) + "\" ack=\"" + toString(SN) + "\" win=\"" + toString(W) + "\" tack=\"" + toString(T) + "\"></header><data>" + D + "</data>");
    out += string("</rdt-segment>");

    return out;
}

string UDT::toA(){
    string out = "";

    out += string("<rdt-segment id=\"xmatya03\">");
    out += string("<header sn=\"" + toString(SN) + "\" ack=\"" + toString(SN) + "\" win=\"" + toString(W) + "\" tack=\"" + toString(T) + "\"></header><data>Something</data>");
    out += string("</rdt-segment>");

    return out;
}
