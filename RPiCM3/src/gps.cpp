#include <pigpio.h>
#include <gps.h>
//#include <altitude.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <nmea.h>

#define GPS_ADDR 0x42

int gpsFd;


using namespace std;

void startGPS() {
    if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
        cout << "GPS Init Failed\n";
    }
}

void readGPS() {
    char nmeaMessage[82];
    i2cWriteByte(gpsFd, 0xFF);
    i2cReadDevice(gpsFd, nmeaMessage, 82);
    cout << "GPS:\n" << nmeaMessage << endl;
    // for (int i = 0; i < 82; i += 32) {
    //     char buffer[32];
    //     i2cReadI2CBlockData(gpsFd, 0xFF, buffer, 32);
    //     for (int j = 0; j < 32; j++) {
    //         nmeaMessage[j+i] = buffer[j];
    //         if((i + j) > 80) break;
    //     }
    // }
}