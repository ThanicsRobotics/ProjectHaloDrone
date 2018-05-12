#ifndef STREAM_H
#define STREAM_H

#define VIDEO 0
#define TELE 1

#include <string.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

class Stream {
    public:
        Stream();
        void startStream(int streamType, char *ip_address, char *port, char *camera_address);
        void closeStream();
        int sendData(uint8_t *data, uint16_t len);
        uint8_t *receiveDataPacket();
        void startTelemetryStream(char *ip_address, char *port);
        bool isActive() { return active; }
    private:
        bool active;
        int sockfd;
        char serverIP[INET6_ADDRSTRLEN];
        void *get_in_addr(struct sockaddr *sa);
};

#endif