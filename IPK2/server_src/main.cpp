/**
 * @file	main.cpp
 * @brief	Projekt IPK 2 - Server
 *
 * @date	2013/03/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <regex.h>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>

#include "IUsersDatabase.h"

#define UNUSED(p){(p) = (p);}

using namespace std;

// Global
int SERVER_SOCKET = 0;
list<pid_t> SLAVES;
IUsersDatabase DATABASE("/etc/passwd");

enum options{
	OPTION_LOGIN		= 1,
	OPTION_USER			= 2,
	OPTION_GROUP		= 4,
	OPTION_NAME			= 8,
	OPTION_HOME			= 16,
	OPTION_SHELL		= 32
};

enum order{
    ORDER_LOGIN	= 1,
    ORDER_UID	= 2
};

int socketWrite(int, string);
int socketRead(int, string&, unsigned int = 0);

void handleConnection(int);
string trim(string);

void checkSlaves();
void freeResources();
void programAbort(int);

int main(int argc, char* argv[]){
    if(argc != 3 || strcmp(argv[1], "-p") != 0){
        cerr << "Usage: " << argv[0] << " -p [port]" << endl;
        exit(1);
    }

    int localPort = atoi(argv[2]);

    if(localPort < 1 || localPort > 65535){
        cerr << "[!] Invalid port" << endl;
        exit(1);
    }

    signal(SIGTERM, programAbort);
    signal(SIGINT, programAbort);

    cout << "[?] Starting server on port " << localPort << endl;

    try{
        //Try to load database
        DATABASE.loadData();

        if((SERVER_SOCKET = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            throw new IException(SOCK_CANNOT_CREATE);

        int x = 1;
        setsockopt(SERVER_SOCKET, SOL_SOCKET, SO_REUSEADDR,(char*)&x, sizeof(x));

        struct sockaddr_in serverOptions;
        serverOptions.sin_addr.s_addr = INADDR_ANY;
        serverOptions.sin_family = AF_INET;
        serverOptions.sin_port = htons(localPort);

        if(bind(SERVER_SOCKET, (struct sockaddr*)&(serverOptions), sizeof(serverOptions)) < 0)
            throw new IException(SOCK_CANNOT_BIND);

        if(listen(SERVER_SOCKET, 10) < 0)
            throw new IException(SOCK_CANNOT_LISTEN);

        fd_set set;
        timeval tv;

        // Main server loop
        do{
            checkSlaves();

            FD_ZERO(&set);
            FD_SET(SERVER_SOCKET, &set);

            tv.tv_sec = 1;
            tv.tv_usec = 0;

            int count = select(SERVER_SOCKET + 1, &set, NULL, NULL, &tv);

            if(count < 0)
                throw new IException(SOCK_CANNOT_SELECT);
            //Timeout
            else if(count == 0)
                continue;
            else{
                int clientSocket = 0;
                struct sockaddr_in clientOptions;
                socklen_t clientAddrlen = sizeof(clientOptions);

                //Accept incoming connection
                if((clientSocket = accept(SERVER_SOCKET, (struct sockaddr*)&(clientOptions), &clientAddrlen)) < 0)
                    throw new IException(SOCK_CANNOT_ACCEPT);

                pid_t pid = fork();

                //Fork error
                if(pid == -1)
                    throw new IException(SYSTEM_CANNOT_FORK);
                // Slave
                else if(pid == 0)
                    handleConnection(clientSocket);
                //Main process
                else{
                    cout << "[?] #"<< pid << " Incoming connection from: " << inet_ntoa(clientOptions.sin_addr) << endl;
                    SLAVES.push_back(pid);
                }
            }
        }while(true);

        shutdown(SERVER_SOCKET, SHUT_RDWR);
        close(SERVER_SOCKET);
    }
    catch(IException *exception){
        cerr << "[!] Exception #" << exception->getCode() << ": "<< exception->getMessage() << endl;

        freeResources();

        delete exception;

        exit(1);
    }

    catch(...){
        cerr << "[!] Cautched undefined exception!" << endl;

        freeResources();

        exit(1);
    }

    return 0;
}

void handleConnection(int sock){
    int optionTransferMode = 0;
    int optionTransferOrder;

    string line;
    timeval tv;
    fd_set set;

    regmatch_t pmatch[3];
    regex_t re;

    if(regcomp(&re, "^([a-zA-Z0-9\\-]+)\\: (.+)$", REG_EXTENDED) != 0)
        throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

    do{
        FD_ZERO(&set);
        FD_SET(sock, &set);

        //Timeout for incoming connection
        tv.tv_sec = 60;
        tv.tv_usec = 0;

        int modify = select(sock + 1, &set, NULL, NULL, &tv);

        //Timeout
        if(modify == 0){
            socketWrite(sock, "408 Connection timeout\n");
            break;
        }
        else{
            if(!socketRead(sock, line))
                break;

            line = trim(line);

            if(line == "Quit")
                break;
            else if(regexec(&re, line.c_str(), 3, pmatch, 0) != REG_NOMATCH){
                string key = line.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
                string val = line.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);

                if(key == "Transfer-mode"){
                    optionTransferMode = atoi(val.c_str());
                }
                else if(key == "Transfer-order"){
                    optionTransferOrder = atoi(val.c_str());
                }
                else if(key == "Get"){
                    cerr << "Get: " << val << " (Mode: " << optionTransferMode << ", order: " << optionTransferOrder << ")" << endl;

                    vector<IUser> users;
                    IUser user;

                    if(optionTransferOrder & ORDER_UID){
                        users = DATABASE.getUserByUID(val);
                    }
                    else
                        users = DATABASE.getUserByLogin(val);

                    stringstream response;

                    if(users.size() != 0){

                        //GARBAGE
                        for(unsigned int i = 0; i < users.size(); i++){

                            user = users.at(i);

                            response << "200 ";

                            if(optionTransferMode == 0)
                                response << user.getLogin() << ":" << user.getUID() << ":" << user.getGID() << ":" << user.getUsername() << ":" << user.getDirectory() << ":" << user.getShell() << "\n";
                            else{
                                vector<string> values;

                                if(optionTransferMode & OPTION_LOGIN) values.push_back(user.getLogin());
                                if(optionTransferMode & OPTION_USER) values.push_back(user.getUID());
                                if(optionTransferMode & OPTION_GROUP) values.push_back(user.getGID());
                                if(optionTransferMode & OPTION_NAME) values.push_back(user.getUsername());
                                if(optionTransferMode & OPTION_HOME) values.push_back(user.getDirectory());
                                if(optionTransferMode & OPTION_SHELL) values.push_back(user.getShell());

                                bool first = true;
                                for(vector<string>::iterator p = values.begin(); p!= values.end();p++)
                                    if(first){
                                        response << *p;
                                        first = !first;
                                    }
                                    else
                                        response << ":" << *p;

                                response << "\n";
                            }
                        }
                        response << "END\n";
                    }
                    else
                        response << "404 User with " << ((optionTransferOrder == ORDER_UID)? "UID" : "login") << ": " << val <<" not found\nEND\n";

                    socketWrite(sock, response.str());
                }
                //Another headers
                else{

                }
            }
            //Unknown request
            else
                socketWrite(sock, "400 Bad Request\n");

            //cerr << "< " << line << endl;
        }
    }while(true);

    regfree(&re);
    shutdown(sock, SHUT_RDWR);
    close(sock);
    exit(0);
}

string trim(string data){
    if(data.find_last_not_of(" \n\r\t") != string::npos)
        data.erase(data.find_last_not_of(" \n\r\t")+1);

    return data;
}

int socketWrite(int sock, string data){
    return write(sock, data.c_str(), data.size());
}

int socketRead(int sock, string& buffer, unsigned int size){
    char buf;
    buffer = "";
    while(read(sock, &buf, 1)){
        buffer += buf;

        if(size == 0 && buf == '\n')
            break;

        if(buffer.size() == size)
            break;
    }

    return buffer.size();
}

void checkSlaves(){
    list<pid_t>::iterator p = SLAVES.begin();
    list<pid_t>::iterator tmp;

    while(p != SLAVES.end()){
        tmp = p++;

        pid_t slave = waitpid(*tmp, NULL, WNOHANG);

        if(slave == *tmp){
            cerr << "[?] Killing slave #" << slave << endl;
            SLAVES.erase(tmp);
        }
    }
}

void freeResources(){
    if(SERVER_SOCKET)
        close(SERVER_SOCKET);

    SERVER_SOCKET = 0;

    for(list<pid_t>::iterator p = SLAVES.begin(); p != SLAVES.end(); p++)
        kill(*p, SIGTERM);

    for(list<pid_t>::iterator p = SLAVES.begin(); p != SLAVES.end(); p++)
        waitpid(*p, NULL, 0);
}

void programAbort(int sig){
    UNUSED(sig);

    cerr << "Aborting.." << endl;

    freeResources();

    exit(1);
}
