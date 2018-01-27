//WiringPi Libraries
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <wiringPi.h>

//Standard Librarues
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <bits/stdc++.h>
#include <iostream>
#include <errno.h>

//I2C address of IO Expander
#define ADDR 0x22
#define INT_PIN 38

//Defining word representations for program readability
#define LOW 0
#define EDGE_FALLING 0
#define HIGH 1
#define EDGE_RISING 1

using namespace std;

//CS0 is barometer, CS1 is STM32 flight controller
int SPI_CS = 0;
int fd;

//Pressure Altitude variables
char baroData[9];
char baroCoefficients[17];

// int pressure;
// int temperature;

//Gyro angle variables
signed int gyroPitch;
signed int gyroRoll;

//Pulse timing variables
long int start_time;
long int pulse_time;
struct timespec gettime_now;
bool pulseComplete = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_p_gain = 3.8;                //Gain setting for the roll P-controller
float pid_i_gain = 0.01;               //Gain setting for the roll I-controller
float pid_d_gain = 20.0;               //Gain setting for the roll D-controller
int pid_max = 400;                     //Maximum output of the PID-controller (+/-)
float pid_error_temp;
float pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;

//Handles IO Expander interrupt (measures ultrasonic sensor echo pulse)
void handleEcho() {
    //Get current time
    clock_gettime(CLOCK_REALTIME, &gettime_now);
	start_time = gettime_now.tv_nsec;

    //Get time when pulse is HIGH
    while(digitalRead(INT_PIN) == HIGH) {
        clock_gettime(CLOCK_REALTIME, &gettime_now);
        //Stop if HIGH for 10ms (timeout)
	    if ((gettime_now.tv_nsec - start_time) > 10000000) {
            pulse_time = 0;
            break;
        }
    }

    //Pulse time is the time difference before and after HIGH pulse
    clock_gettime(CLOCK_REALTIME, &gettime_now);
	pulse_time = gettime_now.tv_nsec - start_time;
    pulseComplete = true;
}

//Utility function for setting individual pin on IO Expander
void digitalIOWrite(int pin, int state) {
    //Figure out port number based on pin number
    int port;
    if (pin < 8) port = 0;
    else if (pin < 18 && pin > 9) port = 1;
    else port = 2;

    //Change output depending on port and pin number
    switch (port) {
        case 0:
            wiringPiI2CWriteReg8(fd, 0x04, state << pin);
            break;
        case 1:
            wiringPiI2CWriteReg8(fd, 0x05, state << (pin - 10));
            break;
        case 2:
            wiringPiI2CWriteReg8(fd, 0x06, state << (pin - 20));
            break;
        default:
            break;
    }
}

//Configures inputs and outputs of IO Expander
void setupIOExpander() {
    fd = wiringPiI2CSetup(ADDR);

    //Configuration bytes (Inputs are 1's, Outputs 0's)
    //Port 0: 01010101
    wiringPiI2CWriteReg8(fd, 0x0C, 0x55);

    //Port 1: 01010101
    wiringPiI2CWriteReg8(fd, 0x0D, 0x55);

    //Port 2: 11000000
    wiringPiI2CWriteReg8(fd, 0x0E, 0xC0);

    //Initialization of IO Expander interrupts
    wiringPiISR(38, INT_EDGE_RISING, handleEcho);
}

//Gets distance value (in centimeters) from downward facing sensor
int getUltrasonicData(int sensor) {
    int pin;

    //Toggles between downward facing sensor 1 and 2
    switch (sensor) {
        case 1:
            pin = 17;
            break;
        case 2:
            pin = 15;
            break;
        default:
            break;
    }

    int totalDistance = 0;
    int invalids = 0;

    //Takes average of 3 distance measurements
    for(int i = 0; i < 3; i++) {
        //Ensuring TRIG pin is LOW
        digitalIOWrite(pin, LOW);
        delayMicroseconds(2);

        //Starting TRIG pulse
        digitalIOWrite(pin, HIGH);
        delayMicroseconds(10);
        digitalIOWrite(pin, LOW);

        //Wait until pulse is complete (when handleEcho is complete)
        while(pulseComplete == false);

        //Calculate distance based on speed of sound and travel time, and
        //factor out invalid results
        int distance = (pulse_time/1000) * 0.034 / 2;
        pulseComplete = false;
        if (distance <= 0 || distance > 400) invalids++;
        else totalDistance += distance;
        delay(3);
    }
    if ((3 - invalids) <= 0) return 0;
    else return totalDistance / (3 - invalids);
    //return distance;
}

//Corrects downward facing distance measurement when vehicle changes attitude
int angleCorrection(int rawDistance) {
    return sqrt(pow(rawDistance, 2) / (1 + pow(tan(gyroPitch),2) + pow(tan(gyroRoll),2)));
}

//Making sure the STM32F446 is listening...
void authFlightController() {
    unsigned char buffer[100];
    unsigned int authKey = 0;
    cout << "Authenticating..." << endl;
    while(authKey != 0x00F4) {
        //Write to Authentication register
        buffer[1] = 0x01;
        wiringPiSPIDataRW(SPI_CS, buffer, 2);

        //Get Auth Key and send it back
        wiringPiSPIDataRW(SPI_CS, buffer, 2);
        authKey = buffer[0] << 8 | buffer[1];
        cout << authKey << endl;
        wiringPiSPIDataRW(SPI_CS, buffer, 2);
        delay(10);
    }
    cout << "Authenticated" << endl;
}

//Request gyro angles from STM32F446 flight controller
void getGyroValues() {
    unsigned char buffer[100];
    
    //Gyro pitch and roll are stored in two incoming bytes
    wiringPiSPIDataRW(SPI_CS, buffer, 2);
    gyroPitch = (signed char)buffer[0];
    gyroRoll = (signed char)buffer[1];
}

//Using gyro angles and raw distance, calculate absolute altitude of vehicle
void calculateAbsoluteAltitude() {
    getGyroValues();
    cout << "Gyro Pitch: " << gyroPitch << " | "  << "Gyro Roll: " << gyroRoll << endl;
    int rawDistance = getUltrasonicData(1);
    //cout << "Raw Distance: " << rawDistance << endl;
    int altitude = angleCorrection(rawDistance);
    cout << "Altitude: " << altitude << endl;
}

// Function to convert binary fractional to decimal
// double binaryToDecimal(string binary, int len) {
//     // Fetch the radix point
//     size_t point = binary.find('.');
 
//     // Update point if not found
//     if (point == string::npos)
//         point = len;
 
//     double intDecimal = 0, fracDecimal = 0, twos = 1;
 
//     // Convert integral part of binary to decimal
//     // equivalent
//     for (int i = point-1; i>=0; --i)
//     {
//         // Subtract '0' to convert character
//         // into integer
//         intDecimal += (binary[i] - '0') * twos;
//         twos *= 2;
//     }
 
//     // Convert fractional part of binary to
//     // decimal equivalent
//     twos = 2;
//     for (int i = point+1; i < len; ++i) {
//         fracDecimal += (binary[i] - '0') / twos;
//         twos *= 2.0;
//     }
 
//     // Add both integral and fractional part
//     return intDecimal + fracDecimal;
// }

// void getPressureDataAndCoefficients() {
//     //Switch to barometer 
//     SPI_CS = 0;
//     wiringPiSPISetup(SPI_CS, 1000000);

//     int result;
//     unsigned char buffer[100];
//     const char dataSetup[9] = {0x80, 0x00, 0x82, 0x00, 0x84, 0x00, 0x86, 0x00, 0x00};
//     const char coefficientsSetup[17] = {0x88, 0x00, 0x8A, 0x00, 0x8C, 0x00, 0x8E, 0x00, 0x90, 0x00, 0x92, 0x00, 0x94, 0x00, 0x96, 0x00, 0x00};

//     // start conversions
//     buffer[0] = 0x24;
//     buffer[1] = 0x00;
//     wiringPiSPIDataRW(SPI_CS, buffer, 2);

//     sleep(3);

//     // Get coefficients
//     for(int i = 0; i < len(coefficientsSetup); i++) {
//         buffer[i] = coefficientsSetup[i]
//     }
//     baroCoefficients = wiringPiSPIDataRW(SPI_CS, buffer, len(coefficientsSetup));
//     sleep(3);

//     // Get pressure and temp data
//     for(int i = 0; i < len(dataSetup); i++) {
//         buffer[i] = dataSetup[i]
//     }
//     baroData = wiringPiSPIDataRW(SPI_CS, buffer, len(dataSetup));
// }

// void calculatePressureAltitude() {
//     getPressureDataAndCoefficients();
//     pressure = ((baroData[0] << 8) | baroData[2]) >> 6;
//     temperature = ((baroData[4] << 8) | baroData[6]) >> 6;

//     //010001010001.101

//     int a0 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
//     //S I11 I10 I9 I8 I7 I6 I5 I4 I3 I2 I1 I0 . F2 F1 F0

//     int b1 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
//     //S I1 I0 . F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

//     int b2 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
//     //S I0 . F13 F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

//     int c12 = ((baroCoefficients[2] << 8) | baroCoefficients[4]) >> 2;      
//     //S 0 . 000 000 000 F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

    
//     /*string n = "110.101";
//     cout << binaryToDecimal(n, n.length()) << "\n";
 
//     n = "101.1101";
//     cout << binaryToDecimal(n, n.length());*/
    
//     float pressureComp = a0 + (b1 + c12 * temperature) * pressure + b2 * temperature;
//     float pressureFinal = pressureComp * (65/1023) + 50;                                //Final pressure in kPa
// }

//Calculate throttle factor for altitude management through PID loop
void calculatePID() {
    pid_error_temp = gyro_input - pid_setpoint;
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;
}

//Main Program loop
int main() {
    //Setup function calls
    wiringPiSetup();
    setupIOExpander();

    //Switch to flight controller, setup SPI @ 1.5MHz
    SPI_CS = 1;
    wiringPiSPISetup(SPI_CS, 1500000);
    authFlightController();


    while(1) {
        //calculatePressureAltitude();
        //cout << "Count: " << count << endl;
        calculateAbsoluteAltitude();
        delay(100);
    }
}