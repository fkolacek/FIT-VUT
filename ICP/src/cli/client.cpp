/**
  * @file client.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of client's behavior
  */

#include "client.h"

/**
* @brief Internal static counter of connected clients
*/
int Client::uidCounter = 1;

/**
* @brief Constructor of class Client
* @param int clientSocket
* @param string name
*/
Client::Client(int clientSocket, string clientName){
    this->uid = Client::uidCounter++;
    this->clientSocket = clientSocket;
    this->name = name;
    this->active = false;
    this->owner = false;
}

/**
* @brief Destructor of class Client
*/
Client::~Client(){
    //dtor
}

/**
* @brief Setter for uid attribute
* @param int uid
*/
void Client::setUID(int uid){
    this->uid = uid;
}

/**
* @brief Setter for clientSocketOpts attribute
* @param sockaddr_in clientSocketOpts
*/
void Client::setClientSocketOpts(sockaddr_in clientSocketOpts){
    this->clientSocketOpts = clientSocketOpts;
}

/**
* @brief Setter for name attribute
* @param string name
*/
void Client::setName(string name){
    this->name = name;
}

/**
* @brief Setter for clientSocketOpts attribute
* @param sockaddr_in clientSocketOpts
*/
void Client::setActive(bool active){
    this->active = active;
}

/**
* @brief Setter for owner attribute
* @param sockaddr_in owner
*/
void Client::setOwner(bool owner){
    this->owner = owner;
}

/**
* @brief Getter for uid attribute
* @return int uid
*/
int Client::getUID(){
    return this->uid;
}

/**
* @brief Getter for clientSocket attribute
* @return int clientSocket
*/
int Client::getClientSocket(){
    return this->clientSocket;
}

/**
* @brief Getter for clientSocketOpts attribute
* @return sockaddr_in clientSocketOpts
*/
sockaddr_in Client::getClientSocketOpts(){
    return this->clientSocketOpts;
}

/**
* @brief Getter for name attribute
* @return string name
*/
string Client::getName(){
    return this->name;
}

/**
* @brief Getter for active attribute
* @return bool active
*/
bool Client::isActive(){
    return this->active;
}

/**
* @brief Getter for owner attribute
* @return bool owner
*/
bool Client::isOwner(){
    return this->owner;
}
