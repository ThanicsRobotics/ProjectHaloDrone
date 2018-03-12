CC=gcc
CFLAGS=-Wall -Wextra
LIBS=-lwiringPi -lrt -lpigpio
SOURCES=vl53l1_api_calibration.c vl53l1_api_core.c vl53l1_api_debug.c vl53l1_api_preset_modes.c vl53l1_api_strings.c vl53l1_api.c vl53l1_core_support.c vl53l1_core.c vl53l1_error_strings.c vl53l1_register_funcs.c vl53l1_silicon_core.c vl53l1_wait.c
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PROJPATH)/inc/
DIR2=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/core/inc/
DIR3=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/platform/inc/
OBJ= $(SOURCES:.c=.o)

all: $(OBJ)

.c.o:
	$(CC) -c $(CFLAGS) -I$(DIR1) -I$(DIR2) -I$(DIR3) $< -o $@ $(LIBS)

clean:
	rm -f *.o