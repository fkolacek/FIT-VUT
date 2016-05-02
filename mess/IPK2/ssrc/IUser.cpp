/**
 * @file	IUser.cpp
 * @brief	Projekt IPK 2 - Server
 *
 * @date	2013/02/21
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IUser.h"

IUser::IUser(){
    IUser("", "", "", "", "", "");
}

IUser::IUser(string login, string UID, string GID, string username, string directory, string shell){
    this->UID = UID;
    this->GID = GID;
    this->login = login;
    this->username = username;
    this->directory = directory;
    this->shell = shell;
}

void IUser::setUID(string UID){
    this->UID = UID;
}

void IUser::setGID(string GID){
    this->GID = GID;
}

void IUser::setLogin(string login){
    this->login = login;
}

void IUser::setUsername(string username){
    this->username = username;
}

void IUser::setDirectory(string directory){
    this->directory = directory;
}

void IUser::setShell(string shell){
    this->shell = shell;
}

string IUser::getUID(){
    return this->UID;
}

string IUser::getGID(){
    return this->GID;
}

string IUser::getLogin(){
    return this->login;
}

string IUser::getUsername(){
    return this->username;
}

string IUser::getDirectory(){
    return this->directory;
}

string IUser::getShell(){
    return this->shell;
}

bool IUser::isValid(){
    return !(this->login == "");
}
