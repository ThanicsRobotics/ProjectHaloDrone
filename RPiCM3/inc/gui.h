#ifndef GUI_H
#define GUI_H

// ASCII decimal for keyboard characters
#define a_KEY 97
#define c_KEY 99
#define d_KEY 100
#define UP_ARROW_KEY 65
#define DOWN_ARROW_KEY 66

void startGUI();
void openMotorThrottleTest();
void openSerialConsole();
void openSensorReadout();
void closeGUI();

extern int lastKey;
extern bool guiActive;
#endif