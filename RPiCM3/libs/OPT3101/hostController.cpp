/*!
* \file hostController.cpp
* \author  Karthik Rajagopal <krthik@ti.com>
* \version 0.8
*
* \section COPYRIGHT
* TEXAS INSTRUMENTS TEXT FILE LICENSE
* Copyright (c) 2018 Texas Instruments Incorporated
* All rights reserved not granted herein.
* Limited License.
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (�TI Devices�).  No hardware patent is licensed hereunder.
* Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution
* Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:
* *	No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.
* *	any redistribution and use are licensed by TI for use only with TI Devices.
* *	Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
* If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:
* *	any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.
* *	any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.
* DISCLAIMER.
* THIS SOFTWARE IS PROVIDED BY TI AND TI�S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI�S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* \section DESCRIPTION
* This file contains the hostController class methods
*/

#include "hostController.h"

#ifdef _WIN32
#ifdef OPT3101_USE_SERIALLIB
#define WINPAUSE system("pause")
#endif
#endif

#ifdef OPT3101_USE_SERIALLIB
/** \brief Serial Command Port  declaration
This global variable declaration with name OPT3101commandPort of class serial::Serial is used by class like OPT3101::deviceRegister for I2C read and writes.
*/
serial::Serial OPT3101commandPort("COM32", 9600, serial::Timeout::simpleTimeout(1000));
#endif 

#ifdef OPT3101_USE_STDIOLIB
const char filePath[] = { "C:/temp/" };
#endif

hostController host;


void hostController::writeI2C(uint8_t address, uint32_t data) {
	/// <b>Algorithm of the method is as follows</b>
#if defined(OPT3101_USE_STDIOLIB) && defined(OPT3101_USE_SERIALLIB)
	std::string returnValue;
	char *writeData;
	writeData = new char[16];
	sprintf(writeData, "REGWx%02xx%06x\r", address, data); /// * Creates WRITE I2C command to send to h/w with address and data specified in arguments
	OPT3101commandPort.write((uint8_t*)writeData, strlen(writeData)); /// * Writes the WRITE I2C command to h/w
	returnValue = OPT3101commandPort.readline();
#endif

}
uint32_t hostController::readI2C(uint8_t address) {
	uint32_t i2cReadValue=0;
#if defined(OPT3101_USE_STDIOLIB) && defined(OPT3101_USE_SERIALLIB)
	char *writeData;
	std::string returnValue;
	uint8_t c0;
	writeData = new char[9];
	
	/// <b>Algorithm of the method is as follows</b>
	sprintf(writeData, "REGRx%02x\r", address); /// * Creates READ I2C command to send to h/w with address and data specified in arguments

	i2cReadValue = 0;

	OPT3101commandPort.write((uint8_t*)writeData, strlen(writeData)); /// * Writes the READ I2C command to h/w
	returnValue = OPT3101commandPort.readline(); /// * Waits and receives response from h/w
	if (returnValue.length() == 18) {
		returnValue = returnValue.substr(10, 6);
		c0 = 0;
		for (std::string::iterator it = returnValue.begin(); it != returnValue.end(); ++it) { /// * Converters the received string output from h/w to uint32_t value
			if (*it >= '0' && *it <= '9')
				i2cReadValue += ((*it) - 48) << ((5 - c0) * 4);
			else if (*it >= 'A' && *it <= 'F')
				i2cReadValue += ((*it) - 65 + 10) << ((5 - c0) * 4);
			else if (*it >= 'a' && *it <= 'f')
				i2cReadValue += ((*it) - 97 + 10) << ((5 - c0) * 4);
			c0++;
		}
	}
#endif
	return i2cReadValue; /// * Returns the data in uint32_t format 
}

void hostController::sleep(uint32_t timeInMilliSeconds) {
	/// <b>Algorithm of the method is as follows</b>
#if defined(HOST_PC) && defined(OPT3101_USE_SERIALLIB)
	Sleep(timeInMilliSeconds);/// * Sleeps for the time specified in the argument timeInMilliSeconds
#endif
}
void hostController::sleepDataReadyCounts(uint16_t dataReadyCounts) {
	/// <b>Algorithm of the method is as follows</b>
	/// * Currently empty function needs to be implemented by user. Based on interrupts from data ready signal from OPT3101, the host needs to count those pulses and wait until dataReadyCounts have reached. 
}

void hostController::pause()
{
	/// <b>Algorithm of the method is as follows</b>
#if defined(HOST_PC) && defined(_WIN32) && defined(OPT3101_USE_SERIALLIB)	
	WINPAUSE; 	/// * Pause for user input
#endif

}

void hostController::resetDevice() {
	// These comments rest the device on power-up
#if defined(OPT3101_USE_STDIOLIB) && defined(OPT3101_USE_SERIALLIB)
	char *writeData;
	writeData = new char[6];
	/// <b>Algorithm of the method is as follows</b>
	sprintf(writeData, "DEVR\r"); /// * Creates a command which specifies host to send RESET Pulse to OPT3101 h/w 
	OPT3101commandPort.write((uint8_t*)writeData, strlen(writeData)); /// * Send the RESET command to the h/w
#endif
}
