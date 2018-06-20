#ifndef TOFSENSOR_H
#define TOFSENSOR_H

class TOFSensor {
public:
    TOFSensor();
    ~TOFSensor();
private:
    int i2cFd = 0;
    bool i2cConfigured = false;

    void setup();
};

#endif