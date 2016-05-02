/**
  * @file client.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for Client class
  */
#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

/**
* @brief Class represents connected Client
*/
class Client{
    public:
        Client(int = 0, string = "");
        virtual ~Client();

        void setUID(int);
        void setClientSocketOpts(sockaddr_in);
        void setName(string);
        void setActive(bool = true);
        void setOwner(bool = true);

        int getUID();
        int getClientSocket();
        sockaddr_in getClientSocketOpts();
        string getName();
        bool isActive();
        bool isOwner();
    private:
        int uid;
        int clientSocket;
        sockaddr_in clientSocketOpts;
        socklen_t addrlen;
        string name;
        bool active;
        bool owner;

        static int uidCounter;
};

#endif // CLIENT_H
