#include <barometer.h>
#include <wiringPi.h>
#include <pigpio.h>
#include <iostream>
#include <bitset>
#include <string.h>
#include <algorithm>
#include <math.h>

#define BARO_ADDR 0x60

Barometer::Barometer() {
    surfaceAltitude = 0;
    calibrated = false;
}

Barometer::~Barometer() {
    
}

void Barometer::setup() {
    if ((baroI2cFd = i2cOpen(1, BARO_ADDR, 0)) < 0) {       //Open I2C address
        printf("%s", strerror(errno));
    }
    i2cConfigured = true;
    i2cWriteByteData(baroI2cFd, 0x26, 0xB0);    //Set OSR = 64         B10110000
    i2cWriteByteData(baroI2cFd, 0x13, 0x07);    //Enable Data Flags     B00000111
    i2cWriteByteData(baroI2cFd, 0x26, 0xB1);    //Set Active            B10110001
    
    //i2cWriteByteData(baroI2cFd, 0x2D, 0); //Set offset to 0 (not working for any other number some reason?)
    baroThread = std::thread([this]{ calibrate(); });
}

void Barometer::close() {
    printf("waiting for baro i2c to stop reading...\n");
    while (readingI2C);
    if(i2cConfigured) i2cClose(baroI2cFd);
    i2cConfigured = false;
    closeCalibrationThread();
}

void Barometer::closeCalibrationThread() {
    baroThread.join();
}

void Barometer::calibrate() {
    printf("Waiting for barometer to acclimate...\n");
    int timer = millis();
    int count = 0;
    while (count < 3) {
        float calibrationData[30];
        for (int i = 0; i < 30; i++) {
            if(!i2cConfigured) return;
            readingI2C = true;
            takeReading();
            readingI2C = false;
            delay(BARO_DELAY);
            readingI2C = true;
            calibrationData[i] = getPressureAltitude();
            
        }
        auto minmax = std::minmax_element(std::begin(calibrationData), std::end(calibrationData));
        float range = *(minmax.second) - *(minmax.first);
        std::cout << "BARO: Range: " << range << std::endl;
        if (range < 2 && range != 0) {
            printf("BARO: successful range\n");
            count++;
        }
        else count = 0;

        if(millis() - timer > 80000) {
            std::cout << "Baro error: failed to acclimate. CNTL-C to exit." << std::endl;
            while(1);
        }
    }
    printf("BARO: Baro acclimated in %d seconds, now calibrating...\n", (millis() - timer)/1000);
    float calibrationSum = 0;
    for (int i = 0; i < 30; i++) {
        if(!i2cConfigured) return;
        readingI2C = true;
        takeReading();
        readingI2C = false;
        delay(BARO_DELAY);
        readingI2C = true;
        calibrationSum += getPressureAltitude();
        
    }
    surfaceAltitude = calibrationSum/30;
    printf("BARO: Surface Altitude: %dm\n", (int)surfaceAltitude);
    fflush(stdout);
    calibrated = true;
}

void Barometer::takeReading() {
    if (i2cConfigured) {
        unsigned char config = i2cReadByteData(baroI2cFd, 0x26);
        config &= ~(1<<1);  //Clear OST bit
        i2cWriteByteData(baroI2cFd, 0x26, config);
        config = i2cReadByteData(baroI2cFd, 0x26);
        config |= (1<<1); //Set OST bit
        i2cWriteByteData(baroI2cFd, 0x26, config);
    }
}

float Barometer::getPressureAltitude() {
    if(i2cConfigured) {
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
                //std::cout << pressureAltitude << std::endl;
                if(calibrated) return abs(pressureAltitude - surfaceAltitude) - 4;
                else return pressureAltitude;
            }
        }
    }
}