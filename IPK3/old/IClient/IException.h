/**
 * @file	IExceptiongit ta.h
 * @brief	Projekt IPK 2 - Client
 *
 * @date	2013/03/02
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IEXCEPTION_H
#define IEXCEPTION_H

#include <string>

using namespace std;

enum IErrorCodes{
    PARAM_INVALID_SOURCE_PORT,
    PARAM_INVALID_DESTINATION_PORT,
    PARAM_INVALID,

    SOCK_INVALID_SOCKET_STATE,
    SOCK_CANNOT_CREATE,
    SOCK_CANNOT_RESOLVE_ADDRESS,
    SOCK_CANNOT_CONNECT_TO_REMOTE_HOST,
    SOCK_CANNOT_READ,
    SOCK_CANNOT_WRITE,
    SOCK_CANNOT_BIND,

    RESPONSE_INVALID,

    SYSTEM_CANNOT_COMPILE_REGEX,
    SYSTEM_CANNOT_OPEN_FILE,
    SYSTEM_UNDEFINED_EXCEPTION
};

class IException
{
    public:
        IException(int, string = "Undefined", int = 0);
        IException(string, int = 0, string = "Undefined", int = 0);
        virtual ~IException();

        string getMessage();
        int getCode();
        string getFile();
        int getLine();

    protected:
        string message;
        int code;
        string file;
        int line;
};

#endif // IEXCEPTION_H
