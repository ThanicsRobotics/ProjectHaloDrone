#include <pigpio.h>
#include <gps.h>
//#include <barometer.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iomanip>

#define GPS_ADDR 0x42

/// @brief Opens the bit-banged I2C port for the uBlox's
/// DDC protocol. Hardware I2C did not work.
void GPS::startGPS() {
    gps = nmea::GPSService(parser);
    parser.log = false;

    // Opened bit banged I2C port.
    bbI2COpen(2,3,400000);
}

void bbWriteByte(uint8_t byte) {
    char inBuf[8] = {4,0x42,2,7,1,byte,3,0};
    uint8_t outBuf[1];
    bbI2CZip(2, inBuf, sizeof(inBuf), (char*)outBuf, sizeof(outBuf));
}

uint8_t *bbReadGPSBlock(uint16_t size) {
    char inBuf[8] = {4,0x42,2,6,size,3,0};
    static uint8_t outBuf[6000];
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

void GPS::readGPSSentence() {
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
                gpsSentence[index] = byte;
                index += 1;
            }
            for (int i = index; i < 100; i++) {
                gpsSentence[index] = '\0';
            }
            gpsSentenceComplete = true;
        }
    }
    try {
			parser.readLine(gpsSentence);
    }
    catch (nmea::NMEAParseError& e) {
        std::cout << "GPS Error: " << e.message << "\n" << std::endl;
    }
}

/// @brief Reads 5 GPS Sentences.
void GPS::readGPS() {
    for(int i = 0; i < 5; i++) {
        readGPSSentence();
    }
}