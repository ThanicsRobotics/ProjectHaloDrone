CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=main.cpp serial.cpp spi.cpp pid.cpp ultrasonic.cpp laser.cpp
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=-I$(PROJPATH)/inc/
DIR2=-I$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/core/inc/
DIR3=-I$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/platform/inc/
OTHER_SOURCES=$(wildcard $(DIR2)/*.c) $(wildcard $(DIR3)/*.c)
OTHER_OBJECTS=$(OTHER_SOURCES:.c=.o)
OBJ= $(SOURCES:.cpp=.o)
EXEC=halo

all: $(EXEC)

$(EXEC): $(OBJ) $(OTHER_OBJECTS)
	$(CC) $(CFLAGS) $(OBJ) $(OTHER_OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) -c $(CFLAGS) $(DIR1) $(DIR2) $(DIR3) $< -o $@ $(LIBS)

clean:
	rm *.o $(EXEC)