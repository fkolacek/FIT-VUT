/**
 * @file	IHttpResponse.h
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/13
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IHTTPRESPONSE_H
#define IHTTPRESPONSE_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include <regex.h>

#include "IException.h"

using namespace std;

enum IHttpResponseCodes{
    IHTTP_CONTINUE          = 100,
    IHTTP_SWITCHING_PROTOCOLS,

    IHTTP_OK                = 200,
    IHTTP_CREATED,
    IHTTP_ACCEPTED,
    IHTTP_NON_AUTHORITIVE_INFORMATION,
    IHTTP_NO_CONTENT,
    IHTTP_RESET_CONTENT,
    IHTTP_PARTIAL_CONTENT,

    IHTTP_MULTIPLE_CHOICES  = 300,
    IHTTP_MOVED_PERMANENTLY,
    IHTTP_FOUND,
    IHTTP_SEE_OTHER,
    IHTTP_NOT_MODIFIED,
    IHTTP_USE_PROXY,
    IHTTP_UNUSED,
    IHTTP_TEMPORARY_REDIRECT,

    IHTTP_BAD_REQUEST       = 400,
    IHTTP_UNAUTHORIZED,
    IHTTP_PAYMENT_REQUIRED,
    IHTTP_FORBIDDEN,
    IHTTP_NOT_FOUND,
    IHTTP_METHOD_NOT_ALLOWED,
    IHTTP_NOT_ACCEPTABLE,
    IHTTP_PROXY_AUTHENTICATION_REQUIRED,
    IHTTP_REQUEST_TIMEOUT,
    IHTTP_CONFLICT,
    IHTTP_GONE,
    IHTTP_LENGTH_REQUIRED,
    IHTTP_PRECONDITION_FAILED,
    IHTTP_REQUEST_ENTITY_TOO_LARGE,
    IHTTP_REQUEST_URI_TOO_LONG,
    IHTTP_UNSUPPORTED_MEDIA_TYPE,
    IHTTP_REQUEST_RANGE_NOT_SATISFIABLE,
    IHTTP_EXPECTATION_FAILED    = 417,

    IHTTP_INTERNAL_SERVER_ERROR = 500,
    IHTTP_NOT_IMPLEMENTED,
    IHTTP_BAD_GATEWAY,
    IHTTP_SERVICE_UNAVAILABLE,
    IHTTP_GATEWAY_TIMEOUT,
    IHTTP_HTTP_VERSION_NOT_SUPPORTED
};

class IHttpResponse
{
    public:
        IHttpResponse();
        virtual ~IHttpResponse();

        void parseHeader(string);
        string trimHeader(string);
        string getHeader(string);

        string getProtocol();
        string getVersion();
        int getStatusCode();
        string getStatusMsg();

        bool isValid();

        void dumpResponse();
    private:
        string pattern;

        string protocol;
        string version;
        int statusCode;
        string statusMsg;

        map<string, string> headers;
};

#endif // IHTTPRESPONSE_H
