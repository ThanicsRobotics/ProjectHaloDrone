CC=g++
CFLAGS=-c -Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
HEADERS=serial.h spi.h pid.h ultrasonic.h
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(EXEC)

$(EXEC): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm *o $(EXEC)