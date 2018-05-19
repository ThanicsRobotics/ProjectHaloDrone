/// @file serial.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Serial class.

#ifndef SERIAL_H
#define SERIAL_H

#include <pthread.h>
#include <inttypes.h>
#include <memory>
#include <thread>

/// @brief Contains functions for opening, writing, 
/// and reading serial port
class Serial
{
public:
    Serial();
    ~Serial();

    /// @brief Opens serial port at specified baud rate.
    /// @param port serial port address, i.e. "/dev/serial0".
    /// @param baud baudrate, i.e. 9600.
    void setupSerial(char* port, int baud);

    /// @brief Closes serial port.
    void closeSerial();

    /// @brief start serial thread loop. (currently not used).
    // void startSerialLoop();

    // void stopSerialLoop();

    /// 
    char *readLine();
    char readChar();
    //int write(std::shared_ptr<uint8_t[]> bytes, uint16_t len);
    int write(uint8_t* bytes, uint16_t len);
    bool isSerialConfigured() const { return serialConfigured; }

private:
    //pthread_t serialThread;
    std::thread serialThread;
    bool serialConfigured;
    int throttleInput;
    int serialFd;

    // void serialLoop();
};

#endif