/*******************************************************************************
 Copyright (C) 2016, STMicroelectronics International N.V.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of STMicroelectronics nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS ARE DISCLAIMED.
 IN NO EVENT SHALL STMICROELECTRONICS INTERNATIONAL N.V. BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/**
 * @file   vl53l1_platform.c
 * @brief  Code function definitions for EwokPlus25 Platform Layer
 *         RANGING SENSOR VERSION
 *
 */
//#include <windows.h>

#include <wiringPi.h>
#include <pigpio.h>

#include <stdio.h>      // sprintf(), vsnprintf(), printf()
#include <stdint.h>
#include <string.h>     // strncpy(), strnlen()

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "vl53l1_platform.h"
#include "vl53l1_platform_user_data.h"
#include "vl53l1_platform_log.h"

// #ifdef PAL_EXTENDED
// 	#include "vl53l1_register_strings.h"
// #else
// 	#define VL53L1_get_register_name(a,b)
// #endif

//#include "ranging_sensor_comms.h"
//#include "power_board_defs.h"

// flag to indicate if power board has been accessed
// const uint32_t _power_board_in_use = 0;

// flag to indicate if we can use the extended voltage ranges (not laser safe!)
// uint32_t _power_board_extended = 0;

// cache of the comms type flag
// uint8_t global_comms_type = 0;

// #define  VL53L1_COMMS_CHUNK_SIZE  32
// #define  VL53L1_COMMS_BUFFER_SIZE 64

// #define GPIO_INTERRUPT          RS_GPIO62
// #define GPIO_POWER_ENABLE       RS_GPIO60
// #define GPIO_XSHUTDOWN          RS_GPIO61
// #define GPIO_SPI_CHIP_SELECT    RS_GPIO51

/*!
 *  The intent of this Abstraction layer is to provide the same API
 *  to the underlying SystemVerilog tasks as the C driver will have
 *  to ST Comms DLL's for the talking to Ewok via the USB + STM32
 *  or if the C-driver is implemented directly on the STM32
 */

/* when not customized by application define dummy one */
#ifndef VL53L1_GetI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#   define VL53L1_GetI2cBus(...) (void)0
#endif

#ifndef VL53L1_PutI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#   define VL53L1_PutI2cBus(...) (void)0
#endif

#define trace_print(level, ...) \
	_LOG_TRACE_PRINT(VL53L1_TRACE_MODULE_PLATFORM, \
	level, VL53L1_TRACE_FUNCTION_NONE, ##__VA_ARGS__)

#define trace_i2c(...) \
	_LOG_TRACE_PRINT(VL53L1_TRACE_MODULE_NONE, \
	VL53L1_TRACE_LEVEL_NONE, VL53L1_TRACE_FUNCTION_I2C, ##__VA_ARGS__)


VL53L1_Error VL53L1_CommsInitialise(
	VL53L1_Dev_t *pdev,
	uint8_t       comms_type,
	uint16_t      comms_speed_khz)
{
	VL53L1_Error status = VL53L1_ERROR_NONE;

	pdev->I2cHandle = i2cOpen(1, pdev->I2cDevAddr, 0);

	// char comms_error_string[ERROR_TEXT_LENGTH];

	// SUPPRESS_UNUSED_WARNING(pdev);
	// SUPPRESS_UNUSED_WARNING(comms_speed_khz);

	// global_comms_type = comms_type;

	// if(global_comms_type == VL53L1_I2C)
	// {
	// 	if ((CP_STATUS)RANGING_SENSOR_COMMS_Init_CCI(0, 0, 0) != CP_STATUS_OK)
	// 	{
	// 		RANGING_SENSOR_COMMS_Get_Error_Text(comms_error_string); /*lint !e534 ignoring return value */
	// 		trace_i2c("VL53L1_CommsInitialise: RANGING_SENSOR_COMMS_Init_CCI() failed\n");
	// 		trace_i2c(comms_error_string);
	// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
	// 	}
	// }
	// else if(global_comms_type == VL53L1_SPI)
	// {
	// 	if ((CP_STATUS)RANGING_SENSOR_COMMS_Init_SPI_V2W8(0, 0, 0) != CP_STATUS_OK)
	// 	{
	// 		RANGING_SENSOR_COMMS_Get_Error_Text(comms_error_string); /*lint !e534 ignoring return value */
	// 		trace_i2c("VL53L1_CommsInitialise: RANGING_SENSOR_COMMS_Init_SPI_V2W8() failed\n");
	// 		trace_i2c(comms_error_string);
	// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
	// 	}
	// }
	// else
	// {
	// 	trace_i2c("VL53L1_CommsInitialise: Comms must be one of VL53L1_I2C or VL53L1_SPI\n");
	// 	status = VL53L1_ERROR_CONTROL_INTERFACE;
	// }

	return status;
}


VL53L1_Error VL53L1_CommsClose(
	VL53L1_Dev_t *pdev)
{
	VL53L1_Error status = VL53L1_ERROR_NONE;

	i2cClose(pdev->I2cHandle);

	// char comms_error_string[ERROR_TEXT_LENGTH];

	// // SUPPRESS_UNUSED_WARNING(pdev);

	// if(global_comms_type == VL53L1_I2C)
	// {
	// 	if((CP_STATUS)RANGING_SENSOR_COMMS_Fini_CCI() != CP_STATUS_OK)
	// 	{
	// 		RANGING_SENSOR_COMMS_Get_Error_Text(comms_error_string); /*lint !e534 ignoring return value */
	// 		trace_i2c("VL53L1_CommsClose: RANGING_SENSOR_COMMS_Fini_CCI() failed\n");
	// 		trace_i2c(comms_error_string);
	// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
	// 	}
	// }
	// else if(global_comms_type == VL53L1_SPI)
	// {
	// 	if((CP_STATUS)RANGING_SENSOR_COMMS_Fini_SPI_V2W8() != CP_STATUS_OK)
	// 	{
	// 		RANGING_SENSOR_COMMS_Get_Error_Text(comms_error_string); /*lint !e534 ignoring return value */
	// 		trace_i2c("VL53L1_CommsClose: RANGING_SENSOR_COMMS_Fini_SPI_V2W8() failed\n");
	// 		trace_i2c(comms_error_string);
	// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
	// 	}
	// }
	// else
	// {
	// 	trace_i2c("VL53L1_CommsClose: Comms must be one of VL53L1_I2C or VL53L1_SPI\n");
	// 	status = VL53L1_ERROR_CONTROL_INTERFACE;
	// }

	return status;
}

/*
 * ----------------- COMMS FUNCTIONS -----------------
 */

uint8_t _I2CBuffer[256];

int _I2CWrite(VL53L1_DEV Dev, uint8_t *pdata, uint32_t count) {
    int status;
    // int i2c_time_out = I2C_TIME_OUT_BASE+ count* I2C_TIME_OUT_BYTE;

    // status = HAL_I2C_Master_Transmit(Dev->I2cHandle, Dev->I2cDevAddr, pdata, count, i2c_time_out);
    status = i2cDeviceWrite(Dev->I2cHandle, pdata, count);
	// if (status) {
    //     //VL6180x_ErrLog("I2C error 0x%x %d len", dev->I2cAddr, len);
    //     //XNUCLEO6180XA1_I2C1_Init(&hi2c1);
    // }
    return status;
}

int _I2CRead(VL53L1_DEV Dev, uint8_t *pdata, uint32_t count) {
    int status;
    // int i2c_time_out = I2C_TIME_OUT_BASE+ count* I2C_TIME_OUT_BYTE;
	status = i2cDeviceRead(Dev->I2cHandle, pdata, count);
    // status = HAL_I2C_Master_Receive(Dev->I2cHandle, Dev->I2cDevAddr|1, pdata, count, i2c_time_out);
    // if (status) {
    //     //VL6180x_ErrLog("I2C error 0x%x %d len", dev->I2cAddr, len);
    //     //XNUCLEO6180XA1_I2C1_Init(&hi2c1);
    // }
    return status;
}

VL53L1_Error VL53L1_WriteMulti(VL53L1_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    int status_int;
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    if (count > sizeof(_I2CBuffer) - 1) {
        return VL53L1_ERROR_INVALID_PARAMS;
    }
    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    memcpy(&_I2CBuffer[2], pdata, count);
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, count + 2);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
    VL53L1_PutI2cBus();
    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L1_Error VL53L1_ReadMulti(VL53L1_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, pdata, count);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
done:
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_WrByte(VL53L1_DEV Dev, uint16_t index, uint8_t data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data;

    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 3);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_WrWord(VL53L1_DEV Dev, uint16_t index, uint16_t data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data >> 8;
    _I2CBuffer[3] = data & 0x00FF;

    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_WrDWord(VL53L1_DEV Dev, uint16_t index, uint32_t data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;
    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = (data >> 24) & 0xFF;
    _I2CBuffer[3] = (data >> 16) & 0xFF;
    _I2CBuffer[4] = (data >> 8)  & 0xFF;
    _I2CBuffer[5] = (data >> 0 ) & 0xFF;
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 6);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_UpdateByte(VL53L1_DEV Dev, uint16_t index, uint8_t AndData, uint8_t OrData) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    uint8_t data;

    Status = VL53L1_RdByte(Dev, index, &data);
    if (Status) {
        goto done;
    }
    data = (data & AndData) | OrData;
    Status = VL53L1_WrByte(Dev, index, data);
done:
    return Status;
}

VL53L1_Error VL53L1_RdByte(VL53L1_DEV Dev, uint16_t index, uint8_t *data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

	_I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if( status_int ){
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, data, 1);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
done:
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_RdWord(VL53L1_DEV Dev, uint16_t index, uint16_t *data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);

    if( status_int ){
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint16_t)_I2CBuffer[0]<<8) + (uint16_t)_I2CBuffer[1];
done:
    VL53L1_PutI2cBus();
    return Status;
}

VL53L1_Error VL53L1_RdDWord(VL53L1_DEV Dev, uint16_t index, uint32_t *data) {
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L1_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint32_t)_I2CBuffer[0]<<24) + ((uint32_t)_I2CBuffer[1]<<16) + ((uint32_t)_I2CBuffer[2]<<8) + (uint32_t)_I2CBuffer[3];

done:
    VL53L1_PutI2cBus();
    return Status;
}

/*
 * ----------------- HOST TIMING FUNCTIONS -----------------
 */

VL53L1_Error VL53L1_WaitUs(
	VL53L1_Dev_t *pdev,
	int32_t       wait_us)
{
	VL53L1_Error status         = VL53L1_ERROR_NONE;

	delayMicroseconds(wait_us);

	// float wait_ms = (float)wait_us/1000.0f; /*lint !e586 float in windows platform*/
	// HANDLE hEvent = CreateEvent(0, TRUE, FALSE, 0);

	// // SUPPRESS_UNUSED_WARNING(pdev);

	// /*
	//  * Use windows event handling to perform non-blocking wait.
	//  */
	// WaitForSingleObject(hEvent, (DWORD)(wait_ms + 0.5f)); /*lint !e534 ignoring return value */

	trace_i2c("WaitUs(%6d);\n", wait_us);

	return status;
}


VL53L1_Error VL53L1_WaitMs(
	VL53L1_Dev_t *pdev,
	int32_t       wait_ms)
{
	return VL53L1_WaitUs(pdev, wait_ms * 1000);
}

/*
 * ----------------- DEVICE TIMING FUNCTIONS -----------------
 */

VL53L1_Error VL53L1_GetTimerFrequency(int32_t *ptimer_freq_hz)
{
	*ptimer_freq_hz = 0;

	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GetTimerFrequency: Freq : %dHz\n", *ptimer_freq_hz);
	return VL53L1_ERROR_NONE;
}


VL53L1_Error VL53L1_GetTimerValue(int32_t *ptimer_count)
{
	*ptimer_count = 0;

	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GetTimerValue: Freq : %dHz\n", *ptimer_count);
	return VL53L1_ERROR_NONE;
}


/*
 * ----------------- GPIO FUNCTIONS -----------------
 */

// VL53L1_Error VL53L1_GpioSetMode(uint8_t pin, uint8_t mode)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Mode((RS_GPIO_Pin)pin, (RS_GPIO_Mode)mode) != CP_STATUS_OK)
// 	{
// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioSetMode: Status %d. Pin %d, Mode %d\n", status, pin, mode);
// 	return status;
// }


// VL53L1_Error  VL53L1_GpioSetValue(uint8_t pin, uint8_t value)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value((RS_GPIO_Pin)pin, value) != CP_STATUS_OK)
// 	{
// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioSetValue: Status %d. Pin %d, Mode %d\n", status, pin, value);
// 	return status;

// }


// VL53L1_Error  VL53L1_GpioGetValue(uint8_t pin, uint8_t *pvalue)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	DWORD value = 0;

// 	if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Get_Value((RS_GPIO_Pin)pin, &value) != CP_STATUS_OK)
// 	{
// 		status = VL53L1_ERROR_CONTROL_INTERFACE;
// 	}
// 	else
// 	{
// 		*pvalue = (uint8_t)value;
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioGetValue: Status %d. Pin %d, Mode %d\n", status, pin, *pvalue);
// 	return status;
// }

/*
 * ----------------- HARDWARE STATE FUNCTIONS -----------------
 */

// VL53L1_Error  VL53L1_GpioXshutdown(uint8_t value)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	if(status == VL53L1_ERROR_NONE) /*lint !e774 always true */
// 	{
// 		status = VL53L1_GpioSetMode((uint8_t)GPIO_XSHUTDOWN, (uint8_t)GPIO_OutputPP);
// 	}

// 	if(status == VL53L1_ERROR_NONE)
// 	{
// 		if(value)
// 		{
// 			if ((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_XSHUTDOWN, (DWORD)Pin_State_High) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 		else
// 		{
// 			if ((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_XSHUTDOWN, (DWORD)Pin_State_Low) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioXShutdown: Status %d. Value %d\n", status, value);
// 	return status;
// }


// VL53L1_Error  VL53L1_GpioCommsSelect(uint8_t value)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	if(status == VL53L1_ERROR_NONE) /*lint !e774 always true */
// 	{
// 		status = VL53L1_GpioSetMode((uint8_t)GPIO_SPI_CHIP_SELECT, (uint8_t)GPIO_OutputPP);
// 	}

// 	if(status == VL53L1_ERROR_NONE)
// 	{
// 		if(value)
// 		{
// 			if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_SPI_CHIP_SELECT, (DWORD)Pin_State_High) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 		else
// 		{
// 			if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_SPI_CHIP_SELECT, (DWORD)Pin_State_Low) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioCommsSelect: Status %d. Value %d\n", status, value);
// 	return status;
// }


// VL53L1_Error  VL53L1_GpioPowerEnable(uint8_t value)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;
// 	POWER_BOARD_CMD power_cmd;

// 	if(status == VL53L1_ERROR_NONE) /*lint !e774 always true */
// 	{
// 		status = VL53L1_GpioSetMode((uint8_t)GPIO_POWER_ENABLE, (uint8_t)GPIO_OutputPP);
// 	}

// 	if(status == VL53L1_ERROR_NONE)
// 	{
// 		if(value)
// 		{
// 			if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_POWER_ENABLE, (DWORD)Pin_State_High) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 		else
// 		{
// 			if((CP_STATUS)RANGING_SENSOR_COMMS_GPIO_Set_Value(GPIO_POWER_ENABLE, (DWORD)Pin_State_Low) != CP_STATUS_OK)
// 			{
// 				status = VL53L1_ERROR_CONTROL_INTERFACE;
// 			}
// 		}
// 	}

// 	if(status == VL53L1_ERROR_NONE && _power_board_in_use == 1 && value) /*lint !e506 !e845 !e774*/
// 	{
// 		memset(&power_cmd, 0, sizeof(POWER_BOARD_CMD));
// 		power_cmd.command = ENABLE_DUT_POWER;

// 		if((CP_STATUS)RANGING_SENSOR_COMMS_Write_System_I2C(
// 			POWER_BOARD_I2C_ADDRESS, sizeof(POWER_BOARD_CMD), (uint8_t*)&power_cmd) != CP_STATUS_OK)
// 		{
// 			status = VL53L1_ERROR_CONTROL_INTERFACE;
// 		}
// 	}

// 	trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GpioPowerEnable: Status %d. Value %d\n", status, value);
// 	return status;
// }


// VL53L1_Error  VL53L1_GpioInterruptEnable(void (*function)(void), uint8_t edge_type)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	// SUPPRESS_UNUSED_WARNING(function);
// 	// SUPPRESS_UNUSED_WARNING(edge_type);

// 	return status;
// }


// VL53L1_Error  VL53L1_GpioInterruptDisable(void)
// {
// 	VL53L1_Error status         = VL53L1_ERROR_NONE;

// 	return status;
// }


VL53L1_Error VL53L1_GetTickCount(
	uint32_t *ptick_count_ms)
{

	/* Returns current tick count in [ms] */

	VL53L1_Error status  = VL53L1_ERROR_NONE;

	// *ptick_count_ms = timeGetTime();
	*ptick_count_ms = 0;

	trace_print(
	VL53L1_TRACE_LEVEL_DEBUG,
	"VL53L1_GetTickCount() = %5u ms;\n",
	*ptick_count_ms);

	return status;

}


VL53L1_Error VL53L1_WaitValueMaskEx(
	VL53L1_Dev_t *pdev,
	uint32_t      timeout_ms,
	uint16_t      index,
	uint8_t       value,
	uint8_t       mask,
	uint32_t      poll_delay_ms)
{
	/*
	 * Platform implementation of WaitValueMaskEx V2WReg script command
	 *
	 * WaitValueMaskEx(
	 *          duration_ms,
	 *          index,
	 *          value,
	 *          mask,
	 *          poll_delay_ms);
	 */

	VL53L1_Error status         = VL53L1_ERROR_NONE;
	uint32_t     start_time_ms   = 0;
	uint32_t     current_time_ms = 0;
	uint8_t      byte_value      = 0;
	uint8_t      found           = 0;
#ifdef VL53L1_LOG_ENABLE
	uint32_t     trace_functions = 0;
#endif

	// _LOG_STRING_BUFFER(register_name);

	// SUPPRESS_UNUSED_WARNING(poll_delay_ms);

#ifdef VL53L1_LOG_ENABLE
	/* look up register name */
	VL53L1_get_register_name(
			index,
			register_name);

	/* Output to I2C logger for FMT/DFT  */
	trace_i2c("WaitValueMaskEx(%5d, %s, 0x%02X, 0x%02X, %5d);\n",
		timeout_ms, register_name, value, mask, poll_delay_ms);
#endif // VL53L1_LOG_ENABLE

	/* calculate time limit in absolute time */

	VL53L1_GetTickCount(&start_time_ms);
	pdev->new_data_ready_poll_duration_ms = 0;

	/* remember current trace functions and temporarily disable
	 * function logging
	 */

#ifdef VL53L1_LOG_ENABLE
	trace_functions = _LOG_GET_TRACE_FUNCTIONS();
#endif
	// _LOG_SET_TRACE_FUNCTIONS(VL53L1_TRACE_FUNCTION_NONE);

	/* wait until value is found, timeout reached on error occurred */

	while ((status == VL53L1_ERROR_NONE) &&
		   (pdev->new_data_ready_poll_duration_ms < timeout_ms) &&
		   (found == 0))
	{
		status = VL53L1_RdByte(
						pdev,
						index,
						&byte_value);

		if ((byte_value & mask) == value)
		{
			found = 1;
		}

		/*if (status == VL53L1_ERROR_NONE  &&
			found == 0 &&
			poll_delay_ms > 0)
			status = VL53L1_WaitMs(
							pdev,
							poll_delay_ms);
		*/

		/* Update polling time (Compare difference rather than absolute to
		negate 32bit wrap around issue) */
		VL53L1_GetTickCount(&current_time_ms);
		pdev->new_data_ready_poll_duration_ms = current_time_ms - start_time_ms;
	}

	/* Restore function logging */
	// _LOG_SET_TRACE_FUNCTIONS(trace_functions);

	if (found == 0 && status == VL53L1_ERROR_NONE)
		status = VL53L1_ERROR_TIME_OUT;

	return status;
}

