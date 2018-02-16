#ifndef SPI_H
#define SPI_H

#include <pthread.h>

extern char stm32_rx_buffer[100];
extern char stm32_tx_buffer[100];
extern bool spiConfigured;
extern bool authenticated;

//CS0 is barometer, CS1 is STM32 flight controller
extern int SPI_CS;
extern int spiFd;

//Mutex for STM32 communication threading
extern pthread_mutex_t stm32_mutex;

void setupSPI();
void authFlightController();
void sendThrottle();

#endif