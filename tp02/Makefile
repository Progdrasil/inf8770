CC := g++
CPPFLAGS := -Wall -g -std=c++11 -Iinclude -Iinclude/compression
LDFLAGS := -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lboost_program_options -lboost_filesystem -lboost_system

OBJ_DIR := ./obj
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET := tp

all: clean $(TARGET)

clean:
	rm -f *.o ./compression_results/* $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
