#include <bits/stdc++.h>
#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#define ADDR 0x22
#define LOW 0
#define HIGH 1

using namespace std;

//CS0 is barometer, CS1 is STM32 flight controller
int SPI_CS = 0;

char baroData[];
char baroCoefficients[];

int pressure;
int temperature;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_p_gain = 3.8;                //Gain setting for the roll P-controller
float pid_i_gain = 0.01;               //Gain setting for the roll I-controller
float pid_d_gain = 20.0;               //Gain setting for the roll D-controller
int pid_max = 400;                     //Maximum output of the PID-controller (+/-)

float pid_error_temp;
float pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;

// Function to convert binary fractional to decimal
double binaryToDecimal(string binary, int len) {
    // Fetch the radix point
    size_t point = binary.find('.');
 
    // Update point if not found
    if (point == string::npos)
        point = len;
 
    double intDecimal = 0, fracDecimal = 0, twos = 1;
 
    // Convert integral part of binary to decimal
    // equivalent
    for (int i = point-1; i>=0; --i)
    {
        // Subtract '0' to convert character
        // into integer
        intDecimal += (binary[i] - '0') * twos;
        twos *= 2;
    }
 
    // Convert fractional part of binary to
    // decimal equivalent
    twos = 2;
    for (int i = point+1; i < len; ++i) {
        fracDecimal += (binary[i] - '0') / twos;
        twos *= 2.0;
    }
 
    // Add both integral and fractional part
    return intDecimal + fracDecimal;
}

void getPressureDataAndCoefficients() {
    int fd, result;
    unsigned char buffer[100];
    const char dataSetup[9] = {0x80, 0x00, 0x82, 0x00, 0x84, 0x00, 0x86, 0x00, 0x00};
    const char coefficientsSetup[17] = {0x88, 0x00, 0x8A, 0x00, 0x8C, 0x00, 0x8E, 0x00, 0x90, 0x00, 0x92, 0x00, 0x94, 0x00, 0x96, 0x00, 0x00};

    cout << "Initializing" << endl;

    // 500000 indicates bus speed.
    fd = wiringPiSPISetup(SPI_CS, 500000);

    cout << "Init result: " << fd << endl;

    // start conversions
    buffer[0] = 0x24;
    buffer[1] = 0x00;
    wiringPiSPIDataRW(SPI_CS, buffer, 2);

    sleep(3);

    // Get coefficients
    for(int i = 0; i < len(coefficientsSetup); i++) {
        buffer[i] = coefficientsSetup[i]
    }
    baroCoefficients = wiringPiSPIDataRW(SPI_CS, buffer, len(coefficientsSetup));
    sleep(3);

    // Get pressure and temp data
    for(int i = 0; i < len(dataSetup); i++) {
        buffer[i] = dataSetup[i]
    }
    baroData = wiringPiSPIDataRW(SPI_CS, buffer, len(dataSetup));
}

void calculatePressureAltitude() {
    getPressureDataAndCoefficients();
    pressure = ((baroData[0] << 8) | baroData[2]) >> 6;
    temperature = ((baroData[4] << 8) | baroData[6]) >> 6;

    //010001010001.101

    int a0 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
    //S I11 I10 I9 I8 I7 I6 I5 I4 I3 I2 I1 I0 . F2 F1 F0

    int b1 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
    //S I1 I0 . F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

    int b2 = (baroCoefficients[2] << 8) | baroCoefficients[4];              
    //S I0 . F13 F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

    int c12 = ((baroCoefficients[2] << 8) | baroCoefficients[4]) >> 2;      
    //S 0 . 000 000 000 F12 F11 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0

    
    /*string n = "110.101";
    cout << binaryToDecimal(n, n.length()) << "\n";
 
    n = "101.1101";
    cout << binaryToDecimal(n, n.length());*/
    
    float pressureComp = a0 + (b1 + c12 * temperature) * pressure + b2 * temperature;
    float pressureFinal = pressureComp * (65/1023) + 50;                                //Final pressure in kPa
}

void calculatePID() {
    pid_error_temp = gyro_input - pid_setpoint;
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_roll_d_error = pid_error_temp;
}

void setupIOExpander() {
    int fd = wiringPiI2CSetup(ADDR);

    //Configuration bytes (Inputs are 1's, Outputs 0's)
    //Port 0: 01010101
    wiringPiI2CWriteReg8(fd, 0x0C, 0x55)

    //Port 1: 01010101
    wiringPiI2CWriteReg8(fd, 0x0D, 0x55)

    //Port 2: 11000000
    wiringPiI2CWriteReg8(fd, 0x0E, 0xC0)

    //Initialization of IO Expander interrupts
    wiringPiISR(38, INT_EDGE_RISING, handleEcho);
}

void handleEcho() {

}

void getUltrasonicData() {

}

void angleCorrection() {

}

void calculateAbsoluteAltitude() {

    
}

void digitalWrite(int pin, int state) {
    int port;
    if (pin < 8) port = 0;
    else if (pin < 18 && pin > 9) port = 1;
    else port = 2;

    switch port {
        case 0:
            wiringPiI2CWriteReg8(fd, 0x04, state << pin);
            break;
        case 1:
            wiringPiI2CWriteReg8(fd, 0x04, state << (pin - 10));
            break;
        case 2:
            wiringPiI2CWriteReg8(fd, 0x04, state << (pin - 20));
            break;
    }
}

int main() {
    while(1) {
        //calculatePressureAltitude();
        calculateAbsoluteAltitude();
    }
}