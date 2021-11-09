# CC = Compiler
CC=g++

# CFLAGS = CompilatorFLAGS
CFLAGS=-c

all: prog

prog: main.o list.o
	$(CC) main.o list.o -o prog

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

list.o: list.cpp
	$(CC) $(CFLAGS) list.cpp

clean:
	rm -rf *.o prog
