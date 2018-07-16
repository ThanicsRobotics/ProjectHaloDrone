/// @file stream.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Stream class.

#ifndef STREAM_H
#define STREAM_H

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <functional>

/// @brief Controls a Wireless LAN (WLAN) radio, like the pDDL2450.
class Stream
{
public:
    /// @brief Initializes private variables.
    Stream();

    void startClient();
    void startHost();
    void write(std::string& msg);
    void read(std::function<void()> callback);

    /// @brief Gives access to whether the socket is active.
    /// @return True if socket is active, false if not.
    bool isActive() const { return active; }

    std::string getCurrentMessage() const { return currentMessage; }

private:
    bool active;
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::resolver resolver;

    std::string currentMessage;
};

#endif