LIB := -L./lib -lmysqlclient -lidn -lssl -lz -lrt -lpthread
EXE := ./mysql
SRC := ./src/*.cpp ./common/*.cpp
INC := -I./common/

all:
	g++ -o $(EXE) $(SRC) $(INC) $(LIB)

clean:
	rm -rf $(EXE)