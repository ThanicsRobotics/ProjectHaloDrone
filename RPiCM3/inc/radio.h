#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

extern volatile int radioFd;
extern volatile int rollPWM;
extern volatile int pitchPWM;
extern volatile int yawPWM;
extern volatile int altitudePWM;

extern volatile bool serialConfigured;

// void setupRadio();
struct buffer {
    uint8_t *buf;
    uint16_t len;
};

struct receivedMessage {
    int rollPWM;
    int pitchPWM;
    int yawPWM;
    int throttlePWM;
    bool heartbeat;
    long timestamp;
};

buffer sendHeartbeat(uint8_t mode, uint8_t status);
void mavlinkReceiveByte(uint8_t data);
void mavlinkReceivePacket(uint8_t *packet);

#endif