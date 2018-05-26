/// @file radio.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of the Radio class template.

#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <memory>
#include <mavlink/common/mavlink.h>
#include <pigpio.h>
#include <iostream>
#include <wiringPi.h>
#include <algorithm>
#include <array>

#include <serial.h>
#include <stream.h>
#include <flightcontroller.h>
#include <types.h>

#define SYSID 1
#define COMPID 1
#define PAYLOAD_LEN 11

/// @brief Custom message structure for communication network.
/// Message payload is exactly PAYLOAD_LEN bytes long.
struct receivedMessage {
    uint8_t msgid;          ///< ID of type of message.
    uint8_t fromid;         ///< ID of sender system.
    uint8_t seqid;          ///< Sequential message number, used for checking message continuity.
    channels rcChannels;
};

/// @brief Contains states of message parsing process.
enum MSG_STATE {
    WAITING = 0,    ///< Waiting for starting header.
    FILLING = 1,    ///< Message has started, and filling buffer.
    FAIL = 2,       ///< Buffer overflowed, or other error.
    DONE = 3        ///< Message footer found, finished filing buffer.
};

/// @brief Class template for controlling the radio, whether a WLAN radio, or UART Serial.
/// Specify "Serial" or "Stream" based Radio in template argument
/// for example: Radio<Serial> will generate a Serial-based radio object
template<typename InterfaceType>
class Radio : public InterfaceType
{
public:
    //radioBuffer& sendHeartbeat(uint8_t mode, uint8_t status);
    //void mavlinkReceiveByte(uint8_t data);
    //void mavlinkReceivePacket(uint8_t *packet);

    /// @brief Checks to see if the receiver thread is running.
    /// @return True if running thread, false if not.
    bool isReceiveThreadActive() const { return receiveThreadActive; }

    //void startReceiveLoop();
    void stopReceiveLoop();

    /// @brief Convience function for inputing each byte received by radio and
    /// parsing it into a completed message.
    /// @param data Byte from radio stream.
    void customReceiveByte(uint8_t data, FlightController& fc);

private:
    channels pwmInputs;         ///< Current PWM control inputs.
    uint8_t controllerStatus;   ///< Status of controller operation (currently not used). 
    std::thread receiveThread;  ///< Thread that will independently get and parse comms data.
    bool receiveThreadActive;   ///< True if receive thread is running, false if not.

    /// @brief Loop executed by receive thread, gets data from interface and parses it.
    //void receiveLoop();

    /// @brief Parses incoming radio data and modifies a receivedMessage object to contain that data.
    /// @param data Byte from radio interface.
    /// @param msg A reference to the receivedMessage object to be modified.
    /// @return True if message is completed, false if error or still filling buffer.
    bool customParseChar(uint8_t data, receivedMessage& msg);
};

// template<>
// void Radio<Serial>::receiveLoop() {
//     int heartbeatTimer = millis();
//     int byteCount = 0;
//     while (receiveThreadActive) {
//         byteCount++;
//         //mavlinkReceiveByte(readChar());
//         //customReceiveByte(readChar(), fc);
        
//         //Every second, send heartbeat to controller
//         // if (millis() - heartbeatTimer > 1000) {
//         //     radioBuffer msg = sendHeartbeat(0,3); //Heartbeat in PREFLIGHT mode and STANDBY state
//         //     this->write(msg.buf, msg.len);
//         //     printf("rate: %dkbps\n", (byteCount*8)/1000);
//         //     byteCount = 0;
//         //     heartbeatTimer = millis();
//         // }
//     }
// }

// template<>
// void Radio<Stream>::receiveLoop() {
//     // int heartbeatTimer = millis();
//     // while (serialThreadActive) {
//     //     mavlinkReceiveByte(readChar());
//     //     //Every second, send heartbeat to controller
//     //     if (millis() - heartbeatTimer > 1000) {
//     //         radioBuffer msg = radio.sendHeartbeat(0,3); //Heartbeat in PREFLIGHT mode and STANDBY state
//     //         radio.write(msg.buf, msg.len);
//     //         heartbeatTimer = millis();
//     //     }
//     // }
// }

/// @brief start serial thread loop.
// template<typename InterfaceType>
// void Radio<InterfaceType>::startReceiveLoop() {
//     //std::cout << "starting thread\n";
//     receiveThreadActive = true;
//     receiveThread = std::thread([this]{ receiveLoop(); });
// }

template<typename InterfaceType>
void Radio<InterfaceType>::stopReceiveLoop() {
    if (receiveThreadActive) {
        receiveThreadActive = false;
        receiveThread.join();
    }
}

template<typename InterfaceType>
void Radio<InterfaceType>::customReceiveByte(uint8_t data, FlightController& fc) {
    static uint32_t timer = 0;
    static receivedMessage msg;
    if (customParseChar(data, msg)) {
        std::cout << "Received msg from " << (int)msg.fromid
            << ", seq: " << (int)msg.seqid
            << "\nThrottle: " << msg.rcChannels.throttlePWM
            << "\nPitch: " << msg.rcChannels.pitchPWM
            << "\nRoll: " << msg.rcChannels.rollPWM
            << "\nYaw: " << msg.rcChannels.yawPWM
            << "\nLast Message: " << micros() - timer << "us"
            << "\n----------" << std::endl;
        timer = micros();

        // Sending incoming PWM inputs into flight controller
        fc.setPWMInputs(msg.rcChannels);
    }
}

template<typename InterfaceType>
bool Radio<InterfaceType>::customParseChar(uint8_t data, receivedMessage& msg) {
    //printf("here2\n");
    static uint8_t msgCache[100];
    static int index = 0;
    static int startIndex = 0;
    static MSG_STATE state = MSG_STATE::WAITING;
    msgCache[index] = data;
    // printf("%x\n", data);

    if (index == 0) {
        index++;
        return false;
    }

    switch (state) {
        case MSG_STATE::WAITING:
            if (msgCache[index - 1] == 0xFF && msgCache[index] == 0xFE) {
                // printf("start\n");
                startIndex = index + 1;
                state = MSG_STATE::FILLING;
            }
            break;
        case MSG_STATE::FILLING:
            if (msgCache[index - 1] == 0xFD && msgCache[index] == 0xFC) {
                // printf("index: %d\n", index);
                // printf("startindex: %d\n", startIndex);
                // printf("range: %d\n", index - startIndex);
                if (index - startIndex - 1 == PAYLOAD_LEN) {
                    receivedMessage outMsg;
                    outMsg.msgid = msgCache[startIndex];
                    outMsg.fromid = msgCache[startIndex + 1];
                    outMsg.seqid = msgCache[startIndex + 2];
                    outMsg.rcChannels.throttlePWM = msgCache[startIndex + 3] << 8 | msgCache[startIndex + 4];
                    outMsg.rcChannels.pitchPWM = msgCache[startIndex + 5] << 8 | msgCache[startIndex + 6];
                    outMsg.rcChannels.rollPWM = msgCache[startIndex + 7] << 8 | msgCache[startIndex + 8];
                    outMsg.rcChannels.yawPWM = msgCache[startIndex + 9] << 8 | msgCache[startIndex + 10];
                    msg = outMsg;

                    //std::copy(msgCache + startIndex, msgCache + index - 2, buf.begin());
                    state = MSG_STATE::DONE;
                    //printf("msgDone\n");
                }
                else {
                    state = MSG_STATE::FAIL;
                }
            }
            break;
        default:
            break;
    }

    if (index == 99) state = MSG_STATE::FAIL;
    if (state >= MSG_STATE::FAIL) {
        std::fill(msgCache, msgCache + 99, 0);
        index = 0;
        startIndex = 0;
        

        if (state == MSG_STATE::DONE) {
            state = MSG_STATE::WAITING;
            return true;
        }
        else {
            state = MSG_STATE::WAITING;
            return false;
        }
    }
    index++;
    return false;
}

///////
///
///  ***MAVLink Usage on hold until I can figure out why it's so slow.
///
///////

// template<typename InterfaceType>
// radioBuffer& Radio<InterfaceType>::sendHeartbeat(uint8_t mode, uint8_t status) {
//     mavlink_message_t msg;
//     uint16_t len;
//     static uint8_t buf[MAVLINK_MAX_PACKET_LEN];

//     mavlink_msg_heartbeat_pack(SYSID, COMPID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, mode, 0, status);
//     len = mavlink_msg_to_send_buffer(buf, &msg);

//     static radioBuffer sendBuffer;
//     //sendBuffer.buf = (std::shared_ptr<uint8_t[]>)buf;
//     sendBuffer.buf = (uint8_t*)buf;
//     sendBuffer.len = len;
//     return sendBuffer;
// }

// template<typename InterfaceType>
// void Radio<InterfaceType>::mavlinkReceivePacket(uint8_t *packet) {
//     uint8_t byte = 1;
//     int i = 0;
//     while (byte != '\0') {
//         printf("byte %d: %d\n", i, byte);
//         fflush(stdout);
//         byte = packet[i];
//         mavlinkReceiveByte(byte);
//         i += 1;
//     }
// }

// template<typename InterfaceType>
// void Radio<InterfaceType>::mavlinkReceiveByte(uint8_t data) {
//     mavlink_message_t msg;
//     mavlink_status_t status;
//     if(mavlink_parse_char(MAVLINK_COMM_0, data, &msg, &status)) {
//         printf("Received message with ID %d, sequence: %d from component %d of system %d, len: %d\n", 
//         msg.msgid, msg.seq, msg.compid, msg.sysid, msg.len);
//         delay(100);
//         switch(msg.msgid) {
//             case MAVLINK_MSG_ID_HEARTBEAT:
//                 mavlink_heartbeat_t hb;
//                 mavlink_msg_heartbeat_decode(&msg, &hb);
//                 controllerStatus = hb.system_status;
//                 // printf("Heartbeat from: %d, mode: %d, status: %d\n", 
//                 //     hb.type, hb.base_mode, controllerStatus);
//                 //     delay(2000);
//                 break;
//             case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
//                 mavlink_rc_channels_raw_t channels;
//                 mavlink_msg_rc_channels_raw_decode(&msg, &channels);
//                 pwmInputs.pitchPWM = channels.chan1_raw;
//                 pwmInputs.rollPWM = channels.chan2_raw;
//                 pwmInputs.yawPWM = channels.chan3_raw;
//                 pwmInputs.throttlePWM = channels.chan4_raw;

//                 std::cout << "Pitch: " << pwmInputs.pitchPWM
//                 << "\nRoll: " << pwmInputs.rollPWM
//                 << "\nYaw: " << pwmInputs.yawPWM
//                 << "\nthrottle: " << pwmInputs.throttlePWM << "\n----\n";
//                 fflush(stdout);
//                 // printf("rcchannels\n");
//                 // delay(2000);
//                 break;
//             case MAVLINK_MSG_ID_COMMAND_LONG:
//                 mavlink_command_long_t command;
//                 switch (command.command) {
//                     case MAV_CMD_NAV_LAND:
//                         break;
//                     case MAV_CMD_NAV_TAKEOFF:
//                         break;
//                 }
//                 break;
//             default:
//                 break;
//         }
//     }
//     //else printf("fail\n");
// }

#endif