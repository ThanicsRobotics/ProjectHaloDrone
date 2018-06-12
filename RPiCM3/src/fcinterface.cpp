#include <fcinterface.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define SEL2 5
#define projectPath std::string("./")

#define GYRO_CAL 0x04
#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0xFF9F
#define STM32_ARM_CONF 0xFF0A
#define STM32_DISARM_TEST 0xFF8F
#define STM32_DISARM_CONF 0xFFFB
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

#define PITCH_COEFF 0x01
#define ROLL_COEFF 0x02

FCInterface::FCInterface(bool *running, channels& pwmInputs, FCInterfaceConfig& cfg)
    : fcRunning(running), interfaceConfig(cfg)
{
    currentMessage.rcChannels = pwmInputs;
    setupSPI();
}

FCInterface::~FCInterface()
{
    interfaceConfig.stm32Resetting = true;
    reset();
    closeSPI();
}

/// @brief Opens the SPI port and sets the spiConfigured flag to true.
void FCInterface::setupSPI()
{
    //Switch to flight controller, setup SPI @ 3MHz
    spiCS = 0;
    if ((spiFd = spiOpen(spiCS, 3000000, 0)) < 0)
    {
        std::cout << "SPI failed: " << strerror(errno) << "\n";
        exit(1);
    }
    else
    {
        std::cout << "Opening SPI. FD: " << spiFd << " ID: " << interfaceThread.get_id() << "\n";
        spiConfigured = true;
    }
}

/// @brief Closes the SPI port if it has been opened, and disarms
/// the motors first if not already.
void FCInterface::closeSPI()
{
    if (armed)
        disarm();
    if (spiConfigured)
    {
        spiClose(spiFd);
        spiConfigured = false;
    }
}

void FCInterface::startInterface()
{
    if (!spiConfigured) setupSPI();
    interfaceThread = std::thread([this] { interfaceLoop(); });

    // Waiting for gyro on STM32 to calibrate
    while (!spiConfigured || !authenticated)
    {
        delay(50);
        // If the drone shuts down before authenticated/SPI is configured, then
        // exit loop
        if (!(*fcRunning))
        {
            return;
        }
    }
    //delay(200);
    std::cout << "Waiting for gyro calibration..." << std::endl;
    int start = millis();
    int repeat = 1;
    while (fcReceivedData != GYRO_CAL)
    {
        std::cout << "Received: " << fcReceivedData << std::endl;
        if (!(*fcRunning))
        {
            std::cout << "Shutting Down\n";
            return;
        }
        delay(50);
    }
    std::cout << "Calibration complete\n";

    // Manual arming process through SSH
    std::cout << "Type 'ARM' to arm the quadcopter: ";
    std::string input = "";
    std::getline(std::cin, input);
    if (input == "ARM")
    {
        requestService(FCInterface::Service::ARM);
    }
}

void FCInterface::stopInterface()
{
    interfaceThread.join();
}

/// @brief Requests the FlightController class to do a Service.
/// @param serviceType A member of the Service enum
/// -> ARM: Flags the armRequest to true, to arm the drone in the next loop
/// -> DISARM: Flags disarmRequest to true, to disarm the drone in the next loop
/// -> AUTH: Flags authRequest to true, to authenticate the presence of the STM32F446 on next loop
/// -> RESET: Immediately resets the STM32F446, should be used outside of interfaceLoop
void FCInterface::requestService(Service serviceType)
{
    switch (serviceType)
    {
    case Service::ARM:
        armRequest = true;
        break;
    case Service::DISARM:
        disarmRequest = true;
        break;
    case Service::AUTH:
        authRequest = true;
        break;
    case Service::RESET:
        this->reset();
        break;
    default:
        break;
    }
}

/// @brief Requests the interfaceLoop to send data to the STM32F446.
/// @param data A data message structured in a fcMessage struct.
void FCInterface::requestSend(fcMessage data)
{
    currentMessage = data;
    //sendRequest = true;
}

/// @brief Sends disarm command to STM32, which disables the motors.
void FCInterface::disarm()
{
    std::cout << "Disarming...\n";
    int data = 0;
    while ((data != STM32_DISARM_CONF) && *fcRunning)
    {
        int disarmCode = STM32_DISARM_TEST;
        stm32_tx_buffer[0] = (disarmCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = disarmCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);

        delay(50);
    }
    armed = false;
}

/// @brief Sends arm command to STM32, which TURNS ON MOTORS.
void FCInterface::arm()
{
    std::cout << "Arming...\n";
    uint16_t data = 0;
    while ((data != STM32_ARM_CONF) && *fcRunning)
    {
        uint16_t armCode = STM32_ARM_TEST;
        if (interfaceConfig.motorTest)
            armCode = MOTOR_TEST;
        else if (interfaceConfig.noMotors)
            armCode = NO_MOTORS;
        stm32_tx_buffer[0] = (armCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = armCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);

        delay(50);
    }
    armed = true;
}

/// @brief Switching Demux to STM32F446 programming pins, and sends reset command.
void FCInterface::reset()
{
    if (interfaceConfig.stm32Resetting)
    {
        pinMode(SEL2, OUTPUT);
        digitalWrite(SEL2, LOW);
        system(("sudo openocd -f " + projectPath + "src/reset.cfg").c_str());
        pinMode(SEL2, INPUT);
    }
}

/// @brief Makes sure the STM32F446 is active and listening.
void FCInterface::auth()
{
    //Reset flight controller using OpenOCD
    reset();

    authenticated = false;
    char buffer[2];
    unsigned int authKey = 0;
    std::cout << "Authenticating...\n";
    int start = millis();
    while (authKey != AUTH_KEY)
    {
        //Write to Authentication register
        buffer[0] = 0x00;
        buffer[1] = 0x01;
        spiWrite(spiFd, buffer, 2);
        delay(5);

        //Get Auth Key and send it back
        spiXfer(spiFd, buffer, buffer, 2);
        authKey = buffer[0] << 8 | buffer[1];
        std::cout << "Key: " << authKey << "\n";
        spiWrite(spiFd, buffer, 2);

        delay(50);
        // if (millis() - start > 8000) {
        //     exit(1);
        // }
        if (!(*fcRunning))
        {
            std::cout << "Shutting Down\n";
            return;
        }
    }
    std::cout << "Authenticated\n";
    authenticated = true;
}



/// @brief Breaks up the message packet into 16-bit packets and
/// sends them to STM32F446
void FCInterface::sendMessage()
{
    std::array<uint8_t, MSG_LEN> msg;
    packMessage(msg);
    std::copy(msg.begin(), msg.end(), stm32_tx_buffer);
    spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, MSG_LEN);
}

/// @brief Packing message to send to Flight Controller over SPI.
/// Format:
/// {0xFF,0xFE,...
///  ^Header of message
/// ...Pitch_H,Pitch_L,Roll_H,Roll_L,Yaw_H,Yaw_L,Throttle_H,Throttle_L}
///    ^PWM control values, high byte followed by low byte
void FCInterface::packMessage(std::array<uint8_t, MSG_LEN> &msg)
{
    msg[0] = 0xA0;
    msg[1] = (currentMessage.rcChannels.pitchPWM >> 8) & 0xFF;
    msg[2] = 0xA1;
    msg[3] = currentMessage.rcChannels.pitchPWM & 0xFF;
    msg[4] = 0xA2;
    msg[5] = (currentMessage.rcChannels.rollPWM >> 8) & 0xFF;
    msg[6] = 0xA3;
    msg[7] = currentMessage.rcChannels.rollPWM & 0xFF;
    msg[8] = 0xA4;
    msg[9] = (currentMessage.rcChannels.yawPWM >> 8) & 0xFF;
    msg[10] = 0xA5;
    msg[11] = currentMessage.rcChannels.yawPWM & 0xFF;
    msg[12] = 0xA6;
    msg[13] = (currentMessage.rcChannels.throttlePWM >> 8) & 0xFF;
    msg[14] = 0xA7;
    msg[15] = currentMessage.rcChannels.throttlePWM & 0xFF;

    // uint16_t *pwmInput = currentMessage.pwm;
    // for (int i = 0; i < MSG_LEN; i += 4)
    // {
    //     uint16_t pwm = *pwmInput;

    //     msg[i] = 0xA0 + i;
    //     msg[i + 1] = (pwm >> 8) & 0xFF;
    //     msg[i + 2] = 0xA1 + i;
    //     msg[i + 3] = pwm & 0xFF;

    //     pwmInput++;
    // }
}

/// @brief Loop being executed by interface thread.
/// Sends messages to STM32F446 based on requests,
/// and decodes incoming messages from STM32F446
void FCInterface::interfaceLoop()
{
    static int timer = micros();

    if (!spiConfigured)
        setupSPI();
    if (!authenticated)
        auth();
    while (*fcRunning)
    {
        if (armRequest)
        {
            //std::cout << "Arming..." << endl;
            arm();
            armRequest = false;
        }
        else if (disarmRequest)
        {
            disarm();
            disarmRequest = false;
        }
        // else if (sendRequest) {
        //     sendMessage();
        //     sendRequest = false;
        // }
        else if (authRequest)
        {
            //std::cout << "Authenticating..." << endl;
            auth();
            authRequest = false;
        }
        else if (interfaceConfig.testGyro)
        {
            spiRead(spiFd, stm32_rx_buffer, 2);
            interfaceConfig.flightPosition.pitch = (signed char)stm32_rx_buffer[0];
            interfaceConfig.flightPosition.roll = (signed char)stm32_rx_buffer[1];
        }
        sendMessage();
        // if (armed) {
        //     std::cout << "spitimer: " << micros() - timer << "\n";
        //     timer = micros();
        // }

        //printf("test3\n");
        fcReceivedData = stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1];
        switch (stm32_rx_buffer[0])
        {
        case PITCH_COEFF:
            interfaceConfig.flightPosition.pitch = stm32_rx_buffer[1];
            break;
        case ROLL_COEFF:
            interfaceConfig.flightPosition.roll = stm32_rx_buffer[1];
            break;
        default:
            break;
        }
    }
    if (armed)
        disarm();
}