#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <stdint.h>

class Barometer
{
public:
    Barometer();
    ~Barometer();
    void setupI2C();
    void takeReading();
    float getPressureAltitude();
    bool isI2CConfigured() { return i2cConfigured; }
private:
    int baroI2cFd;
    bool i2cConfigured;

    int8_t pressureMSB, pressureCSB, pressureLSB;
    int8_t tempMSB, tempLSB;

    float pressureAltitude, surfaceAltitude;
};

#endif