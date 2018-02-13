#include <pigpio.h>

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
char serialBuffer[100];
bool wordEnd = false;
int charCount = 0;

using namespace std;

void readChar() {
    // char thisChar = serialGetchar(serialFd);
    int thisChar;
    if ((thisChar = serReadByte(serialFd)) < 0 ) {
        cout << "read byte failed: " << strerror(errno) << endl;
    }
    
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

// void readLine() {
//     readChar();
//     if (wordEnd == true) {                                                  //If we have finished a message
//         int data = (int)strtol(serialBuffer, NULL, 10);                     //Convert hex data to decimal
//         if (coFlag == true && data > 999 && data <= 2000) {                                 //If we have a coefficient and data for PWM is valid
//             //pthread_mutex_lock(&serial_mutex);
//             throttleInput = data;                                            //Set throttle input
//             //pthread_mutex_unlock(&serial_mutex);
//             coFlag = false;
//             //cout << throttleInput << endl;
//             //fflush(stdout);
//         }
//         else if (data == 3) coFlag = true;                                  //If data is 3 (throttle coefficient), flag the value
//         memset(serialBuffer,0,sizeof(serialBuffer));
//     }
//     else return;
// }

int main() {
    if (gpioInitialise() < 0) {
        cout << "pigpio Library failed: " << strerror(errno) << endl;
        exit(1);
    }
    if ((serialFd = serOpen("/dev/serial0", 9600, 0)) < 0) {
            cout << "Unable to open serial interface: " << strerror(errno) << endl;
            fflush(stdout);
    }
    else {
        cout << serialFd << endl;
        for(int i = 0; i < 10000; i++) {
            readChar();
        }
    }
    serClose(serialFd);
    gpioTerminate();
}