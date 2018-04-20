CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=$(wildcard *.cpp)
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=-I$(PROJPATH)/inc/
DIR2=-I$(PROJPATH)/libs/NemaTode/include/nmeaparse/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $(DIR1) $(DIR2) $< -o $@ $(LIBS)

clean:
	rm -f *.o