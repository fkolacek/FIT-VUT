#include "IClient.h"

IClient::IClient(string hostname, unsigned int port){
    this->hostname = hostname;
    this->port = port;
    this->sock = NULL;
}

IClient::~IClient(){
    this->cleanup();
}


void IClient::setTransferMode(unsigned int transferMode){
    this->transferMode = transferMode;
}

void IClient::setTransferOrder(unsigned int transferOrder){
    this->transferOrder = transferOrder;
}

void IClient::process(vector<string> keys, list<int> columns){

    try{
        //Vytvoreni socketu
        this->sock = new ISocket(this->hostname, this->port);

        //Pripojeni k serveru
        this->sock->connectToRemoteHost();

        //Vytvoreni hlavicky
        stringstream request;
        request << "User-Agent: IPK #2 - xkolac12\n";
        request << "From: xkolac12@stud.fit.vutbr.cz\n";
        request << "Transfer-mode: " << this->transferMode << "\n";
        request << "Transfer-order: " << this->transferOrder << "\n";

        //Odeslani hlavicky
        this->sock->send(request.str());

        string response;

        //Prochazeni klicu pro vyhledavani
        for(vector<string>::iterator p = keys.begin(); p != keys.end(); p++){
            //Vytvoreni requestu
            request.str("");
            request << "Get: " << *p << "\n";

            //Odeslani requestu o informace
            this->sock->send(request.str());

            do{
                response = "";
                //Prijmuti odpovedi
                this->sock->receive(response);
                if(!response.empty()){
                    if(response.find(" ") != string::npos){
                        string statusCode = response.substr(0, response.find(" "));

                        if(statusCode == "200"){
                            string data = this->trim(response.substr(response.find(" ") + 1));

                            IUser user(data, this->transferMode);

                            bool first = true;
                            for(list<int>::iterator p = columns.begin(); p != columns.end(); p++){
                                if(first)
                                    first = !first;
                                else
                                    cout << " ";

                                if(*p == OPTION_LOGIN) cout << user.getLogin();
                                if(*p == OPTION_USER) cout << user.getUID();
                                if(*p == OPTION_GROUP) cout << user.getGID();
                                if(*p == OPTION_NAME) cout << user.getUsername();
                                if(*p == OPTION_HOME) cout << user.getDirectory();
                                if(*p == OPTION_SHELL) cout << user.getShell();
                                
                            }
                            if(columns.size() != 0) cout << endl;
                        }
                        else
                            cerr << response;
                    }
                    else if(response.find("END") != string::npos)
                        break;
                    else
                        throw new IException(RESPONSE_INVALID);
                }
                else
                    throw new IException(RESPONSE_INVALID);

            }while(response != "");
        }

        this->sock->send("Quit\n");

        this->sock->disconnectFromHost();

        this->cleanup();
    }
    catch(IException* exception){
        this->sock->disconnectFromHost();
        this->cleanup();
        throw exception;
    }
    catch(...){
        this->cleanup();
        throw new IException(SYSTEM_UNDEFINED_EXCEPTION);
    }
}

void IClient::cleanup(){
    delete this->sock;

    this->sock = NULL;
}

string IClient::trim(string data){
    if(data.find_last_not_of(" \n\r\t") != string::npos)
        data.erase(data.find_last_not_of(" \n\r\t")+1);

    return data;
}
