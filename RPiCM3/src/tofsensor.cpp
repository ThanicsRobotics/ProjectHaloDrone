#include <tofsensor.h>

#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>

TOFSensor::TOFSensor()
{
    setup();
    ReadPhaseOut();
    CalculateDistance();
}

TOFSensor::~TOFSensor()
{

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

    char outputRegister[2];
    i2cReadBlockData(fd, 0x08, outputRegister);
    phaseOut = (float)(short)((outputRegister[1] << 8) |  (outputRegister[0]));
}

void TOFSensor::CalculateDistance(){

    int distance; //meters
    int speedOfLight = 299792458 //exact in meters per second
    int twoFMod = 2 * 10 //MHz - modulation frequency

    distance = (phaseOut/65536.0) * (speedOfLight/twoFMod);
}

void TOFSensor::ReportDistance(){

while(1){
        int time = millis();                                                 
        std::cout << "Distance: " <<  distance << " meters until impact!" << std::endl;
            while(millis() - time <= 4);
}

