#include <wlanradio.h>
#include <algorithm>

#define PACKET_SIZE 24

WLANRadio::WLANRadio(WLAN::DeviceType deviceType, std::string ipAddress, int port)
    : wlan(deviceType, ipAddress, port), connected(true)
{
    
}

void WLANRadio::connect(WLAN::DeviceType deviceType, std::string ipAddress, int port)
{
    if (!connected) 
    {
        wlan.start(deviceType, ipAddress, port);
        wlan.read();
    }
}

void WLANRadio::send(messagePacket& msg)
{

}

void WLANRadio::setUpdater(std::function<void(std::size_t size)> callback)
{ 
    wlan.setCallback(callback);
    wlan.read();
}

void WLANRadio::checkBuffer()
{
    wlan.checkBuffer();
}

void WLANRadio::update(channels& pwmInputs, Maneuver& maneuver, std::size_t size)
{
    std::array<uint8_t, MAX_BUFFER_SIZE> msg;
    wlan.getCachedMessage(msg);
    for (int i = 0; i < size; i++)
    {
        std::cout << msg[i];
    }
    std::cout << std::endl;
    decode(msg, pwmInputs, maneuver);
    wlan.read();
}

bool WLANRadio::decode(std::array<uint8_t, MAX_BUFFER_SIZE>& packet, channels& pwmInputs, Maneuver& maneuver)
{
    // Find start of message
    auto start = std::find(packet.begin(), packet.end(), 0xFF);

    // Checks:
    // 1. 0xFF was actually found
    // 2. Next header byte is 0xFE
    // 3. The start header starts with enough room in the buffer to store the whole message
    if ((*start != *packet.end()) && (*(++start) == 0xFE) && (packet.end() - start > PACKET_SIZE))
    {
        currentPacket.msgid = *(++start);
        currentPacket.fromid = *(++start);
        currentPacket.seqid = *(++start);
        pwmInputs.pitchPWM = *(++start) << 8 | *(++start);
        pwmInputs.rollPWM = *(++start) << 8 | *(++start);
        pwmInputs.yawPWM = *(++start) << 8 | *(++start);
        pwmInputs.throttlePWM = *(++start) << 8 | *(++start);
        maneuver.type = static_cast<ManeuverType>(*(++start));
        for (int i = 0; i < maneuver.maneuverOptions.size(); i++)
        {
            maneuver.maneuverOptions[i] = *(++start);
        }
        return true;
    }
    return false;
}

void WLANRadio::encode(messagePacket& msg, std::array<uint8_t, MAX_BUFFER_SIZE>& outPacket)
{
    msg.fromid = 0;
    msg.msgid = 0;
    msg.seqid = seq++;
    msg.rcChannels.pitchPWM = 1500;
    msg.rcChannels.rollPWM = 1500;
    msg.rcChannels.yawPWM = 1500;
    msg.rcChannels.throttlePWM = 1000;
    outPacket[0] = 0xFF;
    outPacket[1] = 0xFE;
    outPacket[2] =  msg.fromid;
    outPacket[3] =  msg.msgid;
    outPacket[4] =  msg.seqid;
    outPacket[5] = (msg.rcChannels.pitchPWM >> 8) & 0xFF;
    outPacket[6] = msg.rcChannels.pitchPWM & 0xFF;
    outPacket[7] = (msg.rcChannels.rollPWM >> 8) & 0xFF;
    outPacket[8] = msg.rcChannels.rollPWM & 0xFF;
    outPacket[9] = (msg.rcChannels.yawPWM >> 8) & 0xFF;
    outPacket[10] = msg.rcChannels.yawPWM & 0xFF;
    outPacket[11] = (msg.rcChannels.throttlePWM >> 8) & 0xFF;
    outPacket[12] = msg.rcChannels.throttlePWM & 0xFF;
    outPacket[13] = static_cast<uint8_t>(msg.requestedManeuver.type);
    for (int i = 0; i < msg.requestedManeuver.maneuverOptions.size(); i++)
    {
        outPacket[14 + i] = msg.requestedManeuver.maneuverOptions[i];
    }
}