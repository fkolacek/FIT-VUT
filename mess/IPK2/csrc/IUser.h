/**
 * @file	IUser.h
 * @brief	Projekt IPK 2 - Client
 *
 * @date	2013/03/14
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IUSER_H
#define IUSER_H

#include <string>
#include <deque>

#include <iostream>

using namespace std;

enum options{
	OPTION_LOGIN		= 1,
	OPTION_USER			= 2,
	OPTION_GROUP		= 4,
	OPTION_NAME			= 8,
	OPTION_HOME			= 16,
	OPTION_SHELL		= 32
};

enum order{
    ORDER_LOGIN	= 1,
    ORDER_UID	= 2
};

class IUser
{
    public:
        IUser();
        IUser(string, int);
        IUser(string, string, string, string, string, string);

        void setUID(string);
        void setGID(string);
        void setLogin(string);
        void setUsername(string);
        void setDirectory(string);
        void setShell(string);

        string getUID();
        string getGID();
        string getLogin();
        string getUsername();
        string getDirectory();
        string getShell();

        bool isValid();
    private:
        string UID;
        string GID;

        string login;
        string username;
        string directory;
        string shell;
};

#endif // IUSER_H
