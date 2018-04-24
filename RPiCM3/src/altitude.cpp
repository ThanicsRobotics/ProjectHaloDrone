#include <altitude.h>
#include <pid.h>

#include <wiringPi.h>
#include <pigpio.h>
#include <iostream>
#include <bitset>

#define BARO_ADDR 0x60

int baroI2cFd;

int8_t pressureMSB;
int8_t pressureCSB;
int8_t pressureLSB;
int8_t tempMSB;
int8_t tempLSB;

volatile float pressureAltitude = 0;
volatile float altitude = 0;
float surfaceAltitude = 0;

float loopRate = 0.0;
int loopStartTime = 0;

using namespace std;

void setupBarometer() {
    baroI2cFd = i2cOpen(1, BARO_ADDR, 0);       //Open I2C address
    i2cWriteByteData(baroI2cFd, 0x26, 0xB8);    //Set OSR = 8         B10011000
    i2cWriteByteData(baroI2cFd, 0x13, 0x07);    //Enable Data Flags     B00000111
    i2cWriteByteData(baroI2cFd, 0x26, 0xB9);    //Set Active            B10011001
    takeReading();

    float altitudeSum = 0;
    //Calibrate to current altitude
    for(int i = 0; i < 10; i++) {
        altitudeSum += getPressureAltitude();
        cout << ".";
        fflush(stdout);
    }
    surfaceAltitude = altitudeSum/10;
    cout << endl << "Surface Alt: " << surfaceAltitude << endl;
    delay(600);
    i2cWriteByteData(baroI2cFd, 0x2D, (int)surfaceAltitude);
}

void takeReading() {
    unsigned char config = i2cReadByteData(baroI2cFd, 0x26);
    config &= ~(1<<1);  //Clear OST bit
    i2cWriteByteData(baroI2cFd, 0x26, config);
    config = i2cReadByteData(baroI2cFd, 0x26);
    config |= (1<<1); //Set OST bit
    i2cWriteByteData(baroI2cFd, 0x26, config);
}

float getPressureAltitude() {
    bool gotAltitude = false;
    while (!gotAltitude) {
        int status = i2cReadByteData(baroI2cFd, 0x00);
        if (status & 0x08) {
            pressureMSB = i2cReadByteData(baroI2cFd, 0x01);
            pressureCSB = i2cReadByteData(baroI2cFd, 0x02);
            pressureLSB = i2cReadByteData(baroI2cFd, 0x03);
            tempMSB = i2cReadByteData(baroI2cFd, 0x04);
            tempLSB = i2cReadByteData(baroI2cFd, 0x05);

            pressureAltitude = (float)(pressureMSB << 8 | pressureCSB) + (float)((pressureLSB >> 4)/16.0);
            //bitset<24> x(pressureMSB << 16 | pressureCSB << 8 | pressureLSB); 
            //cout << "Altitude: " << pressureAltitude << endl;
            gotAltitude = true;
            return pressureAltitude;
        }
        else {
            //cout << "Barometer not ready" << endl;
            //takeReading();
        }
    }
    takeReading();
}