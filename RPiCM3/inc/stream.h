#ifndef STREAM_H
#define STREAM_H

#define VIDEO 0
#define TELE 1

#include <string.h>
#include <iostream>

class Stream {
    public:
        Stream(int streamType, char *ip_address, char *port, char *camera_address);
        void closeStream();
        int sendData(uint8_t *data, uint16_t len);
        uint8_t *receiveDataPacket();
};

#endif