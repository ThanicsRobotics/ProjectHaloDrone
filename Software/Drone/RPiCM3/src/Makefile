CC=g++
CFLAGS=-c -Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ) 
\t$(CC) $(OBJ) -o $@ $(LIBS)

.cpp.o:
\t$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
\trm *o halo