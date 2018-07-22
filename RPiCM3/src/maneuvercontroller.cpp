#include <maneuvercontroller.h>
#include <flightcontroller.h>
#include <iostream>
#include <wiringPi.h>

ManeuverController::ManeuverController(std::shared_ptr<bool> shutdown)
    : shutdownIndicator(shutdown), baro(shutdown)
{
    pwmFinalOutputs.pitchPWM = 1500;
    pwmFinalOutputs.rollPWM = 1500;
    pwmFinalOutputs.yawPWM = 1500;
    pwmFinalOutputs.throttlePWM = 1000;
}

// Returns false if requested maneuver is already being executed
bool ManeuverController::executeManeuver(const Maneuver& requestedManeuver)
{
    if (requestedManeuver == activeManeuver) return false;
    else
    {
        activeManeuver = requestedManeuver;
        newManeuver();
    }
}

bool ManeuverController::newManeuver()
{
    stopManeuver = true;
    maneuverThread.join();
    stopManeuver = false;
    switch (activeManeuver.type)
    {
        case ManeuverType::TAKEOFF:
            return startTakeoff();
            break;
        case ManeuverType::HOVER:
            return startHover();
            break;
        default:
            return false;
            break;
    }
}

bool ManeuverController::maneuverLoop(std::function<void()> maneuverFunction, std::function<void()> callback)
{
    maneuverThread = std::thread([this, maneuverFunction, callback](){
        while (!isTimeForShutdown())
        {
            maneuverFunction();
        }
        callback();
    });
    activeManeuver.type = ManeuverType::NONE;
    return true;
}

// First index of maneuverOptions is takeoff height in centimeters
bool ManeuverController::startTakeoff()
{
    const float ascentRate = 50.0; // cm/sec
    const float increment = ascentRate / maneuverRefreshRate;
    int takeoffHeight = activeManeuver.maneuverOptions[0];

    getAltitude();
    float surfaceAltitude = currentAltitude;
    std::cout << "Taking off to " << takeoffHeight << "cm at " << ascentRate << "cm/sec" << std::endl;
    return maneuverLoop([this, surfaceAltitude, increment, takeoffHeight](){
        getAltitude();
        pwmFinalOutputs.throttlePWM = calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude, currentAltitude + increment);
        delay(1000/maneuverRefreshRate); // Updates at 250Hz, same as flight motor control

        // Start hover when current altitude reaches 5cm from target takeoff height
        if(abs(currentAltitude - (takeoffHeight + surfaceAltitude)) < 5) startHover();
    },
    [](){
        std::cout << "Takeoff Canceled. New Maneuver starting." << std::endl;
    });
}

bool ManeuverController::startLanding()
{
    const float descentRate = 30.0; // cm/sec
    const float decrement = descentRate / maneuverRefreshRate;

    getAltitude();
    float startingAltitude = currentAltitude;
    std::cout << "Landing from " << startingAltitude << "cm at " << descentRate << "cm/sec" << std::endl;
    return maneuverLoop([this, startingAltitude, decrement](){
        getAltitude();
        pwmFinalOutputs.throttlePWM = calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude, currentAltitude - decrement);
        delay(1000/maneuverRefreshRate); // Updates at 250Hz, same as flight motor control

        // If altitude is less than 5cm, wait 2 seconds to make sure we are at ground level,
        // then turn off motors
        if (currentAltitude < 5)
        {
            maneuverTimer += 1;
            if (maneuverTimer > maneuverRefreshRate * 2)
            {
                pwmFinalOutputs.throttlePWM = 1000; // motors off
                return;
            }
        }
    },
    [](){
        std::cout << "Landing Canceled. New Maneuver starting." << std::endl;
    });
}

bool ManeuverController::startHover()
{
    getAltitude();
    setAltitude = currentAltitude;
    return maneuverLoop([this](){
        getAltitude();
        pwmFinalOutputs.throttlePWM = calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude, setAltitude);
        delay(1000/maneuverRefreshRate); // Updates at 250Hz, same as flight motor control
    },
    [](){
        std::cout << "Hover Canceled. New Maneuver starting." << std::endl;
    });
}

void ManeuverController::getAltitude()
{
    // Use lidar altimeter
    if (currentAltitude < 10000)
    {
        // currentAltitude = lidarAltimeter.getDistance();
    }
    // Use barometer
    else
    {
        //Every 260ms, get pressure altitude, if barometer is calibrated
        if (baro.isCalibrated() && (millis() - baroTimer > BARO_DELAY)) {
            currentAltitude = baro.getPressureAltitude();
            std::cout << "BARO: Altitude: " << currentAltitude << "m\n";
            baro.takeReading();
            baroTimer = millis();
        }
    }
}

float map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @brief Calculate new throttle based on a altitude-driven PID loop.
/// @param altitudePWM PWM Input from controller.
/// @param altitude Current altitude.
/// @return New throttle value (1000-2000).
uint16_t ManeuverController::calculateThrottlePID(uint16_t altitudePWM, float currentAltitude, float setAltitude)
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
    pid_error_temp = (int)(setAltitude - currentAltitude);

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