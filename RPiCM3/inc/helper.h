#ifndef HELPER_H
#define HELPER_H

#include <pigpio.h>
#include <wiringPi.h>
#include <iostream>
#include <memory>
#include <string>

//Project headers
#include <halocontroller.h>

#define projectPath std::string("./")

//extern bool shuttingDown;
extern std::shared_ptr<bool> shuttingDownPtr;

/// @brief Terminal signal handler (for ending program via terminal).
void signal_callback_handler(int signum);

/// @brief Filtering command line options.
/// @param _argc Pass in argc from main() here.
/// @param _argv Pass in argv from main() here.
void filterCommandLineOptions(int _argc, char *_argv[], CommandLineOptions& clo);

/// @brief Displays message showing how to type options in command line.
/// @param name Name of program, i.e. First string of argv[].
void showUsage(std::string name);

/// @brief Shutting down threads and closing ports.
void shutdown();

#endif