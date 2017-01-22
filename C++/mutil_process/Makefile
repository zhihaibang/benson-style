CC = g++
CFLAGS = -Wall -fPIC -g -o
LIB = -ldl
EXE = ./bin/master
SRC = ./src/main.cpp

all: master module
.PHONY: all

master:
	$(CC) $(CFLAGS) $(EXE) $(SRC) $(LIB)

module:
	cd src/module;make clean;make;make install
	
clean:
	rm -f ./bin/master
	rm -f ./bin/module/*.so

	