#ifndef MANEUVERS_H
#define MANEUVERS_H

#include <functional>
#include <types.h>
#include <thread>
#include <memory>

class ManeuverController
{
public:
    ManeuverController(std::shared_ptr<bool> shutdown);
    bool newManeuver(Maneuvers maneuver);

    int getThrottleFactor() const { return throttleFactor; }
    int getPitchFactor() const { return pitchFactor; }
    int getRollFactor() const { return rollFactor; }
    int getYawFactor() const { return yawFactor; }

private:
    Maneuvers activeManeuver = Maneuvers::NONE;
    std::thread maneuverThread;
    bool stopManeuver = false;
    std::shared_ptr<bool> shutdownIndicator;
    Barometer baro;

    bool startTakeoff();
    bool startHover();
    bool isTimeForShutdown() { return (!stopManeuver && !(*shutdownIndicator)) ? false : true; }
    uint16_t calculateThrottlePID(uint16_t altitudePWM, float altitude);
    
    //Throttle PID Variables and functions for hovering
	int pid_p_gain, pid_i_gain, pid_d_gain; ///< PID gains.
	int pid_max;							///< Maximum output of the PID-controller (+/-)
	int pid_error_temp;						///< PID proportional error
	int pid_i_mem, pid_setpoint, pid_last_d_error;
	int pid_output;

	float currentAltitude, setAltitude;

    int pitchFactor = 0;
    int rollFactor = 0;
    int yawFactor = 0;
    int throttleFactor = 0;
};

#endif