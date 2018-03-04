#ifndef RANGING_H
#define RANGING_H

extern volatile int i2cFd;

int RANGING_SENSOR_COMMS_Init_CCI(int, int, int);
int RANGING_SENSOR_COMMS_Write_CCI(int i2c_slave_address, int, int pos, int data, int size);
int RANGING_SENSOR_COMMS_Fini_CCI();
void RANGING_SENSOR_COMMS_Get_Error_Text();

#endif