/**
  * @file server.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of server's behavior
  */

#include "server.h"

using namespace std;

/**
* @brief Constructor of class Server
* @param int port
* @param int bufferSize
*/
Server::Server(int port, int bufferSize){
    this->port = port;
    this->serverSocket = 0;
    this->serverSocketMaxFd = 0;
    this->bufferSize = bufferSize;
    this->currentGame = NULL;
}

/**
* @brief Destructor of class Server
*/
Server::~Server(){
    if(!this->serverSocket)
        close(this->serverSocket);

    for(vector<Client>::iterator p = this->clients.begin(); p != this->clients.end(); p++){
        int clientSocket = (*p).getClientSocket();

        if(clientSocket)
            close(clientSocket);
    }
}

/**
* @brief Method for starting server
*/
void Server::start(){
    cout << "[*] Starting server on port " << port << endl;

    if((this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        throw new Exception("[!] Cannot create server socket!");

    this->serverSocketMaxFd = this->serverSocket;

    this->serverSocketOpts.sin_family = AF_INET;
    this->serverSocketOpts.sin_port = htons(this->port);
    this->serverSocketOpts.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    if(setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        throw new Exception("[!] Cannot set SO_REUSEADDR to server socket!");

    if(bind(this->serverSocket, (sockaddr *)&(this->serverSocketOpts), sizeof(this->serverSocketOpts)) == -1)
        throw new Exception("[!] Cannot bind server socket!");

    if(listen(this->serverSocket, 10) == -1)
        throw new Exception("[!] Cannot listen on server socket!");

    FD_ZERO(&(this->serverSocketSet));
    FD_SET(this->serverSocket, &(this->serverSocketSet));

    this->serverSocketTimer.tv_sec = 180;
    this->serverSocketTimer.tv_usec = 0;

    cout << "[*] Server is running.." << endl;

    while(true){
        if(select(this->serverSocketMaxFd + 1, &(this->serverSocketSet), NULL, NULL, &(this->serverSocketTimer)) < 0)
            throw new Exception("[!] Caught an error on select");

        //Any change on serverSocket
        if(FD_ISSET(this->serverSocket, &(this->serverSocketSet))){

            sockaddr_in clientSocketOpts;
            socklen_t addrlen = sizeof(clientSocketOpts);

            int clientSocket = accept(this->serverSocket, (sockaddr*)&clientSocketOpts, &addrlen);
            cout << "[*] Incoming connection.." << endl;

            //Create new client
            Client client(clientSocket);
            client.setClientSocketOpts(clientSocketOpts);

            this->clients.push_back(client);
        }

        //Any change on clientSockets
        for(vector<Client>::iterator p = this->clients.begin(); p != this->clients.end(); p++){
            int clientSocket = (*p).getClientSocket();

            //New data on socket
            if(FD_ISSET(clientSocket, &(this->serverSocketSet))){

                try{
                    string buffer = socketRead(clientSocket);

                    this->handleRequest(*p, buffer);
                }
                catch(Exception *e){
                    cout << "[*] Received disconnect.." << endl;

                    close(clientSocket);
                    this->clients.erase(p);
                    delete e;
                    break;
                }
            }
        }

        FD_ZERO(&(this->serverSocketSet));
        FD_SET(this->serverSocket, &(this->serverSocketSet));

        this->serverSocketMaxFd = this->serverSocket;
        //Renew set & find new maxFd
        for(vector<Client>::iterator p = this->clients.begin(); p != this->clients.end(); p++){
            int clientSocket = (*p).getClientSocket();

            FD_SET(clientSocket, &(this->serverSocketSet));

            if(clientSocket > this->serverSocketMaxFd)
                this->serverSocketMaxFd = clientSocket;
        }

        this->serverSocketTimer.tv_sec = 180;
        this->serverSocketTimer.tv_usec = 0;
    }
}

/**
* @brief Private method for reading from socket
* @param int socket
* @return string buffer
*/
string Server::socketRead(int socket){
    string buffer(this->bufferSize, 0);

    int n;
    if((n = read(socket, &buffer[0], this->bufferSize - 1)) <= 0)
        throw new Exception("[*] Cannot read from socket");

    buffer = trim(buffer);

    return buffer;
}

/**
* @brief Private method for writing to socket
* @param int socket
* @param string data
*/
void Server::socketWrite(int socket, string data){
    if(write(socket, data.c_str(), data.length()) < 0)
        throw new Exception("[*] Cannot write to socket");
}

/**
* @brief Private method for handling requests
* @param Client& client
* @param string rawInput
*/
void Server::handleRequest(Client& client, string rawInput){
    string data(rawInput.c_str());

    cout << "[*] Handling request: '" << data << "' from client " << client.getUID() << endl;

    while(data.size() != 0){
        ostringstream response;

        Message msg;
        msg.cmd = parseData(data);
        msg.arg = parseData(data);

        //INFO;;
        if(msg.cmd == "INFO"){
            response << "INFO;Welcome on Labyrinth2015 server;UID;" << client.getUID() << ";CLIENTCOUNT;" << this->clients.size() << ";";
            cout << "[*] INFO requested" << endl;
            this->socketWrite(client.getClientSocket(), response.str());
        }
        //UID;name;
        else if(msg.cmd == "UID"){
            string tmp = msg.arg;

            tmp = stringReplace(tmp, ",", "");
            tmp = stringReplace(tmp, ";", "");
            tmp = stringReplace(tmp, " ", "");

            if(tmp == "_"){
                stringstream s;
                s << "Player" << client.getUID();
                client.setName(s.str());
            }
            else
                client.setName(tmp);

            response << "UID;" << client.getUID() << ";";
            this->socketWrite(client.getClientSocket(), response.str());
        }
        //CLIENTCOUNT;;
        else if(msg.cmd == "CLIENTCOUNT"){
            response << "CLIENTCOUNT;" << this->clients.size() << ";";
            this->socketWrite(client.getClientSocket(), response.str());
        }
        //CLIENTLIST;;
        else if(msg.cmd == "CLIENTLIST"){
            string tmp;
            response << "CLIENTLIST;";

            for(vector<Client>::iterator p = this->clients.begin(); p != this->clients.end(); p++){
                response << (*p).getUID() << "," << (*p).getName() << ",";
            }

            response << ";";

            tmp = response.str();
            tmp = stringReplace(tmp, ",;", ";");

            this->socketWrite(client.getClientSocket(), tmp);
        }
        //GAMES;;
        else if(msg.cmd == "GAMES"){

            try{
                DIR *dirHandler = NULL;
                struct dirent *dirPtr = NULL;
                string fileName;

                dirHandler = opendir("../examples/");

                if(!dirHandler)
                    throw new Exception("Cannot open directory");

                response << "GAMES;";

                while((dirPtr = readdir(dirHandler))){
                    fileName = string(dirPtr->d_name);

                    if(fileName.find(string(".txt")) != string::npos)
                        response << stringReplace(fileName, ".txt", "") << ",";

                }
                response << ";";

                fileName = response.str();
                fileName = stringReplace(fileName, ",;", ";");

                this->socketWrite(client.getClientSocket(), fileName);

                closedir(dirHandler);
            }
            catch(Exception* e){
                delete this->currentGame;
                response << "ERROR;" << e->getMessage() << ";";
                this->socketWrite(client.getClientSocket(), response.str());
                delete e;
            }
        }
        //INVITE;2,3,4;
        else if(msg.cmd == "INVITE"){
            this->acceptedClients.clear();
            this->acceptedClients.push_back(client);

            while(msg.arg.size() != 0){
                string tmp = parseData(msg.arg, ',');
                int id = stringToInt(tmp);

                vector<Client>::iterator p;
                for(p = this->clients.begin(); p != this->clients.end(); p++) if((*p).getUID() == id) break;

                if(id == client.getUID()){
                    response << "ERROR;You can't invite yourself;";
                    this->socketWrite(client.getClientSocket(), response.str());
                    response.str("");
                    response.clear();
                }
                else if(p == this->clients.end()){
                    response << "ERROR;Client " << id << " is not connected so you can't invite him;";
                    this->socketWrite(client.getClientSocket(), response.str());
                    response.str("");
                    response.clear();
                }
                else{
                    response << "INVITE;" << client.getUID() << ";";
                    this->socketWrite((*p).getClientSocket(), response.str());
                    response.str("");
                    response.clear();
                }
            }
        }
        //ACCEPT;1;
        else if(msg.cmd == "ACCEPT"){
            int id = stringToInt(msg.arg);

            if(this->acceptedClients.size() > 0){
                if(id != client.getUID()){
                    if(this->acceptedClients.at(0).getUID() == id){
                        vector<Client>::iterator p;
                        for(p = this->acceptedClients.begin(); p != this->acceptedClients.end(); p++) if((*p).getUID() == client.getUID()) break;

                        if(p == this->acceptedClients.end()){
                            this->acceptedClients.push_back(client);
                            //response << "NOTICE;Invitation has been accepted by player " << client.getUID() << ";";
                            response << "ACCEPTED;Invitation has been accepted by player " << client.getUID() << ";";
                            this->socketWrite(this->acceptedClients.at(0).getClientSocket(), response.str());
                        }
                        else
                            this->socketWrite(client.getClientSocket(), "ERROR;You've already replied to this invitation;");
                    }
                    else
                        this->socketWrite(client.getClientSocket(), "ERROR;Invitation is not current;");
                }
                else
                    this->socketWrite(client.getClientSocket(), "ERROR;You can't accept your invitation;");
            }
            else
                this->socketWrite(client.getClientSocket(), "ERROR;Invitation is not current;");
        }
        //CREATEGAME;N,K,P;
        //N - size {5, 7, 9, 11}
        //K - items {12, 24}
        //P - players {2,3,4}
        else if(msg.cmd == "CREATEGAME"){
            string sN = parseData(msg.arg, ',');
            string sK = parseData(msg.arg, ',');
            string sP = parseData(msg.arg, ',');

            int N = stringToInt(sN);
            int K = stringToInt(sK);
            int P = stringToInt(sP);

            if(this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;There is already game running, stop it first;");
            else if(this->acceptedClients.size() == 0)
                this->socketWrite(client.getClientSocket(), "ERROR;You should invite someone first;");
            else if(this->acceptedClients.at(0).getUID() != client.getUID())
                this->socketWrite(client.getClientSocket(), "ERROR;You should invite someone first;");
            else if(this->acceptedClients.size() < (unsigned int)P)
                this->socketWrite(client.getClientSocket(), "ERROR;Not enought invited players;");
            else{
                try{
                    this->currentGame = new Game("_");
                    this->currentGame->init(P, N, K);

                    for(int i = 0; i < P; i++){
                        this->currentGame->setPlayer(i, this->acceptedClients.at(i));
                        this->currentGame->setPlayerScore(i, 0);
                        this->currentGame->setPlayerCard(i, this->currentGame->drawCard());
                    }
                    for(int i = 0; i < P; i++){
                        //this->socketWrite(this->acceptedClients.at(i).getClientSocket(), "NOTICE;Game has been created;");
                        response << "GAMEINIT;" << this->currentGame->serialize() << ";";
                        response << "TURN;" << this->acceptedClients.at(0).getUID() << ";";
                        //response << "NOTICE;It's player #" << this->acceptedClients.at(0).getUID() << " turn;";
                        this->socketWrite(this->acceptedClients.at(i).getClientSocket(), response.str());
                        response.str("");
                        response.clear();
                    }

                    //this->currentGame->debug();
                }
                catch(Exception* e){
                    delete this->currentGame;
                    this->currentGame = NULL;
                    response << "ERROR;" << e->getMessage() << ";";
                    this->socketWrite(client.getClientSocket(), response.str());
                    delete e;
                }
            }
        }
        //DRAWCARD;;
        else if(msg.cmd == "DRAWCARD"){
            if(!this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;Game is not running;");
            else if(!this->currentGame->isPlayer(client.getUID()))
                this->socketWrite(client.getClientSocket(), "ERROR;You are not player in this game;");
            else{
                stoneItem card = this->currentGame->drawCard();

                int ci = this->currentGame->findPlayerByUID(client.getUID());
                if(ci >= 0)
                    this->currentGame->setPlayerCard(ci, card);

                response << "DRAWCARD;" << client.getUID() << ";";

                for(int i = 0; i < this->currentGame->getPlayersCount(); i++)
                    this->socketWrite(this->currentGame->getPlayer(i).getClientSocket(), response.str());
            }
        }
        //PUT;X,Y,A;
        //X - X pos
        //Y - Y pos
        //A - angle
        else if(msg.cmd == "PUT"){
            if(!this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;Game is not running;");
            else if(!this->currentGame->isPlayer(client.getUID()))
                this->socketWrite(client.getClientSocket(), "ERROR;You are not player in this game;");
            else if(client.getUID() == this->currentGame->getTurnUID()){
                string sX = parseData(msg.arg, ',');
                string sY = parseData(msg.arg, ',');
                string sA = parseData(msg.arg, ',');

                int x = stringToInt(sX);
                int y = stringToInt(sY);
                int a = stringToInt(sA);

                try{
                    this->currentGame->put(x, y, (stoneAngle)a);
                    response << "PUT;" << x << "," << y << "," << a << ";";

                    for(int i = 0; i < this->currentGame->getPlayersCount(); i++)
                        this->socketWrite(this->currentGame->getPlayer(i).getClientSocket(), response.str());
                }
                catch(Exception* e){
                    response << "ERROR;" << e->getMessage() << ";";
                    this->socketWrite(client.getClientSocket(), response.str());
                    delete e;
                }
            }
            else
                this->socketWrite(client.getClientSocket(), "ERROR;It's not your turn;");
        }
        //MOVE;
        else if(msg.cmd == "MOVE"){
            if(!this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;Game is not running;");
            else if(!this->currentGame->isPlayer(client.getUID()))
                this->socketWrite(client.getClientSocket(), "ERROR;You are not player in this game;");
            else if(client.getUID() == this->currentGame->getTurnUID()){
                string sX = parseData(msg.arg, ',');
                string sY = parseData(msg.arg, ',');
                int x = stringToInt(sX);
                int y = stringToInt(sY);

                try{
                    int i = this->currentGame->findPlayerByUID(client.getUID());
                    if(i < 0)
                        throw new Exception("Invalid client");

                    this->currentGame->move(x, y, i);
                    response << "MOVE;" << x << "," << y << "," << i << ";";

                    if(this->currentGame->getPlayerCard(i) == this->currentGame->getStoneItem(x,y)){
                        response << "DRAWCARD;" << client.getUID() << ";";
                        response << "SCORE;" << client.getUID() << ";";

                        stoneItem card = this->currentGame->drawCard();
                        int score = this->currentGame->getPlayerScore(i);
                        score++;

                        this->currentGame->setPlayerCard(i, card);
                        this->currentGame->setPlayerScore(i, score);

                        //We have winner
                        if(score >= (this->currentGame->getK() / this->currentGame->getP())){
                            response << "WINNER;" << client.getUID() << ";";
                            this->currentGame->setStatus(0);
                        }
                    }

                    if(this->currentGame->getStatus() == 1)
                        response << "TURN;" << this->currentGame->getPlayer(this->currentGame->getTurn()).getUID() << ";";

                    for(int i = 0; i < this->currentGame->getPlayersCount(); i++)
                        this->socketWrite(this->currentGame->getPlayer(i).getClientSocket(), response.str());

                }
                catch(Exception* e){
                    response << "ERROR;" << e->getMessage() << ";";
                    this->socketWrite(client.getClientSocket(), response.str());
                    delete e;
                }
            }
            else
                this->socketWrite(client.getClientSocket(), "ERROR;It's not your turn;");
        }
        //LOADGAME;name-of-the-game;
        else if(msg.cmd == "LOADGAME"){
            if(this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;There is already game running, stop it first;");
            else if(this->acceptedClients.size() == 0)
                this->socketWrite(client.getClientSocket(), "ERROR;You should invite someone first;");
            else if(this->acceptedClients.at(0).getUID() != client.getUID())
                this->socketWrite(client.getClientSocket(), "ERROR;You should invite someone first;");
            //else if(this->acceptedClients.size() < (unsigned int)P)
            //    this->socketWrite(client.getClientSocket(), "ERROR;Not enought invited players;");
            else{
                string name = parseData(msg.arg, ';');

                name = stringReplace(name, ",", "");
                name = stringReplace(name, ";", "");
                name = stringReplace(name, " ", "");

                if(name != ""){
                    ostringstream s;
                    s << "../examples/" << name << ".txt";

                    ifstream f;
                    f.open(s.str().c_str(), ios::in);

                    if(f.is_open()){
                        string tmp;

                        getline(f, tmp);

                        f.close();

                        try{
                            this->currentGame = new Game(name);
                            this->currentGame->initFromString(tmp);

                            for(int i = 0; i < this->currentGame->getPlayersCount(); i++){
                                this->currentGame->setPlayer(i, this->acceptedClients.at(i));
                            }
                            for(int i = 0; i < this->currentGame->getPlayersCount(); i++){
                                //this->socketWrite(this->acceptedClients.at(i).getClientSocket(), "NOTICE;Game has been created;");
                                response << "GAMEINIT;" << this->currentGame->serialize() << ";";
                                response << "TURN;" << this->acceptedClients.at(0).getUID() << ";";
                                //response << "NOTICE;It's player #" << this->acceptedClients.at(0).getUID() << " turn;";
                                this->socketWrite(this->acceptedClients.at(i).getClientSocket(), response.str());
                                response.str("");
                                response.clear();
                            }
                        }
                        catch(Exception* e){
                            cout << "[!] " << e->getMessage() << endl;

                            if(this->currentGame)
                                delete this->currentGame;

                            this->currentGame = NULL;

                            delete e;
                        }
                    }
                    else
                        this->socketWrite(client.getClientSocket(), "ERROR;Unable to load game from file;");
                }
                else
                    this->socketWrite(client.getClientSocket(), "ERROR;You have to specify name of the game;");
            }
        }
        //SAVEGAME;name-of-the-game;
        else if(msg.cmd == "SAVEGAME"){
            if(!this->currentGame)
                this->socketWrite(client.getClientSocket(), "ERROR;There is no game running;");
            else{
                string name = parseData(msg.arg, ';');

                name = stringReplace(name, ",", "");
                name = stringReplace(name, ";", "");
                name = stringReplace(name, " ", "");

                if(name != ""){
                    ostringstream s;
                    s << "../examples/" << name << ".txt";

                    ofstream f;
                    f.open(s.str().c_str(), ios::out);

                    if(f.is_open()){
                        string serialized = this->currentGame->serialize();

                        f << serialized;

                        f.close();
                        this->socketWrite(client.getClientSocket(), "NOTICE;Game has been successfully saved;");
                    }
                    else
                        this->socketWrite(client.getClientSocket(), "ERROR;Unable to save game to file;");
                }
                else
                    this->socketWrite(client.getClientSocket(), "ERROR;You have to specify name of the game;");
            }
        }
        //STOPGAME;;
        else if(msg.cmd == "STOPGAME"){
            if(this->currentGame)
                delete this->currentGame;

            this->currentGame = NULL;
        }
        //EXIT;;
        else if(msg.cmd == "EXIT"){
            cout << "[!] EXIT requested, closing connection.." << endl;
            shutdown(client.getClientSocket(),2);
        }
        else{
            cout << "[!] Undefined request '" << msg.cmd << "', ignoring.." << endl;
            this->socketWrite(client.getClientSocket(), "ERROR;Invalid command;");
            //shutdown(client.getClientSocket(),2);
        }
    }
}

/**
* @brief Main function for server
* @param int argc
* @param char** argv
*/
int main(int argc, char* argv[]){
	try{
		if(argc != 2)
			throw new Exception("[!] Invalid usage, you sould specify port!");

        int port = atoi(argv[1]);

        Server server;
        server.setBufferSize(2048);
        server.setPort(port);
        server.start();
	}
	catch(Exception *e){
        cerr << e->getMessage() << endl;
        delete e;
        return 1;
	}
	catch(...){
        cerr << "[!] Caught undefined exception!" << endl;
        return 1;
	}

	return 0;
}
