#ifndef LASER_H
#define LASER_H

#include "../libs/VL53L1X_API_2.2.1/API/platform/inc/vl53l1_platform_user_data.h"

extern VL53L1_Dev_t _laser1;
extern VL53L1_DEV laser1;

void initLaser();
void closeLasers()
void autonomousLowPowerRangingTest();

#endif