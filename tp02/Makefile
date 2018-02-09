CC := g++
CPPFLAGS := -Wall -g -std=c++11 -Iinclude -Iinclude/compression
LDFLAGS := -lopencv_core -lopencv_imgcodecs -lboost_program_options -lboost_filesystem -lboost_system

all: clean tp02

clean:
	rm -f *.o ./compression_results/*

tp02.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

tp02: tp02.o
	$(CC) $(LDFLAGS) -o tp02 tp02.o
