/**
 * @file	main.cpp
 * @brief	Projekt IPK 3 - Server
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
#include <signal.h>
#include <regex.h>

#include <map>

#include "IException.h"
#include "ISegment.h"

#define SEGMENT_SIZE 512

using namespace std;

unsigned int SEGMENT_DATA_SIZE = 400;

typedef struct params{
    unsigned int sPort;
    unsigned int dPort;

    string sHost;
} tParams;

tParams getParam(int, char*[]);

string receiveUDP(int);
void sendUDP(int, unsigned int, ISegment);
void programAbort(int);

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

int main(int argc, char* argv[]){
    try{
        signal(SIGTERM, programAbort);
        signal(SIGINT, programAbort);

        tParams param = getParam(argc, argv);

        int handler = 0;
        struct sockaddr_in options;

        //Create socket
        if((handler = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            throw new IException(SOCK_CANNOT_CREATE);

        fcntl(handler, F_SETFL, O_NONBLOCK);

        options.sin_family = AF_INET;
        options.sin_addr.s_addr = INADDR_ANY;
        options.sin_port = htons(param.sPort);

        //Bind socket
        if(bind(handler, (struct sockaddr*)&options, sizeof(options)) < 0)
            throw new IException(SOCK_CANNOT_BIND);

        //Create set
        fd_set INPUT;
        FD_ZERO(&INPUT);
        FD_SET(handler, &INPUT);

        string inputBuffer;
        map<unsigned int, ISegment> segments;
        unsigned lastAck = 0;
        unsigned toPrint = 1;

        //Any incoming data
        while(select(handler + 1, &INPUT, NULL, NULL, NULL) >= 0){
            //Data avalaible on socket
            if(FD_ISSET(handler, &INPUT)){
                inputBuffer += receiveUDP(handler);

                if(inputBuffer.size() > 0){
                    size_t segmentStartPos = inputBuffer.find("<rdt-segment id=\"xkolac12\">");
                    size_t segmentEndPos = inputBuffer.find("</rdt-segment>");

                    if(segmentStartPos != string::npos && segmentEndPos != string::npos){
                        string segmentRaw = inputBuffer.substr(segmentStartPos, segmentEndPos - segmentStartPos + 14);

                        inputBuffer.erase(inputBuffer.begin() + segmentStartPos, inputBuffer.begin() + segmentEndPos + 14);

                        //<header sn=”seriove cislo” ack=”potvrzeni” win=”velikost okna” tack=”hodnota casovace v ms”>
                        string pattern = "<header sn=\"([0-9]+)\" ack=\"([0-9]+)\" win=\"([0-9]+)\" tack=\"([0-9e\\+\\-\\.]+)\"></header><data>(.+)</data>";

                        regmatch_t pmatch[6];
                        regex_t re;

                        if(regcomp(&re, pattern.c_str(), REG_EXTENDED) != 0)
                            throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

                        if(regexec(&re, segmentRaw.c_str(), 6, pmatch, 0) != REG_NOMATCH){
                            string sn, ack, win, tack, data;

                            sn = segmentRaw.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
                            //ack = segmentRaw.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);
                            win = segmentRaw.substr(pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
                            tack = segmentRaw.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);
                            data = segmentRaw.substr(pmatch[5].rm_so, pmatch[5].rm_eo - pmatch[5].rm_so);

                            unsigned int usn = toUInt(sn);
                            //unsigned int uack = toUInt(ack);
                            unsigned int uwin = toUInt(win);

                            unsigned int utack = toUInt(tack);

                            ISegment segment(usn, data);
                            segment.setWindowSize(uwin);
                            segment.setTimer(utack);

                            if(usn != 0)
                                segments.insert(pair<unsigned int, ISegment>(usn, segment));


                            cerr << "[*] Received segment #" << usn << endl;

                            unsigned int segmentID = lastAck + 1;

                            while(segments.find(toPrint) != segments.end()){
                                cout << segments[toPrint].getData();
                                cout.flush();

                                toPrint++;
                            }


                            if(usn == 0 || segments.find(segmentID) != segments.end()){
                                while(segments.find(segmentID + 1) != segments.end())
                                    segmentID++;

                                if(usn == 0){
                                    segmentID = 0;
                                    toPrint = 1;

                                    //for(map<unsigned int, ISegment>::iterator p = segments.begin(); p != segments.end(); p++)
                                    //    cout << (p->second).getData();

                                    cout.flush();
                                    segments.clear();
                                }

                                if(lastAck != segmentID){
                                    sendUDP(handler, param.dPort, segments[segmentID]);
                                    lastAck = segmentID;
                                }
                            }

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

            FD_ZERO(&INPUT);
            FD_SET(handler, &INPUT);
        }

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

    string buffer = segment.getAck();

    sendto(handler, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&options, sizeof(options));

    cerr << "[*] Sended ack #" << segment.getSequenceNumber() << endl;
}

void programAbort(int sig){
    sig = sig;

    cerr << "[!] Aborting.." << endl;

    exit(1);
}
