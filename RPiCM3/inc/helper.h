

#define GYRO_CAL 0x04
#define projectPath std::string("./")

//Terminal signal handler (for ending program via terminal)
void signal_callback_handler(int signum);

void waitForGyroCalibration();

void filterCommandLineOptions();

void mainLoop();

void showUsage();

void shutdown();