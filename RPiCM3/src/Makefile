CC=g++-8.1.0
CFLAGS=-Wall -Wextra -Wno-ignored-qualifiers -pthread -std=c++17
LIBS=-lwiringPi -lrt -lpigpio -lncurses
SOURCES=$(wildcard *.cpp)
PROJPATH=..
DIR1=-I$(PROJPATH)/inc/
DIR2=-I$(PROJPATH)/libs/NemaTode/include/
DIR3=-I$(PROJPATH)/libs/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $(DIR1) $(DIR2) $(DIR3) $< -o $@ $(LIBS)

clean:
	rm -f *.o