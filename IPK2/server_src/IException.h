/**
 * @file	IExceptiongit ta.h
 * @brief	Projekt IPK 2 - Server
 *
 * @date	2013/03/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IEXCEPTION_H
#define IEXCEPTION_H

#include <string>

using namespace std;

enum IErrorCodes{
    SOCK_CANNOT_CREATE,
    SOCK_CANNOT_BIND,
    SOCK_CANNOT_LISTEN,
    SOCK_CANNOT_ACCEPT,
    SOCK_CANNOT_SELECT,
    SOCK_CANNOT_READ,
    SOCK_CANNOT_WRITE,

    DB_INVALID_FILE,

    SYSTEM_CANNOT_COMPILE_REGEX,
    SYSTEM_CANNOT_OPEN_FILE,
    SYSTEM_CANNOT_FORK,
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
