/// @file flightcontroller.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Implementation for the FlightController class, controls
/// the interfacing between the Raspberry Pi CM3 and the STM32F446.

#include <flightcontroller.h>

#include <types.h>
#include <wiringPi.h>
#include <iostream>
#include <string>
// #include <stdio.h>
// #include <stdlib.h>
#include <errno.h>
#include <unistd.h>
// #include <bitset>

/// @brief Class constructor, initializes private variables.
FlightController::FlightController(std::shared_ptr<bool> shutdown)
    : shutdownIndicator(shutdown), interface(shutdownIndicator), mc(shutdownIndicator),
    radio(WLAN::DeviceType::CLIENT, "thanics-controller.local", 5000)
{
    radio.setUpdater([this](std::size_t size)
    {
        std::cout << "callback" << std::endl;

        // Every time drone receives 
        radio.update(pwmInputs, requestedManeuver, size);
    });
}

FlightController::~FlightController()
{
    std::cout << "FC: Closing\n";
    stopFlight();
}

/// @brief Starts a new thread executing the interfaceLoop, and
/// conducts pre-flight checks.
void FlightController::startFlight()
{
    interface.setConfig(fcConfig);

    // Arms drone (starts motors)
    interface.startInterface();
    preFlightCheck();
    flightLoop();
}

/// @brief Stops interfaceLoop thread.
void FlightController::stopFlight()
{
    run = false;
    interface.stopInterface();
}

void FlightController::checkFail(const std::string& error)
{
    std::cout << "Pre-Flight check failed: " << error << std::endl;
    while (!(*shutdownIndicator)) delay(50);
}

void FlightController::preFlightCheck()
{
    std::cout << "Trying to reconnect...\n";
    if (!radio.isConnected())
    {
        radio.reconnect();
    }

    if (!radio.isConnected())
    {
        checkFail("No connection to controller, CTRL-C to shutdown");
    }
}

void FlightController::flightLoop()
{
    int loopTimer = micros();
    while(!(*shutdownIndicator)) {
        // Checks if radio has anything to read
        radio.checkBuffer();

        // Feed PWM inputs into maneuver controller, get final values
        mc.setPWMInputs(pwmInputs);
        mc.executeManeuver(requestedManeuver);
        channels finalPWMs;
        mc.getPWMFinalOutputs(finalPWMs);

        // Sends final PWMs to motor controller
        //interface.setPWMInputs(finalPWMs);

        std::cout << "Pitch: " << finalPWMs.pitchPWM
            << "\nRoll: " << finalPWMs.rollPWM
            << "\nYaw: " << finalPWMs.yawPWM
            << "\nThrottle: " << finalPWMs.throttlePWM
            << "\nLoop Time: " << micros() - loopTimer << "us\n----\n";

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
