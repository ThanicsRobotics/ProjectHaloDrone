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
    CommandLineOptions clo;
    filterCommandLineOptions(argc, argv, clo);
    {
        // Creating flight controller and starting flight
        shuttingDownPtr = std::make_shared<bool>(false);
        HaloController hc(shuttingDownPtr, clo);
        //hc.startVideoPipelines();
        std::cout << "Starting main loop\n";
        hc.startFlightController();
    }
    gpioTerminate();

    return 0;
}