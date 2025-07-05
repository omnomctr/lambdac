CC = gcc
CFLAGS = -Wall 

all: lambdac

lambdac: y.tab.c main.c term.o lex.yy.c
	$(CC) $(CFLAGS) main.c arena.c y.tab.c term.o lex.yy.c -o lambdac

y.tab.c: lambda.y lambda.l term.h
	yacc -d lambda.y

lex.yy.c: lambda.l lambda.y
	lex lambda.l y.tab.h

term.o: term.c term.h 
	$(CC) $(CFLAGS) -c term.c -o term.o
