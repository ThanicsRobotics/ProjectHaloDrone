CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp
PATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PATH)/inc/
DIR2=$(PATH)/libs/VL53L1X_API_2.2.1/API/core/inc/
DIR3=$(PATH)/libs/VL53L1X_API_2.2.1/API/platform/inc/
OTHER_SOURCES := $(wildcard $(DIR2)/*.c) $(wildcard $(DIR3)/*.c)
OTHER_OBJECTS := $(OTHER_SOURCES:.c=.o)
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(EXEC)

$(EXEC): $(OBJ) $(OTHER_OBJECTS)
	$(CC) $(CFLAGS) $(OBJ) $(OTHER_OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) -c $(CFLAGS) -B $(DIR1) -B $(DIR2) -B $(DIR3) $< -o $@ $(LIBS)

clean:
	rm *.o $(EXEC)