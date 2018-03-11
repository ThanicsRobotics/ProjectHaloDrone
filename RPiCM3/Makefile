CC=g++
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PROJPATH)/src/
DIR2=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/core/src/
DIR3=$(PROJPATH)/libs/VL53L1X_API_2.2.1/API/platform/src/
OBJECTS=$(wildcard $(DIR2)/*.o) $(wildcard $(DIR3)/*.o) $(wildcard $(DIR1)/*.o)
EXEC=halo

all: $(EXEC)
	+$(MAKE) -C src
	+$(MAKE) -C libs

$(EXEC): 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBS)

clean:
	+$(MAKE) clean -C src
	+$(MAKE) clean -C libs