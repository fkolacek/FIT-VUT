PRJClient=client
PRJCFiles=IClient/main.cpp IClient/IException.cpp IClient/ISegment.cpp IClient/base64.cpp

PRJServer=server
PRJSFiles=IServer/main.cpp IClient/IException.cpp IClient/ISegment.cpp IClient/base64.cpp

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

