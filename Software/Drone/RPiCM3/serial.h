#ifndef SERIAL_H
#define SERIAL_H

//Serial UART port file descriptor
extern int serialFd;

extern int charCount = 0;
extern char serialBuffer[100];
extern bool wordEnd = false;
extern bool coFlag = false;
extern bool serialConfigured = false;

extern int throttleInput = 0;

void setupSerial();
void readline();
void handleSerialInterrupt();

#endif