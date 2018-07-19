#ifndef GIMBALPACKETS_H
#define GIMBALPACKETS_H

#include <checksum.h>

#include <inttypes.h>
#include <array>
#include <iostream>

// Packet Structure:
// * Startsign: 0xFA for incoming messages, and 0xFB for outgoing messages
// * Length: length of the payload, i.e. number of bytes of the data packet excluding
//         Start sign, Length byte, Command byte, and crc word
// * Command: the command byte
// * Payload: as many bytes as expected by the command
// * Checksum: x25 16-bit crc excluding start byte, as used by Mavlink

namespace GimbalPacket
{
    namespace Ack
    {
        // Packet Structure:
        // 0xFB 0x01 0x96 data-byte crc-low-byte crc-high-byte
        //
        // This command is send by the STorM32 controller to acknowledge execution of a 
        // received RC command message (if the message itself doesn't lead to a response, 
        // such as e.g. the get parameter command)

        constexpr int size = 6;
        class Packet
        {
        public:
            bool error(const std::array<uint8_t, size>& packet)
            {
                result = (Result)packet[3];
                switch(result)
                {
                    case Result::OK:
                        return false;
                    default:
                        std::cout << "Gimbal ACK error" << std::endl;
                        return true;
                }
            }
            bool isCrcGood(std::array<uint8_t, size>& rxPacket) const
            {
                // CRC accumulates from after start byte up until last 2 bytes
                uint16_t calculatedCrc = crc_calculate(rxPacket.data() + 1, rxPacket.size() - 3);
                // Get last two bytes of rxPacket as CRC and compare to calculated CRC
                if ((rxPacket[size - 1] << 8 | rxPacket[size - 2]) == calculatedCrc) return true;
                else return false;
            }
        private:
            enum Result
            {
                OK = 0,
                ERR_FAIL = 1,
                ERR_ACCESS_DENIED = 2,
                ERR_NOT_SUPPORTED = 3,
                ERR_TIMEOUT = 150,
                ERR_CRC = 151,
                ERR_PAYLOADLEN = 152
            } result;
            //std::array<uint8_t, size> ack = {0xFB, 0x01, 0x96, 0x00, 0x00, 0x00};
        };
    } // Ack
    
    namespace GetVersion
    {
        // TX Packet Structure:
        // 0xFA 0x00 0x01 crc-low-byte crc-high-byte
        //
        // If an error occurred a CMD_ACK message will be emitted. Otherwise a message 
        // containing the firmware version, the setup layout version and board capabilities 
        // in this format will be emitted:
        //
        // RX Packet Structure:
        // 0xFB 0x06 0x00 data1-low data1-high data2-low data2-high data3-low data3-high crc-low-byte crc-high-byte
        // Data1 is the firmware version, data2 the setup layout version, and data3 holds the board capabilities value
        constexpr int txSize = 5;
        constexpr int rxSize = 11;
        class Packet
        {
        public:
            constexpr int txPacketSize() const { return txSize; }
            constexpr int rxPacketSize() const { return rxSize; }
            void txParse(std::array<uint8_t, txSize>& packet)
            {
                uint16_t crc = crc_calculate(txPacket.data() + 1, txPacket.size() - 3);
                txPacket[txPacket.size() - 2] = crc & 0xFF;
                txPacket[txPacket.size() - 1] = (crc >> 8) & 0xFF;
                packet = txPacket;
            }
            bool isCrcGood(std::array<uint8_t, rxSize>& rxPacket) const
            {
                uint16_t calculatedCrc = crc_calculate(rxPacket.data() + 1, rxPacket.size() - 3);
                // Get last two bytes of rxPacket as CRC and compare to calculated CRC
                if ((rxPacket[rxSize - 1] << 8 | rxPacket[rxSize - 2]) == calculatedCrc) return true;
                else return false;
            }            

        private:
            std::array<uint8_t, txSize> txPacket = {0xFA, 0x00, 0x01, 0x00, 0x00};
        };
    }
    
    namespace SetAngles
    {
        // TX Packet Structure:
        // 0xFA 0x0E 0x11 float1 float2 float3 flags-byte type-byte crc-low-byte crc-high-byte
        //
        // The float1, float2, float3 fields represent 4 bytes each. They are of type float, and correspond 
        // to the pitch, roll, and yaw angles in degree. The flags byte allows to modify the behavior of the 
        // angle setting for each axis. They can be in the unlimited or limited mode. In unlimited mode the 
        // respective axis can be set to any angle without restriction, bypassing the RcMin and RcMax settings, 
        // and working for both “relative” and “absolute”. In limited mode the angle setting is subject to the RcMin 
        // and RcMax settings, and works only for “absolute”. The first bit of the flags byte, 0x01, corresponds to 
        // pitch, 0x02 to roll, and 0x04 to yaw, and when set the respective axis is in limited mode. The type byte 
        // is not used currently and has to be set to zero. As response to this command a CMD_ACK message will be emitted.
        constexpr int txSize = 19;
        constexpr int rxSize = Ack::size;
        class Packet
        {
        public:
            constexpr int txPacketSize() const { return txSize; }
            constexpr int rxPacketSize() const { return rxSize; }
            void txParse(std::array<uint8_t, txSize>& packet, float pitch, float roll, float yaw)
            {
                uint16_t crc = crc_calculate(txPacket.data() + 1, txPacket.size() - 3);
                txPacket[txPacket.size() - 2] = crc & 0xFF;
                txPacket[txPacket.size() - 1] = (crc >> 8) & 0xFF;
                embedFloat(packet, 3, pitch);
                embedFloat(packet, 7, roll);
                embedFloat(packet, 11, yaw);
                packet = txPacket;
            }
            bool isCrcGood(std::array<uint8_t, rxSize>& rxPacket) const
            {
                Ack::Packet ack;
                ack.error(rxPacket);
                return ack.isCrcGood(rxPacket);
            }

        private:
            void embedFloat(std::array<uint8_t, txSize>& packet, int startIndex, float floatValue)
            {
                uint32_t value = (uint32_t)floatValue;
                packet[startIndex] = value >> 24;
                packet[startIndex + 1] = (value >> 16) & 0xFF;
                packet[startIndex + 2] = (value >> 8) & 0xFF;
                packet[startIndex + 3] = value & 0xFF;
            }
            std::array<uint8_t, txSize> txPacket = {0xFA, 0x0E, 0x11, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        };
    }
}

#endif