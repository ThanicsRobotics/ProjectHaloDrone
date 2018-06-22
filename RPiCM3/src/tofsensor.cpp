#include <tofsensor.h>

#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>

TOFSensor::TOFSensor()
{
    setup();
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
}