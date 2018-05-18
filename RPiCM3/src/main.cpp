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

//POSIX Thread Library
#include <pthread.h>

//Project headers
#include <barometer.h>
#include <flightcontroller.h>
#include <stream.h>
#include <gps.h>
#include <radio.h>
#include <serial.h>
//#include <gui.h>

#define GYRO_CAL 0x04
#define BARO_DELAY 30
#define a_KEY 97
#define c_KEY 99
#define d_KEY 100
#define UP_ARROW_KEY 65
#define DOWN_ARROW_KEY 66
#define projectPath std::string("./")

bool controllerConnected = false;
bool streamEnabled = false;

//Thread mutex and gyro thread function
//pthread_mutex_t stm32_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_t spiThread;

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int);

std::string camera;
std::string receiver;

Radio<Serial> radio;
FlightController fc;

bool keyLoopActive;
bool shuttingDown = false;
bool doneShuttingDown = false;
bool startCli = false;

//Shutting down threads and closing ports
void shutdown() {
    shuttingDown = true;

    std::cout << "\nClosing Threads and Ports...\n\n";

    //Stop threads
    fc.stopFlight();
    //delay(2000);
    //if (guiActive) closeGUI();

    //Join Threads to main
    //pthread_join(spiThread, NULL);

    std::cout << "Closing I2C, UART, SPI, TCP Socket...\n";
    //Close all interfaces on global instances so that we
    //can close the ports before terminating pigpio
    if(fc.isSPIConfigured()) fc.closeSPI();
    if(radio.isSerialConfigured()) radio.closeSerial();

    //Close ports
    if(teleStream.isActive()) teleStream.closeStream();

    std::cout << "\nResetting Flight Controller...\n\n";
    delay(500);
    
    //Reset command to STM32
    fc.requestService(FlightController::Service::RESET);

    //Terminates pigpio library
    gpioTerminate();

    //signals to main() that it can return now
    doneShuttingDown = true;
}

void mainLoop() {
    std::cout << "Waiting for configuration...\n";
    while(!fc.isSPIConfigured() || !fc.isAuthenticated()) delay(10);
    std::cout << "Starting main loop\n";
    if (fc.isTestGyroActive()) {
        while(fc.isRunning()) {
            std::cout << "Pitch: " << fc.getDronePosition().pitch << "\t| Roll: " << fc.getDronePosition().roll << "\n";
            delay(20);
        }
    }
    if (!controllerConnected) {
        // startGUI();
        int loopTimer = millis();
        float currentAltitude = 0;
        radio.setupSerial("/dev/serial0", 9600);
        Barometer baro;
        printf("Calibrating barometer");
        baro.setupI2C();
        while(fc.isRunning()) {
            //Every second, send heartbeat to controller
            if (millis() - loopTimer > 1000) {
                radioBuffer msg = radio.sendHeartbeat(0,3); //Heartbeat in PREFLIGHT mode and STANDBY state
                radio.write(msg.buf, msg.len);
            }
            else if (millis() - loopTimer > 600) {
                altitude = baro.getPressureAltitude();
                baro.takeReading();
            }
            radio.mavlinkReceiveByte(radio.readChar());
            channels pwmInputs = radio.getRCChannels();

            //Calculate new PID compensated throttle
            uint16_t newThrottle = fc.calculateThrottlePID(pwmInputs.throttlePWM, altitude);


            loopTimer = millis();
        }
        // printf("Sending Heartbeat\n");
        // buffer msg = sendHeartbeat(0,3); //Heartbeat in PREFLIGHT mode and STANDBY state
        // while(1) radio.write(msg.buf, msg.len);
        // printf("Reading MAVLink packets\n");
        // while(1) {
        //     mavlinkReceiveByte(radio.readChar());
        // }

    }
    else {
        while(fc.isRunning()) {
            readGPSSentence();
            delay(100);
            //calculatePID();
        }
    }
}

void showUsage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges (sudo)\n\n"
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
                fc.setTestGyro(true);
            if (std::string(argv[i]) == "-mt" || std::string(argv[i]) == "--motor-test") {
                std::cout << "\n\t\t!!!! TESTING MOTORS BEFORE ARM !!!!\n\n";
                fc.setMotorTest(true);
            }
            if (std::string(argv[i]) == "-nm" || std::string(argv[i]) == "--no-motors")
                fc.setNoMotors(true);
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
    
    //setupBarometer();
    //startGPS();
    
    //Creating threads
    //  -> spiThread
    //pthread_create(&spiThread, NULL, spiLoop, NULL);
    fc.setupSPI();
    fc.startFlight();

    //Wait for gyro calibration, reset calibration if necessary
    while(!fc.isSPIConfigured() || !fc.isAuthenticated()) delay(10);
    delay(200);
    std::cout << "Waiting for gyro calibration...\n";
    fflush(stdout);
    int start = millis();
    int repeat = 1;
    while (fc.getReceivedData() != GYRO_CAL) {
        if (millis() - start > 10000) {
            std::cout << "Gyro not responding, resetting...\n";
            delay(1000);
            fc.requestService(FlightController::Service::AUTH);
            while(!fc.isAuthenticated());
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
    // if (controllerConnected) {
    //     std::cout << "Arm quadcopter using thumb sticks. Throttle down, yaw left.\n";
    //     start = millis();
    //     while (gyroRoll == 4) {
    //         if (millis() - start > 60000) {
    //             std::cout << "No ARM signal, shutting down...\n";
    //             shutdown();
    //             exit(0);
    //         }
    //         delay(200);
    //     }
    //     armed = true;
    // }

    //If Auto-ARM is enabled, drone will arm itself immediately
    if (autoArm) {
        std::cout << "Auto arming... CTRL-C to stop.\n";
        delay(500);
        fc.requestService(FlightController::Service::ARM);
    }

    //Manual arming process through SSH
    else {
        std::cout << "Type 'ARM' to arm the quadcopter: ";
        std::string input = "";
        getline(std::cin, input);
        if (input == "ARM") {
            fc.requestService(FlightController::Service::ARM);
        }
        else {
            shutdown();
            exit(1);
        }
    }

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