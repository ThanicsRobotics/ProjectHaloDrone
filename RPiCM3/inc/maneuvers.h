#ifndef MANEUVERS_H
#define MANEUVERS_H

fcMessage packMessage(float travelAngle, int pitchPWM, int rollPWM, int throttle, 
int yawPWM, int* maneuverCode, int maneuverCodeSize);

#endif