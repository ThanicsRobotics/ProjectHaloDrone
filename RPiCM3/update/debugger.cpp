#include <wiringPi.h>
#include <iostream>
#include <signal.h>

#define SEL2 5

void callback_handler(int signum)
{
    pinMode(SEL2, INPUT);
    exit(0);
}

int main()
{
    wiringPiSetupGpio();
    signal(SIGINT, callback_handler);
    pinMode(SEL2, OUTPUT);

    digitalWrite(SEL2, LOW);
    delay(50);
    system("sudo openocd -f stm32f446-debug.cfg");
    while (1)
        delay(50);
}