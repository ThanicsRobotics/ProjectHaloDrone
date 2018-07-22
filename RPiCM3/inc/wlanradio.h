#ifndef WLANRADIO_H
#define WLANRADIO_H

#include <types.h>
#include <wlan.h>
#include <functional>

class WLANRadio
{
public:
    WLANRadio(WLAN::DeviceType deviceType, std::string ipAddress, int port);
    void connect(WLAN::DeviceType deviceType, std::string ipAddress, int port);
    void send(messagePacket& msg);
    void setUpdater(std::function<void(std::size_t size)> callback);
    void update(channels& pwmInputs, Maneuver& maneuver, std::size_t size);
    void checkBuffer();

private:
    WLAN wlan;
    messagePacket currentPacket;
    bool connected = false;
    unsigned int seq = 0;

    bool decode(std::array<uint8_t, MAX_BUFFER_SIZE>& packet, channels& pwmInputs, Maneuver& maneuver);
    void encode(messagePacket& msg, std::array<uint8_t, MAX_BUFFER_SIZE>& outPacket);
};

#endif