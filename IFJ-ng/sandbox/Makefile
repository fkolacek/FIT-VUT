PRJ=ifj13

PROGS=$(PRJ)
FILES=strings_copy.c ../errors.c ../gc.c ../strings.c ../btree.c
CC=gcc
CFLAGS=-Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)