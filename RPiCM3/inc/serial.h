#ifndef SERIAL_H
#define SERIAL_H

#include <pthread.h>
#include <inttypes.h>

class Serial {
    public:
        Serial();
        ~Serial();
        void setupSerial(char* port, int baud);
        void startSerialLoop();
        char *readLine();
        char readChar();
        int write(uint8_t* bytes, uint16_t len);

    private:
        pthread_t serialThread;
        bool serialConfigured;
        int throttleInput;
        int serialFd;
        void *serialLoop(void*);
};

#endif