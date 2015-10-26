/**
  * @file cli.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for CLI client
  */

#ifndef CLI_H
#define CLI_H
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <deque>
#include <csignal>
#include "exception.h"
#include "client.h"
#include "game.h"
#include "utils.h"

using namespace std;

/**
* @brief Enum for events
*/
enum CLIEvent{
    E_NONE,
    E_CLIENTCOUNT,
    E_CLIENTLIST,
    E_INVITE,
    E_ACCEPTED,
    E_TURN,
    E_GAMES,
    E_PUT,
    E_MOVE,
    E_WINNER,

    E_NOTICE,
    E_ERROR,
};

/**
* @brief Class represents CLI client
*/
class CLI{
    public:
        CLI(bool = true, int = 1024);
        virtual ~CLI();
        void connectToHost(string = "localhost", int = 32890);
        void interract();
        static void* reader(void*);

		/**
        * @brief Setter for attribute hostname
        * @param string hostname
		*/
        inline void setHostname(string hostname = "localhost"){ this->hostname = hostname; }
		/**
        * @brief Setter for attribute port
        * @param int port
		*/
        inline void setPort(int port = 32890){ this->port = port; }
		/**
        * @brief Setter for port attribute bufferSize
        * @param int bufferSize
		*/
        inline void setBufferSize(int bufferSize){ this->bufferSize = bufferSize; }
        /**
        * @brief Add message to queue
        * @param string messae
		*/
        inline void addMessage(string message){ this->clientQueue.push_back(message); }
        
		/**
        * @brief Getter for attribute CLIEvent
        * @param string hostname
		*/
        inline CLIEvent getEvent(){ CLIEvent event = this->events.front(); this->events.pop_front(); return event; }
		/**
        * @brief Add event to events
        * @return CLIEvent
		*/
        inline void addEvent(CLIEvent event){ this->events.push_back(event); }
		/**
        * @brief Get count of events
        * @return int
		*/
        inline int getEvents(){ return this->events.size(); }
		/**
        * @brief Get count of clients
        * @return int
		*/
        inline int getClientCount(){ return this->clientCount; }
		/**
        * @brief Get clients
        * @return vector<Client>
		*/
        inline vector<Client> getClients(){ return this->clients; }
		/**
        * @brief Get current client
        * @return Client*
		*/
        inline Client* getCurrentClient(){ return this->currentClient; }
		/**
        * @brief Get UID of host
        * @return int
		*/
        inline int getInvitedBy(){ return this->invitedBy; }
		/**
        * @brief Get message
        * @return string
		*/
        inline string getMessage(){ return this->message; }
		/**
        * @brief Get current game
        * @return Game*
		*/
        inline Game* getCurrentGame(){ return this->currentGame; }
		/**
        * @brief Get list of available games
        * @return vector<string>
		*/
        inline vector<string> getGames(){ return this->games; }
    private:
        bool standalone;

        int clientSocket;
        sockaddr_in clientSocketOpts;
        fd_set clientSocketSet;
        timeval clientSocketTimer;
        pthread_t clientThread;
        deque<string> clientQueue;


        int clientCount;
        vector<Client> clients;
        deque<CLIEvent> events;
        vector<string> games;
        int invitedBy;
        string message;

        string hostname;
        string address;
        int port;

        int bufferSize;

        Client* currentClient;
        Game* currentGame;

        string socketRead();
        void socketWrite(string);
        void handleRequest(string);
};

#endif // CLI_H
