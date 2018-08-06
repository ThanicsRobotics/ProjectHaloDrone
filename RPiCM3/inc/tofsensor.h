#ifndef TOFSENSOR_H
#define TOFSENSOR_H

#include <OPT3101device.h>
#include <hostController.h>
#include <OPT3101frameData.h>

class TOFSensor {
public:
    TOFSensor();
    ~TOFSensor();
private:
    int i2cFd = 0;
    bool i2cConfigured = false;
    short phaseOut;
    int distance; //meters
    
    OPT3101::device dev;

    void setup();
    void ConfigureMonoshot();
    void ReadPhaseOut();
    void CalculateDistance();
    void ReportDistance();
    
};

#endif