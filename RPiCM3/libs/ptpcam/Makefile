CC=g++-8.1.0
CFLAGS=-std=c++17 -ggdb3
SOURCES=$(wildcard *.cpp)
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o