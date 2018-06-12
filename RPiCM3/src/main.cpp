/// @file main.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Main program implementation.

#include <helper.h>
#include <thetav.h>

int main(int argc, char *argv[]) {
    // Setup GPIO libraries
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        std::cout << "pigpio Library failed: " << strerror(errno) << "\n";
        exit(1);
    }

    // Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);

    // Creating flight controller and starting flight
    // FlightController fc(&shuttingDown);
    // filterCommandLineOptions(argc, argv, fc);

    // std::cout << "Starting main loop\n";
    // fc.startFlight();

    ThetaV camera;

    gpioTerminate();

    return 0;
}