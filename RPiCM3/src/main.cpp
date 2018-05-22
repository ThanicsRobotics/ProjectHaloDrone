/// @file main.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Main program implementation.

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
#include <helper.h>

bool controllerConnected = false;
bool streamEnabled = false;

std::string camera;
std::string receiver;

Radio<Serial> radio;
FlightController fc;
Barometer baro;
std::thread serialThread;

bool keyLoopActive;
bool shuttingDown = false;
bool doneShuttingDown = false;
bool startCli = false;

int main(int argc, char *argv[]) {
    // Setup GPIO libraries
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        std::cout << "pigpio Library failed: " << strerror(errno) << "\n";
        exit(1);
    }

    // Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);

    filterCommandLineOptions();

    // If no options are typed, show how to type them.
    else {
        showUsage(argv[0]);
        return 1;
    }
    
    // Setup Flight Controller and start SPI comms thread.
    fc.setupSPI();
    fc.startFlight();

    // Wait for gyro calibration, reset calibration if necessary
    waitForGyroCalibration();

    // Manual arming process through SSH
    std::cout << "Type 'ARM' to arm the quadcopter: ";
    std::string input = "";
    getline(std::cin, input);
    if (input == "ARM") {
        fc.requestService(FlightController::Service::ARM);
    }

    // Start main loop
    mainLoop();

    // Waits for threads to end, and if interrupt hasn't been
    // triggered, shutdown threads and ports now
    delay(2000);
    if (!shuttingDown) shutdown();
    while (!doneShuttingDown) delay(10);
    return 0;
}