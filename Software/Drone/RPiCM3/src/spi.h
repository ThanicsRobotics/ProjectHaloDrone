#ifndef SPI_H
#define SPI_H

#include <pthread.h>
#include <string.h>
#include <iostream>

extern char stm32_rx_buffer[100];
extern char stm32_tx_buffer[100];
extern bool spiConfigured;
extern bool authenticated;
extern bool preStart;

//CS0 is barometer, CS1 is STM32 flight controller
extern int SPI_CS;
extern int spiFd;

extern int newThrottle;

extern std::string projectPath;

//Mutex for STM32 communication threading
extern pthread_mutex_t stm32_mutex;

void setupSPI();
void authFlightController();
void calculateThrottle();

#endif