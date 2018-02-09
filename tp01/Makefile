CC = g++
CPPFLAGS = -Wall -g -std=c++11 -Iinclude -Iinclude/compression
EDLFLAGS = -lopencv_core -lopencv_imgcodecs -lboost_program_options

all: clean tp

clean:
	rm -f *.o ./compression_results/*

tp.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

tp: tp.o
	$(CC) $(EDLFLAGS) -o tp tp.o
