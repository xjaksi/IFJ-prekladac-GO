PRJ=ifj20
#
PROGS=$(PRJ)
FILES = expression.c exprList.c main.c tokenList.c parser.c scanner.c
CC =gcc
CFLAGS= -Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)