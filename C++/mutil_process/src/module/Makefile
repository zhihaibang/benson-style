CC = g++
CFLAGS = -W -Wall -shared -fPIC -ggdb -O0

TARGET = ./libcounter.so

SO_PREFIX = lib
SO_EXT = .so
CXXSRC_EXT = .cpp

all: $(TARGET)

$(SO_PREFIX)%$(SO_EXT): module_%$(CXXSRC_EXT)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ./*.so

install:
	cp ./*.so ../../bin/module/