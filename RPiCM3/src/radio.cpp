#if 0

#include <radio.h>
#include <mavlink/common/mavlink.h>
#include <pigpio.h>
#include <iostream>

#define SYSID 1
#define COMPID 1

int radioFd;
int rollPWM;
int pitchPWM;
int yawPWM;
int altitudePWM;

using namespace std;

void setupRadio() {
    if ((radioFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
        cout << "Unable to open serial interface: " << strerror(errno) << endl;
        fflush(stdout);
    }
    else {
        cout << "Opening Serial. FD: " << radioFd << " ID: " << pthread_self() << endl;
        serialConfigured = true;
    }
}

void sendHeartbeat(uint8_t mode, uint8_t status) {
    mavlink_message_t msg;
    uint8_t len;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_heartbeat_pack(SYSID, COMPID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT, mode, 0, status);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    serWrite(radioFd, buf, len);
}

void mavlinkReceive() {
    mavlink_message_t msg;
    mavlink_status_t status;
    while (serDataAvailable(radioFd)) {
        uint8_t data = serReadByte(radioFd);
        if(mavlink_parse_char(MAVLINK_COMM_0, data, &msg, &status)) {
            switch(msg.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT:
                    mavlink_heartbeat_t hb;
                    mavlink_msg_heartbeat_decode(&msg, &hb);
                    
                    break;
                case MAVLINK_MSG_ID_RC_CHANNELS:
                    mavlink_rc_channels_t channels;
                    mavlink_msg_rc_channels_decode(&msg, &channels);
                    altitudePWM = channels.chan1_raw;
                    rollPWM = channels.chan2_raw;
                    pitchPWM = channels.chan3_raw;
                    yawPWM = channels.chan4_raw;
                    break;

                case MAVLINK_MSG_ID_COMMAND_LONG:
                    mavlink_command_long_t command;
                    switch (command.command) {
                        case MAV_CMD_NAV_LAND:
                        case MAV_CMD_NAV_TAKEOFF:
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void *serialLoop(void *void_ptr) {
    setupRadio();
    while(run) {
        mavlinkReceive();
    }
    return NULL;
}
#endif