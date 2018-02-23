#ifndef SPI_H
#define SPI_H

#include <pthread.h>
#include <string.h>
#include <iostream>

#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0x9F
#define STM32_ARM_CONF 0x0A

extern char stm32_rx_buffer[100];
extern char stm32_tx_buffer[100];
extern volatile bool spiConfigured;
extern volatile bool authenticated;

extern volatile bool armRequest;
extern volatile bool authRequest;
extern volatile bool armed;
extern volatile bool testGyro;

extern volatile bool run;

//CS0 is barometer, CS1 is STM32 flight controller
extern volatile int SPI_CS;
extern volatile int spiFd;

extern std::string projectPath;

//Mutex for STM32 communication threading
extern pthread_mutex_t stm32_mutex;

void setupSPI();
void arm();
void disarm();
void authFlightController();
void sendThrottle();
void *spiLoop(void *void_ptr);

#endif