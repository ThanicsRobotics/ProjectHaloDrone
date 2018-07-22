#ifndef MANEUVERS_H
#define MANEUVERS_H

#include <barometer.h>
#include <tofsensor.h>

#include <functional>
#include <types.h>
#include <thread>
#include <memory>

class ManeuverController
{
public:
    ManeuverController(std::shared_ptr<bool> shutdown);
    bool executeManeuver(const Maneuver& requestedManeuver);

    int getThrottleFinal() const { return throttleFinal; }
    int getPitchFinal() const { return pitchFinal; }
    int getRollFinal() const { return rollFinal; }
    int getYawFinal() const { return yawFinal; }
    void getPWMFinalOutputs(channels& finalPWM) const { finalPWM = pwmFinalOutputs; }

private:
    Maneuver activeManeuver;
    
    std::thread maneuverThread;
    bool stopManeuver = false;
    std::shared_ptr<bool> shutdownIndicator;
    unsigned int baroTimer = 0;
    Barometer baro;
    channels pwmInputs;
    channels pwmFinalOutputs;
    TOFSensor lidarAltimeter;

    bool startTakeoff();
    bool startHover();
    bool isTimeForShutdown() { return (!stopManeuver && !(*shutdownIndicator)) ? false : true; }
    void getAltitude();
    bool maneuverLoop(std::function<void()> maneuverFunction, std::function<void()> callback);
    bool newManeuver();
    uint16_t calculateThrottlePID(uint16_t altitudePWM, float altitude);
    
    //Throttle PID Variables and functions for hovering
	int pid_p_gain, pid_i_gain, pid_d_gain; ///< PID gains.
	int pid_max;							///< Maximum output of the PID-controller (+/-)
	int pid_error_temp;						///< PID proportional error
	int pid_i_mem, pid_setpoint, pid_last_d_error;
	int pid_output;

	float currentAltitude, setAltitude;

    uint16_t pitchFinal = 0;
    uint16_t rollFinal = 0;
    uint16_t yawFinal = 0;
    uint16_t throttleFinal = 0;
};

#endif