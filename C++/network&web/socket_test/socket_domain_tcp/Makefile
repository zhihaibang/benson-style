#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2016. All Rights Reserved.                 			  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

SERVER_SRC = ./server.cpp
CLIENT_SRC = ./client.cpp
CC = g++
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs

SERVER_EXE = ./server
CLIENT_EXE = ./client
		  
all:
	$(CC) $(CFLAG) $(SERVER_SRC) -o $(SERVER_EXE)
	$(CC) $(CFLAG) $(CLIENT_SRC) -o $(CLIENT_EXE)
clean: 
	rm -rf $(SERVER_EXE)
	rm -rf $(CLIENT_EXE)
