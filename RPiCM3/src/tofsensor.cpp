#include <tofsensor.h>

#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>

TOFSensor::TOFSensor()
{
    setup();
    ReadPhaseOut();
    CalculateDistance();
    ReportDistance();
}

TOFSensor::~TOFSensor()
{
    i2cClose(i2cFd);
}

void TOFSensor::setup()
{
    // Open I2C address
    if ((i2cFd = i2cOpen(1, 0x5F, 0)) < 0) {
        std::cout << strerror(errno) << std::endl;
    }
    else i2cConfigured = true;

    //Configurations
    //NONE?


}

void TOFSensor::ReadPhaseOut(){

    char outputRegister[3];
    i2cReadI2CBlockData(i2cFd, 0x08, outputRegister, 3);
    std::cout << outputRegister[0] << " " << outputRegister[1] << " " << outputRegister[2] << std::endl;
    phaseOut = (short)((outputRegister[1] << 8) |  (outputRegister[0]));
}

void TOFSensor::CalculateDistance(){

    distance = (phaseOut/65536.0) * (299792458/20);
}

void TOFSensor::ReportDistance(){

while(1){
        int time = millis();                                                 
        std::cout << distance << std::endl;
            while(millis() - time <= 4);
}
}
