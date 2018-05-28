#include <helper.h>

bool shuttingDown = false;
//bool doneShuttingDown = false;

/// @brief Displays message showing how to type options in command line.
/// @param name Name of program, i.e. First string of argv[].
void showUsage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)>\n"
        << "**NOTE: Must be run with root privileges (sudo)\n\n"
        << "Options:\n"
        << "\t-h,--help\t\t\tShow this help message\n"
        << "\t-c,--controller-enabled \tRun program to connect with controller\n"
        << "\t-nc,--no-controller \t\tRun program without connecting to controller\n"
        << "\t-c,--camera [camera (ex. /dev/video0)] \tStreaming camera\n"
        << "\t-r,--receiver [IP Address] \tStreaming receiver IP Address\n"
        << "\t-s,--stream \t\t\tEnable streaming\n\n";
}

/// @brief Filtering command line options.
/// @param _argc Pass in argc from main() here.
/// @param _argv Pass in argv from main() here.
void filterCommandLineOptions(int _argc, char *_argv[], FlightController& fc) {
    if (_argc > 1) {
        for (int i = 1; i < _argc; i++) {
            // if (std::string (_argv[i]) == "-c" || std::string (_argv[i]) == "--controller-enabled") 
            //     controllerConnected = true;
            // else if (std::string (_argv[i]) == "-nc" || std::string (_argv[i]) == "--no-controller") 
            //     controllerConnected = false;
            if (std::string (_argv[i]) == "-tg" || std::string (_argv[i]) == "--test-gyro")
                fc.setTestGyro(true);
            if (std::string (_argv[i]) == "-mt" || std::string (_argv[i]) == "--motor-test") {
                std::cout << "\n\t\t!!!! TESTING MOTORS BEFORE ARM !!!!\n\n";
                fc.setMotorTest(true);
            }
            if (std::string (_argv[i]) == "-nm" || std::string (_argv[i]) == "--no-motors")
                fc.setNoMotors(true);
            if (std::string (_argv[i]) == "-c" || std::string (_argv[i]) == "--camera") {
                //camera = std::string (_argv[i+1]);
            }
            if (std::string (_argv[i]) == "-r" || std::string (_argv[i]) == "--receiver") {
                //receiver = std::string (_argv[i+1]);
            }
            if (std::string (_argv[i]) == "-h" || std::string (_argv[i]) == "--help") {
                showUsage (_argv[0]);
                exit(0);
            }
            if (std::string (_argv[i]) == "-sd" || std::string (_argv[i]) == "--stm-debug") {
                fc.setSTM32Resetting(false);
            }
        }
    }
    // If no options are typed, show how to type them.
    else {
        showUsage (_argv[0]);
        exit(0);
    }
}

/// @brief Terminal signal handler (for ending program via terminal).
void signal_callback_handler(int signum) {
    if (!shuttingDown) shutdown();
    // while (!doneShuttingDown);
    // std::cout << "\nProgram End\n";
    // delay(1000);
	// exit(0);
}

/// @brief Main program loop after pre-flight checks.
void mainLoop(int _argc, char *_argv[]) {

    Radio<Serial> radio;
    Barometer baro;
    FlightController fc(&shuttingDown);

    filterCommandLineOptions(_argc, _argv, fc);

    // Setup Flight Controller and start SPI comms thread.
    fc.setupSPI();
    fc.startFlight();

    std::cout << "Waiting for configuration...\n";
    // Waits until Flight Controller is ready, after SPI port is opened,
    // and STM32F446 is authenticated.
    while(!fc.isSPIConfigured() || !fc.isAuthenticated()) {
        if (shuttingDown) break;
        delay(10);
    }
    std::cout << "Starting main loop\n";

    // Checks if program is running in gyro testing mode.
    if (fc.isTestGyroActive()) {
        while(fc.isRunning()) {
            std::cout << "Pitch: " << fc.getDronePosition().pitch 
                << "\t| Roll: " << fc.getDronePosition().roll << "\n";
            delay(20);
        }
    }

    int heartbeatTimer, baroTimer, radioTimer = millis();
    int loopTimer = micros();
    float currentAltitude = 0;
    radio.setupSerial("/dev/serial0", 115200);
    
    printf("Calibrating barometer...\n");
    baro.setup();
    //radio.startReceiveLoop();
    while(fc.isRunning() && !shuttingDown) {
        //Every 260ms, get pressure altitude, if barometer is calibrated
        if (baro.isCalibrated() && (millis() - baroTimer > BARO_DELAY)) {
            currentAltitude = baro.getPressureAltitude();
            std::cout << "BARO: Altitude: " << currentAltitude << "m\n";
            baro.takeReading();
            baroTimer = millis();
        }

        radio.customReceiveByte(radio.readChar(), fc);
        channels pwmInputs;
        fc.getPWMInputs(pwmInputs);
        std::cout << "Pitch: " << pwmInputs.pitchPWM
            << "\nRoll: " << pwmInputs.rollPWM
            << "\nYaw: " << pwmInputs.yawPWM
            << "\nthrottle: " << pwmInputs.throttlePWM
            << "\ntime: " << micros() - loopTimer << "us\n----\n";

        //Calculate new PID compensated throttle
        //uint16_t newThrottle = fc.calculateThrottlePID(pwmInputs.throttlePWM, currentAltitude);
        //std::cout << "Loop Time: " << micros() - loopTimer << "us" << std::endl;
        loopTimer = micros();
    }
}

/// @brief Shutting down threads and closing ports.
void shutdown() {
    shuttingDown = true;

    // std::cout << "\nClosing Threads and Ports...\n\n";

    // // Stop threads
    // //fc.stopFlight();
    // delay(1000);
    // //radio.stopReceiveLoop();
    // //baro.close();

    // std::cout << "Closing I2C, UART, SPI, TCP Socket...\n";

    // // Close all interfaces on global instances so that we
    // // can close the ports before terminating pigpio
    // //fc.closeSPI();
    // //radio.closeSerial();

    // std::cout << "\nResetting Flight Controller...\n\n";
    
    // // Reset command to STM32
    // //fc.requestService(FlightController::Service::RESET);

    // // Terminates pigpio library
    // gpioTerminate();

    // // Signals to main() that it can return now
    // doneShuttingDown = true;
}