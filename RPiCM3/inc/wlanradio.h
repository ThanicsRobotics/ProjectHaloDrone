#ifndef WLANRADIO_H
#define WLANRADIO_H

#include <types.h>
#include <wlan.h>
#include <functional>

class WLANRadio
{
public:
    void connect(std::string ipAddress, int port);
    void send(messagePacket& msg);
    void setUpdater(std::function<void(std::size_t size)> callback) { wlan.setCallback(callback); }
    void update(channels& pwmInputs);
private:
    WLAN wlan;
    messagePacket currentPacket;

    bool decode(std::array<char, MAX_BUFFER_SIZE>& packet, channels& pwmInputs);
};

#endif