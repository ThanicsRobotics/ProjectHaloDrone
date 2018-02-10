#ifndef SERIAL_H
#define SERIAL_H

//Serial UART port file descriptor
int serialFd;

int charCount = 0;
char serialBuffer[100];
bool wordEnd = false;
bool coFlag = false;
bool serialConfigured = false;

void setupSerial();
void readline();
void handleSerialInterrupt();

#endif