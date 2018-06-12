/// @file types.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Global types used by multiple files.

#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>

/// @brief Holds all PWM control signals. All within 1000-2000.
struct channels
{
    uint16_t rollPWM = 0;
    uint16_t pitchPWM = 0;
    uint16_t yawPWM = 0;
    uint16_t throttlePWM = 0;
};

/// @brief Structure for holding drone angular position.
struct dronePosition
{
    int8_t pitch, roll; ///< Pitch and roll angles.
    int16_t yaw;		///< Yaw angle.
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