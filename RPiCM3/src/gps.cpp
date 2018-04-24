#include <pigpio.h>
#include <gps.h>
//#include <altitude.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <nmeaparse/nmea.h>

#define GPS_ADDR 0x42

int gpsFd;


using namespace std;

void startGPS() {
    if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
        cout << "GPS Init Failed\n";
    }
}

void readGPSSentence() {
    //char nmeaMessage[82];
    char gpsSentence[50];
    i2cWriteByte(gpsFd, 0xFF);
    bool gpsSentenceComplete = false;
    int index = 0;
    while (!gpsSentenceComplete) {
        char byte = i2cReadByte(gpsFd);
        if (byte == '$') {
            index = 0;
            gpsSentence[index] = byte;
            index += 1;
            while (byte != '\n') {
                byte = i2cReadByte(gpsFd);
                gpsSentence[index] = byte;
                index += 1;
            }
            gpsSentenceComplete = true;
        }
    }
    cout << "GPS: " << gpsSentence << endl;
    //memset(gpsSentence, 0, sizeof(gpsSentence));
    //i2cReadDevice(gpsFd, nmeaMessage, 82);
    //cout << "GPS:\n" << nmeaMessage << endl;
    // for (int i = 0; i < 82; i += 32) {
    //     char buffer[32];
    //     i2cReadI2CBlockData(gpsFd, 0xFF, buffer, 32);
    //     for (int j = 0; j < 32; j++) {
    //         nmeaMessage[j+i] = buffer[j];
    //         if((i + j) > 80) break;
    //     }
    // }
}