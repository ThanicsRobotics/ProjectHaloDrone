#include "spi.h"
#include "serial.h"
#include "pid.h"

#include <pigpio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define AUTH_KEY 0xF9

char stm32_rx_buffer[100];
char stm32_tx_buffer[100];
bool spiConfigured = false;
bool authenticated = false;

int newThrottle = 0;

//CS0 is barometer, CS1 is STM32 flight controller
int SPI_CS = 1;
int spiFd;

using namespace std;

void setupSPI() {
    if ((spiFd = spiOpen(SPI_CS, 3000000, 0)) < 0) {
        cout << "SPI failed: " << strerror(errno) << endl;
        exit(1);
    }
    else {
        cout << "Opening SPI. FD: " << spiFd << " ID: " << pthread_self() << endl;
        spiConfigured = true;
    }
}

//Making sure the STM32F446 is listening...
void authFlightController() {
    //Reset flight controller using OpenOCD
    system(("sudo openocd -f" + projectPath + "reset.cfg").c_str());

    authenticated = false;
    char buffer[100];
    unsigned int authKey = 0;
    cout << "Authenticating..." << endl;
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
        cout << "Key: " << authKey << endl;
        spiWrite(spiFd, buffer, 2);
        
        delay(50);
        if (millis() - start > 8000) {
            exit(1);
        }
    }
    cout << "Authenticated" << endl;
    authenticated = true;
}

//Send modified throttle value to STM32
void calculateThrottle() {
    //PID compensated throttle calculation
    throttleInput = 1500;
    newThrottle = pid_output;

    if (newThrottle > 1900) newThrottle = 1900;
    if (newThrottle < 1000) newThrottle = 1000;

    pthread_mutex_lock(&stm32_mutex);
    stm32_tx_buffer[1] = (newThrottle - 1000) & 0xFF;
    stm32_tx_buffer[0] = ((newThrottle - 1000) >> 8) & 0xFF;
    pthread_mutex_unlock(&stm32_mutex);

    //CLOCK SPEED TEST
    //unsigned long int clockspeed = buffer[1];
    //cout << " | Clock: " << clockspeed << endl;
}