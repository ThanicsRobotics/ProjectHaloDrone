#include <helper.h>

#include <regex>

// bool shuttingDown = false;
std::shared_ptr<bool> shuttingDownPtr;
//bool doneShuttingDown = false;

void signal_callback_handler(int signum) { *shuttingDownPtr = true; }

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
void filterCommandLineOptions(int _argc, char *_argv[], CommandLineOptions& clo) {
    if (_argc > 1) {
        for (int i = 1; i < _argc; i++) {
            // if (std::string (_argv[i]) == "-c" || std::string (_argv[i]) == "--controller-enabled") 
            //     controllerConnected = true;
            // else if (std::string (_argv[i]) == "-nc" || std::string (_argv[i]) == "--no-controller") 
            //     controllerConnected = false;
            if (std::string (_argv[i]) == "-tg" || std::string (_argv[i]) == "--test-gyro")
                clo.testGyro = true;
            if (std::string (_argv[i]) == "-mt" || std::string (_argv[i]) == "--motor-test") {
                std::cout << "\n\t\t!!!! TESTING MOTORS BEFORE ARM !!!!\n\n";
                clo.testMotors = true;
            }
            if (std::string (_argv[i]) == "-nm" || std::string (_argv[i]) == "--no-motors")
                clo.disableMotors = true;
            if (std::string (_argv[i]) == "-h" || std::string (_argv[i]) == "--help") {
                showUsage (_argv[0]);
                exit(1);
            }
            if (std::string (_argv[i]) == "-sd" || std::string (_argv[i]) == "--stm-debug") {
                clo.enableSTM32Resetting = false;
            }
            if (std::string (_argv[i]) == "-r" || std::string (_argv[i]) == "--record") {
                clo.record = VideoFormat::MONO;
            }
            if (std::string (_argv[i]) == "-rr" || std::string (_argv[i]) == "--record-stereo") {
                clo.record = VideoFormat::STEREO;
            }
            if (std::string (_argv[i]) == "-a" || std::string (_argv[i]) == "--addr") {
                // Check for valid IP address
                std::string address = std::string(_argv[i+1]);
                if (std::regex_match(address, std::regex("[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}")))
                    clo.ipAddress = address;
                else {
                    std::cout << "Invalid IP address" << std::endl;
                    exit(1);
                }
            }
            if (std::string (_argv[i]) == "-p" || std::string (_argv[i]) == "--port") {
                clo.ports[0] = std::string(_argv[i+1]);
            }
            if (std::string (_argv[i]) == "-pp" || std::string (_argv[i]) == "--stereo-ports") {
                clo.ports[0] = std::string(_argv[i+1]);
                clo.ports[1] = std::string(_argv[i+2]);
            }
        }
    }
    // If no options are typed, show how to type them.
    else {
        showUsage (_argv[0]);
        exit(0);
    }
}