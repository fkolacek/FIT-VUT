/**
 * @file	ISocket.h
 * @brief	Projekt IPK 2 - Client
 * @date	2013/02/23
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef ISOCKET_H
#define ISOCKET_H

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sstream>

#include "IException.h"

using namespace std;

enum ISocketStates{
    SOCKET_UNINITIALIZED_STATE,
    SOCKET_UNCONNECTED_STATE,
    SOCKET_HOST_LOOKUP_STATE,
    SOCKET_CONNECTING_STATE,
    SOCKET_CONNECTED_STATE
};

class ISocket
{
    public:
        ISocket();
        ISocket(string, int);
        virtual ~ISocket();

        void connectToRemoteHost(string, int);
        void connectToRemoteHost();
        void disconnectFromHost();

        int send(string);
        int receive(string&, unsigned int = 0);

        ISocketStates getState();

        //ostream& operator<<(ostream&);
        //istream& operator>>(istream&);
    private:
        string remoteHost;
        int remotePort;

        int handler;
        struct sockaddr_in options;

        ISocketStates state;
};

#endif // ISOCKET_H
