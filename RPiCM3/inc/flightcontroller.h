/// @file flightcontroller.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of FlightController class.

#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include <fcinterface.h>
#include <barometer.h>
#include <wlanradio.h>
#include <maneuvercontroller.h>

#include <string>
#include <string.h>
#include <iostream>
#include <stdint.h>
#include <types.h>
#include <memory>

/// @brief Controls flight operations and calculations.
class FlightController
{
  public:
	/// @brief Initializes private variables.
	FlightController(std::shared_ptr<bool> shutdown);
	~FlightController();

	void setPWMInputs(const channels &rcChannels);
	void getPWMInputs(channels &rcChannels);

	/// @brief Starts a new thread executing the interfaceLoop.
	void startFlight();

	/// @brief Stops interfaceLoop thread.
	void stopFlight();

	/// @brief Checks if thread is running.
	/// @return true if thread is active, false if not.
	bool isRunning() const { return run; }

	/// @brief Gets drone's angular position (pitch, roll, yaw).
	AngularPosition getDronePosition();

	bool isTestGyroActive() const { return fcConfig.testGyro; }
    bool isMotorTestActive() const { return fcConfig.motorTest; }
    bool isNoMotorsActive() const { return fcConfig.noMotors; }

    void setTestGyro(bool state) { fcConfig.testGyro = state; }
    void setMotorTest(bool state) { fcConfig.motorTest = state; }
    void setNoMotors(bool state) { fcConfig.noMotors = state; }
    void setSTM32Resetting(bool state) { fcConfig.stm32Resetting = state; }

  private:
	std::shared_ptr<bool> shutdownIndicator;

	ManeuverController mc;
	FCInterfaceConfig fcConfig;
	FCInterface interface;
	WLANRadio radio;
	Maneuver requestedManeuver;
    //Barometer baro;
	
	bool run = true;

	channels pwmInputs;

	void flightLoop();
	void preFlightCheck();
	void checkFail(const std::string& error);
};

#endif