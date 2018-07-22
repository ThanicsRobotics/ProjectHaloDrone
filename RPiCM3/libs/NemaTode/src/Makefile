ifeq ($(TARGET),cross)
    CC = arm-linux-gnueabihf-g++
else
    CC = g++-8.1.0
endif
CFLAGS=-w -std=c++17 -ggdb3
SOURCES=$(wildcard *.cpp)
DIR1=-I../include/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(DIR1) $< -o $@

clean:
	rm -f *.o