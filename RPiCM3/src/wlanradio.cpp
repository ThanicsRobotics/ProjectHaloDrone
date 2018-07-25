#include <wlanradio.h>
#include <checksum.h>

#include <algorithm>

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
    std::array<uint8_t, PACKET_SIZE> packet;
    encode(msg, packet);
    wlan.write(packet);
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
    std::array<uint8_t, PACKET_SIZE> msg;
    wlan.getCachedMessage(msg);
    // for (int i = 0; i < size; i++)
    // {
    //     std::cout << msg[i];
    // }
    // std::cout << std::endl;
    decode(msg, pwmInputs, maneuver);
    wlan.read();
}

bool WLANRadio::decode(std::array<uint8_t, PACKET_SIZE>& packet, channels& pwmInputs, Maneuver& maneuver)
{
    // Find start of message
    auto start = std::find(packet.begin(), packet.end(), 0xFF);

    // Checks:
    // 1. 0xFF was actually found
    // 2. Next header byte is 0xFE
    // 3. The start header starts with enough room in the buffer to store the whole message
    if ((*start != *packet.end()) && (*(start + 1) == 0xFE) && (packet.end() - start > PACKET_SIZE))
    {
        currentPacket.msgid = *(start + 2);
        currentPacket.fromid = *(start + 3);
        currentPacket.seqid = *(start + 4);
        pwmInputs.pitchPWM = *(start + 5) << 8 | *(start + 6);
        pwmInputs.rollPWM = *(start + 7) << 8 | *(start + 8);
        pwmInputs.yawPWM = *(start + 9) << 8 | *(start + 10);
        pwmInputs.throttlePWM = *(start + 11) << 8 | *(start + 12);
        maneuver.type = static_cast<ManeuverType>(*(start + 13));
        for (int i = 0; i < maneuver.maneuverOptions.size(); i++)
        {
            maneuver.maneuverOptions[i] = *(start + 14 + i);
        } // 23
        const uint16_t receivedCrc = *(start + 25) << 8 | *(start + 24);

        // Checks if received CRC matches calculated CRC
        if (crc_calculate(packet.data() + 1, packet.size() - 3) != receivedCrc) return false;
        return true;
    }
    return false;
}

// Packet size by parts
//       Header | FromID | MsgID | SeqID | RC Channels | Maneuver | CRC
// Bytes = 2    +   1   +   1   +   1       +   8       +   11    +   2 = 26
void WLANRadio::encode(messagePacket& msg, std::array<uint8_t, PACKET_SIZE>& outPacket)
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
    uint16_t crc = crc_calculate(outPacket.data() + 1, outPacket.size() - 3);
    outPacket[outPacket.size() - 2] = crc & 0xFF;
    outPacket[outPacket.size() - 1] = crc >> 8;
}