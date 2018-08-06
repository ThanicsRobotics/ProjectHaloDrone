#include <tofsensor.h>
#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>

TOFSensor::TOFSensor()
    : dev(0x5F)


{
    setup();
    ConfigureMonoshot();
    ReadPhaseOut();
    CalculateDistance();
    ReportDistance();
}

TOFSensor::~TOFSensor()
{
    
}

void TOFSensor::setup()
{
    dev.calibrationSession_firstTimeBringUp(); ///* Calls the method to bring up the device first time and calibrate. Calls OPT3101::calibrationSession_firstTimeBringUp
    dev.calibration->report(); ///* Calls report function for all calibration coefficients. Since not all coefficients are done in this example most of them are expected to be zero.
    printf("Waiting for user input to quit program...\n"); ///* Waits for user input before closing the console
}

void TOFSensor::ConfigureMonoshot(){
    // uint32_t configData = 0x400040;
    // andrew.writeI2C(i2cFd, configData);

    // }


void TOFSensor::ReadPhaseOut(){
    OPT3101::frameData& frameData = dev.calibration->phaseOffset.data;
    frameData.capture(dev.host, dev);
    frameData.report();
// letsgetit.capture(andrew, dev);
// letsgetit.report();

}

void TOFSensor::CalculateDistance(){

   // distance = (phaseOut/65536.0) * (299792458/20);
}


void TOFSensor::ReportDistance(){

// while(1){
//         int time = millis();                                                 
         //std::cout << distance << std::endl;
//             while(millis() - time <= 50);
// }
}