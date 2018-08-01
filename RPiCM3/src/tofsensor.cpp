#include <tofsensor.h>
#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>


#define I2C_CONT_RW                 0     // R/W
#define I2C_CONT_RW_DATA            12582976 // R/W  Binary: 010000000000000001000000
#define PHASE_OUT                   8     // R
#define CONTINUOUSandNUMFRAMES      39     // R/W
#define CONTINUOUSandNUMFRAMES_DATA 524159  // R/W
#define OPT3101_USE_STDIOLIB 
#define OPT3101_USE_SERIALLIB 
#define OPT3101_USE_STDIOLIB
 

TOFSensor::TOFSensor()
    : dev(0x5F)
{
    OPT3101::device dev; ///* Declared variable dev of class OPT3101::device 
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

void TOFSensor::setup()
{
    OPT3101::device dev; ///* Declared variable dev of class OPT3101::device 
    dev.calibrationSession_firstTimeBringUp(); ///* Calls the method to bring up the device first time and calibrate. Calls OPT3101::calibrationSession_firstTimeBringUp
    dev.calibration->report(); ///* Calls report function for all calibration coefficients. Since not all coefficients are done in this example most of them are expected to be zero.
    printf("Waiting for user input to quit program...\n"); ///* Waits for user input before closing the console 
    host.pause();
    dev.initialize();
    
    
}

void TOFSensor::ConfigureMonoshot(){
    extern hostController host;
    host.writeI2C(uint8_t (0), uint32_t (12582976));
    host.writeI2C(uint8_t (39), uint32_t (524159));


    }

void TOFSensor::ReadPhaseOut(){
    extern hostController host;
    phaseOut = host.readI2C(uint8_t (8));
}

void TOFSensor::CalculateDistance(){

    distance = (phaseOut/65536.0) * (299792458/20);
}


void TOFSensor::ReportDistance(){

// while(1){
//         int time = millis();                                                 
         std::cout << distance << std::endl;
//             while(millis() - time <= 50);
// }
}

    // // Open I2C address
    // if ((i2cFd = i2cOpen(1, 0x5F, 0)) < 0) {
    //     std::cout << strerror(errno) << std::endl;
    //  }
    // else i2cConfigured = true;