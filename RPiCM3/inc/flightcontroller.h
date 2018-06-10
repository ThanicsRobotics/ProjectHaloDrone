/// @file flightcontroller.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of FlightController class.

#ifndef SPI_H
#define SPI_H

#include <fcinterface.h>
#include <barometer.h>
#include <radio.h>
#include <thread>
#include <string>
#include <string.h>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <array>
#include <types.h>

#define MSG_LEN 16

/// @brief Controls flight operations and calculations.
class FlightController
{
  public:
	/// @brief Structure for holding drone angular position.
	struct dronePosition
	{
		int8_t pitch, roll; ///< Pitch and roll angles.
		int16_t yaw;		///< Yaw angle.
	};

	/// @brief Initializes private variables.
	FlightController(bool *shutdown);

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

	/// @brief Calculate new throttle based on a altitude-driven PID loop.
	/// @param altitudePWM PWM Input from controller.
	/// @param altitude Current altitude.
	/// @return New throttle value (1000-2000).
	uint16_t calculateThrottlePID(uint16_t altitudePWM, float altitude);

  private:
  	bool *shutdownIndicator;

	FCInterface interface(shutdownIndicator);
	Radio<Serial> radio;
    Barometer baro;
	
	bool run = true;

	dronePosition flightPosition;

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