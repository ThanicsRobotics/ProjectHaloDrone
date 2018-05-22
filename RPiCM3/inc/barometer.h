/// @file barometer.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Barometer class.

#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <stdint.h>
#include <thread>

#define BARO_DELAY 260

/// @brief Controls barometer and access to altitude measurements.
class Barometer
{
public:
    /// @brief Initializes private variables.
    Barometer();
    ~Barometer();

    /// @brief Opens I2C port, sets up barometer registers
    /// and starts calibration/acclimation thread.
    void setup();

    /// @brief Closes I2C port and waits for calibraton thread to join.
    void close();
    
    /// @brief Signals to barometer to initiate measurement cycle.
    void takeReading();

    /// @brief Gets the current pressure altitude from barometer.
    /// @return Current pressure altitude.
    float getPressureAltitude();

    /// @brief Gives access to I2C configuration state.
    /// @return True if I2C port is opened, false if not.
    bool isI2CConfigured() const { return i2cConfigured; }

    /// @brief Gives access to barometer acclimation/calibration state.
    /// @return True if calibration is completed, false if not completed/failed.
    bool isCalibrated() const { return calibrated; }

private:
    int baroI2cFd;      ///< File descriptor for the Barometer's I2C port.
    bool i2cConfigured; ///< State of I2C configuration.
    bool calibrated;    ///< State of barometer calibration/acclimation.
    bool readingI2C;    ///< State of reading the I2C port.

    int8_t pressureMSB; ///< First 8 bits of 24 bit pressure value.
    int8_t pressureCSB; ///< Middle 8 bits of 24 bit pressure value.
    int8_t pressureLSB; ///< Last 8 bits of 24 bit pressure value.
    int8_t tempMSB;     ///< First 8 bits of 16 bit temperature value.
    int8_t tempLSB;     ///< Last 8 bits of 16 bit temperature value.

    float pressureAltitude; ///< Current altitude read from barometer.
    float surfaceAltitude;  ///< Altitude above sea level (MSL) which corresponds to ground level.

    std::thread baroThread; ///< Thread that calibrates and lets barometer acclimate independent of main thread.

    /// @brief Executed by baroThread to calibrate/acclimate the barometer.
    void calibrate();

    /// @brief Joins baroThread to main thread.
    void closeCalibrationThread();
};

#endif