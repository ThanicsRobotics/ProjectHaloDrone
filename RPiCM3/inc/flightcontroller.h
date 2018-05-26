/// @file flightcontroller.h
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Definition of FlightController class.

#ifndef SPI_H
#define SPI_H

#include <thread>
#include <string>
#include <string.h>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <array>

#include <types.h>

#define MSG_LEN 11

/// @brief Controls the interfacing between the Raspberry Pi CM3
/// and the STM32F446, and does flight maneuver calculations.
class FlightController
{
public:
    /// @brief Message data structure for sending to STM32F446.
    struct fcMessage {
        float travelAngle; ///< Desired compass angle for the STM32F446 to fly.
        uint16_t pwm[4]; ///< Array containing PWM inputs, in order: pitch, roll, yaw, throttle.
    };

    /// @brief Structure for holding drone angular position.
    struct dronePosition {
        int8_t pitch, roll; ///< Pitch and roll angles.
        int16_t yaw; ///< Yaw angle.
    };

    /// @brief Types of services for the Flight Controller to execute.
    enum Service {
        ARM = 0, ///< Arms the drone and TURNS ON MOTORS.
        DISARM, ///< Disarms the drone, turns off motors.
        AUTH, ///< Authenticates the drone, checks it exists.
        RESET ///< Resets the STM32F446.
    };

    /// @brief Class constrcutor, initializes private variables.
    FlightController();

    ~FlightController();

    /// @brief Opens the SPI port and sets the spiConfigured flag to true.
    void setupSPI();

    /// @brief Closes the SPI port.
    void closeSPI();

    /// @brief Requests the FlightController class to do a Service.
    /// @param serviceType A member of the Service enum
    void requestService(Service serviceType);

    /// @brief Requests the interfaceLoop to send data to the STM32F446.
    /// @param data A data message structured in a fcMessage struct.
    void requestSend(fcMessage data);

    void setPWMInputs(const channels& rcChannels);
    void getPWMInputs(channels& rcChannels);

    /// @brief Starts a new thread executing the interfaceLoop.
    void startFlight();

    /// @brief Stops interfaceLoop thread.
    void stopFlight();

    /// @brief Checks if SPI port has been opened.
    /// @return true if opened, false if closed.
    bool isSPIConfigured() const { return spiConfigured; }

    /// @brief Checks if drone is armed.
    /// @return true if armed, false if disarmed.
    bool isArmed() const { return armed; }

    /// @brief Checks if STM32F446 has been authenticated.
    /// @return true if authenticated, false if not.
    bool isAuthenticated() const { return authenticated; }

    bool isTestGyroActive() const { return testGyro; }
    bool isMotorTestActive() const { return motorTest; }
    bool isNoMotorsActive() const { return noMotors; }

    /// @brief Checks if thread is running.
    /// @return true if thread is active, false if not.
    bool isRunning() const { return run; }

    void setTestGyro(bool state) { testGyro = state; }
    void setMotorTest(bool state) { motorTest = state; }
    void setNoMotors(bool state) { noMotors = state; }

    /// @brief Sets the target altitude for takeoff hover.
    /// @param hoverAltitude Hover altitude in centimeters (max 255 cm).
    void setHoverAltitude(uint8_t hoverAltitude);

    /// @brief Gets drone's angular position (pitch, roll, yaw).
    dronePosition getDronePosition();

    /// @brief Method to access the data received from STM32F446 SPI.
    /// @return 16-bit SPI packet.
    int16_t getReceivedData() { return fcReceivedData; }

    /// @brief Calculate new throttle based on a altitude-driven PID loop.
    /// @param altitudePWM PWM Input from controller.
    /// @param altitude Current altitude.
    /// @return New throttle value (1000-2000).
    uint16_t calculateThrottlePID(uint16_t altitudePWM, float altitude);

private:
    struct spiBuffer {
        uint8_t len;
        //std::shared_ptr<uint8_t[]> buf;
        uint8_t* buf;
    };

    bool run;
    bool spiConfigured;
    bool authenticated;
    bool armRequest, authRequest, disarmRequest, sendRequest;
    bool armed;

    bool testGyro;
    bool motorTest;
    bool noMotors;

    //Gyro angle variables
    int8_t gyroPitch, gyroRoll;
    char stm32_rx_buffer[2], stm32_tx_buffer[2];
    int16_t fcReceivedData;
    dronePosition flightPosition;

    //CS0 is barometer, CS1 is STM32 flight controller
    int spiCS;
    int spiFd;

    //Throttle PID Variables and functions for hovering
    int pid_p_gain, pid_i_gain, pid_d_gain;         ///< PID gains.
    int pid_max;                                    ///< Maximum output of the PID-controller (+/-)
    int pid_error_temp;                             ///< PID proportional error
    int pid_i_mem, pid_setpoint, pid_last_d_error;
    int pid_output;

    int lastAltitude;
    float currentAltitude, setAltitude;

    fcMessage currentMessage;
    std::thread interface;

    void arm();
    void disarm();
    void reset();
    void auth();
    void interfaceLoop();
    void packMessage(std::array<uint8_t, MSG_LEN>& msg);
    void sendMessage();
    void updateMessage();
    
};

#endif