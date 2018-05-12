#ifndef SPI_H
#define SPI_H

#include <thread>
#include <string>
#include <iostream>
#include <stdint.h>
#include <memory>

class FlightController {
    public:
        struct fcMessage {
            float travelAngle;
            uint16_t pitch, roll, yaw, throttle;
        };
        struct dronePosition {
            int8_t pitch, roll;
            int16_t yaw;
        };

        enum Service {
            ARM = 0, DISARM, AUTH, RESET
        };

        FlightController();
        ~FlightController();
        void setupSPI();
        void startFlight();
        void stopFlight();

        bool isSPIConfigured() { return spiConfigured; }
        bool isArmed() { return armed; }
        bool isAuthenticated() { return authenticated; }
        void requestService(Service serviceType);
        void requestSend(fcMessage data);
        void setHoverAltitude(uint8_t altitude); //in cm
        dronePosition getDronePosition();
        uint16_t calculateThrottlePID(uint16_t throttlePWM, float altitude);
    
    private:
        struct spiBuffer {
            uint8_t len;
            std::shared_ptr<uint8_t[]> buf;
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
        signed char gyroPitch, gyroRoll;
        char stm32_rx_buffer[2], stm32_tx_buffer[2];
        short int FCReceivedData;
        dronePosition flightPosition;

        //CS0 is barometer, CS1 is STM32 flight controller
        int spiCS;
        int spiFd;

        void arm();
        void disarm();
        void reset();
        void auth();
        std::thread interface;
        void interfaceLoop();
        spiBuffer packMessage();
        void sendMessage();
        fcMessage currentMessage;

        //Throttle PID Variables and functions for hovering
        int pid_p_gain, pid_i_gain, pid_d_gain;
        int pid_max;                                    //Maximum output of the PID-controller (+/-)
        int pid_error_temp;
        int pid_i_mem, pid_setpoint, pid_last_d_error;
        int pid_output;

        int lastAltitude;
        float currentAltitude, setAltitude;
};

#endif