/// @file stream.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Stream class.

#ifndef STREAM_H
#define STREAM_H

#include <iostream>
#include <boost/asio.hpp>

/// @brief Controls a Wireless LAN (WLAN) radio, like the pDDL2450.
class Stream
{
public:
    /// @brief Initializes private variables.
    Stream();

    void startClient();
    void startHost();

    /// @brief Gives access to whether the socket is active.
    /// @return True if socket is active, false if not.
    bool isActive() const { return active; }
private:
    bool active;
    boost::asio::io_context io;

};

#endif