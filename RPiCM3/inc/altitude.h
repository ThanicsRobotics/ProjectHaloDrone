#ifndef ALTITUDE_H
#define ALTITUDE_H

int baroI2cFd;

char pressureMSB;
char pressureCSB;
char pressureLSB;
char tempMSB;
char tempLSB;

volatile int pressureAltitude;

float loopRate;
int loopStartTime;

void setupBarometer();
void getPressureAltitude();
void calculateAbsoluteAltitude();

#endif