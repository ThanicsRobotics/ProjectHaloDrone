#ifndef SERIAL_H
#define SERIAL_H

//Serial UART port file descriptor
extern int serialFd;

extern int charCount;
extern char serialBuffer[100];
extern bool wordEnd;
extern bool coFlag;
extern bool serialConfigured;

extern int throttleInput;

void setupSerial();
void readChar();
void readLine();

#endif