#include <wlanradio.h>
#include <algorithm>

#define PACKET_SIZE 11

void WLANRadio::connect(std::string ipAddress, int port)
{

}

void WLANRadio::send(messagePacket& msg)
{

}

void WLANRadio::setUpdater(std::function<void(std::size_t size)> callback)
{ 
    wlan.setCallback(callback);
}

void WLANRadio::update(channels& pwmInputs)
{
    wlan.checkBuffer();
    std::array<char, MAX_BUFFER_SIZE> msg;
    wlan.getCachedMessage(msg);
    decode(msg, pwmInputs);
}

bool WLANRadio::decode(std::array<char, MAX_BUFFER_SIZE>& packet, channels& pwmInputs)
{
    // Find start of message
    auto start = std::find(packet.begin(), packet.end(), 0xFF);

    // Checks:
    // 1. 0xFF was actually found
    // 2. Next header byte is 0xFE
    // 3. The start header starts with enough room in the buffer to store the whole message
    if ((*start != *packet.end()) && (*(++start) == 0xFE) && (packet.end() - start > PACKET_SIZE + 1))
    {
        currentPacket.msgid = *(++start);
        currentPacket.fromid = *(++start);
        currentPacket.seqid = *(++start);
        pwmInputs.throttlePWM = *(++start) << 8 | *(++start);
        pwmInputs.pitchPWM = *(++start) << 8 | *(++start);
        pwmInputs.rollPWM = *(++start) << 8 | *(++start);
        pwmInputs.yawPWM = *(++start) << 8 | *(++start);
        return true;
    }
    return false;
}