#ifndef SERIAL_H
#define SERIAL_H

#include <pthread.h>
#include <inttypes.h>
#include <memory>

class Serial {
    public:
        Serial();
        ~Serial();
        void setupSerial(char* port, int baud);
        void closeSerial();
        void startSerialLoop();
        char *readLine();
        char readChar();
        //int write(std::shared_ptr<uint8_t[]> bytes, uint16_t len);
        int write(uint8_t* bytes, uint16_t len);
        bool isSerialConfigured() { return serialConfigured; }

    private:
        //pthread_t serialThread;
        bool serialConfigured;
        int throttleInput;
        int serialFd;
};

#endif