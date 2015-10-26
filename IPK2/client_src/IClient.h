#ifndef ICLIENT_H
#define ICLIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "IException.h"
#include "ISocket.h"

#include "IUser.h"

using namespace std;

class IClient
{
    public:
        IClient(string, unsigned int);
        virtual ~IClient();

        void setTransferMode(unsigned int);
        void setTransferOrder(unsigned int);

        void process(vector<string>, list<int>);
        void cleanup();
    private:
        string trim(string);

        string hostname;
        unsigned int port;
        unsigned int transferMode;
        unsigned int transferOrder;

        ISocket* sock;
};

#endif // ICLIENT_H
