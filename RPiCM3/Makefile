CC=g++
CFLAGS=-Wall -Wextra
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PROJPATH)/src/
DIR2=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/core/src/
DIR3=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/platform/src/
OBJ=$(patsubst %.cpp,%.o,$(wildcard $(DIR1)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(DIR2)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(DIR3)*.cpp))
EXEC=halo

all:
	+$(MAKE) -C src
	+$(MAKE) -C libs
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

clean:
	+$(MAKE) clean -C src
	+$(MAKE) clean -C libs
	rm -f $(EXEC)