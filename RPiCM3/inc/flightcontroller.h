/// @file flightcontroller.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of FlightController class.

#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include <fcinterface.h>
#include <barometer.h>
#include <wlanradio.h>
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

	/// @brief Sets the target altitude for takeoff hover.
	/// @param hoverAltitude Hover altitude in centimeters (max 255 cm).
	void setHoverAltitude(uint8_t hoverAltitude);

	/// @brief Gets drone's angular position (pitch, roll, yaw).
	dronePosition getDronePosition();

	bool isTestGyroActive() const { return fcConfig.testGyro; }
    bool isMotorTestActive() const { return fcConfig.motorTest; }
    bool isNoMotorsActive() const { return fcConfig.noMotors; }

    void setTestGyro(bool state) { fcConfig.testGyro = state; }
    void setMotorTest(bool state) { fcConfig.motorTest = state; }
    void setNoMotors(bool state) { fcConfig.noMotors = state; }
    void setSTM32Resetting(bool state) { fcConfig.stm32Resetting = state; }

	/// @brief Calculate new throttle based on a altitude-driven PID loop.
	/// @param altitudePWM PWM Input from controller.
	/// @param altitude Current altitude.
	/// @return New throttle value (1000-2000).
	uint16_t calculateThrottlePID(uint16_t altitudePWM, float altitude);

  private:
  	//bool *shutdownIndicator = nullptr;
	std::shared_ptr<bool> shutdownIndicator;

	FCInterfaceConfig fcConfig;
	FCInterface interface;
	WLANRadio radio;
    Barometer baro;
	
	bool run = true;

	//dronePosition flightPosition;
	channels pwmInputs;
	//std::shared_ptr<channels> pwmInputs;

	//Throttle PID Variables and functions for hovering
	int pid_p_gain, pid_i_gain, pid_d_gain; ///< PID gains.
	int pid_max;							///< Maximum output of the PID-controller (+/-)
	int pid_error_temp;						///< PID proportional error
	int pid_i_mem, pid_setpoint, pid_last_d_error;
	int pid_output;

	int lastAltitude;
	float currentAltitude, setAltitude;

	void flightLoop();
};

#endif