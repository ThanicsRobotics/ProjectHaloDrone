#include <pigpio.h>
#include <gps.h>
#include <iostream>

#define GPS_ADDR 0x42

int gpsFd;

using namespace std;

void startGPS() {
    if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
        cout << "GPS Init Failed\n";
    }

    
}

void readGPS() {
    char buffer[256];
    cout << i2cReadBlockData(gpsFd, 0xFF, buffer);
}