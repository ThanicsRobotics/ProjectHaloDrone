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

using namespace std;

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

    if ((serialFd = serOpen('/dev/serial0', 9600, 0)) < 0) {
        cout << "Unable to open serial interface: " << strerror(errno) << endl;
    }
    else cout << "Opening Serial: " << serialFd << endl;

    delay(1000);

    int byte;
    for(int i = 0; i < 100000000; i++) {
        if ((byte = serReadByte(serialFd)) < 0) {
            cout << "read byte failed: " << strerror(errno) << endl;
            fflush(stdout);
        }
        else cout << (char)byte << endl;
    }
}


