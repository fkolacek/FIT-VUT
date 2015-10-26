################################################################################
# IFJ Proj 2012
################################################################################

################################################################################
# promenne prekladu


NAME = main
CPP  = g++
CC   = gcc
SCFLAGS = -g -pedantic -Wall -W -O2 -std=c99 -DINLINE
INCS =
CXXINCS =
CXXFLAGS = -Wall $(SCFLAGS)
LOGIN = Tuxilero
TARBALL = Makefile main.c memory.h memory.c ial.h ial.c string.h string.c global.h lexi.h lexi.c interpret.h interpret.c inst_array.h inst_array.c
#CFLAGS = $(INCS)  -fmessage-length=0

.PHONY: gzip
.PHONY: setLd
.PHONY: clear
.PHONY: clean
.PHONY: clnobj


################################################################################
# obecny cil kompilace
all: $(NAME)

################################################################################
# linkovani vysledneho souboru
main: main.o memory.o string.o ial.o lexi.o parser.o interpret.o tkonst.o inst_array.o expr.o
	$(CC) -o $@ main.o memory.o string.o ial.o lexi.o parser.o interpret.o tkonst.o inst_array.o expr.o


################################################################################
# kompilace hlavniho souboru
main.o: main.c
	$(CC) -c main.c -o main.o $(CXXFLAGS)
	
memory.o: memory.c
	$(CC) -c memory.c -o memory.o $(CXXFLAGS)

string.o: string.c
	$(CC) -c string.c -o string.o $(CXXFLAGS)

ial.o: ial.c
	$(CC) -c ial.c -o ial.o $(CXXFLAGS)
	
lexi.o: lexi.c
	$(CC) -c lexi.c -o lexi.o $(CXXFLAGS)

parser.o: parser.c
	$(CC) -c parser.c -o parser.o $(CXXFLAGS)
	
interpret.o: interpret.c
	$(CC) -c interpret.c -o interpret.o $(CXXFLAGS)
	
tkonst.o: tkonst.c
	$(CC) -c tkonst.c -o tkonst.o $(CXXFLAGS)

inst_array.o: inst_array.c
	$(CC) -c inst_array.c -o inst_array.o $(CXXFLAGS)

expr.o: expr.c
	$(CC) -c expr.c -o expr.o $(CXXFLAGS)
################################################################################
# kompilace modulu

# dynamicka knihovna

################################################################################
# cil vycisteni souboru prekladu
clean: clear
clear:
	rm -f *.o $(NAME) $(LIBS) $(LOGIN).tar

clnobj:
	rm -f *.o

setLD:
	export LD_LIBRARY_PATH="."

$(LOGIN).tar: $(TARBALL)
	tar -cf $(LOGIN).tar $(TARBALL)
	
gzip: $(LOGIN).tar
#	gzip $(LOGIN).tar

################################################################################
################################################################################
