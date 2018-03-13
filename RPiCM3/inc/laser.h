#ifndef LASER_H
#define LASER_H

#include <vl53l1_platform_user_data.h>

extern VL53L1_Dev_t _laser1;
extern VL53L1_DEV laser1;

void initLasers();
void closeLasers();
void autonomousLowPowerRangingTest(VL53L1_DEV laser);

#endif