/**
  * @file cli.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of CLI's behavior
  */
#include "cli.h"

/**
  * @brief Constructor of class CLI
  * @param bool standalone
  * @param int buffersize
  */
CLI::CLI(bool standalone, int bufferSize){
    this->standalone = standalone;
    this->bufferSize = bufferSize;
    this->clientSocket = 0;
    this->currentClient = NULL;
    this->currentGame = NULL;

    this->clientCount = 0;
    this->clients.clear();
}

/**
  * @brief Destructor of class CLI
  */
CLI::~CLI(){
    if(this->currentGame)
        delete this->currentGame;

    this->currentGame = NULL;

    if(this->currentClient)
        delete this->currentClient;

    this->currentClient = NULL;

    if(this->clientSocket)
        close(this->clientSocket);

    this->clientSocket = 0;

    if(this->standalone){
        pthread_cancel(this->clientThread);
        pthread_join(this->clientThread,NULL);
    }
}

/**
  * @brief Method for connecting to the remote host
  * @param string hostname
  * @param int port
  */
void CLI::connectToHost(string hostname, int port){
    this->hostname = hostname;
    this->port = port;

    if((this->clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        throw new Exception("[!] Cannot create server socket!");

    this->address = hostname;

    //hostname
    if(inet_addr(this->hostname.c_str()) == (unsigned int)(-1)){
        cout << "[*] Resolving " << this->hostname << endl;

        hostent* hosts;
        in_addr** addresses;

        if((hosts = gethostbyname(this->hostname.c_str())) == NULL)
            throw new Exception("[*] Unable to resolve hostname!");


        addresses = (in_addr**)hosts->h_addr_list;

        for(int i = 0; addresses[i] != NULL; i++){
            this->clientSocketOpts.sin_addr = *addresses[i];

            this->address = string(inet_ntoa(*addresses[i]));
            cout << "[*] Resolved to "<<inet_ntoa(*addresses[i]) << endl;
            break;
        }
    }
    else
        this->clientSocketOpts.sin_addr.s_addr = inet_addr(this->hostname.c_str());

    cout << "[*] Connecting to " << this->hostname << ":" << this->port << " (" << this->address << ")" << endl;

    this->clientSocketOpts.sin_family = AF_INET;
    this->clientSocketOpts.sin_port = htons(this->port);

    if(connect(this->clientSocket, (sockaddr*)&(this->clientSocketOpts) , sizeof(sockaddr_in)) == -1)
        throw new Exception("[!] Unable to connect to remote host!");

    this->currentClient = new Client(this->clientSocket);
    this->currentClient->setClientSocketOpts(clientSocketOpts);
    this->currentClient->setUID(0);

    cout << "[*] Connected" << endl;

    this->clientQueue.push_back("UID;_;");

    if(this->standalone)
        pthread_create(&(this->clientThread), NULL, CLI::reader, this);

    this->interract();
}

/**
  * @brief Main loop for handling communication
  */
void CLI::interract(){
    while(true){
        FD_ZERO(&(this->clientSocketSet));
        FD_SET(this->clientSocket, &(this->clientSocketSet));

        this->clientSocketTimer.tv_sec = 1;
        this->clientSocketTimer.tv_usec = 0;

        int n;
        if((n = select(this->clientSocket + 1, &(this->clientSocketSet), NULL, NULL, &(this->clientSocketTimer))) < 0)
            throw new Exception("[!] Caught an error on select");

        //Nothing interesting
        if(n == 0){
            this->clientSocketTimer.tv_sec = 1;
            this->clientSocketTimer.tv_usec = 0;
        }
        //Communication on socket
        else if(FD_ISSET(this->clientSocket, &(this->clientSocketSet))){
            try{
                string buffer = this->socketRead();

                //handle buffer
                this->handleRequest(buffer);
            }
            catch(Exception* e){
                cout << "[*] Received disconnect.." << endl;
                delete e;
                break;
            }
        }

        if(this->clientQueue.size() > 0){
            while(this->clientQueue.size() != 0){
                string message = this->clientQueue.front();
                this->clientQueue.pop_front();

                message = trim(message);

                this->socketWrite(message);
            }
        }
    }

    close(this->clientSocket);
    this->clientSocket = 0;

    if(this->currentClient)
        delete this->currentClient;

    this->currentClient = 0;

    if(this->standalone)
        pthread_cancel(this->clientThread);
}

/**
  * @brief Private method for writing to socket
  * @param string data
  */
void CLI::socketWrite(string data){
    if(write(this->clientSocket, data.c_str(), data.length()) < 0)
        throw new Exception("[*] Cannot write to socket");
}

/**
* @brief Private method for reading from socket
* @return string buffer
*/
string CLI::socketRead(){
    string buffer(this->bufferSize, 0);

    if(read(this->clientSocket, &buffer[0], this->bufferSize - 1) <= 0)
        throw new Exception("[*] Cannot read from socket");

    buffer = trim(buffer);

    return buffer;
}

/**
  * @brief Method for handling requests
  * @param string rawInput
  */
void CLI::handleRequest(string rawInput){
    string data(rawInput.c_str());

    //cout << endl << "[*] Handling request: '" << data << "'" << endl;

    while(data.size() != 0){
        Message msg;
        msg.cmd = parseData(data);
        msg.arg = parseData(data);

        //INFO;Welcome on....;
        if(msg.cmd == "INFO"){

        }
        //UID;1;
        else if(msg.cmd == "UID"){
            this->currentClient->setUID(stringToInt(msg.arg));
        }
        else if(msg.cmd == "CLIENTCOUNT"){
            this->clientCount = stringToInt(msg.arg);
            this->addEvent(E_CLIENTCOUNT);
            cout << endl << "[*] Online clients: " << msg.arg << endl;
        }
        //CLIENTLIST;1,name,2,name,;
        else if(msg.cmd == "CLIENTLIST"){
            this->clients.clear();
            cout << endl << "[*] Clients" << endl;
            while(msg.arg.size() != 0){
                string sid = parseData(msg.arg, ',');
                string sname = parseData(msg.arg, ',');

                cout << sid << " " << sname << endl;
                Client client;
                client.setUID(stringToInt(sid));
                client.setName(sname);
                this->clients.push_back(client);
            }
            this->addEvent(E_CLIENTLIST);
        }
        //INVITE;sender;
        else if(msg.cmd == "INVITE"){
            this->addEvent(E_INVITE);
            this->invitedBy = stringToInt(msg.arg);
            cout << "[*] You've been invited to game by player " << msg.arg << endl;
        }
        //GAMES;name,name..
        else if(msg.cmd == "GAMES"){
            cout << endl << "[*] Games" << endl;
            this->games.clear();

            while(msg.arg.size() != 0){
                string tmp = parseData(msg.arg, ',');
                this->games.push_back(tmp);

                cout << " " << tmp << endl;
            }
            this->addEvent(E_GAMES);
        }
        //GAMEINIT;blablablabla;
        else if(msg.cmd == "GAMEINIT"){
            if(this->currentGame)
                delete this->currentGame;

            this->currentGame = NULL;

            try{
                this->currentGame = new Game("local");
                this->currentGame->setUID(this->currentClient->getUID());
                cout << "[*] Game is starting" << endl;
                this->currentGame->initFromString(msg.arg);
                cout << "[*] Game has been started" << endl;
                this->currentGame->debug();


            }
            catch(Exception* e){
                cout << "[!] " << e->getMessage() << endl;

                if(this->currentGame)
                    delete this->currentGame;

                this->currentGame = NULL;

                delete e;
            }
        }
        //DRAWCARD;UID;
        else if(msg.cmd == "DRAWCARD"){
            string suid = parseData(msg.arg, ';');

            int uid = stringToInt(suid);

            stoneItem card = this->currentGame->drawCard();

            int client = this->currentGame->findPlayerByUID(uid);
            if(client >= 0){
                this->currentGame->setPlayerCard(client, card);
            }

            this->currentGame->debug();
        }
        //TURN;UID;
        else if(msg.cmd == "TURN"){
            string sUID = parseData(msg.arg, ';');
            int uid = stringToInt(sUID);
            //this->currentGame->turn
            this->invitedBy = uid;
            this->addEvent(E_TURN);


            cout << "[*] It's player #" << uid << " turn;" << endl;
        }
        //WINNEr;UID;
        else if(msg.cmd == "WINNER"){
            string sUID = parseData(msg.arg, ';');
            int uid = stringToInt(sUID);
            this->currentGame->setStatus(0);

            this->invitedBy = uid;
            this->addEvent(E_WINNER);
            cout << "[*] Player #" << uid << " is winner;" << endl;
        }
        //PUT;X,Y,A
        //X
        //Y
        //A
        else if(msg.cmd == "PUT"){
            string sX = parseData(msg.arg, ',');
            string sY = parseData(msg.arg, ',');
            string sA = parseData(msg.arg, ';');

            int x = stringToInt(sX);
            int y = stringToInt(sY);
            int a = stringToInt(sA);

            try{
                this->currentGame->put(x, y, (stoneAngle)a);
                this->currentGame->debug();
                this->addEvent(E_PUT);
            }
            catch(Exception* e){
                cout << "[*] " << e->getMessage() << endl;
                delete e;
                throw new Exception("Client is out of sync, cannot perform PUT operation");
            }
        }
        //MOVE,X,Y,UID;
        else if(msg.cmd == "MOVE"){
            string sX = parseData(msg.arg, ',');
            string sY = parseData(msg.arg, ',');
            string sI = parseData(msg.arg, ';');

            int x = stringToInt(sX);
            int y = stringToInt(sY);
            int i = stringToInt(sI);

            try{
                this->currentGame->move(x, y, i);
                int score = this->currentGame->getPlayerScore(i);
                this->currentGame->setPlayerScore(i, score++);
                this->currentGame->debug();
            }
            catch(Exception* e){
                cout << "[*] " << e->getMessage() << endl;
                delete e;
                throw new Exception("Client is out of sync, cannot perform MOVE operation");
            }

        }
        //SCORE,UID;
        else if(msg.cmd == "SCORE"){
            string sUID = parseData(msg.arg, ';');
            int uid = stringToInt(sUID);

            int client = this->currentGame->findPlayerByUID(uid);
            if(client >= 0){
                int score = this->currentGame->getPlayerScore(client);
                score++;
                this->currentGame->setPlayerScore(client, score);
                this->currentGame->debug();
            }
        }
        //ACCEPTED;message;
        else if(msg.cmd == "ACCEPTED"){
            this->addEvent(E_ACCEPTED);
            this->message = msg.arg;
            cout << "[*] " << msg.arg << endl;
        }
        //NOTICE;error message;
        else if(msg.cmd == "NOTICE"){
            this->addEvent(E_NOTICE);
            this->message = msg.arg;
            cout << "[*] " << msg.arg << endl;
        }
        //ERROR;error message;
        else if(msg.cmd == "ERROR"){
            this->addEvent(E_ERROR);
            this->message = msg.arg;
            cout << "[!] " << msg.arg << endl;
        }
        else
            cout << "[!] Undefined command: " << msg.cmd << endl;
    }
}

/**
  * @brief Method for console interaction (running in separate thread)
  * @param void* args
  * @return void*
  */
void* CLI::reader(void* args){

    string clientBuffer;
    string prompt = "> ";
    CLI* cli = (CLI*)(args);

    while(true){
        if(cli->currentClient){
            ostringstream s;
            if(cli->currentClient->getUID() == 0)
                s << "connected> ";
            else
                s << "player#" << cli->currentClient->getUID() << "> ";

            prompt = s.str();
        }
        else
            prompt = "> ";

        cout << prompt;;
        getline(cin, clientBuffer);

        if(clientBuffer == "")
            continue;

        if(clientBuffer == "print"){
            if(cli->currentGame)
                cli->currentGame->debug();
            else
                cout << "[!] Game is not running" << endl;
        }
        else if(clientBuffer == "left"){
            if(cli->currentGame){
                if(cli->currentGame->getTurnUID() == cli->currentClient->getUID()){
                    cli->currentGame->rotateLeft();
                    cli->currentGame->debug();
                }
                else
                    cout << "[!] It's not your turn" << endl;
            }
            else
                cout << "[!] Game is not running" << endl;
        }
        else if(clientBuffer == "right"){
            if(cli->currentGame){
                if(cli->currentGame->getTurnUID() == cli->currentClient->getUID()){
                    cli->currentGame->rotateRight();
                    cli->currentGame->debug();
                }
                else
                    cout << "[!] It's not your turn" << endl;
            }
            else
                cout << "[!] Game is not running" << endl;
        }
        else if(clientBuffer == "exit"){
            cli->clientQueue.push_back("EXIT;");
            break;
        }
        else
            cli->clientQueue.push_back(clientBuffer);
    }
    return NULL;
}
