CC = gcc
CFLAGS = -Wall

all: lambdac

lambdac: main.c term.o arena.o reader.o parser.o
	$(CC) main.c term.o arena.o reader.o parser.o $(CFLAGS) -lreadline -o lambdac

term.o: term.c term.h
	$(CC) term.c $(CFLAGS) -c

arena.o: arena.c arena.h
	$(CC) arena.c $(CFLAGS) -c

reader.o: parser/reader.c parser/reader.h
	$(CC) parser/reader.c $(CFLAGS) -c

parser.o: parser/parser.c parser/parser.h
	$(CC) parser/parser.c $(CFLAGS) -c

clean:
	rm -f arena.o term.o lambdac reader.o parser.o
