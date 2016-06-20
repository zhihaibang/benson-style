#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2014 Inc. All Rights Reserved.                 |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

SERVER_SRC = ./src/server.cpp
CLIENT_SRC = ./src/client.cpp
CC = g++
INC = -I /usr/libevent/include
LIB = -L /usr/libevent/lib -levent
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs

SERVER_EXE = ./server.o
CLIENT_EXE = ./client.o
		  
all:
	$(CC) $(CFLAG) $(SERVER_SRC) -o $(SERVER_EXE) $(INC) $(LIB)
	$(CC) $(CFLAG) $(CLIENT_SRC) -o $(CLIENT_EXE)
	
clean: 
	rm -rf *.o
