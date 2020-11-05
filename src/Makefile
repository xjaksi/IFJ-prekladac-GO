# MAKEFILE pro prelozeni celeho projektu

PRJ=ifj20
PROGS=$(PRJ)

# pro konkretni pouziti dej do FILES co chces prelozit
FILES=main.c generator.c expession.c scanner.c symtable.c parser.c
CC =gcc
CFLAGS= -Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
