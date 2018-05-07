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

        bool serialConfigured;
        int throttleInput;
    private:
        void *serialLoop(void*);

        pthread_t serialThread;
        int serialFd;
};

#endif