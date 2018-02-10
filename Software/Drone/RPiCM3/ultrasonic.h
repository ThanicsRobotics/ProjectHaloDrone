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

//Pulse timing variables
int start_time = 0;
int pulse_time = 0;
bool pulseComplete = false;
unsigned int lastUltrasonicPulse = 0;

void setupIOExpander();
void handleEcho();
void digitalIOWrite(int pin, int state);
int getUltrasonicData(int sensor, int iterations, unsigned int pulseDelay);
float radian(int degree);
int angleCorrection(int rawDistance);
void testSensor();

#endif