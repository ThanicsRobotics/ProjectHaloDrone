#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <memory>

extern int radioFd;
extern int rollPWM;
extern int pitchPWM;
extern int yawPWM;
extern int altitudePWM;

extern bool serialConfigured;

struct buffer
{
    uint16_t len;
    std::shared_ptr<uint8_t[]> buf;
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