#include "ultrasonic.h"

//WiringPi Libraries
#include <wiringPi.h>

//pigpio Library
#include <pigpio.h>

#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>

using namespace std;

int i2cFd;

//Pulse timing variables
int start_time = 0;
int pulse_time = 0;
bool pulseComplete = false;
unsigned int lastUltrasonicPulse = 0;

//Gyro angle variables
signed int gyroPitch;
signed int gyroRoll;

//Configures inputs and outputs of IO Expander
void setupIOExpander() {
    if ((i2cFd = i2cOpen(1, ADDR, 0)) < 0) {
        cout << "I2C Failed: " << strerror(errno) << endl;
        exit(1);
    }
    //i2cFd = wiringPiI2CSetup(ADDR);

    //Configuration bytes (Inputs are 1's, Outputs 0's)
    //Port 0: 01010101
    //wiringPiI2CWriteReg8(i2cFd, 0x0C, 0x55);
    i2cWriteByteData(i2cFd, 0x0C, 0x55);
    
    //Port 1: 01010101
    //wiringPiI2CWriteReg8(i2cFd, 0x0D, 0x55);
    i2cWriteByteData(i2cFd, 0x0D, 0x55);

    //Port 2: 11000000
    //wiringPiI2CWriteReg8(i2cFd, 0x0E, 0xC0);
    i2cWriteByteData(i2cFd, 0x0E, 0xC0);

    //Initialization of IO Expander interrupts
    wiringPiISR(INT_PIN, INT_EDGE_RISING, handleEcho);
}

//Handles IO Expander interrupt (measures ultrasonic sensor echo pulse)
void handleEcho() {
    //Get current time
    start_time = micros();

    //Get time when pulse is HIGH
    while(digitalRead(INT_PIN) == HIGH) {
        //Stop if HIGH for 100ms (timeout)
        if ((micros() - start_time) > 100000) {
            pulse_time = 0;
            pulseComplete = true;
            return;
        }
    }
    //Pulse time is the time difference before and after HIGH pulse
    pulse_time = micros() - start_time;
    pulseComplete = true;
}

//Utility function for setting individual pin on IO Expander
void digitalIOWrite(int pin, int state) {
    //Figure out port number based on pin number
    int port;
    if (pin < 8) port = 0;
    else if (pin < 18 && pin > 9) port = 1;
    else port = 2;

    //Change output depending on port and pin number
    switch (port) {
        case 0:
            i2cWriteByteData(i2cFd, 0x04, state << pin);
            break;
        case 1:
            i2cWriteByteData(i2cFd, 0x05, state << (pin - 10));
            break;
        case 2:
            i2cWriteByteData(i2cFd, 0x06, state << (pin - 20));
            break;
        default:
            break;
    }
}

//Gets distance value (in centimeters) from downward facing sensor
int getUltrasonicData(int sensor, int iterations, unsigned int pulseDelay) {
    int pin;

    //Toggles between downward facing sensor 1 and 2
    switch (sensor) {
        case 1:
            pin = 17;
            break;
        case 2:
            pin = 15;
            break;
        default:
            break;
    }

    int distances[iterations];
    int loops = 0;

    //Takes median of x distance measurements
    while(loops < iterations) {
        while (millis() - lastUltrasonicPulse < pulseDelay) delay(1);

        //Ensuring TRIG pin is LOW
        digitalIOWrite(pin, LOW);

        //Starting TRIG pulse
        digitalIOWrite(pin, HIGH);
        digitalIOWrite(pin, LOW);

        //Wait until pulse is complete (when handleEcho is complete)
        int start = millis();
        while(!pulseComplete) {
            if (millis() - start > 2000) {
                cout << endl << "Pulse Fail" << endl;
                pulse_time = 0;
                break;
            }
        }

        //Calculate distance based on speed of sound and travel time
        int distance = pulse_time * 340 / 10000 / 2;
        pulseComplete = false;
        lastUltrasonicPulse = millis();

        //factor out invalid results
        if (distance > 0 && distance < 600) {
            distances[loops] = distance;
            loops++;
        }
    }
    sort(distances, distances + iterations);
    return distances[iterations/2];
}

float radian(int degree) {
    return degree * M_PI / 180;
}

//Corrects downward facing distance measurement when vehicle changes attitude
int angleCorrection(int rawDistance) {
    return sqrt(pow(rawDistance, 2) / (1 + pow(tan(radian(gyroPitch)),2) + pow(tan(radian(gyroRoll)),2)));
}

void testSensor() {
    for (int it = 1; it < 12; it += 2) {
        for (int s = 30; s < 250; s += 30) {
            int distances[50];
            for (int i = 0; i < 50; i++) {
                int rawDistance = getUltrasonicData(1, it, s);
                distances[i] = rawDistance;
                cout << ".";
                fflush(stdout);
            }
            sort(distances, distances + 50);
            cout << endl << "Average for " << it << " iterations and " << s << " seconds: " 
            << distances[25] << endl;
        }
    }
}