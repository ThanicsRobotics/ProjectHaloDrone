#ifndef ALTITUDE_H
#define ALTITUDE_H

extern int baroI2cFd;

extern char pressureMSB;
extern char pressureCSB;
extern char pressureLSB;
extern char tempMSB;
extern char tempLSB;

extern volatile int pressureAltitude;

extern float loopRate;
extern int loopStartTime;

void setupBarometer();
void getPressureAltitude();
void calculateAbsoluteAltitude();

#endif