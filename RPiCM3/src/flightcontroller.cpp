/// @file flightcontroller.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Implementation for the FlightController class, controls
/// the interfacing between the Raspberry Pi CM3 and the STM32F446.

#include <flightcontroller.h>
#include <types.h>

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <bitset>

#define SEL2 5
#define projectPath std::string("./")

#define GYRO_CAL 0x04
#define AUTH_KEY 0xF9
#define STM32_ARM_TEST 0xFF9F
#define STM32_ARM_CONF 0xFF0A
#define STM32_DISARM_TEST 0xFF8F
#define STM32_DISARM_CONF 0xFFFB
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

#define PITCH_COEFF 0x01
#define ROLL_COEFF 0x02

/// @brief Class constrcutor, initializes private variables.
FlightController::FlightController(bool* shutdown)
    : shutdownIndicator(shutdown)
{
    std::fill(std::begin(currentMessage.pwm), std::end(currentMessage.pwm), 0);
}

FlightController::~FlightController()
{
    printf("FC: Closing\n");
    stopFlight();
    stm32Resetting = true;
    reset();
    closeSPI();
}

/// @brief Opens the SPI port and sets the spiConfigured flag to true.
void FlightController::setupSPI() {
    //Switch to flight controller, setup SPI @ 3MHz
    spiCS = 0;
    if ((spiFd = spiOpen(spiCS, 3000000, 0)) < 0) {
        std::cout << "SPI failed: " << strerror(errno) << "\n";
        exit(1);
    }
    else {
        std::cout << "Opening SPI. FD: " << spiFd << " ID: " << interface.get_id() << "\n";
        spiConfigured = true;
    }
}

/// @brief Closes the SPI port if it has been opened, and disarms
/// the motors first if not already.
void FlightController::closeSPI() {
    if (armed) disarm();
    if(spiConfigured) {
        spiClose(spiFd);
        spiConfigured = false;
    }
}

/// @brief Requests the FlightController class to do a Service.
/// @param serviceType A member of the Service enum
/// -> ARM: Flags the armRequest to true, to arm the drone in the next loop
/// -> DISARM: Flags disarmRequest to true, to disarm the drone in the next loop
/// -> AUTH: Flags authRequest to true, to authenticate the presence of the STM32F446 on next loop
/// -> RESET: Immediately resets the STM32F446, should be used outside of interfaceLoop
void FlightController::requestService(Service serviceType) {
    switch (serviceType) {
        case Service::ARM:
            armRequest = true;
            break;
        case Service::DISARM:
            disarmRequest = true;
            break;
        case Service::AUTH:
            authRequest = true;
            break;
        case Service::RESET:
            this->reset();
            break;
        default:
            break;
    }
}

/// @brief Requests the interfaceLoop to send data to the STM32F446.
/// @param data A data message structured in a fcMessage struct.
void FlightController::requestSend(fcMessage data) {
    currentMessage = data;
    //sendRequest = true;
}

/// @brief Starts a new thread executing the interfaceLoop, and
/// conducts pre-flight checks.
void FlightController::startFlight() {
    interface = std::thread([this]{ interfaceLoop(); });

    // Waiting for gyro on STM32 to calibrate
    while(!spiConfigured || !authenticated) {
        delay(50);
        if(*shutdownIndicator) {
            return;
        }
    }
    //delay(200);
    std::cout << "Waiting for gyro calibration..." << std::endl;
    int start = millis();
    int repeat = 1;
    while (fcReceivedData != GYRO_CAL) {
        // if (millis() - start > 10000) {
        //     std::cout << "Gyro not responding, press CNTL-C to shut down...\n";
        //     while (*shutdownIndicator);
        //     std::cout << "Shutting down\n";
        //     stopFlight();
        //     return;
        // }
        std::cout << "Received: " << fcReceivedData << std::endl;
        if(*shutdownIndicator) {
            std::cout << "Shutting Down\n";
            run = false;
            return;
        }

        // else if (repeat > 1) {
        //     std::cout << "Gyro still not responding, press CNTL-C to shut down...\n";
        //     while(!shuttingDown) delay(10);
        //     exit(1);
        // }
        delay(50);
    }
    std::cout << "Calibration complete\n";

    // Manual arming process through SSH
    std::cout << "Type 'ARM' to arm the quadcopter: ";
    std::string input = "";
    std::getline(std::cin, input);
    if (input == "ARM") {
        requestService(FlightController::Service::ARM);
    }
}

/// @brief Stops interfaceLoop thread.
void FlightController::stopFlight() {
    run = false;
    interface.join();
}

/// @brief Sends disarm command to STM32, which disables the motors.
void FlightController::disarm() {
    std::cout << "Disarming...\n";
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

/// @brief Sends arm command to STM32, which TURNS ON MOTORS.
void FlightController::arm() {
    std::cout << "Arming...\n";
    uint16_t data = 0;
    while ((data != STM32_ARM_CONF) && run) {
        uint16_t armCode = STM32_ARM_TEST;
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

/// @brief Switching Demux to STM32F446 programming pins, and sends reset command.
void FlightController::reset() {
    if (stm32Resetting) {
        pinMode(SEL2, OUTPUT);
        digitalWrite(SEL2, LOW);
        system(("sudo openocd -f " + projectPath + "src/reset.cfg").c_str());
        pinMode(SEL2, INPUT);
    }
}

/// @brief Makes sure the STM32F446 is active and listening.
void FlightController::auth() {
    //Reset flight controller using OpenOCD
    reset();

    authenticated = false;
    char buffer[2];
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
        // if (millis() - start > 8000) {
        //     exit(1);
        // }
        if(*shutdownIndicator) {
            std::cout << "Shutting Down\n";
            run = false;
            return;
        }
    }
    std::cout << "Authenticated\n";
    authenticated = true;
}

/// @brief Get current PWM inputs from radio and load them into currentMessage.
void FlightController::setPWMInputs(const channels& rcChannels) {
    currentMessage.pwm[0] = rcChannels.pitchPWM; 
    currentMessage.pwm[1] = rcChannels.rollPWM;
    currentMessage.pwm[2] = rcChannels.yawPWM;
    currentMessage.pwm[3] = rcChannels.throttlePWM;
}

void FlightController::getPWMInputs(channels& rcChannels) {
    rcChannels.pitchPWM = currentMessage.pwm[0];
    rcChannels.rollPWM = currentMessage.pwm[1];
    rcChannels.yawPWM = currentMessage.pwm[2];
    rcChannels.throttlePWM = currentMessage.pwm[3];
}

/// @brief Breaks up the message packet into 16-bit packets and 
/// sends them to STM32F446
void FlightController::sendMessage() {
    std::array<uint8_t, MSG_LEN> msg;
    packMessage(msg);
    std::copy(msg.begin(), msg.end(), stm32_tx_buffer);
    // for (int i = 0; i < buffer.len; i++) {
    //     std::bitset<8> x(buffer.buf[i]);
    //     std::cout << x << " | ";
    //     printf("%p\n", buffer.buf);
    // }

    //printf("test\n");
    // for (int i = 0; i < msg.size(); i += 2) {
    //     stm32_tx_buffer[i] = msg[i];
    //     stm32_tx_buffer[i+1] = msg[i+1];
    //     //printf("0: %x, 1: %x\n", msg[i], msg[i+1]);
        
    // }
    
    //for (const char& i : stm32_tx_buffer) std::cout << (int)i << '\n';
    //delay(10);
    // int timer1, timer2;
    // timer1 = micros();
    spiXfer(spiFd, stm32_tx_buffer, stm32_rx_buffer, MSG_LEN);
    // timer2 = micros();
    // std::cout << timer2 - timer1 << std::endl;
    // delay(10);
}

/// @brief Packing message to send to Flight Controller over SPI.
/// Format:
/// {0xFF,0xFE,...
///  ^Header of message
/// ...Pitch_H,Pitch_L,Roll_H,Roll_L,Yaw_H,Yaw_L,Throttle_H,Throttle_L}
///    ^PWM control values, high byte followed by low byte
void FlightController::packMessage(std::array<uint8_t, MSG_LEN>& msg) {
    //const uint8_t msgLen = 11;
    //static uint8_t msg[msgLen];
    //msg[0] = 0xFF;
    //msg[1] = 0xFE;
    //int i;
    uint16_t* pwmInput = currentMessage.pwm;
    for (int i = 0; i < MSG_LEN; i+=4) {
        uint16_t pwm = *pwmInput;
        
        msg[i] = 0xA0 + i;
        msg[i+1] = (pwm >> 8) & 0xFF;
        msg[i+2] = 0xA1 + i;
        msg[i+3] = pwm & 0xFF;
        
        pwmInput++;
    }
    //msg[i] = 0xFD;
    //msg[i+1] = 0xFC;
    //spiBuffer buf;
    //buf.len = msgLen;
    //buf.buf = (std::shared_ptr<uint8_t[]>)msg;
    //buf.buf = (uint8_t*)msg;
    //printf("%p\n", buf.buf);
    //return buf;
}

/// @brief Loop being executed by interface thread.
/// Sends messages to STM32F446 based on requests,
/// and decodes incoming messages from STM32F446
void FlightController::interfaceLoop() {
    static int timer = micros();

    if(!spiConfigured) setupSPI();
    if(!authenticated) auth();
    while(run) {
        if (armRequest) {
            //std::cout << "Arming..." << endl;
            arm();
            armRequest = false;
        }
        else if (disarmRequest) {
            disarm();
            disarmRequest = false;
        }
        // else if (sendRequest) {
        //     sendMessage();
        //     sendRequest = false;
        // }
        else if (authRequest) {
            //std::cout << "Authenticating..." << endl;
            auth();
            authRequest = false;
        }
        else if (testGyro) {
            spiRead(spiFd, stm32_rx_buffer, 2);
            gyroPitch = (signed char)stm32_rx_buffer[0];
            gyroRoll = (signed char)stm32_rx_buffer[1];
        }
        sendMessage();
        // if (armed) {
        //     std::cout << "spitimer: " << micros() - timer << "\n";
        //     timer = micros();
        // }
        
        //printf("test3\n");
        fcReceivedData = stm32_rx_buffer[0] << 8 | stm32_rx_buffer[1];
        switch (stm32_rx_buffer[0]) {
            case PITCH_COEFF:
                gyroPitch = stm32_rx_buffer[1];
                break;
            case ROLL_COEFF:
                gyroRoll = stm32_rx_buffer[1];
                break;
            default:
                break;
        }
    }
    if (armed) disarm();
}

/// @brief Returns the drone's 3-axis angular position.
/// @return dronePosition struct.
FlightController::dronePosition FlightController::getDronePosition() {
    return flightPosition;
}

/// @brief Sets the target altitude for takeoff hover.
/// @param hoverAltitude Hover altitude in centimeters (max 255 cm)
void FlightController::setHoverAltitude(uint8_t hoverAltitude) {
    setAltitude = hoverAltitude;
}

float map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// @brief Calculate new throttle based on a altitude-driven PID loop.
/// @param altitudePWM PWM Input from controller.
/// @param altitude Current altitude.
/// @return New throttle value (1000-2000).
uint16_t FlightController::calculateThrottlePID(uint16_t altitudePWM, float altitude) {
    //Increase or decrease set altitude proportional to stick position
    if (altitudePWM < 1000) altitudePWM = 1500;
    if (altitudePWM >= 1520) setAltitude += 0.2 * map(altitudePWM, 1520, 2000, 1, 20);
    else if (altitudePWM <= 1480) setAltitude -= 0.2 * map(altitudePWM, 1000, 1480, 20, 1);
    if (setAltitude < 0) setAltitude = 0;

    //Proportional error
    pid_error_temp = (int)(setAltitude - altitude);

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

