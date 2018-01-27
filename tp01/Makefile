CC = g++
CPPFLAGS = -Wall -g -std=c++11 -Iinclude -Iinclude/compression 
EDLFLAGS = -lopencv_core -lopencv_imgcodecs -lboost_program_options

all: clean tp01 

clean:
	rm -f *.o ./compression_results/*

tp01.o: main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

tp01: tp01.o
	$(CC) $(EDLFLAGS) -o tp01 tp01.o