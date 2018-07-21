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
FlightController::FlightController(std::shared_ptr<bool> shutdown)
    : shutdownIndicator(shutdown), interface(shutdownIndicator), 
    radio(WLAN::DeviceType::CLIENT, "raspberrypi.local", 5000), mc(shutdown)
{
    
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
    interface.setConfig(fcConfig);
    interface.startInterface();
    flightLoop();
    //stopFlight();
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
    radio.setUpdater([this](std::size_t size)
    {
        std::cout << "callback" << std::endl;
        radio.update(pwmInputs, size);
    });
    // radio.connect("raspberrypi.local", 5000);

    printf("Calibrating barometer...\n");
    //baro.setup();
    while(!(*shutdownIndicator)) {
        //Every 260ms, get pressure altitude, if barometer is calibrated
        // if (baro.isCalibrated() && (millis() - baroTimer > BARO_DELAY)) {
        //     currentAltitude = baro.getPressureAltitude();
        //     std::cout << "BARO: Altitude: " << currentAltitude << "m\n";
        //     baro.takeReading();
        //     baroTimer = millis();
        // }

        radio.checkBuffer();
        // interface.setPWMInputs(pwmInputs);
        std::cout << "Pitch: " << pwmInputs.pitchPWM
            << "\nRoll: " << pwmInputs.rollPWM
            << "\nYaw: " << pwmInputs.yawPWM
            << "\nthrottle: " << pwmInputs.throttlePWM
            << "\ntime: " << micros() - loopTimer << "us\n----\n";

        //Calculate new PID compensated throttle
        //uint16_t newThrottle = calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude);
        //std::cout << "Loop Time: " << micros() - loopTimer << "us" << std::endl;
        loopTimer = micros();
        delay(50);
    }
}

/// @brief Returns the drone's 3-axis angular position.
/// @return dronePosition struct.
AngularPosition FlightController::getDronePosition()
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
