CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PROJPATH)/src/
DIR2=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/core/src/
DIR3=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/platform/src/
OBJ=$(patsubst %.cpp,%.o,$(wildcard $(DIR1)*.cpp)) $(patsubst %.c,%.o,$(wildcard $(DIR2)*.c)) $(patsubst %.c,%.o,$(wildcard $(DIR3)*.c))
EXEC=halo

all:
	+$(MAKE) -C src
	+$(MAKE) -C libs
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

clean:
	+$(MAKE) clean -C src
	+$(MAKE) clean -C libs
	rm -f $(EXEC)

install:
	ln -sf halo /usr/bin/