#ifndef ALTITUDE_H
#define ALTITUDE_H

extern int baroI2cFd;

extern unsigned char pressureMSB;
extern unsigned char pressureCSB;
extern unsigned char pressureLSB;
extern unsigned char tempMSB;
extern unsigned char tempLSB;

extern volatile float pressureAltitude;

extern float loopRate;
extern int loopStartTime;

void setupBarometer();
void takeReading();
void getPressureAltitude();

#endif