CC=g++-8.1.0
CFLAGS=-Wall -Wextra -Wno-ignored-qualifiers -pthread -std=c++17 -ggdb3 -I/usr/local/include
LIBS=-L/usr/local/lib -lwiringPi -lrt -lpigpio -lncurses -lptp2 -lusb
SOURCES=$(wildcard *.cpp)
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -f *.o