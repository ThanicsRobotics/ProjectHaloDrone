#include <tofsensor.h>
#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>
#include <OPT3101device.h>

#define I2C_CONT_RW         0x00     // R/W
#define I2C_CONT_RW_DATA    0xC00040 // R/W  Binary: 010000000000000001000000
#define PHASE_OUT           0x08     // R


TOFSensor::TOFSensor()
{
    OPT3101::device dev;
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
    
    
}

void TOFSensor::ConfigureMonoshot(){
    
    char calibRegister[3];
    calibRegister[0] = 0x40;
    calibRegister[1] = 0x00;
    calibRegister[2] = 0xC0;
    i2cWriteI2CBlockData(i2cFd, I2C_CONT_RW, calibRegister, 3);
    std::cout << calibRegister[0] << calibRegister[1] << calibRegister[2] << std::endl;
    

    char inputRegister[3];
    i2cReadI2CBlockData(i2cFd, I2C_CONT_RW, inputRegister, 3);
    std::cout << inputRegister[0] << inputRegister[1] << inputRegister[2] << std::endl;
    
    

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

// while(1){
//         int time = millis();                                                 
         std::cout << distance << std::endl;
//             while(millis() - time <= 50);
// }
}
