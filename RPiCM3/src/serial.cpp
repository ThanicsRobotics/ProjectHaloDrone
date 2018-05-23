#include <serial.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

Serial::Serial() {
    serialConfigured = false;
}

/// @brief Opens serial port at specified baud rate.
/// @param port serial port address, i.e. "/dev/serial0".
/// @param baud baudrate, i.e. 115200 for Xbee S3B radio.
void Serial::setupSerial(const char* port, int baud) {
    if ((serialFd = serOpen((char*)port, baud, 0)) < 0) {
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
    if (serialConfigured) serClose(serialFd);
}

// void Serial::serialLoop() {
//     int heartbeatTimer = millis();
//     while (serialThreadActive) {
//         mavlinkReceiveByte(readChar());
//         //Every second, send heartbeat to controller
//         if (millis() - heartbeatTimer > 1000) {
//             radioBuffer msg = radio.sendHeartbeat(0,3); //Heartbeat in PREFLIGHT mode and STANDBY state
//             radio.write(msg.buf, msg.len);
//             heartbeatTimer = millis();
//         }
//     }
// }

// /// @brief start serial thread loop.
// void Serial::startSerialLoop() {
//     serialThreadActive = true;
//     serialThread = std::thread([this]{ serialLoop(); })
//     //pthread_t serialThread;
//     //pthread_create(&serialThread, NULL, serialLoop, NULL);
// }

// void Serial::stopSerialLoop() {
//     if (serialThreadActive) {
//         serialThreadActive = false;
//         serialThread.join();
//     }
// }

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
    if(serDataAvailable(serialFd) > 0) {
        return (uint8_t)serReadByte(serialFd);
    }
    else return NULL;
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

Serial::~Serial() {
    //pthread_join(serialThread, NULL);
}