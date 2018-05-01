#include <maneuvers.h>
#include <fcinterface.h>

void takeoff(int hoverHeight) {

}

void land() {
    
}

fcMessage packMessage(float travelAngle, int pitchPWM, int rollPWM, int throttle, 
int yawPWM, int* maneuverCode, int maneuverCodeSize) {
    fcMessage msg {
        .travelAngle = travelAngle;
        .pitchPWM = pitchPWM;
        .rollPWM = rollPWM;
        .throttle = throttle;
        .yawPWM = yawPWM;
    };
    return fcMessage;
}