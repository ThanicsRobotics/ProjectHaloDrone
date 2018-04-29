#include <fcinterface.h>
#include <radio.h>
#include <pid.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define SEL2 5

char stm32_rx_buffer[100];
char stm32_tx_buffer[100];
volatile bool spiConfigured = false;
volatile bool authenticated = false;

volatile bool armRequest = false;
volatile bool disarmRequest = false;
volatile bool authRequest = false;
volatile bool armed = false;
volatile bool testGyro = false;
volatile bool motorTest;
volatile bool noMotors;

//Data received from SPI
volatile short int FCReceivedData = 0;

//Gyro angle variables
volatile signed char gyroPitch = 0;
volatile signed char gyroRoll = 0;

volatile bool run = true;

//CS0 is barometer, CS1 is STM32 flight controller
volatile int SPI_CS = 0;
volatile int spiFd;

void setupSPI() {
    if ((spiFd = spiOpen(SPI_CS, 3000000, 0)) < 0) {
        std::cout << "SPI failed: " << strerror(errno) << "\n";
        exit(1);
    }
    else {
        std::cout << "Opening SPI. FD: " << spiFd << " ID: " << pthread_self() << "\n";
        spiConfigured = true;
    }
}

void disarm() {
    int data = 0;
    while ((data != STM32_DISARM_CONF) && run) {
        int disarmCode = STM32_DISARM_TEST;
        stm32_tx_buffer[0] = (disarmCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = disarmCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        
        delay(50);
    }
    armed = false;
}

void arm() {
    int data = 0;
    while ((data != STM32_ARM_CONF) && run) {
        int armCode = STM32_ARM_TEST;
        if (motorTest) armCode = MOTOR_TEST;
        else if (noMotors) armCode = NO_MOTORS;
        stm32_tx_buffer[0] = (armCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = armCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        
        delay(50);
    }
    armed = true;
}

void resetSTM32F446() {
    pinMode(SEL2, OUTPUT);
    digitalWrite(SEL2, LOW);
    system(("sudo openocd -f " + projectPath + "reset.cfg").c_str());
    pinMode(SEL2, INPUT);
}

//Making sure the STM32F446 is listening...
void authFlightController() {
    //Reset flight controller using OpenOCD
    resetSTM32F446();

    authenticated = false;
    char buffer[100];
    unsigned int authKey = 0;
    std::cout << "Authenticating...\n";
    int start = millis();
    while(authKey != AUTH_KEY) {
        //Write to Authentication register
        buffer[0] = 0x00;
        buffer[1] = 0x01;
        spiWrite(spiFd, buffer, 2);
        delay(5);

        //Get Auth Key and send it back
        spiXfer(spiFd, buffer, buffer, 2);
        authKey = buffer[0] << 8 | buffer[1];
        std::cout << "Key: " << authKey << "\n";
        spiWrite(spiFd, buffer, 2);
        
        delay(50);
        if (millis() - start > 8000) {
            exit(1);
        }
    }
    std::cout << "Authenticated\n";
    authenticated = true;
}

//Send modified throttle value to STM32
void sendThrottle() {
    short int throttle = newThrottle;
    stm32_tx_buffer[1] = (throttle - 1000) & 0xFF;
    stm32_tx_buffer[0] = ((throttle - 1000) >> 8) & 0xFF;
    
    //CLOCK SPEED TEST
    //unsigned long int clockspeed = buffer[1];
    //std::cout << " | Clock: " << clockspeed << endl;
}

void *spiLoop(void*) {
    //Switch to flight controller, setup SPI @ 3MHz
    SPI_CS = 0;
    setupSPI();
    authFlightController();
    while(run) {
        if (armRequest) {
            //std::cout << "Arming..." << endl;
            arm();
            armRequest = false;
        }
        else if (disarmRequest) {
            disarm();
            disarmRequest = false;
        }
        else if (authRequest) {
            //std::cout << "Authenticating..." << endl;
            authFlightController();
            authRequest = false;
        }
        else if (testGyro) {
            spiRead(spiFd, stm32_rx_buffer, 2);
            gyroPitch = (signed char)stm32_rx_buffer[0];
            gyroRoll = (signed char)stm32_rx_buffer[1];
        }
        
        //Calculate new PID compensated throttle
        sendThrottle();
        
        //Use SPI to get gyro angles, send throttle
        spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
        FCReceivedData = (short)(stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1]);
        
    }
    if (armed) disarm();
    return NULL;
}