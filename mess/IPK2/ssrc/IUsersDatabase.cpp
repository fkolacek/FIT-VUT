#include "IUsersDatabase.h"

IUsersDatabase::IUsersDatabase(string fileName){
    this->fileName = fileName;
    this->pattern = "^(.+)\\:.\\:([0-9]+)\\:([0-9]+)\\:(.*)\\:(.*)\\:(.*)$";
}

IUsersDatabase::~IUsersDatabase(){
    //dtor
}

void IUsersDatabase::setFileName(string fileName){
    this->fileName = fileName;
}

void IUsersDatabase::loadData(){
    this->database.clear();

    regmatch_t pmatch[7];
    regex_t re;
    int status;

    if(regcomp(&re, this->pattern.c_str(), REG_EXTENDED) != 0)
        throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

    ifstream handler;

    handler.open(this->fileName.c_str());

    if(!handler.is_open()){
        regfree(&re);
        throw new IException(SYSTEM_CANNOT_OPEN_FILE);
    }

    string line;

    while(getline(handler, line)){

        status = regexec(&re, line.c_str(), 7, pmatch, 0);

        if(status == REG_NOMATCH){
            handler.close();
            regfree(&re);

            throw new IException(DB_INVALID_FILE);
        }
        else{
            string login, UID, GID, username, directory, shell;

            login = line.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);

            UID = line.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);

            GID = line.substr(pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);

            if(pmatch[4].rm_so != -1)
                username = line.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);
            else
                username = "";

            if(pmatch[5].rm_so != -1)
                directory = line.substr(pmatch[5].rm_so, pmatch[5].rm_eo - pmatch[5].rm_so);
            else
                directory = "";

            if(pmatch[6].rm_so != -1)
                shell = line.substr(pmatch[6].rm_so, pmatch[6].rm_eo - pmatch[6].rm_so);
            else
                shell = "";

            //cout << login <<  ":" << UID << ":" << GID << ":" << ":" << username << ":" << directory << ":" << shell << endl;d
            this->database.insert(make_pair(login, IUser(login, UID, GID, username, directory, shell)));
        }
    }

    regfree(&re);

    handler.close();
}

vector<IUser> IUsersDatabase::getUserByLogin(string login){
    vector<IUser> users;

    for(multimap<string, IUser>::iterator p = this->database.begin(); p != this->database.end(); p++){
        if(p->second.getLogin() == login){
            users.push_back(p->second);
        }
    }

    return users;
}

vector<IUser> IUsersDatabase::getUserByUID(string UID){
    vector<IUser> users;

    for(multimap<string, IUser>::iterator p = this->database.begin(); p != this->database.end(); p++){
        if(p->second.getUID() == UID){
            users.push_back(p->second);
        }
    }

    return users;
}
