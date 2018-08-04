#include <tofsensor.h>
#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>


#define I2C_CONT_RW                 0x00     // R/W
#define I2C_CONT_RW_DATA            0xC00040 // R/W  Binary: 010000000000000001000000
#define PHASE_OUT                   0x08     // R
#define CONTINUOUSandNUMFRAMES      0x27     // R/W
#define CONTINUOUSandNUMFRAMES_DATA 0x7FF7F  // R/W



TOFSensor::TOFSensor()
    : droneOne(0x5F)

// TOFSensor::TOFSensor()
//     : host()

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

void TOFSensor::setup()
{
    // Open I2C address
    if ((i2cFd = i2cOpen(1, 0x5F, 0)) < 0) {
        std::cout << strerror(errno) << std::endl;
    }
    else i2cConfigured = true;

    droneOne.calibrationSession_firstTimeBringUp();
    droneOne.initialize();

    
    
}

void TOFSensor::ConfigureMonoshot(){
    
    // char configI2CCONT[3];
    // configI2CCONT[0] = 0xC0;
    // configI2CCONT[1] = 0x00;
    // configI2CCONT[2] = 0x40;
    
    // char configCONT[3];
    // configCONT[0] = 0xFF;
    // configCONT[1] = 0xFF;
    // configCONT[2] = 0xFF;
    
    }


void TOFSensor::ReadPhaseOut(){

    // frameData frameData;
    // frameData.capture(host, droneOne, true);
    // frameData.report();
    //std::cout << host.readI2C(i2cFd, 0x08) << std::endl;

}

void TOFSensor::CalculateDistance(){

    //distance = (phaseOut/65536.0) * (299792458/20);
}


void TOFSensor::ReportDistance(){

// while(1){
//         int time = millis();                                                 
         //std::cout << distance << std::endl;
//             while(millis() - time <= 50);
// }
}
