CC=g++-8.1.0
CFLAGS=-Wall -Wextra -pthread -std=c++17 -ggdb3 -I/usr/local/include
LIBS=`pkg-config --cflags --libs gstreamer-1.0` -L/usr/local/lib -lwiringPi -lrt -lpigpio -lptp2 -lusb
PROJPATH=.
DIR1=$(PROJPATH)/src/
DIR2=$(PROJPATH)/libs/NemaTode/src/
DIR3=$(PROJPATH)/libs/ptpcam/
OBJ=$(patsubst %.cpp,%.o,$(wildcard $(DIR1)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(DIR2)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(DIR3)*.cpp))
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

updater:
	$(CC) $(CFLAGS) update/updater.cpp -o update/updater $(LIBS)