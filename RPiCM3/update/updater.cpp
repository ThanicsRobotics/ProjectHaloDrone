#include <wiringPi.h>
#include <iostream>
#include <string>

#define SEL0 6
#define SEL1 7
#define SEL2 5

void program() {
    std::cout << "Would you like to program(p) or set fuses(f), or both(b)? (p/f/b): ";
    string input = "";
    getline(cin, input);
    if (input == "p") {
        std::cout << "Programming..." << std::endl;
        system("sudo avrdude -p m8 -C avrdude_esc.conf -c pi_1 -v -U flash:w:firmwares/simonkesc.hex:i");
    }
    if (input == "f") {
        std::cout << "Setting fuses..." << std::endl;
        system("sudo avrdude -p m8 -C avrdude_esc.conf -c pi_1 -v -U lfuse:w:0x2E:m -U hfuse:w:0xC9:m");
    }
    if (input == "b") {
        std::cout << "Programming and setting fuses..." << std::endl;
        //system("sudo avrdude -p m8 -C avrdude_esc.conf -c pi_1 -v -U lfuse:w:0x2E:m -U hfuse:w:0xC9:m -U flash:w:firmwares/simonkesc.hex:i");
        system("sudo avrdude -p m8 -C avrdude_esc.conf -c pi_1 -v -U lfuse:w:0x2F:m -U hfuse:w:0xCA:m -U flash:w:firmwares/simonkesc.hex:i");
    }
}

void updateSTM32F446() {
    std::cout << "Updating STM32F446..." << std::endl;
    digitalWrite(SEL2, LOW);
    system("sudo openocd -f stm32f446.cfg");
}

void updateSTM32F103() {
    std::cout << "Updating STM32F103..." << std::endl;
    digitalWrite(SEL2, HIGH);
    system("sudo openocd -f stm32f103.cfg");
}

void updateESC1() {
    std::cout << "Updating ESC1..." << std::endl;
    digitalWrite(SEL0, HIGH);
    digitalWrite(SEL1, LOW);
    program();
}

void updateESC2() {
    std::cout << "Updating ESC2..." << std::endl;
    digitalWrite(SEL0, LOW);
    digitalWrite(SEL1, LOW);
    program();

}

void updateESC3() {
    std::cout << "Updating ESC3..." << std::endl;
    digitalWrite(SEL0, LOW);
    digitalWrite(SEL1, HIGH);
    program();
}

void updateESC4() {
    std::cout << "Updating ESC4..." << std::endl;
    digitalWrite(SEL0, HIGH);
    digitalWrite(SEL1, HIGH);
    program();
}

void updateAllESCs() {
    updateESC1();
    updateESC2();
    updateESC3();
    updateESC4();
}

int main() {
    std::cout << "**Make sure you have put all firmwares being updated in the firmwares/ folder**" << std::endl << "Type 'ok' to continue: ";
    string input = "";
    getline(cin, input);
    if (input != "ok") exit(1);

    wiringPiSetupGpio();
    pinMode(SEL0, OUTPUT);
    pinMode(SEL1, OUTPUT);
    pinMode(SEL2, OUTPUT);

    std::cout << "Which processor to update?\n" << "\t1 = STM32F446 (Flight Motor Controller)\n" << "\t2 = STM32F103 (Gimbal Controller)\n"
        << "\t3 = ATMEGA8 (ESC1)\n" << "\t4 = ATMEGA8 (ESC2)\n" << "\t5 = ATMEGA8 (ESC3)\n" << "\t6 = ATMEGA8 (ESC4)\n" << "\t7 = ALL ESCs\n"
        << "\t8 = ALL Processors\n" << "Choice: ";
    
    input = "";
    std::getline(std::cin, input);
    int intInput = atoi(input.c_str());

    switch (intInput) {
        case 1:
            updateSTM32F446();
            break;
        case 2:
            updateSTM32F103();
            break;
        case 3:
            updateESC1();
            break;
        case 4:
            updateESC2();
            break;
        case 5:
            updateESC3();
            break;
        case 6:
            updateESC4();
            break;
        case 7:
            updateAllESCs();
            break;
        case 8:
            updateSTM32F446();
            updateSTM32F103();
            updateAllESCs();
            break;
        default:
            std::cout << "Invalid Response. Shutting Down." << std::endl;
            exit(0);
    }
    pinMode(SEL0, INPUT);
    pinMode(SEL1, INPUT);
    pinMode(SEL2, INPUT);
}
