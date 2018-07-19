#include <tofsensor.h>

#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>

#define I2C_CONT_RW         0x00     // R/W
#define I2C_CONT_RW_DATA    0xC00040 // R/W  Binary: 010000000000000001000000
#define PHASE_OUT           0x08     // R


TOFSensor::TOFSensor()
{
    setup();
    ConfigureMonoshot();
    ReadPhaseOut();
    CalculateDistance();
    ReportDistance();
}

TOFSensor::~TOFSensor()
{
    i2cClose(i2cFd);
}

void TOFSensor::ConfigureMonoshot(){

    i2cWriteByteData(i2cFd, I2C_CONT_RW, 0xC0); //Break 24 bit I2C_CONT_RW_DATA into 3 bytes for slave to recieve.
    i2cWriteByteData(i2cFd, I2C_CONT_RW, 0x00);
    i2cWriteByteData(i2cFd, I2C_CONT_RW, 0x40);
    std:cout << "Configured" << std::endl;

    }

void TOFSensor::setup()
{
    // Open I2C address
    if ((i2cFd = i2cOpen(1, 0x5F, 0)) < 0) {
        std::cout << strerror(errno) << std::endl;
    }
    else i2cConfigured = true;


}

void TOFSensor::ReadPhaseOut(){

    char outputRegister[3];
    i2cReadI2CBlockData(i2cFd, PHASE_OUT, outputRegister, 3);
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
            while(millis() - time <= 50);
}
}
