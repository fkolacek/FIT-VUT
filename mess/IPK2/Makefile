PRJClient=client
PRJCFiles=csrc/main.cpp csrc/IException.cpp csrc/ISocket.cpp csrc/IClient.cpp csrc/IUser.cpp

PRJServer=server
PRJSFiles=ssrc/main.cpp ssrc/IException.cpp ssrc/IUser.cpp ssrc/IUsersDatabase.cpp
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

