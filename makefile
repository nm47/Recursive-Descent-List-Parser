CC=g++
CFLAGS=-I.

listparser: lexer.o parser.o
	$(CC) -o listparser parser.cpp lexer.cpp
