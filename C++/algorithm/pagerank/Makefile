#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2016. All Rights Reserved.                 			  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

INC =  -I./include
SRC = ./src/*.cpp
FLAGE = -pthread
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
CC = g++
EXE = ./bin/pagerank
all:
	$(CC) -o $(EXE) $(CFLAG) $(FLAGE) $(SRC) $(INC)
clean: 
	rm -rf *.o
	rm -rf *.so
	rm -rf $(EXE)
