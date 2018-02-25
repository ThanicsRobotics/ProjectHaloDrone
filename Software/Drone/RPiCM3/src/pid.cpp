#include "pid.h"
#include "serial.h"

int pid_p_gain = 20;                    //Gain setting for the roll P-controller
int pid_i_gain = 0;                     //Gain setting for the roll I-controller
int pid_d_gain = 0;                   //Gain setting for the roll D-controller
int pid_max = 500;                      //Maximum output of the PID-controller (+/-)
int pid_error_temp;
int pid_i_mem, pid_setpoint, pid_last_d_error;
volatile int pid_output;

//Target Altitude in centimeters
float setAltitude = 50.0;

volatile short int newThrottle = 0;

float map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Calculate throttle factor for altitude management through PID loop
void calculatePID() {
    //Increase or decrease set altitude proportional to stick position
    if (throttleInput < 1000) throttleInput = 1500;
    if (throttleInput >= 1520) setAltitude += 0.01 * map(throttleInput, 1520, 2000, 1, 5);
    else if (throttleInput <= 1480) setAltitude -= 0.01 * map(throttleInput, 1000, 1480, 1, 5);

    pid_error_temp = (int)setAltitude - altitude;
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;

    //PID compensated throttle calculation
    newThrottle = 1500 + pid_output;

    if (newThrottle > 1900) newThrottle = 1900;
    else if (newThrottle < 1000) newThrottle = 1000;
}