#include "serial.h"
#include <pigpio.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int serialFd;

int charCount = 0;
char serialBuffer[100];
bool wordEnd = false;
bool coFlag = false;
bool serialConfigured = false;

int throttleInput = 0;

void setupSerial() {
    // if ((serialFd = serialOpen("/dev/serial0", 9600)) < 0) {
    //     cout << "Unable to open serial interface: " << strerror(errno) << endl;
    //     fflush(stdout);
    // }

    if ((serialFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
        cout << "Unable to open serial interface: " << strerror(errno) << endl;
        fflush(stdout);
    }
    else {
        cout << "Opening Serial: " << serialFd << endl;
        serialConfigured = true;
    }
}

void readChar() {
    // char thisChar = serialGetchar(serialFd);
    char thisChar = (char)serReadByte(serialFd);
    
    //Check if this character is the end of message
    if (thisChar == '\n') {
        wordEnd = true;
        serialBuffer[charCount] = '\0';
        charCount = 0;
        return;
    }
    
    //If we just finished a message, start a new one in the buffer
    else if (wordEnd == true && (int)thisChar >= 48 && (int)thisChar <= 57) {
        serialBuffer[charCount] = thisChar;
        charCount += 1;
        wordEnd = false;
        return;
    }

    //Assign the next character to the current buffer
    else if ((int)thisChar >= 48 && (int)thisChar <= 57) {
        serialBuffer[charCount] = thisChar;
        charCount += 1;
        return;
    }
}

void readLine() {
    readChar();
    if (wordEnd) {                                                  //If we have finished a message
        int data = (int)strtol(serialBuffer, NULL, 10);                     //Convert hex data to decimal
        if (coFlag && data > 999 && data <= 2000) {                                 //If we have a coefficient and data for PWM is valid
            //pthread_mutex_lock(&serial_mutex);
            throttleInput = data;                                            //Set throttle input
            //pthread_mutex_unlock(&serial_mutex);
            coFlag = false;
            // cout << throttleInput << endl;
            // fflush(stdout);
        }
        else if (data == 3) coFlag = true;                                  //If data is 3 (throttle coefficient), flag the value
        memset(serialBuffer,0,sizeof(serialBuffer));
    }
    else return;
}