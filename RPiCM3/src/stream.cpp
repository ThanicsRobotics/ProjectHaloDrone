#include <stream.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

void startVideoStream(std::string camera, std::string address) {
    system(("gst-launch-1.0 -v v4l2src device=" + camera + " ! jpegenc ! rtpjpegpay ! udpsink host=" + address + " port=5000").c_str());
}

// void startTelemetryStream() {
//     int network_socket = socket(AF_INET, SOCK_STREAM, 0);
//     struct sockaddr_in server_address;
//     server_address.sin_family = AF_INET;
//     server_address.sin_port = htons(9002);
//     server_address.sin_addr.s_addr = INADDR_ANY;
//     int connection = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
//     if(connection < 0) {
//         std::cout << "Error\n";
//     }
//     char server_response[256];
//     recv(network_socket, &server_response, sizeof(server_response), 0);
// }