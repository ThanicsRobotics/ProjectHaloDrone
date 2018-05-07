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
extern bool spiConfigured;
extern bool authenticated;

extern bool armRequest;
extern bool authRequest;
extern bool disarmRequest;
extern bool armed;
extern bool testGyro;
extern bool motorTest;
extern bool noMotors;

//Gyro angle variables
extern signed char gyroPitch;
extern signed char gyroRoll;
extern short int FCReceivedData;

extern bool run;

//CS0 is barometer, CS1 is STM32 flight controller
extern int SPI_CS;
extern int spiFd;

extern std::string projectPath;

//Mutex for STM32 communication threading
//extern pthread_mutex_t stm32_mutex;

struct fcMessage {
    float travelAngle;
    uint16_t pwm[4]; //Pitch, Roll, Yaw, Throttle
};

void setupSPI();
void arm();
void disarm();
void resetSTM32F446();
void authFlightController();
void sendThrottle();
char *packMessage(fcMessage info);
void *spiLoop(void*);

#endif