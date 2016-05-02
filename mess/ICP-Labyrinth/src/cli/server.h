/**
  * @file server.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for Server class
  */
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <dirent.h>
#include <fstream>
#include "exception.h"
#include "client.h"
#include "game.h"
#include "utils.h"

/**
* @brief Class represents gaming Server
*/
class Server{
    public:
        Server(int = 32890, int = 1024);
        virtual ~Server();
        void start();

        /**
        * @brief Setter for port attribute
        * @param int port
        */
        inline void setPort(int port){ this->port = port; }
        /**
        * @brief Setter for bufferSize attribute
        * @param int bufferSize
        */
        inline void setBufferSize(int bufferSize){ this->bufferSize = bufferSize; }
        /**
        * @brief Getter for port attribute
        * @return int port
        */
        inline int getPort(){ return this->port; }
        /**
        * @brief Getter for clients attribute
        * @param int clients
        */
        inline int getClients(){ return this->clients.size(); }
    private:
        int port;
        vector<Client> clients;
        vector<Client> acceptedClients;

        int serverSocket;
        sockaddr_in serverSocketOpts;
        fd_set serverSocketSet;
        timeval serverSocketTimer;

        int serverSocketMaxFd;
        int bufferSize;

        Game* currentGame;

        string socketRead(int);
        void socketWrite(int, string);
        void handleRequest(Client&, string);
};
#endif // SERVER_H
