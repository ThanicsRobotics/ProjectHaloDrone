#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <memory>
#include <serial.h>
#include <stream.h>
#include <flightcontroller.h>
#include <mavlink/common/mavlink.h>
#include <pigpio.h>
#include <iostream>
#include <wiringPi.h>

#define SYSID 1
#define COMPID 1

struct radioBuffer
{
    uint16_t len;
    //std::shared_ptr<uint8_t[]> buf;
    uint8_t* buf;
};
struct channels
{
    uint16_t rollPWM, pitchPWM, yawPWM, throttlePWM;
};

//Specify "Serial" or "Stream" based Radio in template argument
//for example: Radio<Serial> will generate a Serial-based radio object
template<typename InterfaceType>
class Radio : public InterfaceType
{
public:
    radioBuffer& sendHeartbeat(uint8_t mode, uint8_t status);
    void mavlinkReceiveByte(uint8_t data);
    void mavlinkReceivePacket(uint8_t *packet);
    channels& getRCChannels();

private:
    channels pwmInputs;
    uint8_t controllerStatus;
};

template<typename InterfaceType>
radioBuffer& Radio<InterfaceType>::sendHeartbeat(uint8_t mode, uint8_t status) {
    mavlink_message_t msg;
    uint16_t len;
    static uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_heartbeat_pack(SYSID, COMPID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, mode, 0, status);
    len = mavlink_msg_to_send_buffer(buf, &msg);

    static radioBuffer sendBuffer;
    //sendBuffer.buf = (std::shared_ptr<uint8_t[]>)buf;
    sendBuffer.buf = (uint8_t*)buf;
    sendBuffer.len = len;
    return sendBuffer;
}

template<typename InterfaceType>
channels& Radio<InterfaceType>::getRCChannels() {
    static channels modInputs;
    modInputs.pitchPWM = pwmInputs.pitchPWM < 1000 ? 1500 : pwmInputs.pitchPWM;
    modInputs.rollPWM = pwmInputs.rollPWM < 1000 ? 1500 : pwmInputs.rollPWM;
    modInputs.yawPWM = pwmInputs.yawPWM < 1000 ? 1500 : pwmInputs.yawPWM;
    modInputs.throttlePWM = pwmInputs.throttlePWM < 1000 ? 1500 : pwmInputs.throttlePWM;
    return modInputs;
}

template<typename InterfaceType>
void Radio<InterfaceType>::mavlinkReceivePacket(uint8_t *packet) {
    uint8_t byte = 1;
    int i = 0;
    while (byte != '\0') {
        printf("byte %d: %d\n", i, byte);
        fflush(stdout);
        byte = packet[i];
        mavlinkReceiveByte(byte);
        i += 1;
    }
}

template<typename InterfaceType>
void Radio<InterfaceType>::mavlinkReceiveByte(uint8_t data) {
    mavlink_message_t msg;
    mavlink_status_t status;
    if(mavlink_parse_char(MAVLINK_COMM_0, data, &msg, &status)) {
        printf("Received message with ID %d, sequence: %d from component %d of system %d\n", 
        msg.msgid, msg.seq, msg.compid, msg.sysid);
        switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t hb;
                mavlink_msg_heartbeat_decode(&msg, &hb);
                controllerStatus = hb.system_status;
                printf("Heartbeat from: %d, mode: %d, status: %d\n", 
                    hb.type, hb.base_mode, controllerStatus);
                break;
            case MAVLINK_MSG_ID_RC_CHANNELS:
                mavlink_rc_channels_t channels;
                mavlink_msg_rc_channels_decode(&msg, &channels);
                pwmInputs.pitchPWM = channels.chan1_raw;
                pwmInputs.rollPWM = channels.chan2_raw;
                pwmInputs.yawPWM = channels.chan3_raw;
                pwmInputs.throttlePWM = channels.chan4_raw;
                break;
            case MAVLINK_MSG_ID_COMMAND_LONG:
                mavlink_command_long_t command;
                switch (command.command) {
                    case MAV_CMD_NAV_LAND:
                        break;
                    case MAV_CMD_NAV_TAKEOFF:
                        break;
                }
                break;
            default:
                break;
        }
    }
    //else printf("fail\n");
}

// receivedMessage getControllerData(Stream stream) {
//     receivedMessage msg;
//     mavlinkReceivePacket(stream.receiveDataPacket());
// }

#endif