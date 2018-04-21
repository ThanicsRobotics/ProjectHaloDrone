#include <stream.h>

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

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void startVideoStream(char *ip_address, char *camera_address) {
    system(("gst-launch-1.0 -v v4l2src device=" + camera_address + " ! jpegenc ! rtpjpegpay ! udpsink host=" + ip_address + " port=5000").c_str());
}

// void startTelemetryStream() {
//     int network_socket = socket(AF_INET, SOCK_STREAM, 0);
//     struct sockaddr_in server_address;
//     server_address.sin_family = AF_INET;
//     server_address.sin_port = htons(9002);
//     //server_address.sin_addr.s_addr = INADDR_ANY;
//     inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr));
//     int connection = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
//     if(connection < 0) {
//         std::cout << "Error\n";
//     }
//     char server_response[256];
//     recv(network_socket, &server_response, sizeof(server_response), 0);
//     std::cout << server_response << "\n";
//     close(network_socket);
// }

void startTelemetryStream(char *ip_address, char *port) {
    printf("Starting telemetry stream...\n");
    
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(ip_address, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
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
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);
}

Stream::Stream(int streamType, char *ip_address, char *port, char *camera_address) {
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

