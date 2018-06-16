#ifndef HELPER_H
#define HELPER_H

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
#include <memory>

//Project headers
#include <barometer.h>
#include <flightcontroller.h>
#include <stream.h>
#include <gps.h>
#include <radio.h>
#include <serial.h>

#define projectPath std::string("./")

//extern bool shuttingDown;
extern std::shared_ptr<bool> shuttingDownPtr;

/// @brief Terminal signal handler (for ending program via terminal).
void signal_callback_handler(int signum);

/// @brief Filtering command line options.
/// @param _argc Pass in argc from main() here.
/// @param _argv Pass in argv from main() here.
void filterCommandLineOptions(int _argc, char *_argv[], FlightController& fc);

/// @brief Displays message showing how to type options in command line.
/// @param name Name of program, i.e. First string of argv[].
void showUsage(std::string name);

/// @brief Shutting down threads and closing ports.
void shutdown();

#endif