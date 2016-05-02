/**
 * @file	main.cpp
 * @brief	Projekt IPK 3 - Client
 *
 * @date	2013/05/06
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <regex.h>
#include <netinet/in.h>

#include <deque>

#include "IException.h"
#include "ISegment.h"

#define SEGMENT_SIZE 512

using namespace std;

unsigned int SEGMENT_DATA_SIZE = 200;
unsigned int SEGMENT_WINDOW_SIZE = 10;

// In ms
deque<unsigned long int> SEGMENT_TIMERS;

void setTimeout(unsigned long int timeout){
    SEGMENT_TIMERS.push_front(timeout);

    if(SEGMENT_TIMERS.size() > 3)
        SEGMENT_TIMERS.pop_back();
}

unsigned long int getTimeout(){
    if(SEGMENT_TIMERS.size() != 3)
        return 10;

    double tmp = (SEGMENT_TIMERS.at(1) + ((2 * SEGMENT_TIMERS.at(0) + SEGMENT_TIMERS.at(1) + SEGMENT_TIMERS.at(2)) / 4.0)) / 2.0;
    unsigned long int tmp2 = (unsigned long int)(tmp * 1.3);

    return (tmp2 == 0 || tmp2 > 1000)? 10 : tmp2;
}

typedef struct params{
    unsigned int sPort;
    unsigned int dPort;

    string sHost;
} tParams;

tParams getParam(int, char*[]);

deque<ISegment> lazyLoad(unsigned int&, unsigned int);

string receiveUDP(int);
void sendUDP(int, unsigned int, ISegment);

template<class T> string toString(T input){
    stringstream stream;
    stream << input;

    return stream.str();
}

template<class T> unsigned int toUInt(T input){
    stringstream stream;
    unsigned int output;
    stream << input;
    stream >> output;

    return output;
}

unsigned int getNow(){
    unsigned int elapsedTime;
    struct timeval now;

    gettimeofday(&now, NULL);

    elapsedTime = (now.tv_sec) * 1000.0;
    elapsedTime += (now.tv_usec) / 1000.0;
    return elapsedTime;
}

int main(int argc, char* argv[]){
    try{
        tParams param = getParam(argc, argv);

        unsigned int segmentCounter = 1;
        deque<ISegment> segments = lazyLoad(segmentCounter, SEGMENT_WINDOW_SIZE);

        /*

        */

        int handler = 0;
        struct sockaddr_in options;

        if((handler = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            throw new IException(SOCK_CANNOT_CREATE);

        fcntl(handler, F_SETFL, O_NONBLOCK);

        options.sin_family = AF_INET;
        options.sin_addr.s_addr = INADDR_ANY;
        options.sin_port = htons(param.sPort);

        if(bind(handler, (struct sockaddr*)&options, sizeof(options)) < 0)
            throw new IException(SOCK_CANNOT_BIND);

        fd_set INPUT;
        FD_ZERO(&INPUT);
        FD_SET(handler, &INPUT);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100;

        cerr << "[*] Starting transfer (segments: " << segments.size() << ", window size: " << SEGMENT_WINDOW_SIZE << ")" << endl;

        deque<ISegment> window;
        bool transferred = false;
        string inputBuffer;

        while(select(handler + 1, &INPUT, NULL, NULL, &tv) >= 0){
            //Pending ACKs
            if(FD_ISSET(handler, &INPUT)){
                //Read data from handler
                inputBuffer += receiveUDP(handler);

                //There is some data
                if(inputBuffer.size() > 0){
                    size_t segmentStartPos = inputBuffer.find("<rdt-segment id=\"xkolac12\">");
                    size_t segmentEndPos = inputBuffer.find("</rdt-segment>");

                    //Valid ACK
                    if(segmentStartPos != string::npos && segmentEndPos != string::npos){
                        string segmentRaw = inputBuffer.substr(segmentStartPos, segmentEndPos - segmentStartPos + 14);

                        inputBuffer.erase(inputBuffer.begin() + segmentStartPos, inputBuffer.begin() + segmentEndPos + 14);

                        //<header sn=”seriove cislo” ack=”potvrzeni” win=”velikost okna” tack=”hodnota casovace v ms”>
                        string pattern = "<header sn=\"([0-9]+)\" ack=\"([0-9]+)\" win=\"([0-9]+)\" tack=\"([0-9e\\+\\-\\.]+)\"></header><data>(.+)</data>";

                        regmatch_t pmatch[5];
                        regex_t re;

                        if(regcomp(&re, pattern.c_str(), REG_EXTENDED) != 0)
                            throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

                        if(regexec(&re, segmentRaw.c_str(), 5, pmatch, 0) != REG_NOMATCH){
                            string sn, ack, win, tack;

                            sn = segmentRaw.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
                            //ack = segmentRaw.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);
                            win = segmentRaw.substr(pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
                            tack = segmentRaw.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);

                            unsigned int usn = toUInt(sn);
                            //unsigned int uack = toUInt(ack);
                            //unsigned int uwin = toUInt(win);
                            unsigned long int utack = toUInt(tack);

                            cerr << "[*] Received ACK #" << usn << endl;
                            cerr << " - " << getNow() << " " << utack << endl;
                            if(usn != 0){
                                setTimeout(getNow() - utack);
                                cerr << "[*] New timeout: " << getTimeout() << endl;
                            }

                            //Finally
                            if(usn == 0)
                                break;

                            deque<ISegment>::iterator w;
                            while(window.size() != 0 && window.at(0).getSequenceNumber() <= usn)
                                window.pop_front();

                            SEGMENT_WINDOW_SIZE++;

                            cerr << "[*] Window size was increased to: " << SEGMENT_WINDOW_SIZE << endl;
                        }
                        else{
                            regfree(&re);
                            cerr << "DEBUG: " << segmentRaw << endl;
                            throw new IException(RESPONSE_INVALID);
                        }

                        regfree(&re);
                    }
                }
            }

            //Check all timers
            unsigned int debugCounter = 0;
            for(deque<ISegment>::iterator p = window.begin(); p != window.end(); p++){
                unsigned int timerNow = getNow();

                if( (timerNow - (*p).getTimer()) > getTimeout()){
                    SEGMENT_WINDOW_SIZE = (int)((SEGMENT_WINDOW_SIZE) / 3);

                    if(SEGMENT_WINDOW_SIZE <= 1)
                        SEGMENT_WINDOW_SIZE = 1;

                    cerr << "[*] Window size was decreased to: " << SEGMENT_WINDOW_SIZE << endl;

                    (*p).setTimer(getNow());
                    sendUDP(handler, param.dPort, (*p));
                }
                debugCounter++;

                if(debugCounter >= SEGMENT_WINDOW_SIZE)
                    break;
            }

            //If we can fill window by segments
            if(window.size() < SEGMENT_WINDOW_SIZE && segments.size() > 0){
                while(window.size() < SEGMENT_WINDOW_SIZE && segments.size() > 0){
                    ISegment segment = segments.at(0);

                    segment.setWindowSize(SEGMENT_WINDOW_SIZE);
                    segment.setTimer(getNow());

                    window.push_back(segment);

                    segments.pop_front();

                    sendUDP(handler, param.dPort, segment);

                    deque<ISegment> tmp = lazyLoad(segmentCounter, 1);

                    if(tmp.size() > 0)
                        segments.push_back(tmp.at(0));
                }
            }

            if(!transferred && segments.empty() && window.empty()){
                ISegment END_OF_WORLD(0, "END OF WORLD");
                END_OF_WORLD.setWindowSize(SEGMENT_WINDOW_SIZE);
                END_OF_WORLD.setTimer(getNow());

                window.push_back(END_OF_WORLD);

                sendUDP(handler, param.dPort, END_OF_WORLD);

                transferred = true;
            }

            FD_ZERO(&INPUT);
            FD_SET(handler, &INPUT);

            tv.tv_sec = 0;
            tv.tv_usec = 100;
        }

        cerr << "[*] Sucessfully sended" << endl;
    }
    catch(IException* exception){
        cerr << "[!] " << exception->getMessage() << endl;
        delete exception;
        return 1;
    }
    catch(...){
        cerr << "[!] Unhandlered exception" << endl;
        return 1;
    }

    return 0;
}

tParams getParam(int argc, char* argv[]){
    if(argc == 1) throw new IException(PARAM_INVALID);

    tParams param;
    param.sPort = 0;
    param.dPort = 0;
    param.sHost = "127.0.0.1";

    int c;

    while((c = getopt(argc, argv, "s:d:h:")) != EOF){
        switch(c){
            case 's':
                param.sPort = atoi(optarg);
                break;
            case 'd':
                param.dPort = atoi(optarg);
                break;
            case 'h':
                param.sHost = optarg;
                break;
            default:
                throw new IException(PARAM_INVALID);
                break;
        }
    }

    if(param.sPort == 0) throw new IException(PARAM_INVALID_SOURCE_PORT);
    if(param.dPort == 0) throw new IException(PARAM_INVALID_DESTINATION_PORT);
    if(param.sPort == param.dPort) throw new IException(PARAM_INVALID);

    return param;
}

deque<ISegment> lazyLoad(unsigned int& segmentCounter, unsigned int maxCount){
    deque<ISegment> segments;

    int input;
    string inputBuffer;

    while(segments.size() < maxCount && (input = fgetc(stdin)) != EOF){
        inputBuffer += input;

        if(inputBuffer.size() == SEGMENT_DATA_SIZE){
            segments.push_back(ISegment(segmentCounter, inputBuffer));

            inputBuffer.clear();
            segmentCounter++;
        }
    }

    if(inputBuffer.size() > 0)
        segments.push_back(ISegment(segmentCounter, inputBuffer));

    return segments;
}

string receiveUDP(int handler){
    char rawBuffer[SEGMENT_SIZE];
    string buffer;

    struct stat info;
    if(fstat(handler, &info) != 0)
        throw new IException(SOCK_INVALID_SOCKET_STATE);

    struct sockaddr_in options;
    socklen_t optionsLen = sizeof(options);

    int count = recvfrom(handler, &rawBuffer, SEGMENT_SIZE, MSG_DONTWAIT, (struct sockaddr*)&options, &optionsLen);
    rawBuffer[count] = '\0';
    buffer = string(rawBuffer);

    return buffer;
}

void sendUDP(int handler, unsigned int port, ISegment segment){
    struct stat info;

    if(fstat(handler, &info) != 0)
        throw new IException(SOCK_INVALID_SOCKET_STATE);

    struct sockaddr_in options;
    options.sin_family = AF_INET;
    //options.sin_addr.s_addr = htonl(0x7f000001);
    options.sin_addr.s_addr = inet_addr("127.0.0.1");
    options.sin_port = htons(port);

    string buffer = segment.getSegment();

    sendto(handler, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&options, sizeof(options));

    cerr << "[*] Sended segment #" << segment.getSequenceNumber() << endl;
}
