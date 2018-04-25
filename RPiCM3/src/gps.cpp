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

    if ((gpsFd = i2cOpen(1, GPS_ADDR, 0)) < 0) {
        cout << "GPS Init Failed\n";
    }
}

void readGPSSentence() {
    char nmeaMessage[6000];
    int messageSize = i2cReadByteData(gpsFd, 0xFD) << 8 | i2cReadByteData(gpsFd, 0xFE);
    printf("size: %d\n", messageSize);
    if (messageSize > 0) {
        i2cReadI2CBlockData(gpsFd, 0xFF, nmeaMessage, messageSize);
        printf("GPS:\n%s\n", nmeaMessage);
    }
    // char gpsSentence[100];
    // i2cWriteByte(gpsFd, 0xFF);
    // bool gpsSentenceComplete = false;
    // int index = 0;
    // while (!gpsSentenceComplete) {
    //     char byte = i2cReadByte(gpsFd);
    //     if (byte == '$') {
    //         index = 0;
    //         gpsSentence[index] = byte;
    //         index += 1;
    //         while (byte != '\n' ) {
    //             byte = i2cReadByte(gpsFd);
    //             // if (byte < 128 && byte > 0) {
    //                 gpsSentence[index] = byte;
    //                 index += 1;
    //             // }
    //         }
    //         for (int i = index; i < 100; i++) {
    //             gpsSentence[index] = '\0';
    //         }
    //         gpsSentenceComplete = true;
    //     }
    // }
    // cout << "GPS: " << gpsSentence << endl;
    // try {
	// 		parser.readLine(gpsSentence);
    // }
    // catch (NMEAParseError& e) {
    //     cout << e.message << endl << endl;
    //     // You can keep feeding data to the gps service...
    //     // The previous data is ignored and the parser is reset.
    // }
    // cout << gps.fix.toString() << endl;
}