#include <pigpio.h>
#include <wiringPi.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int serialFd;
int i2cFd;
int charCount = 0;
char serialBuffer[10];
bool wordEnd = false;

using namespace std;

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
    else if ((int)thisChar >= 48 && (int)thisChar <= 57){
        serialBuffer[charCount] = thisChar;
        charCount += 1;
        return;
    }
}

void shutdown() {
    cout << "Closing Serial: " << serialFd << endl;

    serClose(serialFd);
    i2cClose(i2cFd);
    gpioTerminate();
}

void signal_callback_handler(int signum) {
	cout << endl << "Caught signal: " << signum << endl;
    shutdown();

    cout << endl << "Program End" << endl;
    delay(1000);
	exit(signum);
}

int main() {
    wiringPiSetup();
    if (gpioInitialise() < 0) {
        cout << "pigpio Library failed: " << strerror(errno) << endl;
        exit(1);
    }
    signal(SIGINT, signal_callback_handler);

    if ((i2cFd = i2cOpen(1, 0x22, 0)) < 0) {
        cout << "I2C Failed: " << strerror(errno) << endl;
    }
    else cout << "Opening I2C: " << i2cFd << endl;

    if ((serialFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
        cout << "Unable to open serial interface: " << strerror(errno) << endl;
    }
    else cout << "Opening Serial: " << serialFd << endl;

    delay(1000);

    int byte;
    for(int i = 0; i < 100000000; i++) {
        // if ((byte = serReadByte(serialFd)) < 0) {
        //     cout << "read byte failed: " << strerror(errno) << endl;
        //     fflush(stdout);
        // }
        // byte = serReadByte(serialFd);
        // cout << (char)byte << endl;
        readChar();
        if (wordEnd) {
            cout << (int)strtol(serialBuffer, NULL, 10) << endl;
            memset(serialBuffer,0,sizeof(serialBuffer));
        }

        if ((i2cWriteByteData(i2cFd, 0x0C, 0x55)) < 0) {
            cout << "I2C write failed: " << strerror(errno) << endl;
        }
    }
}


