/**
 * @file	main.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/13
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <iostream>
#include <stdlib.h>

#include "IHttpClient.h"

using namespace std;

int main(int argc, char* argv[]){
    string rawUri;
    bool verbose = false;

    if(argc == 3 && (strcmp(argv[1], "-v") == 0)){
        rawUri = string(argv[2]);
        verbose = true;
    }
    else if(argc == 2)
        rawUri = string(argv[1]);
    else{
        cerr << "Usage: " << argv[0] << " [URI]" << endl;
        return 1;
    }

    try{
        IHttpClient client(rawUri, verbose);

        client.setMaxRedirects(5);
        client.process();
    }
    catch(IException* exception){
        cerr << "Error #" << exception->getCode() << " - " << exception->getMessage() << endl;
        delete exception;
        return 1;
    }
    catch(...){
        cerr << "Cautched unexcpected exception" << endl;
        return 1;
    }

    return 0;
}
