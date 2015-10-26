#ifndef ISEGMENT_H
#define ISEGMENT_H

#include <string>
#include <sstream>
#include <sys/time.h>

#include "IConfig.h"

using namespace std;

class ISegment
{
    public:
        ISegment(unsigned int, string);
        virtual ~ISegment();

        void setWin(unsigned int);
        void setTimer();

        unsigned int getSeq();
        unsigned int getWin();
        long int getTimer();

        string getData();
        string getSegment();
        string getAck();
    private:
        unsigned int seq;
        unsigned int win;
        struct timeval timer;
        string data;
};

#endif // ISEGMENT_H
