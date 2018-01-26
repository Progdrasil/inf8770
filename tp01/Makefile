CC 		 = g++
CPPFLAGS = -Wall -g -std=c++11 -Iinclude -Iinclude/compression -lboost_program_options

all: clean tp01.o

clean:
	rm *.o

tp01.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) $< -o $@
