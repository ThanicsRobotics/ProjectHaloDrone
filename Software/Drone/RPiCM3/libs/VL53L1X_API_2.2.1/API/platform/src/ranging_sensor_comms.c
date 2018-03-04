#include "ranging_sensor_comms.h"
#include <pigpio.h>
#include <iostream>
#include <stdio.h>

volatile int i2cFd;

int RANGING_SENSOR_COMMS_Init_CCI(int, int, int) {
    if ((i2cFd = i2cOpen(1, ADDR, 0)) < 0) {
        return -1;
    }
    std::cout << "Opening I2C. FD: " << i2cFd << " ID: " << pthread_self() << std::endl;
}

int RANGING_SENSOR_COMMS_Write_CCI(int i2c_slave_address, int none, int reg, int data, int size) {
    if (i2cWriteBlockData(i2cFd, reg, data, size)) < 0 {

    }
    return 0;
}

int RANGING_SENSOR_COMMS_Fini_CCI() {

}

void RANGING_SENSOR_COMMS_Get_Error_Text() {

}
