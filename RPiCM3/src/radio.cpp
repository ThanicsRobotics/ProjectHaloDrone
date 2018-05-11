#include <radio.h>
#include <fcinterface.h>
#include <mavlink/common/mavlink.h>
#include <pigpio.h>
#include <iostream>
#include <wiringPi.h>

#define SYSID 1
#define COMPID 1

int radioFd;
int rollPWM;
int pitchPWM;
int yawPWM;
int altitudePWM;
int controllerStatus;

buffer sendHeartbeat(uint8_t mode, uint8_t status) {
    mavlink_message_t msg;
    uint16_t len;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_heartbeat_pack(SYSID, COMPID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, mode, 0, status);
    len = mavlink_msg_to_send_buffer(buf, &msg);

    std::unique_ptr<uint8_t[]> buffer_ptr = std::make_unique<uint8_t[]>(len);
    buffer_ptr = buf;
    buffer sendBuffer;
    sendBuffer.buf = buffer_ptr;
    sendBuffer.len = len;
    return sendBuffer;
}

void mavlinkReceivePacket(uint8_t *packet) {
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

void mavlinkReceiveByte(uint8_t data) {
    mavlink_message_t msg;
    mavlink_status_t status;
    if(mavlink_parse_char(MAVLINK_COMM_0, data, &msg, &status)) {
        printf("Received message with ID %d, sequence: %d from component %d of system %d\n", 
        msg.msgid, msg.seq, msg.compid, msg.sysid);
        // switch(msg.msgid) {
        //     case MAVLINK_MSG_ID_HEARTBEAT:
        //         mavlink_heartbeat_t hb;
        //         mavlink_msg_heartbeat_decode(&msg, &hb);
        //         controllerStatus = hb.system_status;
        //         printf("Heartbeat from: %d, mode: %d, status: %d\n", 
        //         hb.type, hb.base_mode, controllerStatus);
        //         break;
        //     case MAVLINK_MSG_ID_RC_CHANNELS:
        //         mavlink_rc_channels_t channels;
        //         mavlink_msg_rc_channels_decode(&msg, &channels);
        //         altitudePWM = channels.chan1_raw;
        //         rollPWM = channels.chan2_raw;
        //         pitchPWM = channels.chan3_raw;
        //         yawPWM = channels.chan4_raw;
        //         break;

        //     case MAVLINK_MSG_ID_COMMAND_LONG:
        //         mavlink_command_long_t command;
        //         switch (command.command) {
        //             case MAV_CMD_NAV_LAND:
        //                 break;
        //             case MAV_CMD_NAV_TAKEOFF:
        //                 break;
        //         }
        //         break;
        //     default:
        //         break;
        // }
    }
    //else printf("fail\n");
}

// receivedMessage getControllerData(Stream stream) {
//     receivedMessage msg;
//     mavlinkReceivePacket(stream.receiveDataPacket());
// }

