#ifndef SPI_H
#define SPI_H

#include <pthread.h>
#include <string.h>
#include <iostream>

#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0xFF9F
#define STM32_ARM_CONF 0xFF0A
#define STM32_DISARM_TEST 0xFF8F
#define STM32_DISARM_CONF 0xFFFB
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

extern char stm32_rx_buffer[100];
extern char stm32_tx_buffer[100];
extern volatile bool spiConfigured;
extern volatile bool authenticated;

extern volatile bool armRequest;
extern volatile bool authRequest;
extern volatile bool disarmRequest;
extern volatile bool armed;
extern volatile bool testGyro;
extern volatile bool motorTest;
extern volatile bool noMotors;

//Gyro angle variables
extern volatile signed char gyroPitch;
extern volatile signed char gyroRoll;
extern volatile short int FCReceivedData;

extern volatile bool run;

//CS0 is barometer, CS1 is STM32 flight controller
extern volatile int SPI_CS;
extern volatile int spiFd;

extern std::string projectPath;

//Mutex for STM32 communication threading
extern pthread_mutex_t stm32_mutex;

struct fcMessage {
    float travelAngle;
    int pitchPWM;
    int rollPWM;
    int throttle;
    int yawPWM;
};

void setupSPI();
void arm();
void disarm();
void resetSTM32F446();
void authFlightController();
void sendThrottle();
void *spiLoop(void *void_ptr);

#endif