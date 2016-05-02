NAME=proj3
OBJFILES=$(NAME).o proj3matrix.o proj3matrixUtils.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -W -Wextra -g

# univerzální pravidlo pro generování všech objektových souborů
%.o : %.c
	$(CC) $(CFLAGS) -c $<

# Startovací pravidlo
all: $(NAME)

## ## ## 
# pravidla bez těla - to se sem doplní z univerzálního pravidla
proj3matrix.o: proj3matrix.c proj3matrix.h
proj3matrixUtils.o: proj3matrixUtils.c proj3matrixUtils.h
proj3.o: proj3.c proj3matrix.h proj3matrixUtils.h
## ## ## 

# Slinkování všech objektových souborů do jednoho spustitelného programu.
$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@
