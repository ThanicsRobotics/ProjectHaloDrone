CC=g++
SOURCES=$(wildcard *.cpp)
DIR1=-I../include/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(DIR1) $< -o $@

clean:
	rm -f *.o