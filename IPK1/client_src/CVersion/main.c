/**
 * @file	main.c
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/07
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "extstring.h"

#define REDIRECT_COUNT 5

enum errors{
    E_OK = 0,
    E_PARAM,
    E_PARAM_URL,
    E_SOCKET_CREATE,
    E_SOCKET_OPT,
    E_SOCKET_DNS,
    E_SOCKET_CONNECT,


    E_MAX_REDIRECTS,
};

char* ERROR_MESSAGES[] = {
    [E_OK]    = "",
    [E_PARAM] = "Invalid parametres",
    [E_PARAM_URL] = "Invalid URL entered",
    [E_SOCKET_CREATE] = "Cannot create socket",
    [E_SOCKET_OPT] = "Cannot set option to socket",
    [E_SOCKET_DNS] = "Cannot translate domain",
    [E_SOCKET_CONNECT] = "Cannot connect to remote host",


    [E_MAX_REDIRECTS] = "Redirect limit exceeded"
};

typedef struct param{
    int error;
    char* data;
} tParam;

typedef struct request{
    int error;
    char* rawUri;
    char* rawHeader;
    char* rawHost;

    char* serverSchema;
    char* serverHost;
    int serverPort;
    char* serverParam;
} tRequest;

typedef struct{
    int error;

    int handler;
    struct sockaddr_in options;
} tSocket;

tParam getParam(int, char*[]);

tRequest requestInit(char*);
char* requestMakeHeader(tRequest);
void requestDispose(tRequest);

tSocket socketInit(tRequest);
int socketProcess(tSocket, tRequest);
void socketDispose(tSocket);

void printHelp();
void printError(int);

int main(int argc, char* argv[]){

    tParam param = getParam(argc, argv);

    if(param.error != E_OK){
        printHelp();
        exit(param.error);
    }

    int redirects = 0;

    do{
        tRequest request = requestInit(param.data);

        if(request.error != E_OK){
            printError(E_PARAM_URL);
            exit(request.error);
        }

        tSocket sock = socketInit(request);

        if(sock.error != E_OK){
            requestDispose(request);
            printError(sock.error);
            exit(sock.error);
        }

        int status = socketProcess(sock, request);

        socketDispose(sock);
        requestDispose(request);

        exit(E_OK); // ODSTRANIT

    }while(++redirects < REDIRECT_COUNT);

    printError(E_MAX_REDIRECTS);
    exit(E_MAX_REDIRECTS);
}

tParam getParam(int argc, char* argv[]){
    tParam param = {
        .error = E_OK,
        .data = NULL
    };

    if(argc == 2)
        param.data = argv[1];
    else
        param.error = E_PARAM;

    return param;
}

//http://www.fit.vutbr.cz:80/common/img/fit_logo_cz.gif
tRequest requestInit(char* uri){
    tRequest request = {
        .error = E_OK,
        .rawUri = uri,
        .rawHeader = NULL,
        .rawHost = NULL,

        .serverSchema = NULL,
        .serverHost = NULL,
        .serverPort = 80,
        .serverParam = NULL,

    };

    char* que = request.rawUri;
    //char* tmp = NULL;
    int pos = 0;
    int pos1 = 0;

    //Pokud je zadan odkaz i se schema http://
    if(strstr(que, "http://")){
        request.serverSchema = malloc(sizeof(char) * 8);
        strcpy(request.serverSchema, "http://");
        que += 7;
    }

    //Pokud neexistuje / v URL -> chyba
    if((pos = extStrPos(que, '/', 0)) == -1){
        request.error = E_PARAM_URL;
        return request;
    }

    //ServerHost[:port]
    //tmp = malloc(sizeof(char) * (pos + 1));
    //tmp[pos] = '\0';
    //strncpy(tmp, que, pos);

    request.rawHost = malloc(sizeof(char) * (pos + 1));
    strncpy(request.rawHost, que, pos);
    request.rawHost[pos] = '\0';

    //Pokud neni zadan port, neni vlozena dvojtecka
    if((pos1 = extStrPos(request.rawHost, ':', 0)) == -1)
        request.serverHost = extSubStr(request.rawHost, 0, 0);
    //Pokud je zadan port
    else{
        request.serverHost = extSubStr(request.rawHost, 0, pos1);
        char* pointer = extSubStr(request.rawHost, pos1 + 1, 0);

        request.serverPort = atoi(pointer);

        free(pointer);
    }

    //free(tmp);
    //tmp = NULL;

    //Posuneme se za adresovou cast
    que += pos;

    request.serverParam = extSubStr(que, 0, 0);

    que = NULL;


    request.rawHeader = requestMakeHeader(request);

    printf("Request\n");
    printf(" - rawUri: %s\n", request.rawUri);
    printf(" - schema: %s\n", request.serverSchema);
    printf(" - rawHost: %s\n", request.rawHost);
    printf(" - host: %s\n", request.serverHost);
    printf(" - port: %d\n", request.serverPort);
    printf(" - param: %s\n", request.serverParam);
    printf("-------------------------------------\n%s-------------------------------------\n", request.rawHeader);

    return request;
}

char* requestMakeHeader(tRequest request){
    int size = strlen(request.serverParam) + strlen(request.rawHost);
    //21
    size += 91;

    char* header = malloc(sizeof(char) * size);

    snprintf(header, size,
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "From: xkolac12@stud.fit.vutbr.cz\r\n"
        "User-Agent: IPK #1 - xkolac12\r\n"
        "\r\n",
        request.serverParam,
        request.rawHost
    );

    return header;
}

void requestDispose(tRequest request){
    free(request.rawHeader);
    free(request.rawHost);
    free(request.serverSchema);
    free(request.serverHost);
    free(request.serverParam);
}

tSocket socketInit(tRequest request){

    tSocket sock = {
        .error = E_OK,
        .handler = 0
    };

    if((sock.handler = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        sock.error = E_SOCKET_CREATE;
        return sock;
    }
/*
    int flag = 1;
    int result = setsockopt(sock.handler, IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(int));

    if(result < 0){
        sock.error = E_SOCKET_OPT;
        return sock;
    }
*/
    sock.options.sin_family = AF_INET;
    sock.options.sin_port = htons(request.serverPort);

    struct hostent* hostInfo;

    if(!(hostInfo = gethostbyname(request.serverHost))){
        sock.error = E_SOCKET_DNS;
        return sock;
    }

    //printf("Hostname: %s\n", hostInfo->h_name);
    //printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)hostInfo->h_addr_list[0])));

    memcpy(&(sock.options.sin_addr), (struct in_addr*)hostInfo->h_addr_list[0], sizeof(sock.options.sin_addr));

    //Old
    //inet_aton(request.serverHost, &(sock.options.sin_addr));

    int socketStatus = connect(sock.handler, (struct sockaddr*)&(sock.options), sizeof(sock.options));

    if(socketStatus == -1){
        sock.error = E_SOCKET_CONNECT;
        close(sock.handler);
    }

    return sock;
}

int socketProcess(tSocket sock, tRequest request){
    int status = write(sock.handler, request.rawHeader, strlen(request.rawHeader));
    printf("Sended: %d (of %d)\n", status, strlen(request.rawHeader));

    return status;
}

void socketDispose(tSocket sock){
    if(sock.handler){
        shutdown(sock.handler, SHUT_RDWR);
        close(sock.handler);
    }
}

void printHelp(){
    printf("Usage: ./webclient [URL]\n");
}

void printError(int errorCode){
    fprintf(stderr, "%s\n", ERROR_MESSAGES[errorCode]);
}
