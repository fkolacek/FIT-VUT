#ifndef IUSERSDATABASE_H
#define IUSERSDATABASE_H

#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <regex.h>

#include "IException.h"
#include "IUser.h"

using namespace std;

class IUsersDatabase
{
    public:
        IUsersDatabase(string);
        virtual ~IUsersDatabase();

        void setFileName(string);
        void loadData();

        vector<IUser> getUserByUID(string);
        vector<IUser> getUserByLogin(string);
    private:
        string fileName;
        string pattern;
        multimap<string, IUser> database;
};

#endif // IUSERSDATABASE_H
