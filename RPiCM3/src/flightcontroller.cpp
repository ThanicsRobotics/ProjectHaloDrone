#include <flightcontroller.h>
#include <radio.h>
#include <pid.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define SEL2 5
#define projectPath "./"

#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0xFF9F
#define STM32_ARM_CONF 0xFF0A
#define STM32_DISARM_TEST 0xFF8F
#define STM32_DISARM_CONF 0xFFFB
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

FlightController::FlightController()
{
    run = true;
    spiCS = 0;
}

void FlightController::setupSPI() {
    //Switch to flight controller, setup SPI @ 3MHz
    SPI_CS = 0;
    if ((spiFd = spiOpen(SPI_CS, 3000000, 0)) < 0) {
        std::cout << "SPI failed: " << strerror(errno) << "\n";
        exit(1);
    }
    else {
        std::cout << "Opening SPI. FD: " << spiFd << " ID: " << pthread_self() << "\n";
        spiConfigured = true;
    }
}

void FlightController::requestService(Service serviceType) {
    using namespace Service;
    switch (serviceType) {
        case ARM:
            armRequest = true;
            break;
        case DISARM:
            disarmRequest = true;
            break;
        case AUTH:
            authRequest = true;
            break;
        case RESET:
            this->reset();
            break;
        default:
            break;
    }
}

//Start thread
void FlightController::startFlight() {
    interface = std::thread(interfaceLoop);
}

//Stop threads
void FlightController::stopFlight() {
    run = false;
    interface.join();
}

void FlightController::disarm() {
    int data = 0;
    while ((data != STM32_DISARM_CONF) && run) {
        int disarmCode = STM32_DISARM_TEST;
        stm32_tx_buffer[0] = (disarmCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = disarmCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        
        delay(50);
    }
    armed = false;
}

void FlightController::arm() {
    int data = 0;
    while ((data != STM32_ARM_CONF) && run) {
        int armCode = STM32_ARM_TEST;
        if (motorTest) armCode = MOTOR_TEST;
        else if (noMotors) armCode = NO_MOTORS;
        stm32_tx_buffer[0] = (armCode >> 8) & 0xFF;
        stm32_tx_buffer[1] = armCode & 0xFF;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        delay(5);

        spiXfer(spiFd, stm32_tx_buffer, stm32_tx_buffer, 2);
        data = stm32_tx_buffer[0] << 8 | stm32_tx_buffer[1];
        //std::cout << "ARM Response: " << data << endl;
        spiWrite(spiFd, stm32_tx_buffer, 2);
        
        delay(50);
    }
    armed = true;
}

void FlightController::reset() {
    pinMode(SEL2, OUTPUT);
    digitalWrite(SEL2, LOW);
    system(("sudo openocd -f " + projectPath + "src/reset.cfg").c_str());
    pinMode(SEL2, INPUT);
}

//Making sure the STM32F446 is listening...
void FlightController::auth() {
    //Reset flight controller using OpenOCD
    reset();

    authenticated = false;
    char buffer[100];
    unsigned int authKey = 0;
    std::cout << "Authenticating...\n";
    int start = millis();
    while(authKey != AUTH_KEY) {
        //Write to Authentication register
        buffer[0] = 0x00;
        buffer[1] = 0x01;
        spiWrite(spiFd, buffer, 2);
        delay(5);

        //Get Auth Key and send it back
        spiXfer(spiFd, buffer, buffer, 2);
        authKey = buffer[0] << 8 | buffer[1];
        std::cout << "Key: " << authKey << "\n";
        spiWrite(spiFd, buffer, 2);
        
        delay(50);
        if (millis() - start > 8000) {
            exit(1);
        }
    }
    std::cout << "Authenticated\n";
    authenticated = true;
}

//Send modified throttle value to STM32
void FlightController::sendData() {
    short int throttle = newThrottle;
    stm32_tx_buffer[1] = (throttle - 1000) & 0xFF;
    stm32_tx_buffer[0] = ((throttle - 1000) >> 8) & 0xFF;
    
    //CLOCK SPEED TEST
    //unsigned long int clockspeed = buffer[1];
    //std::cout << " | Clock: " << clockspeed << endl;
}

// Packing message to send to Flight Controller over SPI
// Format:
// {0x00,0x00,...
//  ^Two 0's always lead start of message
// ...Pitch_H,Pitch_L,Roll_H,Roll_L,Yaw_H,Yaw_L,Throttle_H,Throttle_L}
//    ^PWM control values, high byte followed by low byte
char *FlightController::packMessage(fcMessage data) {
    char msg[11];
    msg[0] = 0;
    msg[1] = 0;
    int i;
    for (i = 2; i < 2+sizeof(info.pwm); i+=2) {
        uint16_t pwm = info.pwm[(i-2)/2];
        msg[i] = (pwm >> 8) & 0xFF;
        msg[i+1] = pwm & 0xFF;
    }
    msg[i] = '\0';
    return msg;
}

void FlightController::interfaceLoop() {
    if(!spiConfigured) this->setupSPI();
    if(!authenticated) this->auth();
    while(run) {
        if (armRequest) {
            //std::cout << "Arming..." << endl;
            this->arm();
            armRequest = false;
        }
        else if (disarmRequest) {
            this->disarm();
            disarmRequest = false;
        }
        else if (authRequest) {
            //std::cout << "Authenticating..." << endl;
            this->auth();
            authRequest = false;
        }
        else if (testGyro) {
            spiRead(spiFd, stm32_rx_buffer, 2);
            gyroPitch = (signed char)stm32_rx_buffer[0];
            gyroRoll = (signed char)stm32_rx_buffer[1];
        }
        
        

        //Use SPI to get gyro angles, send throttle
        spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, 2);
        FCReceivedData = (short)(stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1]);
        
    }
    if (armed) this->disarm();
    return NULL;
}

dronePosition FlightController::getDronePosition() {
    return flightPosition;
}

void FlightController::setHoverAltitude(uint8_t altitude) {
    setAltitude = altitude;
}

static float map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Calculate throttle factor for altitude management through PID loop
uint16_t FlightController::calculateThrottlePID(uint16_t altitudePWM) {
    //Increase or decrease set altitude proportional to stick position
    if (altitudePWM < 1000) altitudePWM = 1500;
    if (altitudePWM >= 1520) setAltitude += 0.2 * map(altitudePWM, 1520, 2000, 1, 20);
    else if (altitudePWM <= 1480) setAltitude -= 0.2 * map(altitudePWM, 1000, 1480, 20, 1);
    if (setAltitude < 0) setAltitude = 0;

    //Proportional error
    pid_error_temp = (int)setAltitude - altitude;

    //Integrating error over time
    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > pid_max)pid_i_mem = pid_max;
    else if(pid_i_mem < pid_max * -1)pid_i_mem = pid_max * -1;

    //Combining all controllers into one output
    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
    if(pid_output > pid_max)pid_output = pid_max;
    else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

    pid_last_d_error = pid_error_temp;

    //PID compensated throttle calculation
    uint16_t newThrottle = 1500 + pid_output;

    if (newThrottle > 2000) newThrottle = 2000;
    else if (newThrottle < 1000) newThrottle = 1000;
    return newThrottle;
}

FlightController::~FlightController() {
    if (armed) this->disarm();
    if (spiConfigured) spiClose(spiFd);
}