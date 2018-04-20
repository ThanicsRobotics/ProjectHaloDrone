CC=g++
CFLAGS=-Wall -Wextra
SOURCES=$(wildcard *.cpp)
DIR1=-I../include/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $(DIR1) $< -o $@

clean:
	rm -f *.o