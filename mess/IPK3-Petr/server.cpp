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
#include <signal.h>

#include <map>

#define gSS 512

#include "udt.h"

using namespace std;

typedef struct params{
    unsigned int sourcePort;
    unsigned int destionationPort;
} paramsT;

paramsT parseArg(int, char*[]);

string recFrom();
void sendT(unsigned int, UDT);
void programAbort(int);

unsigned int toU(string);

int sock = 0;

int main(int argc, char* argv[]){
	signal(SIGTERM, programAbort);
	signal(SIGINT, programAbort);

	paramsT param = parseArg(argc, argv);

    struct sockaddr_in settings;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		cerr << "Cannot create socket" << endl;
		exit(1);
	}

	fcntl(sock, F_SETFL, O_NONBLOCK);

	settings.sin_family = AF_INET;
	settings.sin_addr.s_addr = INADDR_ANY;
	settings.sin_port = htons(param.sourcePort);

	if(bind(sock, (struct sockaddr*)&settings, sizeof(settings)) < 0){
		cerr << "Cannot bind port" << endl;
		exit(1);
	}

	fd_set qu;
	FD_ZERO(&qu);
	FD_SET(sock, &qu);

    string buffer;
	map<unsigned int, UDT> packets;
	unsigned int lastAck = 0, toPrint = 1;
	
	const string pattern = "<header sn=\"([\\.e0-9\\+\\-]+)\" ack=\"([\\.e0-9]+)\" win=\"([\\.e0-9]+)\" tack=\"([0-9e\\+\\-\\.]+)\"></header><data>(.+)</data>";

	while(select(sock + 1, &qu, NULL, NULL, NULL) >= 0){
		if(FD_ISSET(sock, &qu)){
			buffer += recFrom();

			if(buffer.size() > 0){
				string strSTART = "<rdt-segment id=\"xmatya03\">";
				string strEND = "</rdt-segment>";
				
				int packetSP = buffer.find(strSTART);
				int packetEP = buffer.find(strEND);

				if(packetSP != string::npos && packetEP != string::npos){
					string packetRaw = buffer.substr(packetSP, packetEP - packetSP + 14);

					buffer.erase(buffer.begin() + packetSP, buffer.begin() + packetEP + 14);
					regmatch_t regexMatch[6];
					regex_t regex;

					if(regcomp(&regex, pattern.c_str(), REG_EXTENDED) != 0){
						cerr << "Cannot compile regex" << endl;
						exit(1);
					}

					if(regexec(&regex, packetRaw.c_str(), 6, regexMatch, 0) != REG_NOMATCH){
						string sn, ack, win, tack, data;
						unsigned int packetID = 0;

						sn = packetRaw.substr(regexMatch[1].rm_so, regexMatch[1].rm_eo - regexMatch[1].rm_so);
						win = packetRaw.substr(regexMatch[3].rm_so, regexMatch[3].rm_eo - regexMatch[3].rm_so);
						tack = packetRaw.substr(regexMatch[4].rm_so, regexMatch[4].rm_eo - regexMatch[4].rm_so);
						data = packetRaw.substr(regexMatch[5].rm_so, regexMatch[5].rm_eo - regexMatch[5].rm_so);

						unsigned int usn = toU(sn);
						unsigned int uwin = toU(win);
						unsigned int utack = toU(tack);

						UDT packet(usn, data);
						packet.setT(utack);
						packet.setW(uwin);

						if(usn != 0)
							packets.insert(pair<unsigned int, UDT>(usn, packet));

						cerr << "[*] Received packet #" << usn << endl;						

						while(packets.find(packetID + 1) != packets.end())
							packetID++;

						//Skoncime
						if(usn == 0){
							packets.clear();
							
							packetID = 0;
							toPrint = 1;							
						}

						if(lastAck != packetID){
							sendT(param.destionationPort, packets[packetID]);
							lastAck = packetID;
						}

						//Vypiseme
						while(packets.find(toPrint) != packets.end()){
							cout << packets[toPrint].getD();
							cout.flush();

							toPrint++;
						}

					}
					else{
						cerr << "Invalid response" << endl;
						exit(1);
                   }
				}
			}
		}

		FD_ZERO(&qu);
		FD_SET(sock, &qu);
	}

    return 0;
}

paramsT parseArg(int argc, char* argv[]){
    paramsT param;
    param.sourcePort = 0;
    param.destionationPort = 0;
	
    if(argc == 1){
		cerr << "Invalid arguments" << endl;
		exit(1);		
	}

    int c;

	while((c = getopt(argc, argv, "s:d:")) != EOF){
		switch(c){
			case 's':
				param.sourcePort = atoi(optarg);
				break;
			case 'd':
				param.destionationPort = atoi(optarg);
				break;
			default:
				cerr << "Invalid arguments" << endl;
				exit(1);
				break;
		}
	}

    if(param.sourcePort == 0 || param.destionationPort == 0){
		cerr << "Invalid arguments" << endl;
		exit(1);
	}

    return param;
}

string recFrom(){
    char rawBuffer[gSS];
    string buffer;

    struct stat info;
    if(fstat(sock, &info) != 0){
		cerr << "Socket invalid" << endl;
		exit(1);		
	}

    struct sockaddr_in settings;
    socklen_t settingsLen = sizeof(settings);

    int count = recvfrom(sock, &rawBuffer, gSS, MSG_DONTWAIT, (struct sockaddr*)&settings, &settingsLen);
    rawBuffer[count] = '\0';
    buffer = string(rawBuffer);

    return buffer;
}

void sendT(unsigned int port, UDT packet){
    struct stat info;
    if(fstat(sock, &info) != 0){
		cerr << "Socket invalid" << endl;
		exit(1);		
	}

    struct sockaddr_in settings;
    settings.sin_family = AF_INET;
    settings.sin_addr.s_addr = inet_addr("127.0.0.1");
    settings.sin_port = htons(port);

    string buffer = packet.toA();

    if(packet.getT() == 0 && packet.getSN() != 0)
        return;
	
    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&settings, sizeof(settings));

    cerr << "[*] Sended ack #" << packet.getSN() << endl;
}

unsigned int toU(string input){
    stringstream stream;
    unsigned int output;
    stream << input;
    stream >> output;

    return output;
}

void programAbort(int sig){
    exit(sig);
}
