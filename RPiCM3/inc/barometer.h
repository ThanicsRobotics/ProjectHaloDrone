#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <stdint.h>
#include <thread>

#define BARO_DELAY 260

class Barometer
{
public:
    Barometer();
    ~Barometer();
    void setup();
    void close();
    void closeCalibrationThread();
    void takeReading();
    float getPressureAltitude();
    bool isI2CConfigured() const { return i2cConfigured; }
    bool isCalibrated() const { return calibrated; }

private:
    int baroI2cFd;
    bool i2cConfigured;
    bool calibrated;
    bool readingI2C;

    std::thread baroThread;
    void calibrate();

    int8_t pressureMSB, pressureCSB, pressureLSB;
    int8_t tempMSB, tempLSB;

    float pressureAltitude, surfaceAltitude;
};

#endif