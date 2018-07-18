/// @file types.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Global types used by multiple files.

#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>
#include <string>

///////////////////////
/////
///// Communication Protocol Types
/////
///////////////////////

/// @brief Holds all PWM control signals. All within 1000-2000.
struct channels
{
    uint16_t rollPWM = 0;
    uint16_t pitchPWM = 0;
    uint16_t yawPWM = 0;
    uint16_t throttlePWM = 0;
};

/// @brief Custom message structure for communication network.
/// Message payload is exactly PAYLOAD_LEN bytes long.
struct messagePacket {
    uint8_t msgid;          ///< ID of type of message.
    uint8_t fromid;         ///< ID of sender system.
    uint8_t seqid;          ///< Sequential message number, used for checking message continuity.
    channels rcChannels;
};

/// @brief Contains states of message parsing process.
enum MSG_STATE {
    WAITING = 0,    ///< Waiting for starting header.
    FILLING = 1,    ///< Message has started, and filling buffer.
    FAIL = 2,       ///< Buffer overflowed, or other error.
    DONE = 3        ///< Message footer found, finished filing buffer.
};


///////////////////////
/////
///// Streaming Types
/////
///////////////////////

enum VideoFormat
{
    NONE = 0,
    MONO,
    STEREO
};

///////////////////////
/////
///// Misc Types
/////
///////////////////////

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