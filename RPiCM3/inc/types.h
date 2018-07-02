/// @file types.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Global types used by multiple files.

#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>
#include <string>

/// @brief Holds all PWM control signals. All within 1000-2000.
struct channels
{
    uint16_t rollPWM = 0;
    uint16_t pitchPWM = 0;
    uint16_t yawPWM = 0;
    uint16_t throttlePWM = 0;
};

enum VideoFormat
{
    NONE = 0,
    MONO,
    STEREO
};

struct CommandLineOptions
{
    bool testGyro = false;
    bool testMotors = false;
    bool disableMotors = false;
    bool enableSTM32Resetting = true;
    VideoFormat record = VideoFormat::NONE;
    std::string ipAddress = "";
    std::string ports[2] = {"", ""};
};

/// @brief Structure for holding drone angular position.
struct dronePosition
{
    int8_t pitch = 0;   ///< Pitch angle.
    int8_t roll = 0;    ///< Roll angle.
    int16_t yaw = 0;	///< Yaw angle.
};

struct FCInterfaceConfig
{
    bool testGyro = false;
    bool motorTest = false;
    bool noMotors = false;
    bool stm32Resetting = true;
    dronePosition flightPosition;
};

#endif