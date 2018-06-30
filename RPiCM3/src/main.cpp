/// @file main.cpp
/// @author Andrew Loomis
/// @date 5/17/2018
/// @brief Main program implementation.

#include <helper.h>
#include <thetav.h>
#include <videostream.h>

int main(int argc, char *argv[]) {
    // Setup GPIO libraries
    wiringPiSetupGpio();
    if (gpioInitialise() < 0) {
        std::cout << "pigpio Library failed: " << strerror(errno) << "\n";
        exit(1);
    }

    // Override pigpio SIGINT handling
    signal(SIGINT, signal_callback_handler);
    {
    // Creating flight controller and starting flight
    shuttingDownPtr = std::make_shared<bool>(false);
    FlightController fc(shuttingDownPtr);
    CommandLineOptions clo;
    filterCommandLineOptions(argc, argv, clo);

    std::cout << "Starting main loop\n";

    VideoStream::VideoSettings settings;
    settings.bitrate = 2000000;
    settings.vFlip = true;
    settings.hFlip = true;
    settings.height = 900;
    settings.width = 896;
    settings.fps = 40;

    VideoStream::ReceiveAddress addr;
    addr.ip = clo.ipAddress;
    addr.port = clo.port;

    VideoStream stream1(shuttingDownPtr, settings, clo.record, 0, addr);
    stream1.startPipeline();
    fc.startFlight();

    // ThetaV camera;
    // delay(2000);
    // camera.switchMode(ThetaV::Mode::PHOTO);
    // camera.takePicture();
    // delay(1000);
    // camera.startVideo();
    // delay(5000);
    // camera.stopVideo(false);
    }
    gpioTerminate();

    return 0;
}