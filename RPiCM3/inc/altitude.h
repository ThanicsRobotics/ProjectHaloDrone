#ifndef ALTITUDE_H
#define ALTITUDE_H

extern int baroI2cFd;

extern char pressureMSB;
extern char pressureCSB;
extern char pressureLSB;
extern char tempMSB;
extern char tempLSB;

extern volatile float pressureAltitude;

extern float loopRate;
extern int loopStartTime;

void setupBarometer();
void takeReading();
void getPressureAltitude();

#endif