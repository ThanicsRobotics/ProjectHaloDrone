#include <pigpio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <bitset>
#include <pthread.h>

//Project headers
#include <barometer.h>
#include <flightcontroller.h>
#include <stream.h>
#include <gps.h>
#include <radio.h>
#include <serial.h>

#define GYRO_CAL 0x04
#define projectPath std::string("./")

extern bool controllerConnected;

extern std::string camera;
extern std::string receiver;

extern Radio<Serial> radio;
extern FlightController fc;
extern Barometer baro;
extern std::thread serialThread;

extern bool keyLoopActive;
extern bool shuttingDown;
extern bool doneShuttingDown;
extern bool startCli;

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int signum);

void waitForGyroCalibration();

void filterCommandLineOptions(int _argc, char *_argv[]);

void mainLoop();

void showUsage(std::string name);

void shutdown();