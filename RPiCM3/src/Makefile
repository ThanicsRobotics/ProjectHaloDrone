CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=$(wildcard *.cpp)
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=-I$(PROJPATH)/inc/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $(DIR1) $< -o $@ $(LIBS)

clean:
	rm -f *.o