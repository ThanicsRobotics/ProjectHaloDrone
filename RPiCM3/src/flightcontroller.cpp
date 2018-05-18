/// @file flightcontroller.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Implementation for the FlightController class, controls
/// the interfacing between the Raspberry Pi CM3 and the STM32F446.

#include <flightcontroller.h>
#include <radio.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <bitset>

#define SEL2 5
#define projectPath std::string("./")

#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0xFF9F
#define STM32_ARM_CONF 0xFF0A
#define STM32_DISARM_TEST 0xFF8F
#define STM32_DISARM_CONF 0xFFFB
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

#define PITCH_COEFF 0x01
#define ROLL_COEFF 0x02

/// @brief Class constrcutor, initializes private variables.
FlightController::FlightController()
{
    run = true;
    spiCS = 0;
}

/// @brief Opens the SPI port and sets the spiConfigured flag to true.
void FlightController::setupSPI() {
    //Switch to flight controller, setup SPI @ 3MHz
    spiCS = 0;
    if ((spiFd = spiOpen(spiCS, 3000000, 0)) < 0) {
        std::cout << "SPI failed: " << strerror(errno) << "\n";
        exit(1);
    }
    else {
        std::cout << "Opening SPI. FD: " << spiFd << " ID: " << interface.get_id() << "\n";
        spiConfigured = true;
    }
}

/// @brief Closes the SPI port if it has been opened, and disarms
/// the motors first if not already.
void FlightController::closeSPI() {
    if (armed) this->disarm();
    if(spiConfigured) spiClose(spiFd);
}

/// @brief Requests the FlightController class to do a Service.
/// @param serviceType A member of the Service enum
/// -> ARM: Flags the armRequest to true, to arm the drone in the next loop
/// -> DISARM: Flags disarmRequest to true, to disarm the drone in the next loop
/// -> AUTH: Flags authRequest to true, to authenticate the presence of the STM32F446 on next loop
/// -> RESET: Immediately resets the STM32F446, should be used outside of interfaceLoop
void FlightController::requestService(Service serviceType) {
    switch (serviceType) {
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
void FlightController::requestSend(fcMessage data) {
    data = currentMessage;
    sendRequest = true;
}

/// @brief Starts a new thread executing the interfaceLoop.
void FlightController::startFlight() {
    interface = std::thread([this]{ interfaceLoop(); });
}

/// @brief Stops interfaceLoop thread.
void FlightController::stopFlight() {
    run = false;
    interface.join();
}

/// @brief Sends disarm command to STM32, which disables the motors.
void FlightController::disarm() {
    int data = 0;
    while ((data != STM32_DISARM_CONF) && run) {
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
void FlightController::arm() {
    int data = 0;
    while ((data != STM32_ARM_CONF) && run) {
        int armCode = STM32_ARM_TEST;
        if (motorTest) armCode = MOTOR_TEST;
        else if (noMotors) armCode = NO_MOTORS;
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
void FlightController::reset() {
    pinMode(SEL2, OUTPUT);
    digitalWrite(SEL2, LOW);
    system(("sudo openocd -f " + projectPath + "src/reset.cfg").c_str());
    pinMode(SEL2, INPUT);
}

/// @brief Makes sure the STM32F446 is active and listening.
void FlightController::auth() {
    //Reset flight controller using OpenOCD
    reset();

    authenticated = false;
    char buffer[100];
    unsigned int authKey = 0;
    std::cout << "Authenticating...\n";
    int start = millis();
    while(authKey != AUTH_KEY) {
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
        if (millis() - start > 8000) {
            exit(1);
        }
    }
    std::cout << "Authenticated\n";
    authenticated = true;
}

/// @brief Breaks up the message packet into 16-bit packets and 
/// sends them to STM32F446
void FlightController::sendMessage() {
    spiBuffer buffer = packMessage();
    // for (int i = 0; i < buffer.len; i++) {
    //     std::bitset<8> x(buffer.buf[i]);
    //     std::cout << x << " | ";
    //     printf("%p\n", buffer.buf);
    // }

    //printf("test\n");
    for (int i = 0; i < buffer.len; i += 2) {
        stm32_tx_buffer[1] = buffer.buf[i+1];
        stm32_tx_buffer[0] = buffer.buf[i];
        spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
    }
    //printf("test2\n");
    //CLOCK SPEED TEST
    //unsigned long int clockspeed = buffer[1];
    //std::cout << " | Clock: " << clockspeed << endl;
}

/// @brief Packing message to send to Flight Controller over SPI.
/// Format:
/// {0x00,0x00,...
///  ^Two 0's always lead start of message
/// ...Pitch_H,Pitch_L,Roll_H,Roll_L,Yaw_H,Yaw_L,Throttle_H,Throttle_L}
///    ^PWM control values, high byte followed by low byte
FlightController::spiBuffer FlightController::packMessage() {
    const uint8_t msgLen = 11;
    static uint8_t msg[msgLen];
    msg[0] = 0;
    msg[1] = 0;
    int i;
    uint16_t* pwmInput = currentMessage.pwm;
    for (i = 2; i < (msgLen-1); i+=2) {
        uint16_t pwm = *pwmInput;
        msg[i] = (pwm >> 8) & 0xFF;
        msg[i+1] = pwm & 0xFF;
        pwmInput++;
    }
    msg[i] = '\0';
    spiBuffer buf;
    buf.len = msgLen;
    //buf.buf = (std::shared_ptr<uint8_t[]>)msg;
    buf.buf = (uint8_t*)msg;
    //printf("%p\n", buf.buf);
    return buf;
}

/// @brief Loop being executed by interface thread.
/// Sends messages to STM32F446 based on requests,
/// and decodes incoming messages from STM32F446
void FlightController::interfaceLoop() {
    if(!spiConfigured) this->setupSPI();
    if(!authenticated) this->auth();
    while(run) {
        if (armRequest) {
            //std::cout << "Arming..." << endl;
            this->arm();
            armRequest = false;
        }
        else if (disarmRequest) {
            this->disarm();
            disarmRequest = false;
        }
        else if (sendRequest) {
            this->sendMessage();
            sendRequest = false;
        }
        else if (authRequest) {
            //std::cout << "Authenticating..." << endl;
            this->auth();
            authRequest = false;
        }
        else if (testGyro) {
            spiRead(spiFd, stm32_rx_buffer, 2);
            gyroPitch = (signed char)stm32_rx_buffer[0];
            gyroRoll = (signed char)stm32_rx_buffer[1];
        }
        sendMessage();
        //printf("test3\n");
        fcReceivedData = (short)(stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1]);
        switch (stm32_rx_buffer[0]) {
            case PITCH_COEFF:
                gyroPitch = stm32_rx_buffer[1];
                break;
            case ROLL_COEFF:
                gyroRoll = stm32_rx_buffer[1];
                break;
            default:
                break;
        }
    }
    if (armed) this->disarm();
}

/// @brief Returns the drone's 3-axis angular position.
/// @return dronePosition struct.
FlightController::dronePosition FlightController::getDronePosition() {
    return flightPosition;
}

/// @brief Sets the target altitude for takeoff hover.
/// @param hoverAltitude Hover altitude in centimeters (max 255 cm)
void FlightController::setHoverAltitude(uint8_t hoverAltitude) {
    setAltitude = altitude;
}

static float map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @brief Calculate new throttle based on a altitude-driven PID loop.
/// @param altitudePWM PWM Input from controller.
/// @param altitude Current altitude.
/// @return New throttle value (1000-2000).
uint16_t FlightController::calculateThrottlePID(uint16_t altitudePWM, float altitude) {
    //Increase or decrease set altitude proportional to stick position
    if (altitudePWM < 1000) altitudePWM = 1500;
    if (altitudePWM >= 1520) setAltitude += 0.2 * map(altitudePWM, 1520, 2000, 1, 20);
    else if (altitudePWM <= 1480) setAltitude -= 0.2 * map(altitudePWM, 1000, 1480, 20, 1);
    if (setAltitude < 0) setAltitude = 0;

    //Proportional error
    pid_error_temp = (int)(setAltitude - altitude);

    //Integrating error over time
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    //Combining all controllers into one output
    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;

    //PID compensated throttle calculation
    uint16_t newThrottle = 1500 + pid_output;

    if (newThrottle > 2000) newThrottle = 2000;
    else if (newThrottle < 1000) newThrottle = 1000;
    return newThrottle;
}

FlightController::~FlightController() {
}