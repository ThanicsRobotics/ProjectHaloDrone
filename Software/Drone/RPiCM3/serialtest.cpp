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
char serialBuffer[100];
bool wordEnd = false;
int charCount = 0;

using namespace std;

void readChar() {
    // char thisChar = serialGetchar(serialFd);
    char buf[2];
    if ((serRead(serialFd, buf, 1)) < 0 ) {
        cout << "read byte failed: " << strerror(errno) << endl;
        fflush(stdout);
    }
    char thisChar = buf[0];
    
    //Check if this character is the end of message
    if (thisChar == '\n') {
        wordEnd = true;
        serialBuffer[charCount] = '\0';
        charCount = 0;
        cout << serialBuffer << endl;
        return;
    }
    
    //If we just finished a message, start a new one in the buffer
    else if (wordEnd == true) {
        serialBuffer[charCount] = thisChar;
        charCount += 1;
        wordEnd = false;
        return;
    }

    //Assign the next character to the current buffer
    else {
        serialBuffer[charCount] = thisChar;
        charCount += 1;
        return;
    }
}

void shutdown() {
    cout << endl << "Closing Threads and Ports..." << endl << endl;
    delay(1000);

    cout << "Closing I2C: " << i2cFd << endl;
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
    if (gpioInitialise() < 0) {
        cout << "pigpio Library failed: " << strerror(errno) << endl;
        exit(1);
    }
    signal(SIGINT, signal_callback_handler);

    if ((serialFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
            cout << "Unable to open serial interface: " << strerror(errno) << endl;
            fflush(stdout);
    }
    else cout << "Opening Serial: " << serialFd << endl;

    if ((i2cFd = i2cOpen(1, 0x22, 0)) < 0) {
        cout << "I2C Failed: " << strerror(errno) << endl;
        exit(1);
    }
    else cout << "Opening I2C: " << i2cFd << endl;

    delay(1000);
    for(int i = 0; i < 100000000; i++) {
        readChar();
        i2cWriteByteData(i2cFd, 0x0C, 0x55);
    }
}


