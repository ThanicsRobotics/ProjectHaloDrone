#include "pid.h"
#include "serial.h"

int pid_p_gain = 2;                    //Gain setting for the roll P-controller
int pid_i_gain = 0;                     //Gain setting for the roll I-controller
int pid_d_gain = 0;                   //Gain setting for the roll D-controller
int pid_max = 400;                      //Maximum output of the PID-controller (+/-)
int pid_error_temp;
int pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;

//Calculate throttle factor for altitude management through PID loop
void calculatePID() {
    if (throttleInput >= 1520 && throttleInput <= 1480) lastAltitude = altitude;

    pid_error_temp = altitude - lastAltitude + (throttleInput - 1500)/10;
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;
}