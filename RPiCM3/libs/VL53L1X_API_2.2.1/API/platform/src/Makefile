CC=gcc
CFLAGS=-Wall -Wextra
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=vl53l1_platform.c vl53l1_platform_init.c vl53l1_platform_log.c
PATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PATH)/inc/
DIR2=$(PATH)/libs/VL53L1X_API_2.2.1/API/core/inc/
DIR3=$(PATH)/libs/VL53L1X_API_2.2.1/API/platform/inc/
OBJ= $(SOURCES:.c=.o)

all: $(OBJ)

.c.o:
	$(CC) -c $(CFLAGS) -I$(DIR1) -I$(DIR2) -I$(DIR3) $< -o $@ $(LIBS)

clean:
	rm *.o $(EXEC)