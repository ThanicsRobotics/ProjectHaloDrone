#include <altitude.h>
#include <ultrasonic.h>
#include <serial.h>
#include <pid.h>

#include <wiringPi.h>
#include <pigpio.h>
#include <iostream>

#define BARO_ADDR 0xC0

int baroI2cFd;

char pressureMSB;
char pressureCSB;
char pressureLSB;
char tempMSB;
char tempLSB;

volatile int pressureAltitude = 0;
volatile int altitude = 0;

float loopRate = 0.0;
int loopStartTime = 0;

using namespace std;

void setupBarometer() {
    baroI2cFd = i2cOpen(1, BARO_ADDR, 0);       //Open I2C address
    i2cWriteByteData(baroI2cFd, 0x26, 0x98);    //Set OSR = 8         B10011000
    i2cWriteByteData(baroI2cFd, 0x13, 0x07);    //Enable Data Flags     B00000111
    i2cWriteByteData(baroI2cFd, 0x26, 0xB9);    //Set Active            B10111001
}

void getPressureAltitude() {
    int status = i2cReadByteData(baroI2cFd, 0x00);
    if (status & 0x08) {
        pressureMSB = i2cReadByteData(baroI2cFd, 0x01);
        pressureCSB = i2cReadByteData(baroI2cFd, 0x02);
        pressureLSB = i2cReadByteData(baroI2cFd, 0x03);
        tempMSB = i2cReadByteData(baroI2cFd, 0x04);
        tempLSB = i2cReadByteData(baroI2cFd, 0x05);

        pressureAltitude = (int)((float)(pressureMSB << 16 | pressureCSB << 8 | pressureLSB)*100);
    }
    else {
        cout << "Barometer not ready" << endl;
    }
}

//Using gyro angles and raw distance, calculate absolute altitude of vehicle
void calculateAbsoluteAltitude() {
    loopRate = 1.0 / ((millis() - loopStartTime) / 1000.0);
    loopStartTime = millis();
    int rawDistance = getUltrasonicData(1, 3, 30);
    altitude = angleCorrection(rawDistance);
    cout << "Pitch: " << (int)gyroPitch << " | Roll: " << (int)gyroRoll << endl
        << " | Raw Distance: " << rawDistance << " | Altitude: " << altitude 
        << " | Set Altitude: " << setAltitude << endl
        << "RX Input: " << throttleInput << " | Throttle: " << newThrottle 
        << " | Hz: " << loopRate << endl << endl;
    fflush(stdout);
}