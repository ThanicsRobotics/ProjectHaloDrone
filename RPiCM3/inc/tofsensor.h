#ifndef TOFSENSOR_H
#define TOFSENSOR_H

#include <OPT3101device.h>

class TOFSensor {
public:
    TOFSensor();
    ~TOFSensor();
private:
    int i2cFd = 0;
    bool i2cConfigured = false;
    short phaseOut;
    int distance; //meters
<<<<<<< HEAD
=======
    OPT3101::device dev;
>>>>>>> 851bdb616b3aa9da468f5cade96b142250def9f1

    void setup();
    void ConfigureMonoshot();
    void ReadPhaseOut();
    void CalculateDistance();
    void ReportDistance();
    
};

#endif