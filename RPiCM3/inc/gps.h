/// @file gps.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of GPS class.

#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <nmeaparse/nmea.h>

/// @brief Contains all functions related to operating the
/// uBlox NEO M8 GPS.
class GPS {
public:
    /// @brief Contains data from GPS Fix.
    struct gpsData {
        double altitude;    ///< GPS Altitude.
        double latitude;    ///< Latitude in degrees.
        double longitude;   ///< Longitude in degrees.
        double speed;       ///< Speed in m/s.
        double travelAngle; ///< Compass angle in degrees.
        char status;        ///< Status of GPS Fix, 'A' = Active, 'V' = No Fix
        uint8_t type;       ///< Type of fix.
    };

    /// @brief Opens the bit-banged I2C port for the uBlox's
    /// DDC protocol. Hardware I2C did not work.
    void startGPS();

    /// @brief Reads 5 GPS Sentences.
    void readGPS() const;
private:
    nmea::GPSService gps;
    nmea::GPSService gps(parser);
    int gpsFd;

    void readGPSSentence();
};

#endif