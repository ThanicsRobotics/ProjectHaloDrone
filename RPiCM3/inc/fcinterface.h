#include <inttypes.h>

/// @brief Controls the interfacing between the Raspberry Pi CM3
/// and the STM32F446
class FCInterface
{
  public:
    FCInterface(bool *shutdown);
    ~FCInterface();

    /// @brief Message data structure for sending to STM32F446.
    struct fcMessage
    {
        float travelAngle; ///< Desired compass angle for the STM32F446 to fly.
        uint16_t pwm[4];   ///< Array containing PWM inputs, in order: pitch, roll, yaw, throttle.
    };

    /// @brief Types of services for the Flight Controller to execute.
    enum Service
    {
        ARM = 0, ///< Arms the drone and TURNS ON MOTORS.
        DISARM,  ///< Disarms the drone, turns off motors.
        AUTH,    ///< Authenticates the drone, checks it exists.
        RESET    ///< Resets the STM32F446.
    };

    /// @brief Opens the SPI port and sets the spiConfigured flag to true.
    void setupSPI();

    /// @brief Closes the SPI port.
    void closeSPI();

	void startInterface();
	void stopInterface();

    /// @brief Requests the FlightController class to do a Service.
    /// @param serviceType A member of the Service enum
    void requestService(Service serviceType);

    /// @brief Requests the interfaceLoop to send data to the STM32F446.
    /// @param data A data message structured in a fcMessage struct.
    void requestSend(fcMessage data);

    /// @brief Checks if SPI port has been opened.
    /// @return true if opened, false if closed.
    bool isSPIConfigured() const { return spiConfigured; }

    /// @brief Checks if drone is armed.
    /// @return true if armed, false if disarmed.
    bool isArmed() const { return armed; }

    /// @brief Checks if STM32F446 has been authenticated.
    /// @return true if authenticated, false if not.
    bool isAuthenticated() const { return authenticated; }

    bool isTestGyroActive() const { return testGyro; }
    bool isMotorTestActive() const { return motorTest; }
    bool isNoMotorsActive() const { return noMotors; }

    void setTestGyro(bool state) { testGyro = state; }
    void setMotorTest(bool state) { motorTest = state; }
    void setNoMotors(bool state) { noMotors = state; }
    void setSTM32Resetting(bool state) { stm32Resetting = state; }

    /// @brief Method to access the data received from STM32F446 SPI.
    /// @return 16-bit SPI packet.
    int16_t getReceivedData() { return fcReceivedData; }

  private:
	bool *fcRunning;
    bool stm32Resetting = true;
    bool spiConfigured = false;
    bool authenticated;
    bool armRequest, authRequest, disarmRequest, sendRequest;
    bool armed;

    bool testGyro;
    bool motorTest;
    bool noMotors;

    char stm32_rx_buffer[MSG_LEN], stm32_tx_buffer[MSG_LEN];
    uint16_t fcReceivedData;

    //CS0 is barometer, CS1 is STM32 flight controller
    int spiCS = 0;
    int spiFd;

    fcMessage currentMessage;
    std::thread interfaceThread;

    void arm();
    void disarm();
    void reset();
    void auth();
    void packMessage(std::array<uint8_t, MSG_LEN> &msg);
    void sendMessage();
    void updateMessage();
	void interfaceLoop();
};