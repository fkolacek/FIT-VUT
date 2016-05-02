/**
 * @file	IHttpRequest.h
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/17
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IHTTPREQUEST_H
#define IHTTPREQUEST_H

#include <string>
#include <sstream>
#include <regex.h>

#include <iostream>

#include "IException.h"

using namespace std;

class IHttpRequest
{
    public:
        IHttpRequest(string);
        virtual ~IHttpRequest();

        void parseURI();

        bool isValid();
        string getRequest();

        string getRawUri();
        string getRawHost();
        string getServerSchema();
        string getServerHost();
        int getServerPort();
        string getServerParam();
        string getServerFile();

        string replaceAll(string, string, string);
        string encode(string);

        void dumpRequest();
    private:
        string pattern;
        bool valid;

        string rawUri;
        string rawHost;

        string serverSchema;
        string serverHost;
        int serverPort;
        string serverParam;
        string serverFile;
};

#endif // IHTTPREQUEST_H
