#ifndef GUI_H
#define GUI_H

void startGUI();
void openMotorThrottleTest();
void openSerialConsole();
void openSensorReadout();
void closeGUI();

extern int lastKey;
extern bool guiActive;
#endif