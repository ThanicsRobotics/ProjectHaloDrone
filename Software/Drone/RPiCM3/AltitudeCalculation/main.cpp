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

//POSIX Thread Library
#include <pthread.h>

//Helper libraries
#include "ultrasonic.h"
#include "serial.h"
#include "spi.h"
#include "pid.h"

#define GYRO_CAL 0x0004
#define STM32_ARM_TEST 0x009F
#define STM32_ARM_CONF 0x000A

using namespace std;

//Thread mutex and gyro thread function
pthread_mutex_t stm32_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t spiThread, serialThread;
void *spiLoop(void *void_ptr);
void *serialLoop(void *void_ptr);
bool run = true;

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

//Pressure Altitude variables
char baroData[9];
char baroCoefficients[17];

// int pressure;
// int temperature;
int altitude;
int lastAltitude;

bool armRequest = false;
bool armed = false;

//Shutting down threads and closing ports
void shutdown() {
    cout << endl << "Closing Threads and Ports..." << endl << endl;
    
    //Stop threads
    run = false;
    delay(1000);

    //Join Threads to main
    int start = millis();
    
    pthread_join(serialThread, NULL);
    pthread_join(spiThread, NULL);

    cout << "Closing I2C: " << i2cFd << endl;
    cout << "Closing Serial: " << serialFd << endl;
    cout << "Closing SPI: " << spiFd << endl;

    //Close ports
    spiClose(spiFd);
    serClose(serialFd);
    i2cClose(i2cFd);
    gpioTerminate();

    cout << endl << "Halting Flight Controller..." << endl << endl;
    delay(500);
    
    //Halt command to STM32
    system("sudo openocd -f /home/pi/ProjectHalo/Software/Drone/RPiCM3/AltitudeCalculation/halt.cfg");
}

//Using gyro angles and raw distance, calculate absolute altitude of vehicle
void calculateAbsoluteAltitude() {
    cout << "Gyro Pitch: " << gyroPitch << " | Gyro Roll: " << gyroRoll;
    int rawDistance = getUltrasonicData(1, 3, 30);
    cout << " | Raw Distance: " << rawDistance;
    altitude = angleCorrection(rawDistance);
    cout << " | Altitude: " << altitude << " | Input: " << throttleInput
        << " | Throttle: " << newThrottle << endl;
}

void mainLoop() {
    while(!serialConfigured || !spiConfigured || !authenticated || !armed) delay(10);
    while(run) {
        calculateAbsoluteAltitude();
        calculatePID();
    }
}

void *spiLoop(void *void_ptr) {
    //Switch to flight controller, setup SPI @ 1.5MHz
    SPI_CS = 1;
    setupSPI();
    authFlightController();
    while(run) {
        //Calculate new PID compensated throttle
        calculateThrottle();
        
        //Use SPI to get gyro angles, send throttle
        spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
        //gyroPitch = (signed char)stm32_rx_buffer[0];
        //gyroRoll = (signed char)stm32_rx_buffer[1];
        gyroRoll = (int)(stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1]);

        if (armRequest) {
            stm32_tx_buffer[1] = STM32_ARM_TEST;
            spiWrite(spiFd, stm32_tx_buffer, 2);
            int data = 0;
            while (data != STM32_ARM_CONF) {
                spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
                data = stm32_rx_buffer[1];
                cout << "ARM Response: " << data << endl;
                fflush(stdout);
                delay(50);
            }
            armed = true;
            armRequest = false;
        }
        memset(stm32_tx_buffer,0,sizeof(stm32_tx_buffer));
    }
    return NULL;
}

void *serialLoop(void *void_ptr) {
    setupSerial();
    while(run) {
        readLine();
    }
    return NULL;
}

void showUsage(string name) {
    cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller"
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
        if (millis() - start > 15000) {
            cout << "Gyro not responding, resetting..." << endl;
            delay(1000);
            authFlightController();
            start = 0;
            repeat++;
        }
        else if (repeat > 1) {
            shutdown();
            return 1;
        }
        delay(50);
    }

    if (controllerConnected) {
        cout << "Calibration complete. Arm quadcopter." << endl;
        start = millis();
        while (gyroRoll == 4) {
            if (millis() - start > 60000) {
                cout << "No ARM signal, shutting down..." << endl;
                shutdown();
                exit(0);
            }
        }
    }
    else {
        cout << "Calibration complete\nType 'ARM' to arm the quadcopter: ";
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