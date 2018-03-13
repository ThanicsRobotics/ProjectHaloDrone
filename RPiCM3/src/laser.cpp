#include <laser.h>
#include <vl53l1_api.h>
#include <vl53l1_platform_init.h>

VL53L1_Dev_t _laser1;
VL53L1_DEV laser1 = &_laser1;
bool isInterrupt = false;

void initLasers() {
    VL53L1_platform_init(laser1, 0x29, 0, 400);
}

void closeLasers() {
    VL53L1_platform_terminate(laser1);
}

void autonomousLowPowerRangingTest(VL53L1_DEV laser) {
    int status = VL53L1_ERROR_NONE;
    static VL53L1_RangingMeasurementData_t RangingData;
    printf("Autonomous Ranging Test\n");
    status = VL53L1_WaitDeviceBooted(laser);
    status = VL53L1_DataInit(laser);
    status = VL53L1_StaticInit(laser);
    status = VL53L1_SetDistanceMode(laser, VL53L1_DISTANCEMODE_LONG);
    status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(laser, 50000);
    status = VL53L1_SetInterMeasurementPeriodMilliSeconds(laser, 500);
    status = VL53L1_StartMeasurement(laser);

    if(status != 0){
        printf("VL53L1_StartMeasurement failed \n");
        while(1);
    }
    if (isInterrupt){
        // do // interrupt mode
        // {
        //     __WFI();
        //     if(IntCount !=0 ){
        //         IntCount=0;
        //         status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
        //         if(status==0){
        //             printf("%d,%d,%.2f,%.2f\n", RangingData.RangeStatus,RangingData.RangeMilliMeter,
        //                             RangingData.SignalRateRtnMegaCps/65536.0,RangingData.AmbientRateRtnMegaCps/65336.0);
        //         }
        //         status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
        //     }
        // }
        // while(1);
    }
    else {
        do // polling mode
        {
            status = VL53L1_WaitMeasurementDataReady(laser);
            if(!status)
            {
                status = VL53L1_GetRangingMeasurementData(laser, &RangingData);
                if(status==0){
                    printf("%d,%d,%.2f,%.2f\n", RangingData.RangeStatus,RangingData.RangeMilliMeter,
                        (RangingData.SignalRateRtnMegaCps/65536.0),RangingData.AmbientRateRtnMegaCps/65336.0);
                }
                status = VL53L1_ClearInterruptAndStartMeasurement(laser);
            }
        }
        while (1);
	}
//  return status;
}