//pigpio Library
#include <pigpio.h>

//WiringPi Library
#include <wiringPi.h>

//Other Libraries
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
#include <ncurses.h>

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
#define a_KEY 97
#define d_KEY 100
#define UP_ARROW_KEY 65
#define DOWN_ARROW_KEY 66

bool controllerConnected = false;
bool streamEnabled = false;

//Thread mutex and gyro thread function
pthread_mutex_t stm32_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t spiThread, serialThread;

std::string projectPath = "/home/pi/ProjectHaloDrone/RPiCM3/src/";

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

volatile int lastAltitude = 0;

std::string camera;
std::string receiver;
Stream teleStream;

volatile bool keyLoopActive;
volatile bool shuttingDown = false;
volatile bool doneShuttingDown = false;
volatile int lastKey;

//Shutting down threads and closing ports
void shutdown() {
    shuttingDown = true;
    //Stop threads
    run = false;
    delay(2000);
    endwin();

    std::cout << "\nClosing Threads and Ports...\n\n";

    //Join Threads to main  
    pthread_join(serialThread, NULL);
    pthread_join(spiThread, NULL);

    std::cout << "Closing I2C, UART, SPI, TCP Socket...\n";

    //Close ports
    spiClose(spiFd);
    i2cClose(baroI2cFd);
    gpioTerminate();
    if(teleStream.isActive) teleStream.closeStream();

    std::cout << "\nResetting Flight Controller...\n\n";
    delay(500);
    
    //Reset command to STM32
    resetSTM32F446();
    doneShuttingDown = true;
}

void *keyLoop(void*) {
    int throttle = 1000;
    keyLoopActive = true;
    while (keyLoopActive) {
        int key = getch();
        lastKey = key;
        if (!armed && key == a_KEY) armRequest = true;
        else if (armed && key == d_KEY) disarmRequest = true;
        else if (key == UP_ARROW_KEY) throttle += 50;
        else if (key == DOWN_ARROW_KEY) throttle -= 50;
        if (throttle < 1000) throttle = 1000;
        else if (throttle > 2000) throttle = 2000;
        newThrottle = throttle;
        delay(50);
    }
    return NULL;
}

void mainLoop() {
    std::cout << "Waiting for configuration...\n";
    while(!spiConfigured || !authenticated) delay(10);
    std::cout << "Starting main loop\n";
    if (testGyro) {
        while(run) {
            std::cout << "Pitch: " << (int)gyroPitch << "\t| Roll: " << (int)gyroRoll << "\n";
            delay(20);
        }
    }
    if ((int)gyroPitch == 0 && (int)gyroRoll == 10) {
        std::cout << "Re-arming...\n";
        armed = false;
        armRequest = true;
        while(!armed);
    }
    if (!controllerConnected) {
        pthread_t keyThread;
        pthread_create(&keyThread, NULL, keyLoop, NULL);
        initscr();
        cbreak();
        noecho();
        printw("Welcome to Halo");
        mvprintw(1,0,"Press 'a' to arm: currently NOT ARMED");
        mvprintw(2,0,"Throttle: ");
        mvprintw(3,0,"GPS Status: ");
        mvprintw(4,0,"Last Key: ");
        refresh();
        keyLoopActive = true;
        while(run) {
            if (armed) mvprintw(1,0,"Press 'd' to disarm: currently *ARMED*");
            if (!armed) mvprintw(1,0,"Press 'a' to arm: currently NOT ARMED");
            //readGPS();
            mvprintw(2,11,"%d", newThrottle);
            //mvprintw(3,13,"%c", gps.fix.status);
            mvprintw(4,11,"%d", lastKey);
            refresh();
            //delay(200);
        }
        keyLoopActive = false;
        delay(1000);
        pthread_join(keyThread, NULL);
    }
    else {
        while(run) {
            
            //altitude = getPressureAltitude();

            //mavlinkReceivePacket(teleStream.receiveDataPacket());
            
            readGPSSentence();
            delay(100);
            //calculatePID();
        }
    }
}

void showUsage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller\n"
        << "\t-aa,--auto-arm \t\t\tDrone automatically ARMS after gyro calibration\n"
        << "\t-c,--camera [camera (ex. /dev/video0)] \tStreaming camera\n"
        << "\t-r,--receiver [IP Address] \tStreaming receiver IP Address\n"
        << "\t-s,--stream \t\t\tEnable streaming\n\n";
}

//Main Program loop
int main(int argc, char *argv[]) {
    //Setup function calls
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        std::cout << "pigpio Library failed: " << strerror(errno) << "\n";
        exit(1);
    }
    //Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);

    bool autoArm = false;

    //Filtering command line options
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--controller-enabled") 
                controllerConnected = true;
            else if (std::string(argv[i]) == "-nc" || std::string(argv[i]) == "--no-controller") 
                controllerConnected = false;
            if (std::string(argv[i]) == "-aa" || std::string(argv[i]) == "--auto-arm")
                autoArm = true;
            if (std::string(argv[i]) == "-tg" || std::string(argv[i]) == "--test-gyro")
                testGyro = true;
            if (std::string(argv[i]) == "-mt" || std::string(argv[i]) == "--motor-test") {
                std::cout << "\n\t\t!!!! TESTING MOTORS BEFORE ARM !!!!\n\n";
                motorTest = true;
            }
            if (std::string(argv[i]) == "-nm" || std::string(argv[i]) == "--no-motors")
                noMotors = true;
            if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--camera") {
                camera = std::string(argv[i+1]);
            }
            if (std::string(argv[i]) == "-r" || std::string(argv[i]) == "--receiver") {
                receiver = std::string(argv[i+1]);
            }
            if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "--stream")
                streamEnabled = true;
            if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
                showUsage(argv[0]);
                return 1;
            }
        }   
    }
    else {
        showUsage(argv[0]);
        return 1;
    }
    
    std::cout << "Waiting for barometer calibration";
    fflush(stdout);
    //setupBarometer();
    startGPS();
    
    //Creating threads
    //  -> spiThread
    //  -> serialThread
    pthread_create(&serialThread, NULL, serialLoop, NULL);
    pthread_create(&spiThread, NULL, spiLoop, NULL);

    //Wait for gyro calibration, reset calibration if necessary
    while(!spiConfigured || !authenticated) delay(10);
    delay(200);
    std::cout << "Waiting for gyro calibration...\n";
    fflush(stdout);
    int start = millis();
    int repeat = 1;
    while (FCReceivedData != GYRO_CAL) {
        if (millis() - start > 10000) {
            std::cout << "Gyro not responding, resetting...\n";
            delay(1000);
            authenticated = false;
            authRequest = true;
            while(!authenticated);
            start = millis();
            repeat += 1;
        }
        else if (repeat > 1) {
            std::cout << "Gyro still not responding, shutting down...\n";
            shutdown();
            exit(1);
        }
        delay(50);
    }
    std::cout << "Calibration complete\n";

    //Arming process depends on program parameters
    //If Controller is connected, arm through controller thumbsticks
    if (controllerConnected) {
        std::cout << "Arm quadcopter using thumb sticks. Throttle down, yaw left.\n";
        start = millis();
        while (gyroRoll == 4) {
            if (millis() - start > 60000) {
                std::cout << "No ARM signal, shutting down...\n";
                shutdown();
                exit(0);
            }
            delay(200);
        }
        armed = true;
    }

    //If Auto-ARM is enabled, drone will arm itself immediately
    else if (autoArm) {
        std::cout << "Auto arming... CTRL-C to stop.\n";
        delay(500);
        armRequest = true;
    }

    //Manual arming process through SSH
    // else {
    //     std::cout << "Type 'ARM' to arm the quadcopter: ";
    //     std::string input = "";
    //     getline(cin, input);
    //     if (input == "ARM") {
    //         armRequest = true;
    //     }
    //     else {
    //         shutdown();
    //         exit(1);
    //     }
    // }

    //Start main loop
    mainLoop();
    delay(2000);
    if (!shuttingDown) shutdown();
    while (!doneShuttingDown);
    return 0;
}

void signal_callback_handler(int signum) {
    if (!shuttingDown) shutdown();
    while (!doneShuttingDown);
    std::cout << "\nProgram End\n";
    delay(1000);
	exit(0);
}