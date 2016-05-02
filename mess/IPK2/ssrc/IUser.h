/**
 * @file	IUser.h
 * @brief	Projekt IPK 2 - Server
 *
 * @date	2013/02/21
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef IUSER_H
#define IUSER_H

#include <string>

using namespace std;

class IUser
{
    public:
        IUser();
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
