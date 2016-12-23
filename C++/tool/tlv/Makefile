#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2014 Inc. All Rights Reserved.                 		  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  				  |
#  +----------------------------------------------------------------------+

INC =  -I./
SRC = ./*.cpp
CC = g++
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
EXE = ./message.o
all:
	$(CC) $(CFLAG) $(INC) $(SRC) -o $(EXE)
clean: 
	rm -rf *.o
	rm -rf *.so
	rm -rf $(EXE)
