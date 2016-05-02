CC = g++
CFLAGS = -std=c++0x
BOOSTFLAGS = -lpthread
RM = rm -f
RMD = rm -rf
TAR = tar -czf
TAR_FILE = xkolac12-xnikdo92-100-0.tar.gz

CONSOLE=$(shell /usr/bin/which xterm 2>/dev/null || /usr/bin/which console)

all: labyrinth2015-cli labyrinth2015-server labyrinth2015

labyrinth2015-server:
	@mkdir -p ../bin
	$(CC) $(CFLAGS) cli/utils.cpp cli/exception.cpp cli/server.cpp cli/game.cpp cli/client.cpp  -o ../bin/labyrinth2015-server $(BOOSTFLAGS)

labyrinth2015-cli:
	@mkdir -p ../bin
	$(CC) $(CFLAGS) cli/utils.cpp cli/exception.cpp cli/cli.cpp cli/game.cpp cli/client.cpp cli/console.cpp -o ../bin/labyrinth2015-cli $(BOOSTFLAGS)

labyrinth2015:
	@mkdir -p ../bin
	cd gui; qmake labyrinth2015.pro; make && mv labyrinth2015 ../../bin/

doxygen:
	@doxygen Doxyfile
	@mv doc ../

pack: clean
	@cd .. && $(TAR) $(TAR_FILE) *

clean:
	$(RM) *.o
	$(RMD) ./obj
	$(RMD) ./bin
	@cd .. && $(RMD) ./doc
	@cd .. && $(RMD) ./bin
	@cd .. && $(RMD) *.tgz

run:
	bash -c "echo 'Starting client'; echo; ../bin/./labyrinth2015;" &
	bash -c "echo 'Starting client'; echo; ../bin/./labyrinth2015;" &
	bash -c "echo 'Starting server'; echo; ../bin/./labyrinth2015-server 32890;"


