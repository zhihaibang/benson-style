CFLAG = -pthread

all:
	g++ $(CFLAG) -o process1.o process1.cpp
	g++ $(CFLAG) -o process2.o process2.cpp

clean:
	rm -rf *.o
	rm -rf file_lock
