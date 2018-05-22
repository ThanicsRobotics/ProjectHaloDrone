/// @file stream.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Stream class.

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

/// @brief Controls a Wireless LAN (WLAN) radio, like the pDDL2450.
class Stream
{
public:
    /// @brief Initializes private variables.
    Stream();

    /// @brief Opens a telemetry stream.
    /// @param ip_address IP Address of receiver, i.e. "192.168.168.34"
    /// @param port Port to IP Address, i.e. "6847"
    void startStream(const char *ip_address, const char *port);

    /// @brief Closes stream.
    void closeStream();

    /// @brief Sends bytes through the socket.
    /// @param data Pointer to start of data bytes.
    /// @param len Number of data bytes to send.
    int sendData(uint8_t *data, uint16_t len);

    /// @brief Gives access to received data from the socket.
    /// @return Pointer to start of received data.
    uint8_t *receiveDataPacket();

    /// @brief Gives access to whether the socket is active.
    /// @return True if socket is active, false if not.
    bool isActive() { return active; }
private:
    bool active;
    int sockfd;
    char serverIP[INET6_ADDRSTRLEN];
    void *get_in_addr(struct sockaddr *sa);
};

#endif