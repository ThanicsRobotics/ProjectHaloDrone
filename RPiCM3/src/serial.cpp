#include <serial.h>
#include <fcinterface.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// int charCount = 0;
// char serialBuffer[100];
// bool wordEnd = false;
// bool coFlag = false;

Serial::Serial() {
    serialConfigured = false;
}

void Serial::setupSerial(char* port, int baud) {
    if ((serialFd = serOpen(port, baud, 0)) < 0) {
        std::cout << "Unable to open serial interface: " << strerror(errno) << '\n';
        fflush(stdout);
    }
    else {
        std::cout << "Opening Serial. FD: " << serialFd << " ID: " << pthread_self() << '\n';
        serialConfigured = true;
    }
}

void *Serial::serialLoop(void*) {
    if(!serialConfigured) this->setupSerial("/dev/serial0", 9600);
    while(run) {
        //this->readLine();
        //delayMicroseconds(500);
    }
    return NULL;
}

void Serial::startSerialLoop() {
    pthread_t serialThread;
    //pthread_create(&serialThread, NULL, serialLoop, NULL);
}

char *Serial::readLine() {
    if(!serialConfigured) return '\0';
    char buffer[300];
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

char Serial::readChar() {
    if(!serialConfigured) return '\0';
    if(serDataAvailable(serialFd) > 0) {
        return (char)serReadByte(serialFd);
    }
    else return '\0';
}

int Serial::write(std::unique_ptr<uint8_t[]> bytes, uint16_t len) {
    int status = 0;
    for (int i = 0; i < len; i++) {
        if((status = serWriteByte(serialFd, bytes[i])) < 0) {
            printf("%d\n", status);
            return status;
        }
        printf("byte %d/%d: %d\n", i+1, len, bytes[i]);
    }
    return status;
}

// void readChar() {
//     // char thisChar = serialGetchar(serialFd);
//     char thisChar = (char)serReadByte(serialFd);
    
//     //Check if this character is the end of message
//     if (thisChar == '\n') {
//         wordEnd = true;
//         serialBuffer[charCount] = '\0';
//         charCount = 0;
//         return;
//     }
    
//     //If we just finished a message, start a new one in the buffer
//     else if (wordEnd == true && (int)thisChar >= 48 && (int)thisChar <= 57) {
//         serialBuffer[charCount] = thisChar;
//         charCount += 1;
//         wordEnd = false;
//         return;
//     }

//     //Assign the next character to the current buffer
//     else if ((int)thisChar >= 48 && (int)thisChar <= 57) {
//         serialBuffer[charCount] = thisChar;
//         charCount += 1;
//         return;
//     }
// }

// void readLine() {
//     //while(!wordEnd) readChar();
//     readChar();
//     if (wordEnd) {                                                  //If we have finished a message
//         int data = (int)strtol(serialBuffer, NULL, 10);                     //Convert hex data to decimal
//         if (coFlag && data > 999 && data <= 2000) {                                 //If we have a coefficient and data for PWM is valid
//             //pthread_mutex_lock(&serial_mutex);
//             throttleInput = data;                                            //Set throttle input
//             //pthread_mutex_unlock(&serial_mutex);
//             coFlag = false;
//             // std::cout << throttleInput << endl;
//             // fflush(stdout);
//         }
//         else if (data == 3) coFlag = true;                                  //If data is 3 (throttle coefficient), flag the value
//         memset(serialBuffer,0,sizeof(serialBuffer));
//     }
//     else return;
// }

Serial::~Serial() {
    pthread_join(serialThread, NULL);
}