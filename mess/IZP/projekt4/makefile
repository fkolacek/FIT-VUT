NAME=proj4
OBJFILES=$(NAME).o proj4listUtils.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -W -Wextra -g

# univerzální pravidlo pro generování všech objektových souborů
%.o : %.c
	$(CC) $(CFLAGS) -c $<

# Startovací pravidlo
all: $(NAME)

## ## ## 
# pravidla bez těla - to se sem doplní z univerzálního pravidla
proj4listUtils.o: proj4listUtils.c proj4listUtils.h
proj4.o: proj4.c proj4listUtils.h
## ## ## 

# Slinkování všech objektových souborů do jednoho spustitelného programu.
$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@
