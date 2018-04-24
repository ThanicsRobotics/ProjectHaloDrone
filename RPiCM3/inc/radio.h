#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

extern volatile int radioFd;
extern volatile int rollPWM;
extern volatile int pitchPWM;
extern volatile int yawPWM;
extern volatile int altitudePWM;

extern volatile bool serialConfigured;

void setupRadio();
uint8_t *sendHeartbeat(uint8_t mode, uint8_t status);
void mavlinkReceiveByte();
void mavlinkReceivePacket();
void *serialLoop(void *void_ptr);

#endif