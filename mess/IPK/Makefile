PRJ=webclient
#
PROGS=$(PRJ)
FILES=main.cpp ISocket.cpp IHttpRequest.cpp IHttpResponse.cpp IHttpClient.cpp IException.cpp
CC=g++
CFLAGS=-Wall -W -Wextra -g -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)

