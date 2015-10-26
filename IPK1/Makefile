PRJ=webclient
#
PROGS=$(PRJ)
FILES=client_src/main.cpp client_src/ISocket.cpp client_src/IHttpRequest.cpp client_src/IHttpResponse.cpp client_src/IHttpClient.cpp client_src/IException.cpp
CC=g++
CFLAGS=-Wall -W -Wextra -g -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)

