#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <memory>
#include <serial.h>
#include <stream.h>

//Specify "Serial" or "Stream" based Radio in template argument
//for example: Radio<Serial> will generate a Serial-based radio object
template<class InterfaceType>
class Radio : public InterfaceType
{
public:
    struct buffer
    {
        uint16_t len;
        std::shared_ptr<uint8_t[]> buf;
    };

    struct channels {
        uint16_t rollPWM, pitchPWM, yawPWM, throttlePWM;
    };

    buffer sendHeartbeat(uint8_t mode, uint8_t status);
    void mavlinkReceiveByte(uint8_t data);
    void mavlinkReceivePacket(uint8_t *packet);
    channels getRCChannels();

private:
    channels pwmInputs;
};

#endif