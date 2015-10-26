/**
  * @file console.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Main function for CLI client
  */

#include "cli.h"

using namespace std;

/**
* @brief Main function for CLI client
* @param int argc
* @param char*argv[]
* @return int
*/
int main(int argc, char* argv[]){
	try{
		if(argc != 3)
			throw new Exception("[!] Invalid usage, you sould specify hostname and port!");

        string hostname = string(argv[1]);
        int port = atoi(argv[2]);

        CLI cli;
        cli.setBufferSize(2048);
        cli.connectToHost(hostname, port);
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
