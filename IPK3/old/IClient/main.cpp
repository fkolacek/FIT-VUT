/**
 * @file	main.cpp
 * @brief	Projekt IPK 3 - Client
 *
 * @date	2013/05/06
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <iostream>
#include <deque>
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

#include "IConfig.h"
#include "IException.h"
#include "ISegment.h"
#include "base64.h"

using namespace std;

//Dynamic sliding window size
unsigned int SEGMENT_WIN_STATUS;
unsigned int SEGMENT_TACK_STATUS;

typedef struct params{
    unsigned int sPort;
    unsigned int dPort;

    string sHost;
} tParams;

tParams getParam(int, char*[]);

void prinHelp();

void sendUDP(int, unsigned int, ISegment);
string receiveUDP(int);

long int getNow();
long int getTi(deque<long int> timers);

int main(int argc, char* argv[]){
    try{
        tParams param = getParam(argc, argv);

        SEGMENT_WIN_STATUS = SEGMENT_WIN;
        SEGMENT_TACK_STATUS = SEGMENT_TACK * 1000;

        deque<ISegment> window;
        deque<long int> timers;
        timers.push_back(SEGMENT_TACK_STATUS);
        timers.push_back(SEGMENT_TACK_STATUS);
        timers.push_back(SEGMENT_TACK_STATUS);

        /* Loading segments to memory */

        int input;
        string inputBuffer;
        unsigned int segmentID = 1;
        deque<ISegment> segments;

        while((input = fgetc(stdin)) != EOF){

            inputBuffer += input;

            if(inputBuffer.size() == SEGMENT_DATA){
                segments.push_back(ISegment(segmentID, base64_encode(reinterpret_cast<const unsigned char*>(inputBuffer.c_str()), inputBuffer.size())));

                inputBuffer = "";
                segmentID++;
            }
        }

        if(inputBuffer.size() != 0)
                segments.push_back(ISegment(segmentID, base64_encode(reinterpret_cast<const unsigned char*>(inputBuffer.c_str()), inputBuffer.size())));

        /* Creating socket */

        int sock = 0;
        struct sockaddr_in options;

        //Create socket
        if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            throw new IException(SOCK_CANNOT_CREATE);

        fcntl(sock, F_SETFL, O_NONBLOCK);

        options.sin_family = AF_INET;
        options.sin_addr.s_addr = INADDR_ANY;
        options.sin_port = htons(param.sPort);

        //Bind socket
        if(bind(sock, (struct sockaddr*)&options, sizeof(options)) < 0)
            throw new IException(SOCK_CANNOT_BIND);

        //Create set
        fd_set INPUT;
        FD_ZERO(&INPUT);
        FD_SET(sock, &INPUT);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100;

        string ackBuffer = "";
        bool active = true;

        cerr << "[*] Starting transfer (segments: " << segments.size() << ", window size: " << SEGMENT_WIN_STATUS << ")" << endl;

        //Any incoming acks
        while(select(sock + 1, &INPUT, NULL, NULL, &tv) >= 0){
            //Pending acks on sock
            if(FD_ISSET(sock, &INPUT)){
                ackBuffer += receiveUDP(sock);

                //Loaded some ACKs
                if(ackBuffer.size() > 0){
                    size_t segmentStartPos = ackBuffer.find("<rdt-segment id=\"xkolac12\">");
                    size_t segmentEndPos = ackBuffer.find("</rdt-segment>");

                    //Valid ACK
                    if(segmentStartPos != string::npos && segmentEndPos != string::npos){
                        string segmentRaw = ackBuffer.substr(segmentStartPos, segmentEndPos - segmentStartPos + 14);

                        //Crop current segment from buffer
                        ackBuffer.erase(ackBuffer.begin() + segmentStartPos, ackBuffer.begin() + segmentEndPos + 14);

                        //Find sequence number
                        size_t headerStartPos = segmentRaw.find("<header");
                        headerStartPos = segmentRaw.find(">", headerStartPos + 1);
                        size_t headerEndPos = segmentRaw.find("</header>");
                        string strSeq = segmentRaw.substr(headerStartPos + 1, headerEndPos - headerStartPos - 1);

                        //Cast string to unsigned long
                        stringstream stream;
                        unsigned int seq;
                        stream << strSeq;
                        stream >> seq;

                        //Find data
                        size_t dataStartPos = segmentRaw.find("<data>");
                        size_t dataEndPos = segmentRaw.find("</data>");
                        string data = segmentRaw.substr(dataStartPos + 6, dataEndPos - dataStartPos - 6);

                        cerr << "[*] Received ACK #" << seq << endl;

                        //Got final ACK, exit
                        if(seq == 0)
                            break;

                        long int timerNow = getNow();

                        //Add new timeout
                        for(deque<ISegment>::iterator p = window.begin(); p != window.end(); p++){
                            if((*p).getSeq() == seq){
                                long int timerSegment = (*p).getTimer();

                                cout << "Timer: " << timerNow << "|" << timerSegment << "|" << timerNow - timerSegment << endl;

                                timers.push_front(timerNow - timerSegment);
                                if(timers.size() > 3)
                                    timers.pop_back();

                                SEGMENT_TACK_STATUS = getTi(timers);
                                cerr << "[*] Timeout was changed to: " << SEGMENT_TACK_STATUS << endl;

                                break;
                            }
                        }


                        deque<ISegment>::iterator w;
                        while(window.size() != 0 && window.at(0).getSeq() <= seq)
                            window.pop_front();

                        SEGMENT_WIN_STATUS++;
                        cerr << "[*] Window size was increased to: " << SEGMENT_WIN_STATUS << endl;
                    }

                }
            }

            //Check all timers
            for(deque<ISegment>::iterator p = window.begin(); p != window.end(); p++){
                long int timerNow = getNow();
                //When was packet send?
                long int timerSegment = (*p).getTimer();

                if((timerNow - timerSegment) > SEGMENT_TACK_STATUS){
                    SEGMENT_WIN_STATUS = (int)(SEGMENT_WIN_STATUS / 3);

                    if(SEGMENT_WIN_STATUS == 0)
                        SEGMENT_WIN_STATUS = 1;

                    cerr << "[*] Window size was decreased to: " << SEGMENT_WIN_STATUS << endl;

                    (*p).setTimer();
                    sendUDP(sock, param.dPort, (*p));
                }
            }

            //If can fill sliding window
            if(window.size() < SEGMENT_WIN_STATUS && segments.size() > 0){
                while(window.size() < SEGMENT_WIN_STATUS && segments.size() > 0){
                    ISegment segment = segments.at(0);

                    segment.setWin(SEGMENT_WIN_STATUS);
                    segment.setTimer();

                    window.push_back(segment);
                    segments.pop_front();

                    sendUDP(sock, param.dPort, segment);
                }
            }

            //Successfully sended, so lets go to send 0 seq
            if(active && segments.empty() && window.empty()){
                ISegment final(0, "");

                window.push_back(final);
                sendUDP(sock, param.dPort, final);

                active = false;
            }

            FD_ZERO(&INPUT);
            FD_SET(sock, &INPUT);

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

void printHelp(){

}

void sendUDP(int sock, unsigned int port, ISegment segment){
    struct stat info;

    if(fstat(sock, &info) != 0)
        throw new IException(SOCK_INVALID_SOCKET_STATE);

    struct sockaddr_in options;
    options.sin_family = AF_INET;
    //options.sin_addr.s_addr = htonl(0x7f000001);
    options.sin_addr.s_addr = inet_addr("127.0.0.1");
    options.sin_port = htons(port);

    string buffer = segment.getSegment();

    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&options, sizeof(options));

    cerr << "[*] Sended segment #" << segment.getData() << endl;
}

string receiveUDP(int socket){
    char rawBuffer[READ_LINE];
    string buffer;

    struct stat info;
    if(fstat(socket, &info) != 0)
        throw new IException(SOCK_INVALID_SOCKET_STATE);

    struct sockaddr_in options;
    socklen_t optionsLen = sizeof(options);

    int count = recvfrom(socket, &rawBuffer, READ_LINE, MSG_DONTWAIT, (struct sockaddr*)&options, &optionsLen);
    rawBuffer[count] = '\0';
    buffer = string(rawBuffer);

    return buffer;
}

long int getNow(){
    struct timeval now;
    gettimeofday(&now, NULL);

    long int seconds = now.tv_sec;
    long int useconds = now.tv_usec;
    return (seconds * 1000 + useconds / 1000);
}

long int getTi(deque<long int> timers){
    if(timers.size() < 3)
        return 1000;

    cout << timers.at(0) << "|" << timers.at(1) << "|" << timers.at(2) << endl;

    double Ti = (timers.at(1) + ((2 * timers.at(0) + timers.at(1) + timers.at(2)) / 4.0)) / 2.0;

    long int T = (long int)(Ti * 1.3);

    return (T == 0)? 1 : T;
}
