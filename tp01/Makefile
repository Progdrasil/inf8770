CC 		 = g++
CPPFLAGS = -Wall -g -std=c++11 -Iinclude -Iinclude/compression

all: clean tp01.o run

clean:
	rm *.o

tp01.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) $< -o $@

run:
	./tp01.o
