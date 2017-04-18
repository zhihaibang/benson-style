FLAGE = -pthread
CFLAG = -g -O3 -Wall -Wno-deprecated -lrt #-DSHOW_DEBUG #-pipe  -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT  -z defs
CC = g++

EXE = ./my_lockfree.o \
	./my_spsc_queue.o \
	./my_spsc_queue_int.o \
	./pthread_mutex.o \
		
all: $(EXE)

./%.o:./%.cpp
	$(CC) $(CFLAG) $(FLAGE) $< -o $@
	
clean: 
	 rm -rf ./*.o
