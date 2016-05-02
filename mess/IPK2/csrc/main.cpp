/**
 * @file	main.cpp
 * @brief	Projekt IPK 2 - Client
 *
 * @date	2013/03/02
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "IException.h"
#include "IClient.h"

typedef struct params{
    string hostname;
    int port;
    int mode;
    int order;
    vector<string> keys;
    list<int> columns;
} tParams;

using namespace std;

tParams getParam(int, char*[]);

int main(int argc, char* argv[]){
    try{
        tParams param = getParam(argc, argv);

        IClient client(param.hostname, param.port);

        client.setTransferMode(param.mode);
        client.setTransferOrder(param.order);

        client.process(param.keys, param.columns);
    }
    catch(IException* exception){
        cout << exception->getMessage() << endl;
        delete exception;
        return 1;
    }
    catch(...){
        cout << "Cautched unhandlered exception" << endl;
        return 1;
    }

    return 0;
}

tParams getParam(int argc, char* argv[]){
    stringstream stream;
    tParams param;

    param.port = 0;
    param.mode = 0;
	param.order = 0;

    int c;

	while((c = getopt(argc, argv, "h:p:LUGNHSl:u:")) != EOF){
		switch(c){
			case 'h': param.hostname = string(optarg); break;
            case 'p':
                param.port = atoi(optarg);
                break;
			case 'L': param.columns.remove(OPTION_LOGIN); param.columns.push_back(OPTION_LOGIN); break;
			case 'U': param.columns.remove(OPTION_USER); param.columns.push_back(OPTION_USER); break;
			case 'G': param.columns.remove(OPTION_GROUP); param.columns.push_back(OPTION_GROUP); break;
			case 'N': param.columns.remove(OPTION_NAME); param.columns.push_back(OPTION_NAME); break;
			case 'H': param.columns.remove(OPTION_HOME); param.columns.push_back(OPTION_HOME); break;
			case 'S': param.columns.remove(OPTION_SHELL); param.columns.push_back(OPTION_SHELL); break;
			case 'l':
				param.order = ORDER_LOGIN;
                param.keys.clear();
				optind--;

				for( ;optind < argc && *argv[optind] != '-'; optind++)
					param.keys.push_back(string(argv[optind]));
				break;
			case 'u':
				param.order = ORDER_UID;
				param.keys.clear();

				optind--;

				for( ;optind < argc && *argv[optind] != '-'; optind++)
					param.keys.push_back(string(argv[optind]));
				break;
			case '?':
				if(optopt == 'h')
                    throw new IException(PARAM_INVALID_HOSTNAME);
				else if(optopt == 'p')
                    throw new IException(PARAM_INVALID_PORT);
				else if(optopt == 'l')
					throw new IException(PARAM_INVALID_LOGIN);
				else if(optopt == 'u')
					throw new IException(PARAM_INVALID_UID);
				else
					throw new IException(PARAM_INVALID);
				break;
			default:
				throw new IException(PARAM_INVALID);
				break;
		}
	}

	for(list<int>::iterator p = param.columns.begin(); p != param.columns.end(); p++)
        param.mode += *p;

	if(param.hostname == "") throw new IException(PARAM_INVALID_HOSTNAME);
	if(param.port == 0) throw new IException(PARAM_INVALID_PORT);
	if(param.order == 0) throw new IException(PARAM_INVALID_KEY);

	return param;
}
