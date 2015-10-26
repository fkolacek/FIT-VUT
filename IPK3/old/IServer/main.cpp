/**
 * @file	main.cpp
 * @brief	Projekt IPK 3 - Server
 *
 * @date	2013/05/06
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "../IClient/IConfig.h"
#include "../IClient/IException.h"
#include "../IClient/ISegment.h"
#include "../IClient/base64.h"

using namespace std;

typedef struct params{
    unsigned int sPort;
    unsigned int dPort;

    string sHost;
} tParams;

tParams getParam(int, char*[]);

void prinHelp();

void sendUDP(int, unsigned int, ISegment);
string receiveUDP(int);
void programAbort(int);

int main(int argc, char* argv[]){
    try{
        signal(SIGTERM, programAbort);
        signal(SIGINT, programAbort);

        tParams param = getParam(argc, argv);

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

        string inputBuffer = "";
        map<unsigned int, ISegment> segments;
        unsigned int lastAckID = 0;

        //Any incoming acks
        while(select(sock + 1, &INPUT, NULL, NULL, NULL) >= 0){
            //Data avalaible on socket
            if(FD_ISSET(sock, &INPUT)){
                inputBuffer += receiveUDP(sock);

                //Loaded some data
                if(inputBuffer.size() > 0){
                    size_t segmentStartPos = inputBuffer.find("<rdt-segment id=\"xkolac12\">");
                    size_t segmentEndPos = inputBuffer.find("</rdt-segment>");

                    //Valid segment
                    if(segmentStartPos != string::npos && segmentEndPos != string::npos){
                        string segmentRaw = inputBuffer.substr(segmentStartPos, segmentEndPos - segmentStartPos + 14);

                        //Crop current segment from buffer
                        inputBuffer.erase(inputBuffer.begin() + segmentStartPos, inputBuffer.begin() + segmentEndPos + 14);

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

                        ISegment segment(seq, data);
                        segments.insert(pair<unsigned int, ISegment>(seq, segment));

                        cerr << "[*] Received segment #" << seq << endl;


                        unsigned int segmentID = lastAckID + 1;

                        if(seq == 0 || segments.find(segmentID) != segments.end()){
                            while(segments.find(segmentID + 1) != segments.end())
                                segmentID++;

                            //Get final segment
                            if(seq == 0){
                                segmentID = 0;

                                for(map<unsigned int, ISegment>::iterator p = segments.begin(); p != segments.end(); p++)
                                    //cout << (p->second).getData();
                                    cout << base64_decode((p->second).getData());

                                cout.flush();
                                segments.clear();
                            }

                            if(lastAckID != segmentID){
                                sendUDP(sock, param.dPort, ISegment(segmentID, "ACK"));
                                lastAckID = segmentID;
                            }
                        }

                    }
                }
            }

            FD_ZERO(&INPUT);
            FD_SET(sock, &INPUT);
        }
    }
    catch(IException* exception){
        cerr << "[!] " << exception->getMessage() << endl;
        delete exception;
        return 1;
    }
    catch(...){
        cerr << "[!] Cautched unhandlered exception" << endl;
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

    segment.setTimer();
    string buffer = segment.getAck();

    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&options, sizeof(options));

    cerr << "[*] Sended ACK #" << segment.getSeq() << endl;
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

void programAbort(int sig){
    sig = sig;

    cerr << "[!] Aborting.." << endl;
    exit(1);
}
