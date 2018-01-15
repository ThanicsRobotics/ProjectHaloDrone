#include <bits/stdc++.h>
#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>

using namespace std;

// channel is the wiringPi name for the chip select (or chip enable) pin.
// Set this to 0 or 1, depending on how it's connected.
static const int CHANNEL = 0;

char baroData[];
char baroCoefficients[];
 
// Function to convert binary fractional to
// decimal
double binaryToDecimal(string binary, int len)
{
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
    for (int i = point+1; i < len; ++i)
    {
        fracDecimal += (binary[i] - '0') / twos;
        twos *= 2.0;
    }
 
    // Add both integral and fractional part
    return intDecimal + fracDecimal;
}

void getDataAndCoefficients() {
    int fd, result;
    unsigned char buffer[100];
    char dataSetup[9] = {0x80, 0x00, 0x82, 0x00, 0x84, 0x00, 0x86, 0x00, 0x00};
    char coefficientsSetup[17] = {0x88, 0x00, 0x8A, 0x00, 0x8C, 0x00, 0x8E, 0x00, 0x90, 0x00, 0x92, 0x00, 0x94, 0x00, 0x96, 0x00, 0x00};

    cout << "Initializing" << endl;

    // Configure the interface.
    // CHANNEL insicates chip select,
    // 500000 indicates bus speed.
    fd = wiringPiSPISetup(CHANNEL, 500000);

    cout << "Init result: " << fd << endl;

    // start conversions
    buffer[0] = 0x24;
    buffer[1] = 0x00;
    wiringPiSPIDataRW(CHANNEL, buffer, 2);

    sleep(3);

    // Get coefficients
    for(int i = 0; i < len(coefficientsSetup); i++) {
        buffer[i] = coefficientsSetup[i]
    }
    baroCoefficients = wiringPiSPIDataRW(CHANNEL, buffer, len(coefficientsSetup));
    sleep(3);

    // Get pressure and temp data
    for(int i = 0; i < len(dataSetup); i++) {
        buffer[i] = dataSetup[i]
    }
    baroData = wiringPiSPIDataRW(CHANNEL, buffer, len(dataSetup));
}   
 
int main()
{
    getDataAndCoefficients();
    
    


    string n = "110.101";
    cout << binaryToDecimal(n, n.length()) << "\n";
 
    n = "101.1101";
    cout << binaryToDecimal(n, n.length());
 
    return 0;
}