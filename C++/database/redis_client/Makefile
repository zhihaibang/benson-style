#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2016 Inc. All Rights Reserved.                 		  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				                  |
#  +----------------------------------------------------------------------+

INC =  -I./common
LIB += -L./lib -lhiredis
SRC = ./src/*.cpp ./common/*.cpp
FLAGE = -pthread
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
CC = g++
EXE = ./redis.o
all:
	$(CC) -o $(EXE) $(CFLAG) $(FLAGE) $(SRC) $(INC) $(LIB)
clean: 
	rm -rf *.o
	rm -rf *.so
	rm -rf $(EXE)
