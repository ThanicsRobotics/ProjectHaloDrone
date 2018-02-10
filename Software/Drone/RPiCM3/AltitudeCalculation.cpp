//pigpio Library
#include <pigpio.h>

//WiringPi Library
#include <wiringPi.h>
#include <wiringSerial.h>

//Standard Libraries
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
#include <algorithm>

//POSIX Thread Library
#include <pthread.h>

//Helper libraries
#include "ultrasonic.h"
#include "serial.h"

using namespace std;

//Thread mutex and gyro thread function
//pthread_mutex_t gyro_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t run_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t gyroThread, serialThread;
void *gyroLoop(void *void_ptr);
void *serialLoop(void *void_ptr);
bool run = true;

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

char gyroBuffer[100];
bool spiConfigured = false;
bool authenticated = false;

//CS0 is barometer, CS1 is STM32 flight controller
int SPI_CS = 1;
int spiFd;

//Pressure Altitude variables
char baroData[9];
char baroCoefficients[17];

// int pressure;
// int temperature;
int altitude;
int lastAltitude;

//Gyro angle variables
signed int gyroPitch;
signed int gyroRoll;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pid_p_gain = 2;                    //Gain setting for the roll P-controller
int pid_i_gain = 0;                     //Gain setting for the roll I-controller
int pid_d_gain = 0;                   //Gain setting for the roll D-controller
int pid_max = 400;                      //Maximum output of the PID-controller (+/-)
int pid_error_temp;
int pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;
int throttleInput = 0;

void shutdown() {
    cout << endl << "Closing Threads and Ports..." << endl;
    run = false;
    delay(1000);
    pthread_join(serialThread, NULL);
    pthread_join(gyroThread, NULL);

    spiClose(spiFd);
    gpioTerminate();

    cout << endl << "Halting Flight Controller..." << endl << endl;
    delay(500);
    system("sudo openocd -f halt.cfg");
}

//Request gyro angles from STM32F446 flight controller
void getGyroValues() {

    //Gyro pitch and roll are stored in two incoming bytes
    //wiringPiSPIDataRW(SPI_CS, gyroBuffer, 2);
    spiXfer(spiFd, gyroBuffer, gyroBuffer, 2);
    gyroPitch = (signed char)gyroBuffer[0];
    gyroRoll = (signed char)gyroBuffer[1];
}

void setupSPI() {
    if (gpioInitialise() < 0) {
        cout << "pigpio Library failed: " << strerror(errno) << endl;
        exit(1);
    }
    signal(SIGINT, signal_callback_handler);
    if ((spiFd = spiOpen(SPI_CS, 3000000, 0)) < 0) {
        cout << "SPI failed: " << strerror(errno) << endl;
        exit(1);
    }
    else spiConfigured = true;
}

//Making sure the STM32F446 is listening...
void authFlightController() {
    //Reset flight controller using OpenOCD
    system("sudo openocd -f reset.cfg");

    authenticated = false;
    char buffer[100];
    unsigned int authKey = 0;
    cout << "Authenticating..." << endl;
    int start = millis();
    while(authKey != 0x00F9) {
        //Write to Authentication register
        buffer[0] = 0x00;
        buffer[1] = 0x01;
        //wiringPiSPIDataRW(SPI_CS, buffer, 2);
        spiWrite(spiFd, buffer, 2);
        delay(5);

        //Get Auth Key and send it back
        //wiringPiSPIDataRW(SPI_CS, buffer, 2);
        spiXfer(spiFd, buffer, buffer, 2);
        authKey = buffer[0] << 8 | buffer[1];
        cout << "Key: " << authKey << endl;
        //wiringPiSPIDataRW(SPI_CS, buffer, 2);
        //spiWrite(spiFd, buffer, 2);
        delay(50);
        if (millis() - start > 8000) {
            return;
        }
    }
    cout << "Authenticated" << endl;
    authenticated = true;
}

//Using gyro angles and raw distance, calculate absolute altitude of vehicle
void calculateAbsoluteAltitude() {
    cout << "Gyro Pitch: " << gyroPitch << " | "  << "Gyro Roll: " << gyroRoll;
    int rawDistance = getUltrasonicData(1, 3, 30);
    cout << " | Raw Distance: " << rawDistance;
    altitude = angleCorrection(rawDistance);
    cout << " | Altitude: " << altitude;
}

//Calculate throttle factor for altitude management through PID loop
void calculatePID() {
    if (throttleInput >= 1520 && throttleInput <= 1480) lastAltitude = altitude;

    pid_error_temp = altitude - lastAltitude + (throttleInput - 1500)/10;
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;
}

void sendThrottle() {
    unsigned char buffer[100];

    //pthread_mutex_lock(&serial_mutex);
    int input = throttleInput;
    // pthread_mutex_unlock(&serial_mutex);
    cout << " | input: " << input;
    int newThrottle = input + pid_output;
    
    if (newThrottle > 1900) newThrottle = 1900;
    //if (newThrottle < 1000) newThrottle = 1000;

    cout << " | Throttle: " << newThrottle << endl;

    buffer[1] = (newThrottle - 1000) & 0xFF;
    buffer[0] = ((newThrottle - 1000) & 0xFF00) >> 8;
    //wiringPiSPIDataRW(SPI_CS, buffer, 2);

    //CLOCK SPEED TEST
    //unsigned long int clockspeed = buffer[1];
    //cout << " | Clock: " << clockspeed << endl;
}

void mainLoop() {
    while(!serialConfigured || !spiConfigured || !authenticated) delay(10);
    // int response = 0;
    // while (response != 0x2222) {
    //     unsigned char buffer[5];
    //     buffer[0] = 0xF9;
    //     buffer[1] = 0xFF;
    //     wiringPiSPIDataRW(SPI_CS, buffer, 2);
    //     response = buffer[0] << 8 | buffer[1];
    // }

    while(run) {
        // cout << throttleInput << endl;
        // fflush(stdout);
        calculateAbsoluteAltitude();
        calculatePID();
        sendThrottle();
    }
}

void *gyroLoop(void *void_ptr) {
    //Switch to flight controller, setup SPI @ 1.5MHz
    SPI_CS = 1;
    // if (wiringPiSPISetup(SPI_CS, 1500000) < 0) {
    //     cout << "SPI Setup Failed: " << strerror(errno) << endl;
    //     fflush(stdout);
    // }
    setupSPI();
    authFlightController();
    while(run) {
        getGyroValues();
    }
    // unsigned char buffer[5];
    // buffer[0] = 0xFF;
    // buffer[1] = 0xF7;
    // wiringPiSPIDataRW(SPI_CS, buffer, 2);
    return NULL;
}

void *serialLoop(void *void_ptr) {
    setupSerial();
    serialFlush(serialFd);
    while(run) {
        handleSerialInterrupt();
        //delay(0.5);
    }
    serialClose(serialFd);
    return NULL;
}

void showUsage(string name) {
    cerr << "Usage: " << name << " <option(s)>\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller"
        << endl;
}

//Main Program loop
int main(int argc, char *argv[]) {
    bool controllerConnected = false;
    if (argc > 1) {
        if (string(argv[1]) == "-c" || string(argv[1]) == "--controller-enabled") 
            controllerConnected = true;
        else if (string(argv[1]) == "-nc" || string(argv[1]) == "--no-controller") 
            controllerConnected = false;
        else if (string(argv[1]) == "-h" || string(argv[1]) == "--help") {
            showUsage(argv[0]);
            return 1;
        }
    }
    else {
        showUsage(argv[0]);
        return 1;
    }

    //Setup function calls
    wiringPiSetupGpio();
    setupIOExpander();
    

    pthread_create(&serialThread, NULL, serialLoop, NULL);
    pthread_create(&gyroThread, NULL, gyroLoop, NULL);

    // while(!serialConfigured || !spiConfigured || !authenticated) delay(10);
    // delay(200);
    // cout << "Waiting for gyro calibration..." << endl;
    // fflush(stdout);
    // int start = millis();
    // int repeat = 1;
    // while (gyroRoll != 4) {
    //     repeat = 1;
    //     if (millis() - start > 15000) {
    //         cout << "Gyro not responding, resetting..." << endl;
    //         delay(1000);
    //         authFlightController();
    //         start = 0;
    //         repeat++;
    //     }
    //     else if (repeat > 1) {
    //         shutdown();
    //         return 1;
    //     }
    //     delay(50);
    // }

    // if (controllerConnected) {
    //     cout << "Calibration complete. Arm quadcopter." << endl;
    //     start = millis();
    //     repeat = 1;
    //     while (gyroRoll == 4) {
    //         if (millis() - start > 45000) {
    //             cout << "Gyro not responding, resetting..." << endl;
    //             delay(1000);
    //             authFlightController();
    //             start = 0;
    //             repeat++;
    //         }
    //         if (repeat > 1) {
    //             shutdown();
    //             return 1;
    //         }
    //     }
    // }
    // else {
    //     cout << "Calibration complete. Quadcopter self-arming." << endl;
    // }
    mainLoop();
    delay(2000);
    return 0;
}

void signal_callback_handler(int signum) {
	cout << endl << "Caught signal: " << signum << endl;
    shutdown();

    cout << endl << "Program End" << endl;
    delay(1000);
	exit(signum);
}