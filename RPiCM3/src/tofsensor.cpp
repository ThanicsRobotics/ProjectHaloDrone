#include <tofsensor.h>
#include <pigpio.h>

TOFSensor::TOFSensor()
{
    setup();
}

TOFSensor::~TOFSensor()
{

}

TOFSensor::setup()
{
    // Open I2C address
    if ((i2cFd = i2cOpen(1, 0x5F, 0)) < 0) {
        printf("%s", strerror(errno));
    }
    else i2cConfigured = true;
}