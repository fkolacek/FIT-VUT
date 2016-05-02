/**
 * @file	ISegment.h
 * @brief	Projekt IPK 3 - Client
 *
 * @date	2013/03/02
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */


#ifndef ISEGMENT_H
#define ISEGMENT_H

#include <string>
#include <sstream>
#include <sys/time.h>

using namespace std;

class ISegment
{
    public:
        ISegment();
        ISegment(unsigned int, string);
        virtual ~ISegment();

        void setWindowSize(unsigned int);
        void setTimer(unsigned int);

        unsigned int getSequenceNumber();
        unsigned int getWindowSize();
        unsigned int getTimer();

        string getData();
        string getSegment();
        string getAck();
    private:
        unsigned int sequenceNumber;
        unsigned int windowSize;
        unsigned int timer;
        string data;
};

#endif // ISEGMENT_H
