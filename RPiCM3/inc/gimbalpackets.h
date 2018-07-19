#ifndef GIMBALPACKETS_H
#define GIMBALPACKETS_H

#include <inttypes.h>
#include <checksum.h>
#include <array>

// Packet Structure:
// * Startsign: 0xFA for incoming messages, and 0xFB for outgoing messages
// * Length: length of the payload, i.e. number of bytes of the data packet excluding
//         Start sign, Length byte, Command byte, and crc word
// * Command: the command byte
// * Payload: as many bytes as expected by the command
// * Checksum: x25 16-bit crc excluding start byte, as used by Mavlink

namespace GimbalPacket
{
    class GetVersion
    {
    public:
        void txParse(std::array<uint8_t, 5>& packet)
        {
            crc = crc_calculate(txPacket.data() + 1, txPacket.size() - 3);
            txPacket[txPacket.size() - 2] = crc & 0xFF;
            txPacket[txPacket.size() - 1] = (crc >> 8) & 0xFF;
            packet = txPacket;
        }
        bool checkCrc(std::array<uint8_t, 11>& rxPacket, const uint16_t& rxCrc) const
        {
            uint16_t calculatedCrc = crc_calculate(rxPacket.data() + 1, rxPacket.size() - 3);
            if (rxCrc == calculatedCrc) return true;
            else return false;
        }
        uint16_t getCrc() const { return crc; }
        constexpr int txSize() const { return txPacket.size(); }
        constexpr int rxSize() const { return 11; }

        struct Rx
        {
            uint8_t start = 0xFA;
            uint8_t length;
            uint8_t command;
            uint8_t receive;
            uint16_t crc;
        };
    private:
        std::array<uint8_t, 5> txPacket = {0xFA, 0x00, 0x01, 0x00, 0x00};
        uint16_t crc = 0;
    };
    

}

#endif