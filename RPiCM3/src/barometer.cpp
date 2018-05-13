#include <barometer.h>
#include <wiringPi.h>
#include <pigpio.h>
#include <iostream>
#include <bitset>
#include <string.h>

#define BARO_ADDR 0x60
#define BARO_DELAY 500

Barometer::Barometer() {
    surfaceAltitude = 0;
}

Barometer::~Barometer() {
    
}

void Barometer::setupI2C() {
    if ((baroI2cFd = i2cOpen(1, BARO_ADDR, 0)) < 0) {       //Open I2C address
        printf("%s", strerror(errno));
    }
    i2cConfigured = true;
    i2cWriteByteData(baroI2cFd, 0x26, 0xB8);    //Set OSR = 8         B10011000
    i2cWriteByteData(baroI2cFd, 0x13, 0x07);    //Enable Data Flags     B00000111
    i2cWriteByteData(baroI2cFd, 0x26, 0xB9);    //Set Active            B10011001
    this->takeReading();

    float altitudeSum = 0;
    const int iterations = 5;
    //Calibrate to current altitude
    for(int i = 0; i < iterations; i++) {
        altitudeSum += getPressureAltitude();
        //printf("%f\n", altitudeSum);
        takeReading();
        delay(BARO_DELAY);
        std::cout << ".";
        fflush(stdout);
    }
    surfaceAltitude = altitudeSum/iterations;
    std::cout << "\nSurface Alt: " << surfaceAltitude << "\n";
    delay(600);
    i2cWriteByteData(baroI2cFd, 0x2D, 0); //Set offset to 0 (not working for any other number some reason?)
}

void Barometer::takeReading() {
    unsigned char config = i2cReadByteData(baroI2cFd, 0x26);
    config &= ~(1<<1);  //Clear OST bit
    i2cWriteByteData(baroI2cFd, 0x26, config);
    config = i2cReadByteData(baroI2cFd, 0x26);
    config |= (1<<1); //Set OST bit
    i2cWriteByteData(baroI2cFd, 0x26, config);
}

float Barometer::getPressureAltitude() {
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
            gotAltitude = true;
            return pressureAltitude - surfaceAltitude;
        }
    }
}