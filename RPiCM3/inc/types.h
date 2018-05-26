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
    uint16_t rollPWM, pitchPWM, yawPWM, throttlePWM;
};

#endif