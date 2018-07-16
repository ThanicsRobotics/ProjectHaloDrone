#include <serialradio.h>

#include <stdint.h>
#include <memory>
#include <pigpio.h>
#include <iostream>
#include <wiringPi.h>
#include <algorithm>
#include <array>

void SerialRadio::customReceiveByte(uint8_t data, channels& rcChannels) {
    static uint32_t timer = 0;
    static messagePacket msg;
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
        rcChannels.pitchPWM = msg.rcChannels.pitchPWM;
        rcChannels.rollPWM = msg.rcChannels.rollPWM;
        rcChannels.yawPWM = msg.rcChannels.yawPWM;
        rcChannels.throttlePWM = msg.rcChannels.throttlePWM;
    }
}

bool SerialRadio::customParseChar(uint8_t data, messagePacket& msg) {
    static uint8_t msgCache[100];
    static int index = 0;
    static int startIndex = 0;
    static MSG_STATE state = MSG_STATE::WAITING;
    msgCache[index] = data;

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
                    messagePacket outMsg;
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

//
// radioBuffer& SerialRadio::sendHeartbeat(uint8_t mode, uint8_t status) {
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

//
// void SerialRadio::mavlinkReceivePacket(uint8_t *packet) {
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

//
// void SerialRadio::mavlinkReceiveByte(uint8_t data) {
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