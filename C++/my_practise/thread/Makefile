#  +----------------------------------------------------------------------+
#  | Makefile                                                             |
#  +----------------------------------------------------------------------+
#  | Copyright (c) 2016. All Rights Reserved.                 			  |
#  +----------------------------------------------------------------------+
#  | Authors: zhb                       				  |
#  +----------------------------------------------------------------------+

INC =  -I./include
SRC = ./src
FLAGE = -pthread
CFLAG = -g -O3 -Wall -Wno-deprecated #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
CC = g++
EXE_DIR = ./bin
EXE = $(EXE_DIR)/main.o \
	  $(EXE_DIR)/hello.o \
	  $(EXE_DIR)/para.o \
	  $(EXE_DIR)/mutex.o \
	  $(EXE_DIR)/signal.o \
	
all: $(EXE)

$(EXE_DIR)/%.o:$(SRC)/%.cpp
	$(CC) $(CFLAG) $(FLAGE) $(INC) $< -o $@
	
clean: 
	rm -rf $(EXE_DIR)/*.o
	rm -rf $(EXE)
