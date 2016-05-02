CC=g++
CFLAGS=-g -Wall -W -Wextra -pedantic

all: rdtclient rdtserver

rdtclient: $(FILES)
	$(CC) $(CFLAGS) -o $@ -lm client.cpp udt.cpp

rdtserver: $(FILES)
	$(CC) $(CFLAGS) -o $@ -lm server.cpp udt.cpp

clean:
	rm -f *.o *.out rdtclient rdtserver

