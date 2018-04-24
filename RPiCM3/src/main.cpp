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
#include <altitude.h>
#include <fcinterface.h>
#include <pid.h>
#include <stream.h>
#include <gps.h>
#include <radio.h>

#define GYRO_CAL 0x04
#define BARO_DELAY 30

using namespace std;

//Thread mutex and gyro thread function
pthread_mutex_t stm32_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t spiThread, serialThread;

string projectPath = "/home/pi/ProjectHaloDrone/RPiCM3/src/";

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

volatile int lastAltitude = 0;

string camera;
string receiver;

Stream teleStream(TELE, "192.168.168.232", "9999", NULL);

//Shutting down threads and closing ports
void shutdown() {
    cout << endl << "Closing Threads and Ports..." << endl << endl;
    
    //Stop threads
    run = false;
    //delay(1000);

    //Join Threads to main  
    pthread_join(serialThread, NULL);
    pthread_join(spiThread, NULL);

    cout << "Closing I2C, UART, SPI..." << endl;

    //Close ports
    spiClose(spiFd);
    i2cClose(baroI2cFd);
    i2cClose(gpsFd);
    gpioTerminate();
    teleStream.closeStream();

    cout << endl << "Resetting Flight Controller..." << endl << endl;
    delay(500);
    
    //Reset command to STM32
    resetSTM32F446();
}

void mainLoop() {
    cout << "Waiting for configuration..." << endl;
    while(!serialConfigured || !spiConfigured || !authenticated || !armed) delay(10);
    cout << "Starting main loop" << endl;
    if (testGyro) {
        while(run) {
            cout << "Pitch: " << (int)gyroPitch << "\t| Roll: " << (int)gyroRoll << endl;
            delay(20);
        }
    }
    if ((int)gyroPitch == 0 && (int)gyroRoll == 10) {
        cout << "Re-arming..." << endl;
        armed = false;
        armRequest = true;
        while(!armed);
    }
    while(run) {
        altitude = getPressureAltitude();
        cout << "Altitude: " << altitude << endl;
        //readGPS();
        delay(100);
        // calculatePID();
        // while(millis() - loopStartTime < BARO_DELAY);
    }
}

void showUsage(string name) {
    cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller\n"
        << "\t-aa,--auto-arm \t\t\tDrone automatically ARMS after gyro calibration\n"
        << "\t-c,--camera [camera (ex. /dev/video0)] \tStreaming camera\n"
        << "\t-r,--receiver [IP Address] \tStreaming receiver IP Address\n"
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

    //Filtering command line options
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-c" || string(argv[i]) == "--controller-enabled") 
                controllerConnected = true;
            else if (string(argv[i]) == "-nc" || string(argv[i]) == "--no-controller") 
                controllerConnected = false;
            if (string(argv[i]) == "-aa" || string(argv[i]) == "--auto-arm")
                autoArm = true;
            if (string(argv[i]) == "-tg" || string(argv[i]) == "--test-gyro")
                testGyro = true;
            if (string(argv[i]) == "-mt" || string(argv[i]) == "--motor-test") {
                cout << endl << "\t\t!!!! TESTING MOTORS BEFORE ARM !!!!" << endl << endl;
                motorTest = true;
            }
            if (string(argv[i]) == "-nm" || string(argv[i]) == "--no-motors")
                noMotors = true;
            if (string(argv[i]) == "-c" || string(argv[i]) == "--camera") {
                camera = string(argv[i+1]);
            }
            if (string(argv[i]) == "-r" || string(argv[i]) == "--receiver") {
                receiver = string(argv[i+1]);
            }
            if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
                showUsage(argv[0]);
                return 1;
            }
        }   
    }
    else {
        showUsage(argv[0]);
        return 1;
    }
    mavlinkReceivePacket(teleStream.receiveDataPacket());
    for(int i = 0; i < 11; i++) {
        teleStream.receiveDataPacket();
    }
    teleStream.sendData("Hello from drone");

    cout << "Waiting for barometer calibration";
    fflush(stdout);
    setupBarometer();
    cout << "Done" << endl;
    fflush(stdout);
    startGPS();
    
    //Creating threads
    //  -> spiThread
    //  -> serialThread
    pthread_create(&serialThread, NULL, serialLoop, NULL);
    pthread_create(&spiThread, NULL, spiLoop, NULL);

    //Wait for gyro calibration, reset calibration if necessary
    while(!serialConfigured || !spiConfigured || !authenticated) delay(10);
    delay(200);
    cout << "Waiting for gyro calibration..." << endl;
    fflush(stdout);
    int start = millis();
    int repeat = 1;
    while (FCReceivedData != GYRO_CAL) {
        if (millis() - start > 10000) {
            cout << "Gyro not responding, resetting..." << endl;
            delay(1000);
            authenticated = false;
            authRequest = true;
            while(!authenticated);
            start = millis();
            repeat += 1;
        }
        else if (repeat > 1) {
            cout << "Gyro still not responding, shutting down..." << endl;
            shutdown();
            exit(1);
        }
        delay(50);
    }
    cout << "Calibration complete" << endl;

    //Arming process depends on program parameters
    //If Controller is connected, arm through controller thumbsticks
    if (controllerConnected) {
        cout << "Arm quadcopter using thumb sticks. Throttle down, yaw left." << endl;
        start = millis();
        while (gyroRoll == 4) {
            if (millis() - start > 60000) {
                cout << "No ARM signal, shutting down..." << endl;
                shutdown();
                exit(0);
            }
            delay(200);
        }
        armed = true;
    }

    //If Auto-ARM is enabled, drone will arm itself immediately
    else if (autoArm) {
        cout << "Auto arming... CTRL-C to stop." << endl;
        delay(500);
        armRequest = true;
    }

    //Manual arming process through SSH
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

    //Start main loop
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