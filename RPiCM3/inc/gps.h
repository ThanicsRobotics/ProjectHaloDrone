#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <nmeaparse/nmea.h>

extern int gpsFd;

// struct gpsData {
//     double altitude;
//     double latitude;
//     double longitude;
//     double speed;
//     double travelAngle;
//     char status;
//     uint8_t type;
//     bool locked;
// };

void startGPS();
void readGPSSentence();
GPSService readGPS();

#endif