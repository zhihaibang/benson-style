INC = -I./include
LNKDIR = -L./lib
LNK = -lcurl -lssl -lcrypto -lidn -lrt

SRC = ./src/*.cpp
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
CC = g++
EXE = ./http.o

all:
	$(CC) $(CFLAG) $(INC) $(LNKDIR) $(SRC) -o $(EXE) $(LNK)
clean: 
	rm -rf *.o
	rm -rf *.so
	rm -rf $(EXE)
