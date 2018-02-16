#ifndef SPI_H
#define SPI_H

extern char stm32_rx_buffer[100];
extern char stm32_tx_buffer[100];
extern bool spiConfigured;
extern bool authenticated;

//CS0 is barometer, CS1 is STM32 flight controller
extern int SPI_CS;
extern int spiFd;

void setupSPI();
void authFlightController();
void sendThrottle();

#endif