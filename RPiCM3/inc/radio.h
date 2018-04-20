#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

extern int radioFd;
extern int rollPWM;
extern int pitchPWM;
extern int yawPWM;
extern int altitudePWM;

extern volatile bool serialConfigured;

void setupRadio();
void sendHeartbeat(uint8_t mode, uint8_t status);
void mavlinkReceive();
void *serialLoop(void *void_ptr);

#endif