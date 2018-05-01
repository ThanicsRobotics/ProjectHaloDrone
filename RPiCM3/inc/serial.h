#ifndef SERIAL_H
#define SERIAL_H

extern volatile int throttleInput;

class Serial {
    public:
        Serial();
        void setupSerial(char* port, int baud);
        char *readLine();
        void *serialLoop(void*);
    private:
        volatile int serialFd;
};

#endif