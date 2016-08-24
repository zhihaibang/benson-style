INCPATH = -I./include
LIB = -L./lib/ -lwebapp -lfcgi

all:
	g++ $(INCPATH) ./src/*.cpp -o ./bin/fcgisample $(LIB)

clean:
	rm -rf ./bin/fcgisample
