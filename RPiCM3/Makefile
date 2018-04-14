CC=g++
CFLAGS=-Wall -Wextra -pthread
LIBS=-lwiringPi -lrt -lpigpio
PROJPATH=/home/pi/ProjectHaloDrone/RPiCM3
DIR1=$(PROJPATH)/src/
OBJ=$(patsubst %.cpp,%.o,$(wildcard $(DIR1)*.cpp))
EXEC=halo

all:
	+$(MAKE) -C src
	# +$(MAKE) -C libs
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

clean:
	+$(MAKE) clean -C src
	# +$(MAKE) clean -C libs
	rm -f $(EXEC)

install:
	ln -sf halo /usr/bin/

updater:
	$(CC) $(CFLAGS) update/updater.cpp -o update/updater $(LIBS)