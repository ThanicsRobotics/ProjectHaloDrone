CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
HEADERS=serial.h spi.h pid.h ultrasonic.h
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(EXEC)

$(EXEC): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm *.o $(EXEC)