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

#define gSS 512

#include "udt.h"

using namespace std;

typedef struct params{
    int sourceP;
    int destinationP;
} paramsT;

void setT(unsigned long int);
long int getT();
int getCurrentMS();
paramsT parseArg(int, char*[]);
deque<UDT> loadFromFile(int, int&);
string recFrom();
void sendT(int, UDT);
int toInt(string);

int gSDS = 350, gSWS = 10;
int sock = 0;

deque<unsigned long int> gST;

int main(int argc, char* argv[]){
	paramsT param = parseArg(argc, argv);

    struct sockaddr_in settings;
	int packetCounter = 1;
	deque<UDT> packets = loadFromFile(gSWS, packetCounter);
	
	fd_set qu;
	FD_ZERO(&qu);
	FD_SET(sock, &qu);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 200;
	
	deque<UDT> window;
	bool transferred = false;
	string buffer;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		cerr << "Cannot create socket" << endl;
		exit(1);
	}

	fcntl(sock, F_SETFL, O_NONBLOCK);

	settings.sin_family = AF_INET;
	settings.sin_port = htons(param.sourceP);
	settings.sin_addr.s_addr = INADDR_ANY;
	
	const string pattern = "<header sn=\"([\\.e0-9\\+\\-]+)\" ack=\"([\\.e0-9]+)\" win=\"([\\.e0-9]+)\" tack=\"([0-9e\\+\\-\\.]+)\"></header><data>(.+)</data>";	

	if(bind(sock, (struct sockaddr*)&settings, sizeof(settings)) < 0){
		cerr << "Cannot bind port" << endl;
		exit(1);
	}

	while(select(sock + 1, &qu, NULL, NULL, &tv) >= 0){
		int dCounter = 0;
		
		if(FD_ISSET(sock, &qu)){
			buffer += recFrom();			

			if(buffer.size() > 0){
				string strSTART = "<rdt-segment id=\"xmatya03\">";
				string strEND = "</rdt-segment>";
				
				int packetSP = buffer.find(strSTART);
				int packetEP = buffer.find(strEND);
											
				if(packetSP != string::npos && packetEP != string::npos){
					string packetRaw = buffer.substr(packetSP, packetEP - packetSP + 21 - 7);
					regmatch_t pmatch[5];
					regex_t re;

					buffer.erase(buffer.begin() + packetSP, buffer.begin() + packetEP + 21 - 7);

					if(regcomp(&re, pattern.c_str(), REG_EXTENDED) != 0){
						cerr << "Cannot compile regex" << endl;
						exit(1);
					}

					if(regexec(&re, packetRaw.c_str(), 5, pmatch, 0) != REG_NOMATCH){
						string sn, tack;
						long int usn, utack;
						
						tack = packetRaw.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);
						sn = packetRaw.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);

						usn = toInt(sn);
						utack = toInt(tack);

						cerr << "[*] Received ACK #" << usn << endl;
						
						if(usn != 0)
							setT(getCurrentMS() - utack);

						if(usn == 0) break;

						while(window.size() != 0 && window.at(0).getSN() <= usn)
							window.pop_front();

							gSWS++;
					}
					else{
						cerr << "Invalid response" << endl;
						exit(1);
                   }
				}
			}
		}
		
		deque<UDT>::iterator it;
		for(it = window.begin(); it != window.end(); it++){
			dCounter++;
			
			int timerNow = getCurrentMS();
			long int timer = (*it).getT();
			long int timeout = getT();

			if( (timerNow - timer) > timeout){
				gSWS = (int)((gSWS) / 3);

				if(gSWS < 1)
					gSWS = 1;

				(*it).setT(getCurrentMS());
				sendT(param.destinationP, (*it));
			}
			
			if(dCounter >= gSWS)
				break;

		}

		if(packets.size() > 0 && window.size() < gSWS){
			while(window.size() < gSWS && packets.size() > 0){
				UDT packet = packets.at(0);
				packet.setT(getCurrentMS());
				packet.setW(gSWS);				

				window.push_back(packet);

				packets.pop_front();				

				deque<UDT> t = loadFromFile(1, packetCounter);
				sendT(param.destinationP, packet);

				if(t.size() > 0){
					packets.push_back(t.at(0));
				}
			}
		}

		if(!transferred && packets.empty() && window.empty()){
			transferred = true;
			UDT finalPacket(0, "final");
			finalPacket.setT(getCurrentMS());
			finalPacket.setW(gSWS);						

			sendT(param.destinationP, finalPacket);
			window.push_back(finalPacket);			
		}
		
		tv.tv_sec = 0;
		tv.tv_usec = 200;

		FD_ZERO(&qu);
		FD_SET(sock, &qu);


    }

    return 0;
}

paramsT parseArg(int argc, char* argv[]){
    paramsT param;
    param.sourceP = 0;
    param.destinationP = 0;
	
    if(argc == 1){
		cerr << "Invalid arguments" << endl;
		exit(1);		
	}

    int c;

	while((c = getopt(argc, argv, "s:d:")) != EOF){
		switch(c){
			case 's':
				param.sourceP = atoi(optarg);
				break;
			case 'd':
				param.destinationP = atoi(optarg);
				break;
			default:
				cerr << "Invalid arguments" << endl;
				exit(1);
				break;
		}
	}

    if(param.sourceP == 0 || param.destinationP == 0){
		cerr << "Invalid arguments" << endl;
		exit(1);
	}

    return param;
}

deque<UDT> loadFromFile(int maxCount, int& packetCounter){
    deque<UDT> packets;

    int input;
    string buffer;

    while(packets.size() < maxCount && (input = fgetc(stdin)) != EOF){
        buffer += input;

        if(buffer.size() == gSDS){
            packets.push_back(UDT(packetCounter, buffer));
			
			packetCounter++;
            buffer.clear();            
        }
    }

    if(buffer.size() > 0)
        packets.push_back(UDT(packetCounter, buffer));

    return packets;
}

void setT(unsigned long int timeout){
	gST.push_front(timeout);
	
    if(gST.size() > 3)
        gST.pop_back();	
}

 long int getT(){
    if(gST.size() != 3){
        return 100;
	}
	else{
		double t = (gST.at(1) + ((2 * gST.at(0) + gST.at(1) + gST.at(2)) / 4.0)) / 2.0;
		unsigned long int t2 = (unsigned long int)(t * 1.3);

		if(t2 > 2000 || t2 <= 0)
			return 100;
		else
			return t2;
	}
}

int getCurrentMS(){
    unsigned int tmp;
    struct timeval now;

    gettimeofday(&now, NULL);

    tmp = (now.tv_sec) * 1000.0 + (now.tv_usec) / 1000.0;

    return tmp;
}

string recFrom(){
    char rawBuffer[gSS];
    string buffer;
    
    struct sockaddr_in settings;
    socklen_t settingsLen = sizeof(settings);

    struct stat info;
    if(fstat(sock, &info) != 0){
		cerr << "Socket invalid" << endl;
		exit(1);		
	}

    int count = recvfrom(sock, &rawBuffer, gSS, MSG_DONTWAIT, (struct sockaddr*)&settings, &settingsLen);
    rawBuffer[count] = '\0';
    buffer = string(rawBuffer);

    return buffer;
}

void sendT(int port, UDT packet){
    struct stat info;

    struct sockaddr_in settings;
    settings.sin_family = AF_INET;
    settings.sin_addr.s_addr = inet_addr("127.0.0.1");
    settings.sin_port = htons(port);

    string buffer = packet.toS();
	
    if(fstat(sock, &info) != 0){
		cerr << "Socket invalid" << endl;
		exit(1);		
	}
	cerr << "Sending segment #" << packet.getSN() << endl;

    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr*)&settings, sizeof(settings));
}

int toInt(string input){
    stringstream stream;
    int output;
    stream << input;
    stream >> output;

    return output;
}