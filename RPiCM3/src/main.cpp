/// @file main.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Main program implementation.

#include <helper.h>

int main(int argc, char *argv[]) {
    // Setup GPIO libraries
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        std::cout << "pigpio Library failed: " << strerror(errno) << "\n";
        exit(1);
    }

    // Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);

    filterCommandLineOptions(argc, argv);

    // Setup Flight Controller and start SPI comms thread.
    fc.setupSPI();
    fc.startFlight();

    // Wait for gyro calibration, reset calibration if necessary
    waitForGyroCalibration();

    // Manual arming process through SSH
    std::cout << "Type 'ARM' to arm the quadcopter: ";
    std::string input = "";
    getline(std::cin, input);
    if (input == "ARM") {
        fc.requestService(FlightController::Service::ARM);
    }

    // Start main loop
    mainLoop();

    // Waits for threads to end, and if interrupt hasn't been
    // triggered, shutdown threads and ports now
    delay(2000);
    if (!shuttingDown) shutdown();
    while (!doneShuttingDown) delay(10);
    return 0;
}