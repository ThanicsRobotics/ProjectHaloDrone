#include <serial.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

Serial::Serial(std::string port, int baud)
{
    setupSerial(port, baud);
}

Serial::Serial() {
}

Serial::~Serial() {
    printf("SERIAL: Closing\n");
    closeSerial();
}

/// @brief Opens serial port at specified baud rate.
/// @param port serial port address, i.e. "/dev/serial0".
/// @param baud baudrate, i.e. 115200 for Xbee S3B radio.
void Serial::setupSerial(std::string port, int baud) {
    if ((serialFd = serOpen((char*)port.c_str(), baud, 0)) < 0) {
        std::cout << "Unable to open serial interface: " << strerror(errno) << '\n';
        fflush(stdout);
    }
    else {
        std::cout << "Opening Serial. FD: " << serialFd << " ID: " << pthread_self() << '\n';
        serialConfigured = true;
    }
}

/// @brief Closes serial port.
void Serial::closeSerial() {
    if (serialConfigured) {
        serClose(serialFd);
        serialConfigured = false;
    }
}

char *Serial::readLine() {
    if(!serialConfigured) return "\0";
    static char buffer[300];
    memset(buffer, 0, sizeof(buffer));
    int i = 0;
    for (char thisChar = (char)serReadByte(serialFd); thisChar != '\n'; 
    thisChar = (char)serReadByte(serialFd)) {
        buffer[i] = thisChar;
        i += 1;
    }
    buffer[i] = '\n';
    buffer[i+1] = '\0';
    return buffer;
}

uint8_t Serial::readChar() {
    if(!serialConfigured) return NULL;
    int timer = micros();
    while((serDataAvailable(serialFd) < 1) && (micros() - timer < 1000));
    // if(serDataAvailable(serialFd) > 0) {
        return (uint8_t)serReadByte(serialFd);
    // }
    // else return NULL;
}

int Serial::write(uint8_t* bytes, uint16_t len) {
    int status = 0;
    // for (int i = 0; i < len; i++) {
    //     // if((status = serWriteByte(serialFd, bytes.get()[i])) < 0) {
    //     //     printf("%d\n", status);
    //     //     return status;
    //     // }
    //     printf("byte %d/%d: %d\n", i+1, len, bytes.get()[i]);
    // }
    if((status = serWrite(serialFd, (char*)bytes, len)) < 0) {
        printf("%s\n", strerror(status));
    }
    return status;
}

