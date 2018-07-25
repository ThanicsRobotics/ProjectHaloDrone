/// @file stream.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of WLAN class.

#ifndef WLAN_H
#define WLAN_H

#define PACKET_SIZE 26

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <functional>
#include <array>

/// @brief Controls a Wireless LAN (WLAN) radio, like the pDDL2450.
class WLAN
{
public:
    enum DeviceType {
        HOST = 0,
        CLIENT
    };

    /// @brief Initializes private variables.
    WLAN();
    WLAN(DeviceType type, std::string ipAddress, int port);

    void start(DeviceType type, std::string ipAddress, int port);
    void write(std::array<uint8_t, PACKET_SIZE>& msg);
    void read();
    void setCallback(std::function<void(std::size_t)> callback);
    void checkBuffer();

    /// @brief Gives access to whether the socket is active.
    /// @return True if socket is active, false if not.
    bool isActive() const { return active; }

    void getCachedMessage(std::array<uint8_t, PACKET_SIZE>& msg) const { msg = cachedMessage; }

private:
    void startHost();
    void startClient(std::string ipAddress, int port);

    bool active = false;
    bool connected = false;

    // Cached for repeated attempts
    DeviceType deviceType;
    std::string hostname;
    int port;

    boost::asio::io_context io;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::resolver resolver;

    std::function<void(std::size_t size)> receiveCallback;
    bool callbackSet = false;

    std::array<uint8_t, PACKET_SIZE> cachedMessage;
};

#endif