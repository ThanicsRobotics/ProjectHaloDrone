#include <pigpio.h>
#include <gps.h>
//#include <altitude.h>
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
    // i2cClose(baroI2cFd);
    // bbI2COpen(2, 3, 400000);    //SDA,SCL,BAUD
    // char i2cCommands[20] = {4,GPS_ADDR,2,7,1,0xFF,6,32,3,0};
    // char buffer[256];
    // bbI2CZip(2, i2cCommands, sizeof(i2cCommands), buffer, sizeof(buffer));
    
    char buffer[1000];
    i2cReadI2CBlockData(gpsFd, 0xFF, buffer, 32);
    cout << buffer << endl;
}