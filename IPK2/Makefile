PRJClient=client
PRJCFiles=$(PRJClient)_src/main.cpp $(PRJClient)_src/IException.cpp $(PRJClient)_src/ISocket.cpp $(PRJClient)_src/IClient.cpp $(PRJClient)_src/IUser.cpp

PRJServer=server
PRJSFiles=$(PRJServer)_src/main.cpp $(PRJServer)_src/IException.cpp $(PRJServer)_src/IUser.cpp $(PRJServer)_src/IUsersDatabase.cpp
#
PROGS=$(PRJClient) $(PRJServer)
CC=g++
CFLAGS=-Wall -W -Wextra -g -pedantic

all: $(PROGS)

$(PRJClient): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(PRJCFiles) -lm

$(PRJServer): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(PRJSFiles) -lm	
	
clean:
	rm -f *.o *.out $(PROGS)

