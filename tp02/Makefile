CC := g++
CPPFLAGS := -Wall -g -std=c++11 -Iinclude -Iinclude/compression
LDFLAGS := -lopencv_core -lopencv_imgcodecs -lboost_program_options -lboost_filesystem -lboost_system

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
TARGET := tp

all: clean $(TARGET)

clean:
	rm -f *.o ./compression_results/* $(TARGET)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< 

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
