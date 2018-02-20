#ifndef PID_H
#define PID_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int pid_p_gain;                    //Gain setting for the roll P-controller
extern int pid_i_gain;                     //Gain setting for the roll I-controller
extern int pid_d_gain;                   //Gain setting for the roll D-controller
extern int pid_max;                      //Maximum output of the PID-controller (+/-)
extern int pid_error_temp;
extern int pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;

extern int lastAltitude;
extern int altitude;

float map(int x, int in_min, int in_max, int out_min, int out_max);
void calculatePID();

#endif