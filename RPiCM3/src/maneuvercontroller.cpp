#include <maneuvercontroller.h>
#include <flightcontroller.h>
#include <iostream>

ManeuverController::ManeuverController(std::shared_ptr<bool> shutdown)
    : shutdownIndicator(shutdown), baro(shutdown)
{

}

bool ManeuverController::newManeuver(Maneuvers maneuver)
{
    stopManeuver = true;
    maneuverThread.join();
    stopManeuver = false;
    switch (maneuver)
    {
        case Maneuvers::TAKEOFF:
            return startTakeoff();
            break;
        case Maneuvers::HOVER:
            return startHover();
            break;
        default:
            return false;
            break;
    }
}

bool ManeuverController::startTakeoff()
{

}

bool ManeuverController::startHover()
{
    if(activeManeuver == Maneuvers::NONE)
    {
        maneuverThread = std::thread([this](){
            while (!isTimeForShutdown())
            {

            }
            std::cout << "Hover Canceled. New Maneuver starting." << std::endl;
        });
        return true;
    }
    else return false;
}

float map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @brief Calculate new throttle based on a altitude-driven PID loop.
/// @param altitudePWM PWM Input from controller.
/// @param altitude Current altitude.
/// @return New throttle value (1000-2000).
uint16_t ManeuverController::calculateThrottlePID(uint16_t altitudePWM, float altitude)
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