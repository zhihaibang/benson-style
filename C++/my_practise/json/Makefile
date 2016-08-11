#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2016 Inc. All Rights Reserved.                 		  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				                  |
#  +----------------------------------------------------------------------+

INC =  -I./include
LIB += -L./lib -ljsoncpp
SRC = ./*.cpp
FLAGE = -pthread 
CFLAG = -g -O3 -Wall -Wno-deprecated
CC = g++
EXE = ./main.o
all:
	$(CC) -o $(EXE) $(CFLAG) $(FLAGE) $(SRC) $(INC) $(LIB)
clean: 
	rm -rf *.o
	rm -rf *.so
	rm -rf $(EXE)
