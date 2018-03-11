#ifndef ULTRASONIC_H
#define ULTRASONIC_H

//I2C address of IO Expander
#define ADDR 0x22
#define INT_PIN 38

//Defining word representations for program readability
#define LOW 0
#define EDGE_FALLING 0
#define HIGH 1
#define EDGE_RISING 1

extern volatile int gpioI2cFd;

//Pulse timing variables
extern int start_time;
extern int pulse_time;
extern volatile bool pulseComplete;
extern unsigned int lastUltrasonicPulse;

//Gyro angle variables
extern volatile signed char gyroPitch;
extern volatile signed char gyroRoll;

void setupIOExpander();
void handleEcho();
void digitalIOWrite(int pin, int state);
int getUltrasonicData(int sensor, int iterations, unsigned int pulseDelay);
float radian(int degree);
int angleCorrection(int rawDistance);
void testSensor();

#endif