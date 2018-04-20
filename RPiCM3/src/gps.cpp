#include <pigpio.h>
#include <gps.h>
//#include <altitude.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

#define GPS_ADDR 0x42

int gpsFd;

using namespace std;

void startGPS() {
    if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
        cout << "GPS Init Failed\n";
    }
}

void readGPS() {
    // i2cClose(baroI2cFd);
    // bbI2COpen(2, 3, 400000);    //SDA,SCL,BAUD
    // char i2cCommands[20] = {4,GPS_ADDR,2,7,1,0xFF,6,32,3,0};
    // char buffer[256];
    // bbI2CZip(2, i2cCommands, sizeof(i2cCommands), buffer, sizeof(buffer));
    char nmeaMessage[82];
    for (int i = 0; i < 82; i += 32) {
        char buffer[32];
        i2cReadI2CBlockData(gpsFd, 0xFF, buffer, 32);
        for (int j = 0; j < 32; j++) {
            nmeaMessage[j+i] = buffer[j];
            if((i + j) > 80) break;
        }
    }
    cout << nmeaMessage << endl;
    memset(nmeaMessage, 0, sizeof(nmeaMessage));
}