CC := g++
CPPFLAGS := -Wall -g -std=c++11 -Iinclude -Iinclude/compression
LDFLAGS := -lopencv_core -lopencv_imgcodecs -lboost_program_options -lboost_filesystem -lboost_system

all: clean tp

clean:
	rm -f *.o ./compression_results/*

tp.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

tp: tp.o
	$(CC) $(LDFLAGS) -o tp tp.o
