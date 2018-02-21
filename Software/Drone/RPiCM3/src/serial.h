#ifndef SERIAL_H
#define SERIAL_H

//Serial UART port file descriptor
extern volatile int serialFd;

extern int charCount;
extern char serialBuffer[100];
extern bool wordEnd;
extern bool coFlag;
extern volatile bool serialConfigured;

extern volatile int throttleInput;

void setupSerial();
void readChar();
void readLine();

#endif