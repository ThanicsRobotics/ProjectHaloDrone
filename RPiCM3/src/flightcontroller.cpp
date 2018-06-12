/// @file flightcontroller.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Implementation for the FlightController class, controls
/// the interfacing between the Raspberry Pi CM3 and the STM32F446.

#include <flightcontroller.h>

#include <types.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <bitset>

/// @brief Class constructor, initializes private variables.
FlightController::FlightController(bool *shutdown)
    : shutdownIndicator(shutdown), interface(shutdownIndicator, pwmInputs, fcConfig)
{
    // shutdownIndicator = shutdown;
    // interface = FCInterface(shutdownIndicator);
}

FlightController::~FlightController()
{
    printf("FC: Closing\n");
    stopFlight();
}

/// @brief Starts a new thread executing the interfaceLoop, and
/// conducts pre-flight checks.
void FlightController::startFlight()
{
    interface.startInterface();
    flightLoop();
    stopFlight();
}

/// @brief Stops interfaceLoop thread.
void FlightController::stopFlight()
{
    run = false;
    interface.stopInterface();
}

void FlightController::flightLoop()
{
    int heartbeatTimer, baroTimer, radioTimer = millis();
    int loopTimer = micros();
    float currentAltitude = 0;
    radio.setupSerial("/dev/serial0", 115200);
    
    printf("Calibrating barometer...\n");
    baro.setup();
    //radio.startReceiveLoop();
    while(!(*shutdownIndicator)) {
        //Every 260ms, get pressure altitude, if barometer is calibrated
        if (baro.isCalibrated() && (millis() - baroTimer > BARO_DELAY)) {
            currentAltitude = baro.getPressureAltitude();
            std::cout << "BARO: Altitude: " << currentAltitude << "m\n";
            baro.takeReading();
            baroTimer = millis();
        }

        radio.customReceiveByte(radio.readChar(), pwmInputs);
        std::cout << "Pitch: " << pwmInputs.pitchPWM
            << "\nRoll: " << pwmInputs.rollPWM
            << "\nYaw: " << pwmInputs.yawPWM
            << "\nthrottle: " << pwmInputs.throttlePWM
            << "\ntime: " << micros() - loopTimer << "us\n----\n";

        //Calculate new PID compensated throttle
        uint16_t newThrottle = calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude);
        //std::cout << "Loop Time: " << micros() - loopTimer << "us" << std::endl;
        loopTimer = micros();
    }
}

/// @brief Returns the drone's 3-axis angular position.
/// @return dronePosition struct.
dronePosition FlightController::getDronePosition()
{
    return fcConfig.flightPosition;
}

/// @brief Get current PWM inputs from radio and load them into currentMessage.
void FlightController::setPWMInputs(const channels &rcChannels)
{
    pwmInputs.pitchPWM = rcChannels.pitchPWM;
    pwmInputs.rollPWM = rcChannels.rollPWM;
    pwmInputs.yawPWM = rcChannels.yawPWM;
    pwmInputs.throttlePWM = rcChannels.throttlePWM;
}

void FlightController::getPWMInputs(channels &rcChannels)
{
    rcChannels.pitchPWM = pwmInputs.pitchPWM;
    rcChannels.rollPWM = pwmInputs.rollPWM;
    rcChannels.yawPWM = pwmInputs.yawPWM;
    rcChannels.throttlePWM = pwmInputs.throttlePWM;
}

/// @brief Sets the target altitude for takeoff hover.
/// @param hoverAltitude Hover altitude in centimeters (max 255 cm)
void FlightController::setHoverAltitude(uint8_t hoverAltitude)
{
    setAltitude = hoverAltitude;
}

float map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @brief Calculate new throttle based on a altitude-driven PID loop.
/// @param altitudePWM PWM Input from controller.
/// @param altitude Current altitude.
/// @return New throttle value (1000-2000).
uint16_t FlightController::calculateThrottlePID(uint16_t altitudePWM, float altitude)
{
    //Increase or decrease set altitude proportional to stick position
    if (altitudePWM < 1000)
        altitudePWM = 1500;
    if (altitudePWM >= 1520)
        setAltitude += 0.2 * map(altitudePWM, 1520, 2000, 1, 20);
    else if (altitudePWM <= 1480)
        setAltitude -= 0.2 * map(altitudePWM, 1000, 1480, 20, 1);
    if (setAltitude < 0)
        setAltitude = 0;

    //Proportional error
    pid_error_temp = (int)(setAltitude - altitude);

    //Integrating error over time
    pid_i_mem += pid_i_gain * pid_error_temp;
    if (pid_i_mem > pid_max)
        pid_i_mem = pid_max;
    else if (pid_i_mem < pid_max * -1)
        pid_i_mem = pid_max * -1;

    //Combining all controllers into one output
    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if (pid_output > pid_max)
        pid_output = pid_max;
    else if (pid_output < pid_max * -1)
        pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;

    //PID compensated throttle calculation
    uint16_t newThrottle = 1500 + pid_output;

    if (newThrottle > 2000)
        newThrottle = 2000;
    else if (newThrottle < 1000)
        newThrottle = 1000;
    return newThrottle;
}
