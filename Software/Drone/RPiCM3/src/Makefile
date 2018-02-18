CC=g++
CFLAGS=-c -Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ) 
	$(CC) $(OBJ) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm *o halo