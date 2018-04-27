#include <pigpio.h>
#include <gps.h>
//#include <altitude.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <nmeaparse/nmea.h>

#define GPS_ADDR 0x42

using namespace std;
using namespace nmea;

int gpsFd;
NMEAParser parser;
GPSService gps(parser);

void startGPS() {
    gps.onUpdate += [&gps](){
		cout << (gps.fix.locked() ? "[*] " : "[ ] ") << setw(2) << setfill(' ') << gps.fix.trackingSatellites 
        << "/" << setw(2) << setfill(' ') << gps.fix.visibleSatellites << " ";
		cout << fixed << setprecision(2) << setw(5) << setfill(' ') << gps.fix.almanac.averageSNR() << " dB   ";
		cout << fixed << setprecision(2) << setw(6) << setfill(' ') << gps.fix.speed << " km/h [" 
        << GPSFix::travelAngleToCompassDirection(gps.fix.travelAngle, true) << "]  ";
		cout << fixed << setprecision(6) << gps.fix.latitude << "\xF8 " "N, " << gps.fix.longitude << "\xF8 " "E" << "  ";
		cout << "+/- " << setprecision(1) << gps.fix.horizontalAccuracy() << "m  ";
		cout << endl;
	};
    parser.log = false;

    // if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
    //     cout << "GPS Init Failed\n";
    // }
    bbI2COpen(2,3,400000);
}

void bbWriteByte(uint8_t byte) {
    char inBuf[8] = {4,0x42,2,7,1,byte,3,0};
    uint8_t outBuf[1];
    bbI2CZip(2, inBuf, sizeof(inBuf), (char*)outBuf, sizeof(outBuf));
    //return outBuf[0];
}

uint8_t *bbReadGPSBlock(uint16_t size) {
    char inBuf[8] = {4,0x42,2,6,size,3,0};
    uint8_t outBuf[6000];
    bbI2CZip(2, inBuf, sizeof(inBuf), (char*)outBuf, sizeof(outBuf));
    return outBuf;
}

uint8_t bbReadByteData(uint8_t reg) {
    uint8_t outBuf[1];
    char inBuf[20] = {4,0x42,2,7,1,reg,3,0};
    bbI2CZip(2, inBuf, sizeof(inBuf), (char*)outBuf, sizeof(outBuf));
    char inBuf2[20] = {4,0x42,2,6,1,3,0};
    bbI2CZip(2, inBuf2, sizeof(inBuf2), (char*)outBuf, sizeof(outBuf));
    return outBuf[0];
}

uint8_t bbReadByte() {
    uint8_t outBuf[1];
    char inBuf[20] = {4,0x42,2,6,1,3,0};
    bbI2CZip(2, inBuf, sizeof(inBuf), (char*)outBuf, sizeof(outBuf));
    return outBuf[0];
}

void readGPSSentence() {
    char gpsSentence[100];
    bbWriteByte(0xFF);
    bool gpsSentenceComplete = false;
    int index = 0;
    while (!gpsSentenceComplete) {
        char byte = (char)bbReadByte();
        if (byte == '$') {
            index = 0;
            gpsSentence[index] = byte;
            index += 1;
            while (byte != '\n' ) {
                byte = (char)bbReadByte();
                // if (byte < 128 && byte > 0) {
                    gpsSentence[index] = byte;
                    index += 1;
                // }
            }
            for (int i = index; i < 100; i++) {
                gpsSentence[index] = '\0';
            }
            gpsSentenceComplete = true;
        }
    }
    cout << "GPS: " << gpsSentence << endl;
    try {
			parser.readLine(gpsSentence);
    }
    catch (NMEAParseError& e) {
        cout << e.message << endl << endl;
        // You can keep feeding data to the gps service...
        // The previous data is ignored and the parser is reset.
    }
    //cout << gps.fix.toString() << endl;
}

GPSService readGPS() {
    for(int i = 0; i < 5; i++) {
        readGPSSentence();
    }
    return gps;
}