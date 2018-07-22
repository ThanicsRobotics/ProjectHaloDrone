/// @file barometer.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of Barometer class.

#ifndef BAROMETER_H
#define BAROMETER_H

#include <inttypes.h>
#include <thread>
#include <memory>

#define BARO_DELAY 260

/// @brief Controls barometer and access to altitude measurements.
class Barometer
{
public:
    /// @brief Initializes private variables.
    Barometer(std::shared_ptr<bool> shutdown);
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
    std::shared_ptr<bool> shutdownIndicator;
    // std::shared_ptr<ThreadStatus> threadStatusIndicator;

    enum CalibrationState {
        NOT_CALIBRATED = 0,
        IN_PROGRESS,
        DONE,
        THREAD_CLOSED
    } calState = NOT_CALIBRATED;

    bool alreadySetup = false;
    int baroI2cFd;      ///< File descriptor for the Barometer's I2C port.
    bool i2cConfigured = false; ///< State of I2C configuration.
    bool calibrated = false;    ///< State of barometer calibration/acclimation.
    bool readingI2C = false;    ///< State of reading the I2C port.

    int8_t pressureMSB = 0; ///< First 8 bits of 24 bit pressure value.
    int8_t pressureCSB = 0; ///< Middle 8 bits of 24 bit pressure value.
    int8_t pressureLSB = 0; ///< Last 8 bits of 24 bit pressure value.
    int8_t tempMSB = 0;     ///< First 8 bits of 16 bit temperature value.
    int8_t tempLSB = 0;     ///< Last 8 bits of 16 bit temperature value.

    float pressureAltitude = 0.0; ///< Current altitude read from barometer.
    float surfaceAltitude = 0.0;  ///< Altitude above sea level (MSL) which corresponds to ground level.

    std::thread baroThread; ///< Thread that calibrates and lets barometer acclimate independent of main thread.

    /// @brief Executed by baroThread to calibrate/acclimate the barometer.
    void calibrate();

    /// @brief Joins baroThread to main thread.
    void closeCalibrationThread();
};

#endif