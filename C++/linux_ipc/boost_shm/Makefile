INC = -I./common
CFLAG = -pthread
LIB = -lrt

all:
	g++ $(CFLAG) -o shmwrite.o shm_map_write.cpp $(INC) $(LIB)
	g++ $(CFLAG) -o shmread.o shm_map_read.cpp $(INC) $(LIB)
	g++ $(CFLAG) -o shm_map_write_c.o shm_map_write_c.cpp $(INC) $(LIB)
	g++ $(CFLAG) -o shm_map_read_c.o shm_map_read_c.cpp $(INC) $(LIB)

clean:
	rm -rf *.o
