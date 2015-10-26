PRJClient=client
PRJCFiles=IClient.cpp IException.cpp ISegment.cpp

PRJServer=server
PRJSFiles=IServer.cpp IException.cpp ISegment.cpp

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

