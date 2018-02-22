//pigpio Library
#include <pigpio.h>

//WiringPi Library
#include <wiringPi.h>

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
#include <bitset>

//POSIX Thread Library
#include <pthread.h>

//Project headers
#include "ultrasonic.h"
#include "serial.h"
#include "spi.h"
#include "pid.h"

#define GYRO_CAL 0x04
#define STM32_ARM_TEST 0x9F
#define STM32_ARM_CONF 0x0A

using namespace std;

//Thread mutex and gyro thread function
pthread_mutex_t stm32_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t spiThread, serialThread;
void *spiLoop(void *void_ptr);
void *serialLoop(void *void_ptr);
volatile bool run = true;

string projectPath = "/home/pi/ProjectHalo/Software/Drone/RPiCM3/src/";

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

//Pressure Altitude variables
char baroData[9];
char baroCoefficients[17];

// int pressure;
// int temperature;
volatile int altitude = 0;
volatile int lastAltitude = 0;

volatile bool armRequest = false;
volatile bool authRequest = false;
volatile bool armed = false;
volatile bool preStart = false;

float loopRate = 0.0;
int loopStartTime = 0;

//Shutting down threads and closing ports
void shutdown() {
    cout << endl << "Closing Threads and Ports..." << endl << endl;
    
    //Stop threads
    run = false;
    delay(1000);

    //Join Threads to main    
    pthread_join(serialThread, NULL);
    pthread_join(spiThread, NULL);

    cout << "Closing I2C. FD: " << i2cFd << " ID: " << pthread_self() << endl;
    cout << "Closing Serial. FD:  " << serialFd << " ID: " << pthread_self() << endl;
    cout << "Closing SPI. FD:  " << spiFd << " ID: " << pthread_self() << endl;

    //Close ports
    spiClose(spiFd);
    serClose(serialFd);
    i2cClose(i2cFd);
    gpioTerminate();

    cout << endl << "Halting Flight Controller..." << endl << endl;
    delay(500);
    
    //Reset command to STM32
    system(("sudo openocd -f " + projectPath + "reset.cfg").c_str());
}

//Using gyro angles and raw distance, calculate absolute altitude of vehicle
void calculateAbsoluteAltitude() {
    loopRate = 1.0 / ((millis() - loopStartTime) / 1000.0);
    loopStartTime = millis();
    //cout << "Gyro Pitch: " << gyroPitch << " | Gyro Roll: " << gyroRoll;
    //int rawDistance = getUltrasonicData(1, 3, 30);
    //cout << " | Raw Distance: " << rawDistance;
    int rawDistance = 45;
    altitude = angleCorrection(rawDistance);
    //cout << " | Altitude: " << altitude << " | Input: " << throttleInput
    //    << " | Throttle: " << newThrottle << endl;
    cout << "Pitch: " << (int)gyroPitch << " | Roll: " << (int)gyroRoll
        << " | Raw Distance: " << rawDistance << " | Altitude: " << altitude << endl
        << "RX Input: " << throttleInput << " | Throttle: " << newThrottle 
        << " | Hz: " << loopRate << endl << endl;
    fflush(stdout);
}

void mainLoop() {
    cout << "Waiting for configuration..." << endl;
    while(!serialConfigured || !spiConfigured || !authenticated || !armed) delay(10);
    cout << "Starting main loop" << endl;
    while(run) {
        // int count = 0;
        // while(preStart) {
        //     calculateAbsoluteAltitude();
        //     calculatePID();
        //     if (count > 20) {
        //         count = 0;
        //         cout << endl << "Do you want to start motors? (y/n): ";
        //         string input = "";
        //         getline(cin, input);
        //         if (input == "y") {
        //             preStart = false;
        //             break;
        //         }
        //     }
        //     count += 1;
        // }
        calculateAbsoluteAltitude();
        calculatePID();
        while(millis() - loopStartTime < 120);
    }
}

void disarm() {
    //send disarm command
}

void arm() {
    int data = 0;
    while ((data != STM32_ARM_CONF) && run) {
        stm32_tx_buffer[0] = 0x00;
        stm32_tx_buffer[1] = STM32_ARM_TEST;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        
        delay(50);
    }
    armed = true;
}

void *spiLoop(void *void_ptr) {
    //Switch to flight controller, setup SPI @ 1.5MHz
    SPI_CS = 1;
    setupSPI();
    authFlightController();
    while(run) {
        if (armRequest) {
            cout << "Arming..." << endl;
            arm();
            armRequest = false;
        }
        else if (authRequest) {
            cout << "Authenticating..." << endl;
            authFlightController();
            authRequest = false;
        }
        else {
            //Calculate new PID compensated throttle
            sendThrottle();
            
            //Use SPI to get gyro angles, send throttle
            spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
            gyroPitch = (signed char)stm32_rx_buffer[0];
            gyroRoll = (signed char)stm32_rx_buffer[1];
            //gyroRoll = (int)(stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1]);
        }
    }
    disarm();
    return NULL;
}

void *serialLoop(void *void_ptr) {
    setupSerial();
    while(run) {
        readLine();
        delay(1);
    }
    return NULL;
}

void showUsage(string name) {
    cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller\n"
        << "\t-aa,--auto-arm \t\t\tDrone automatically ARMS after gyro calibration\n"
        << "\t-pre,--pre-start \t\t\tDrone gives data before starting motors\n"
        << endl;
}

//Main Program loop
int main(int argc, char *argv[]) {
    //Setup function calls
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        cout << "pigpio Library failed: " << strerror(errno) << endl;
        exit(1);
    }
    //Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);

    bool autoArm = false;
    bool controllerConnected = false;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-c" || string(argv[i]) == "--controller-enabled") 
                controllerConnected = true;
            else if (string(argv[i]) == "-nc" || string(argv[i]) == "--no-controller") 
                controllerConnected = false;
            else if (string(argv[i]) == "-aa" || string(argv[i]) == "--auto-arm")
                autoArm = true;
            else if (string(argv[i]) == "-pre" || string(argv[i]) == "--pre-start")
                preStart = true;
            else if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
                showUsage(argv[0]);
                return 1;
            }
        }   
    }
    else {
        showUsage(argv[0]);
        return 1;
    }

    setupIOExpander();

    pthread_create(&serialThread, NULL, serialLoop, NULL);
    pthread_create(&spiThread, NULL, spiLoop, NULL);

    while(!serialConfigured || !spiConfigured || !authenticated) delay(10);
    delay(200);
    cout << "Waiting for gyro calibration..." << endl;
    fflush(stdout);
    int start = millis();
    int repeat = 1;
    while (gyroRoll != GYRO_CAL) {
        repeat = 1;
        if (millis() - start > 20000) {
            cout << "Gyro not responding, resetting..." << endl;
            delay(1000);
            authRequest = true;
            start = millis();
            repeat++;
        }
        else if (repeat > 1) {
            shutdown();
            exit(1);
        }
        delay(50);
    }
    cout << "Calibration complete" << endl;

    if (controllerConnected) {
        cout << "Arm quadcopter using thumb sticks. Throttle down, yaw left." << endl;
        start = millis();
        while (gyroRoll == 4) {
            if (millis() - start > 60000) {
                cout << "No ARM signal, shutting down..." << endl;
                shutdown();
                exit(0);
            }
        }
    }
    else if (autoArm) {
        cout << "Auto arming... CTRL-C to stop." << endl;
        delay(500);
        armRequest = true;
    }
    else {
        cout << "Type 'ARM' to arm the quadcopter: ";
        string input = "";
        getline(cin, input);
        if (input == "ARM") {
            armRequest = true;
        }
        else {
            shutdown();
            exit(1);
        }
    }

    mainLoop();
    delay(2000);
    return 0;
}

void signal_callback_handler(int signum) {
	cout << endl << "Caught signal: " << signum << endl;
    shutdown();

    cout << endl << "Program End" << endl;
    delay(1000);
	exit(0);
}