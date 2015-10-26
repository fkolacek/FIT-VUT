/**
 * @file	ISocket.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/13
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "ISocket.h"

ISocket::ISocket(){
    this->handler = 0;
    this->state = SOCKET_UNINITIALIZED_STATE;
}

ISocket::ISocket(string remoteHost, int remotePort){
    this->state = SOCKET_UNCONNECTED_STATE;
    this->remoteHost = remoteHost;
    this->remotePort = remotePort;
}

ISocket::~ISocket(){
    this->disconnectFromHost();
}

void ISocket::connectToRemoteHost(string remoteHost, int remotePort){
    this->state = SOCKET_UNCONNECTED_STATE;
    this->remoteHost = remoteHost;
    this->remotePort = remotePort;

    this->connectToRemoteHost();
}

void ISocket::connectToRemoteHost(){
    if(this->state != SOCKET_UNCONNECTED_STATE)
        throw new IException(SOCK_INVALID_SOCKET_STATE);

    if((this->handler = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw new IException(SOCK_CANNOT_CREATE);

    this->state = SOCKET_HOST_LOOKUP_STATE;

    struct hostent* hostInfo;

    if((hostInfo = gethostbyname(this->remoteHost.c_str())) == NULL)
        throw new IException(SOCK_CANNOT_RESOLVE_ADDRESS);

    //memcpy(&(this->options), hostInfo->h_addr, hostInfo->h_length);
    memcpy(&(this->options.sin_addr), (struct in_addr*)hostInfo->h_addr_list[0], sizeof(this->options.sin_addr));

    this->options.sin_family = AF_INET;
    this->options.sin_port = htons(this->remotePort);

    this->state = SOCKET_CONNECTING_STATE;

    if(connect(this->handler, (struct sockaddr*)&(this->options), sizeof(this->options)) < 0)
        throw new IException(SOCK_CANNOT_CONNECT_TO_REMOTE_HOST);

    this->state = SOCKET_CONNECTED_STATE;
}

void ISocket::disconnectFromHost(){
    if(this->state <= SOCKET_UNCONNECTED_STATE)
        return;

    shutdown(this->handler, SHUT_RDWR);
    close(this->handler);

    this->state = SOCKET_UNCONNECTED_STATE;
}

int ISocket::send(string data){
    if(this->state != SOCKET_CONNECTED_STATE)
        throw new IException(SOCK_CANNOT_WRITE);

    return write(this->handler, data.c_str(), data.size());
}

int ISocket::receive(string& buffer, unsigned int size){
    if(this->state != SOCKET_CONNECTED_STATE)
        throw new IException(SOCK_CANNOT_READ);

    char buf;
    buffer = "";
    while(read(this->handler, &buf, 1)){
        buffer += buf;

        if(size == 0 && buf == '\n')
            break;

        if(buffer.size() == size)
            break;
    }


    return buffer.size();
}

ISocketStates ISocket::getState(){
    return this->state;
}
