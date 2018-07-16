/// @file radio.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of the Radio class template.

#ifndef SERIALRADIO_H
#define SERIALRADIO_H

#include <serial.h>
#include <types.h>

#define PAYLOAD_LEN 11

/// @brief Class template for controlling the radio, whether a WLAN radio, or UART Serial.
/// Specify "Serial" or "Stream" based Radio in template argument
/// for example: Radio<Serial> will generate a Serial-based radio object

class SerialRadio
{
public:
    /// @brief Convience function for inputing each byte received by radio and
    /// parsing it into a completed message.
    /// @param data Byte from radio stream.
    void customReceiveByte(uint8_t data, channels& rcChannels);

private:
    Serial serial;
    channels pwmInputs;         ///< Current PWM control inputs.
    uint8_t controllerStatus;   ///< Status of controller operation (currently not used). 

    /// @brief Loop executed by receive thread, gets data from interface and parses it.
    //void receiveLoop();

    /// @brief Parses incoming radio data and modifies a receivedMessage object to contain that data.
    /// @param data Byte from radio interface.
    /// @param msg A reference to the receivedMessage object to be modified.
    /// @return True if message is completed, false if error or still filling buffer.
    bool customParseChar(uint8_t data, messagePacket& msg);
};

#endif