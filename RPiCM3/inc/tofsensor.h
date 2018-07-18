#ifndef TOFSENSOR_H
#define TOFSENSOR_H

class TOFSensor {
public:
    TOFSensor();
    ~TOFSensor();
private:
    int i2cFd = 0;
    bool i2cConfigured = false;
    short phaseOut;
    int distance; //meters


    void setup();
    void ReadPhaseOut();
    void CalculateDistance();
    void ReportDistance();
    
};

#endif