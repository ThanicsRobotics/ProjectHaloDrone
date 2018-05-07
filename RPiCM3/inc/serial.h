#ifndef SERIAL_H
#define SERIAL_H

#include <pthread.h>

class Serial {
    public:
        Serial();
        ~Serial();
        void setupSerial(char* port, int baud);
        void startSerialLoop();
        char *readLine();
        char readChar();
        int write(uint8_t* bytes, uint16_t len);

        bool serialConfigured;
        int throttleInput;
    private:
        pthread_t serialThread;
        int serialFd;
        void *serialLoop(void*);
};

#endif