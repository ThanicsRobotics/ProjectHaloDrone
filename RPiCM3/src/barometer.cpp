#include <barometer.h>
#include <types.h>

#include <wiringPi.h>
#include <pigpio.h>
#include <iostream>
#include <bitset>
#include <string.h>
#include <algorithm>
#include <cmath>

#define BARO_ADDR 0x60
#define BARO_TOLERANCE 2
#define SURFACE_OFFSET -1

/// @brief Initializes private variables.
Barometer::Barometer(std::shared_ptr<bool> shutdown)
    : shutdownIndicator(shutdown)
{
    alreadySetup = true;
    setup();
}

Barometer::~Barometer() {
    close();
}

/// @brief Opens I2C port, sets up barometer registers
/// and starts calibration/acclimation thread.
void Barometer::setup() {
    if (alreadySetup) return;
    // Open I2C address
    if ((baroI2cFd = i2cOpen(1, BARO_ADDR, 0)) < 0) {
        printf("%s", strerror(errno));
    }
    i2cConfigured = true;

    i2cWriteByteData(baroI2cFd, 0x26, 0xB0);    //Set OSR = 64          B10110000
    i2cWriteByteData(baroI2cFd, 0x13, 0x07);    //Enable Data Flags     B00000111
    i2cWriteByteData(baroI2cFd, 0x26, 0xB1);    //Set Active            B10110001
    
    //i2cWriteByteData(baroI2cFd, 0x2D, 0); //Set offset to 0 (not working for any other number some reason?)

    // Start calibration thread
    baroThread = std::thread([this]{ calibrate(); });
}

/// @brief Closes I2C port and waits for calibraton thread to join.
void Barometer::close() {
    printf("BARO: Closing\n");

    // Wait for baroThread to join to main thread
    closeCalibrationThread();

    // Waiting for barometer's I2C to stop reading
    // while (readingI2C);

    // Close I2C port
    if(i2cConfigured) i2cClose(baroI2cFd);
    i2cConfigured = false;
}

/// @brief Joins baroThread to main thread.
void Barometer::closeCalibrationThread() {
    while (calState != CalibrationState::DONE);
    // if (calState == CalibrationState::DONE)
    // {
        baroThread.join();
        printf("BARO: Thread joined\n");
        calState = CalibrationState::THREAD_CLOSED;
    // }
}

/// @brief Executed by baroThread to calibrate/acclimate the barometer.
void Barometer::calibrate() {
    printf("Waiting for barometer to acclimate...\n");
    int timer = millis();
    int count = 0;

    // In this loop until range of 30 samples is less than BARO_TOLERANCE in meters,
    // and repeats 2 other times in a row to make sure.
    while (count < 3) {
        float calibrationData[30];
        // Taking 30 samples
        for (int i = 0; i < 30; i++) {
            if(*shutdownIndicator) {
                calState = CalibrationState::DONE;
                return;
            }
            readingI2C = true;
            takeReading();
            readingI2C = false;
            delay(BARO_DELAY);
            readingI2C = true;
            calibrationData[i] = getPressureAltitude();
            
        }
        readingI2C = false;

        // Finds range of the 30 samples, if less than BARO_TOLERANCE, this sample is valid
        // and barometer is acclimated
        auto minmax = std::minmax_element(std::begin(calibrationData), std::end(calibrationData));
        float range = *(minmax.second) - *(minmax.first);
        std::cout << "BARO: Range: " << range << std::endl;
        if (range < BARO_TOLERANCE && range != 0) {
            printf("BARO: successful range\n");
            count++;
        }
        else count = 0;

        // 80 second timout if barometer has not acclimated yet
        if(millis() - timer > 80000) {
            std::cout << "Baro error: failed to acclimate. CNTL-C to exit." << std::endl;
            while(!(*shutdownIndicator));
        }
    }
    printf("BARO: Baro acclimated in %d seconds, now calibrating...\n", (millis() - timer)/1000);
    float calibrationSum = 0;

    // After barometer has acclimated, we take 30 samples from barometer and find average
    // to calculate sea level altitude that corresponds to ground level.
    for (int i = 0; i < 30; i++) {
        if(*shutdownIndicator) {
            calState = CalibrationState::DONE;
            return;
        }
        readingI2C = true;
        takeReading();
        readingI2C = false;
        delay(BARO_DELAY);
        readingI2C = true;
        calibrationSum += getPressureAltitude();
        
    }
    readingI2C = false;
    surfaceAltitude = calibrationSum/30;
    printf("BARO: Surface Altitude: %dm\n", (int)surfaceAltitude);
    fflush(stdout);
    calibrated = true;
    calState = CalibrationState::DONE;
}

/// @brief Signals to barometer to initiate measurement cycle.
void Barometer::takeReading() {
    if (i2cConfigured) {
        unsigned char config = i2cReadByteData(baroI2cFd, 0x26);
        config &= ~(1<<1);  //Clear OST bit
        i2cWriteByteData(baroI2cFd, 0x26, config);
        config = i2cReadByteData(baroI2cFd, 0x26);
        config |= (1<<1);   //Set OST bit
        i2cWriteByteData(baroI2cFd, 0x26, config);
    }
}

/// @brief Gets the current pressure altitude from barometer.
/// @return Current pressure altitude.
float Barometer::getPressureAltitude() {
    if(i2cConfigured) {
        bool gotAltitude = false;
        while (!gotAltitude) {
            if(*shutdownIndicator) return 0.0;
            readingI2C = true;
            int status = i2cReadByteData(baroI2cFd, 0x00);
            if (status & 0x08) {
                pressureMSB = i2cReadByteData(baroI2cFd, 0x01);
                pressureCSB = i2cReadByteData(baroI2cFd, 0x02);
                pressureLSB = i2cReadByteData(baroI2cFd, 0x03);
                tempMSB = i2cReadByteData(baroI2cFd, 0x04);
                tempLSB = i2cReadByteData(baroI2cFd, 0x05);
                readingI2C = false;

                // First 16 bits of 24 bit pressure value are whole numbers,
                // next 4 bits are fractional, so we divide them by 4 bit total (16),
                // and add to whole numbers.
                pressureAltitude = (float)(pressureMSB << 8 | pressureCSB) + (float)((pressureLSB >> 4)/16.0);

                gotAltitude = true;
                if(calibrated) return abs(pressureAltitude - surfaceAltitude) + SURFACE_OFFSET;
                else return pressureAltitude;
            }
        }
    }
}