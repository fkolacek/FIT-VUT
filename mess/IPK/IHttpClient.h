/**
 * @file	IHttpClient.h
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/13
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IHTTPCLIENT_H
#define IHTTPCLIENT_H

#include <string>
#include <fstream>

#include "IException.h"
#include "ISocket.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

using namespace std;

class IHttpClient
{
    public:
        IHttpClient(string, bool = false, int = 1);
        virtual ~IHttpClient();

        void setMaxRedirects(int);

        void process();
        void cleanup();
    private:
        bool verbose;
        string URI;
        int maxRedirects;

        ISocket* sock;
        IHttpRequest* request;
        IHttpResponse* response;
};

#endif // IHTTPCLIENT_H
