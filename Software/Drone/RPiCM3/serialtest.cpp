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
    cout << "Closing Serial: " << serialFd << endl;

    serClose(serialFd);
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

    if ((serialFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
        cout << "Unable to open serial interface: " << strerror(errno) << endl;
    }
    else cout << "Opening Serial: " << serialFd << endl;

    delay(1000);
    for(int i = 0; i < 100000000; i++) {
        readChar();
        if (wordEnd) {
            cout << serialBuffer << endl;
            memset(serialBuffer,0,sizeof(serialBuffer));
        }
    }
}


