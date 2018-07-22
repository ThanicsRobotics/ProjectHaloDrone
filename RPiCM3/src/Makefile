CC = g++-8.1.0
CFLAGS=-Wall -Wextra -Wno-ignored-qualifiers -pthread -std=c++17 -ggdb3 -I/usr/local/include
LIBS=-L/usr/local/lib -lwiringPi -lrt -lpigpio -lncurses -lptp2 -lusb 
SOURCES=$(wildcard *.cpp)
PROJPATH=..
DIR1=-I$(PROJPATH)/inc/
DIR2=-I$(PROJPATH)/libs/NemaTode/include/
DIR3=-I$(PROJPATH)/libs/
OBJ= $(SOURCES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@ $(DIR1) $(DIR2) $(DIR3) `pkg-config --cflags --libs gstreamer-1.0` $(LIBS)

clean:
	rm -f *.o