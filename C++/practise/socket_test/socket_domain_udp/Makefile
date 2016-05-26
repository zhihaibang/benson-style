#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2014 jd Inc. All Rights Reserved.                 |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

SERVER_SRC = ./src/server.cpp
CLIENT_SRC = ./src/client.cpp
CC = g++
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs

SERVER_EXE = ./bin/server.o
CLIENT_EXE = ./bin/client.o
		  
all:
	$(CC) $(CFLAG) $(SERVER_SRC) -o $(SERVER_EXE)
	$(CC) $(CFLAG) $(CLIENT_SRC) -o $(CLIENT_EXE)
clean: 
	rm -rf *.o
	rm -rf $(SERVER_EXE)
	rm -rf $(CLIENT_EXE)
