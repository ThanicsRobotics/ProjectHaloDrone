#include <stream.h>
#include <fcinterface.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXDATASIZE 500 // max number of bytes we can get at once 

int sockfd;
char serverIP[INET6_ADDRSTRLEN];
bool isActive = false;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void startVideoStream(char *ip_address, char *camera_address) {
    system(("gst-launch-1.0 -v v4l2src device=" + (std::string)camera_address + " ! jpegenc ! rtpjpegpay ! udpsink host=" + (std::string)ip_address + " port=5000").c_str());
}

void Stream::startTelemetryStream(char *ip_address, char *port) {
    printf("Starting telemetry stream...\n");

    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(ip_address, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to connect\n");
        exit(1);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            serverIP, sizeof serverIP);
    printf("Connecting to %s as client\n", serverIP);

    freeaddrinfo(servinfo); // all done with this structure

    std::string message = "Hello from drone!\n";
    this->sendData((uint8_t*)message.c_str(), sizeof(message));
}

Stream::Stream() {

}

void Stream::startStream(int streamType, char *ip_address, char *port, char *camera_address) {
    switch(streamType) {
        case VIDEO:
            startVideoStream(ip_address, camera_address);
            break;
        case TELE:
            startTelemetryStream(ip_address, port);
            break;
        default:
            break;
    }
}

int Stream::sendData(uint8_t *data, uint16_t len)
{
    int numbytes;
    if ((numbytes = send(sockfd, data, len, 0)) == -1) {
        perror("send");
        exit(1);
    }
    printf("Sent %d bytes\n", numbytes);
    return numbytes;
}

// int Stream::openReceiveChannel() {
//     pthread_create(&receiveChannel, NULL, receiveLoop, NULL);
// }

uint8_t *Stream::receiveDataPacket()
{
    int numbytes;
    uint8_t buf[MAXDATASIZE];

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("Received %d bytes: '%s'\n", numbytes, buf);
    uint8_t *bufPointer = buf;
    return bufPointer;
}

void Stream::closeStream() {
    printf("Closing Stream with %s\n", serverIP);
    close(sockfd);
}
