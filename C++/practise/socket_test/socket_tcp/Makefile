#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2014 jd Inc. All Rights Reserved.                 |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

SERVER_SRC = ./src/server.cpp
CLIENT_SRC = ./src/client.cpp
SERVER_EPOLL_SRC = ./src/server_epoll.cpp
CLIENT_NONBLOCK_SRC = ./src/client_nonblock.cpp
CC = g++
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs

SERVER_EXE = ./bin/server.o
CLIENT_EXE = ./bin/client.o
SERVER_EPOLL_EXE = ./bin/server_epoll.o
CLIENT_NONBLOCK_EXE = ./bin/client_nonblock.o
		  
all:
	$(CC) $(CFLAG) $(SERVER_SRC) -o $(SERVER_EXE)
	$(CC) $(CFLAG) $(CLIENT_SRC) -o $(CLIENT_EXE)
	$(CC) $(CFLAG) $(SERVER_EPOLL_SRC) -o $(SERVER_EPOLL_EXE)
	$(CC) $(CFLAG) $(CLIENT_NONBLOCK_SRC) -o $(CLIENT_NONBLOCK_EXE)
clean: 
	rm -rf *.o
	rm -rf ./bin/*
