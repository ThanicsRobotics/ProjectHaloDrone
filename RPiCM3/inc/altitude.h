#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <stdint.h>

extern int baroI2cFd;

extern int8_t pressureMSB;
extern int8_t pressureCSB;
extern int8_t pressureLSB;
extern int8_t tempMSB;
extern int8_t tempLSB;

extern volatile float pressureAltitude;
extern float surfaceAltitude;

extern float loopRate;
extern int loopStartTime;

void setupBarometer();
void takeReading();
void getPressureAltitude();

#endif