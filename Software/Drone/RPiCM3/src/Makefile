CC=g++
CFLAGS=-c -Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
    $(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
    $(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm *o halo