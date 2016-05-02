/**
 * @file	IHttpClient.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/17
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IHttpClient.h"

IHttpClient::IHttpClient(string URI, bool verbose, int maxRedirects){
    this->URI = URI;
    this->verbose = verbose;
    this->maxRedirects = maxRedirects;

    this->request = NULL;
    this->sock = NULL;
    this->response = NULL;
}

IHttpClient::~IHttpClient(){
    this->cleanup();
}

void IHttpClient::setMaxRedirects(int maxRedirects){
    this->maxRedirects = maxRedirects;
}

void IHttpClient::process(){
    int i;
    ofstream file;
    int chunkSize = -1;
    stringstream stream;

    for(i = 0; i < this->maxRedirects; i++){

        try{
            //Vytvoreni pozadavku z URI
            this->request = new IHttpRequest(this->URI);

            if(this->verbose)
                this->request->dumpRequest();

            //Vytvoreni objektu pro odpoved
            this->response = new IHttpResponse();

            try{
                //Vytvoreni socketu
                this->sock = new ISocket(this->request->getServerHost(), this->request->getServerPort());

                //Pripojeni k serveru
                this->sock->connectToRemoteHost();

                //Odeslani pozadavku
                if(sock->send(this->request->getRequest()) <= 0)
                    throw new IException(CLIENT_CANNOT_SEND_DATA);


                string line;
                //Precteni hlavicek ze serveru
                while(this->sock->receive(line) > 0){
                    this->response->parseHeader(line);

                    if(line == "\r\n")
                        break;
                }

                if(this->verbose)
                    this->response->dumpResponse();

                if(!this->response->isValid())
                    throw new IException(CLIENT_INVALID_RESPONSE);

                //Rozhodovani co dal na zaklade statusCode
                switch(this->response->getStatusCode()){
                    case 200:
                        //Pokusime se otevrit soubor
                        file.open(this->request->getServerFile().c_str(), ios::binary);

                        if(!file.is_open())
                            throw new IException(SYSTEM_CANNOT_OPEN_FILE);

                        //Chunked read
                        if(this->response->getHeader("Transfer-Encoding") == "chunked"){

                            do{
                                this->sock->receive(line);

                                if(line == "\r\n")
                                    continue;

                                stream.clear();
                                stream << this->request->replaceAll(line, "\r\n", "");
                                stream >> std::hex >> chunkSize;

                                this->sock->receive(line, chunkSize);
                                file.write(line.c_str(), chunkSize);

                            }while(chunkSize != 0);


                            //cout << "Chunk size: '" << this->request->replaceAll(line, "\r\n", "") << "'" <<endl;
                            //
                            //cout << "Data: '" << line << "'" <<endl;


                        }
                        //Raw read
                        else{
                            stream.clear();
                            stream << this->response->getHeader("Content-Length");
                            stream >> chunkSize;

                            if(chunkSize > 0){
                                this->sock->receive(line, chunkSize);
                                file.write(line.c_str(), chunkSize);
                            }
                        }

                        file.close();
                        this->sock->disconnectFromHost();
                        this->cleanup();
                        return;
                        break;

                    //Redirections
                    case 301:
                    case 302:
                        this->URI = this->response->getHeader("Location");

                        //Local redirect (stejny server)
                        if(this->URI.at(0) == '/')
                            this->URI = this->request->getRawHost() + this->URI;
                        break;
                    //Zbytek jsou zatim nepodporovane stavove kody -> chyba
                    default:
                        throw new IException(this->response->getStatusMsg(), this->response->getStatusCode());
                        break;
                }
            }
            //Chyba pripojeni k serveru (vytvareni socketu, prekladu adresy a pripojovani k serveru)
            //Chyba zapisu a cteni do/ze socketu
            //Chyba prijeti neplatne odpovedi
            catch(IException* exception){
                this->cleanup();
                throw exception;
            }
            //Chyba neodchycena vyjimka
            catch(...){
                this->cleanup();
                throw new IException(SYSTEM_UNDEFINED_EXCEPTION);
            }
        }
        //Chyba parsovani URL (vytvareni requestu)
        //Chyba pripojeni k serveru (vytvareni socketu, prekladu adresy a pripojovani k serveru)
        //Chyba zapisu a cteni do/ze socketu
        //Chyba prijeti neplatne odpovedi
        catch(IException* exception){
            this->cleanup();
            throw exception;
        }
        //Chyba neodchycena vyjimka
        catch(...){
            this->cleanup();
            throw new IException(SYSTEM_UNDEFINED_EXCEPTION);
        }

        this->sock->disconnectFromHost();

        this->cleanup();
    }

    this->cleanup();

    if(i == this->maxRedirects)
        throw new IException(CLIENT_MAX_REDIRECT_COUNT_EXCEEDED);
}

void IHttpClient::cleanup(){
    delete this->response;
    delete this->sock;
    delete this->request;

    this->response = NULL;
    this->request = NULL;
    this->sock = NULL;
}
