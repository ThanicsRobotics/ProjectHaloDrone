/*!
* \file OPT3101Calibration.cpp
* \author  Karthik Rajagopal <krthik@ti.com>
* \version 0.8
*
* \section COPYRIGHT
* TEXAS INSTRUMENTS TEXT FILE LICENSE
* Copyright (c) 2018 Texas Instruments Incorporated
* All rights reserved not granted herein.
* Limited License.
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
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
* THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* \section DESCRIPTION
* This file contains the method definitions for class OPT3101::calibrationC
*/

#include "OPT3101Calibration.h"

void OPT3101::calibrationC::findCrosstalkTempRegisterValues(OPT3101::crosstalkTempCoffC *illumXtalkCoff, uint8_t size, OPT3101::crosstalkC *illumXtalk) {
	uint8_t c0, scaleCoff;
	double maxCoff = -1;
	double *magnitude;

	magnitude = new double[size];

	for (c0 = 0; c0 < size; c0++) {
		magnitude[c0] = illumXtalk->magnitude();
		maxCoff = abs(illumXtalkCoff[c0].coffI*magnitude[c0]) > maxCoff ? abs(illumXtalkCoff[c0].coffI*magnitude[c0]) : maxCoff;
		maxCoff = abs(illumXtalkCoff[c0].coffQ*magnitude[c0]) > maxCoff ? abs(illumXtalkCoff[c0].coffQ*magnitude[c0]) : maxCoff;
	}
	scaleCoff = 7;
	for (c0 = 12; c0 >= 5; c0--)
		scaleCoff = maxCoff < (1 << (c0)) ? (12 - c0) : scaleCoff;
	for (c0 = 0; c0 < size; c0++) {
		if (scaleCoff >= 5) {
			illumXtalkCoff[c0].coffIReg = (uint8_t)((illumXtalkCoff[c0].coffI*magnitude[c0])*(1 << (scaleCoff - 5)));
			illumXtalkCoff[c0].coffQReg = (uint8_t)((illumXtalkCoff[c0].coffQ*magnitude[c0])*(1 << (scaleCoff - 5)));
		}
		else {
			illumXtalkCoff[c0].coffIReg = (uint8_t)((illumXtalkCoff[c0].coffI*magnitude[c0]) / (1 << (5 - scaleCoff)));
			illumXtalkCoff[c0].coffQReg = (uint8_t)((illumXtalkCoff[c0].coffQ*magnitude[c0]) / (1 << (5 - scaleCoff)));
		}
		illumXtalkCoff[c0].commonScale = scaleCoff;
	}
}

void OPT3101::calibrationC::findPhaseTempRegisterValues(OPT3101::phaseTempCoffC* phaseTempCoff, uint8_t size, uint16_t freqCount) {
	uint8_t scaleCoff;
	int8_t c0;
	double maxCoff = -1;
	/// <b>Algorithm of the method is as follows</b>
	for (c0 = 0; c0 < size; c0++)
		maxCoff = abs(phaseTempCoff[c0].coff) > maxCoff ? abs(phaseTempCoff[c0].coff) : maxCoff; ///* Identifies max of absolute of OPT3101::phaseTempCoffC::coff values 
	maxCoff *= 16384 / freqCount; ///* scales the max coefficient with input frequencyCount 
								  // 3200 
	scaleCoff = 0;
	for (c0 = 7; c0 >= 0; c0--)
		scaleCoff = (maxCoff*(1 << (10 - c0))) < (1 << 11) ? c0 : scaleCoff; ///* Finds a OPT3101::phaseTempCoffC::commonScale which can fit the OPT3101::phaseTempCoffC::coff to 12 bit registers
	for (c0 = 0; c0 < size; c0++) {
		phaseTempCoff[c0].coffReg = ((int16_t)(16384.0 / freqCount * phaseTempCoff[c0].coff*(1 << (10 - scaleCoff)))) & 0xFFF;///* Finds OPT3101::phaseTempCoffC::coffReg values (12 bit register values) based on OPT3101::phaseTempCoffC::coff and OPT3101::phaseTempCoffC::commonScale
		phaseTempCoff[c0].commonScale = scaleCoff;
	}
}

void OPT3101::calibrationC::report()
{
	uint8_t c0;
#ifdef OPT3101_USE_STDIOLIB
	/// <b>Algorithm of the method is as follows</b>
	printf("------------------------\n");
	printf("Calibration Class Report\n");
	printf("------------------------\n"); ///* Prints all the members and values of members on screen. 
#endif
	this->internalCrosstalk[0].report();
	
	for (c0 = 0; c0 < this->recordLength; c0++)
		this->illumCrosstalk[c0].report();
	for (c0 = 0; c0 < this->recordLength; c0++)
		this->phaseOffset[c0].report();
	for (c0 = 0; c0 < this->recordLength; c0++)
		this->illumCrosstalkTempCoff[c0].report();
	for (c0 = 0; c0 < this->recordLength; c0++)
		this->phaseTempCoff[c0].report();
	this->phaseAmbientCoff[0].report();

#ifdef OPT3101_USE_STDIOLIB
	printf("EEPROM_connected=%d\n", this->EEPROM_connected);
	printf("extTempSensor_connected=%d\n", this->extTempSensor_connected);
	printf("------------------------\n");
#endif
}

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::calibrationC::storeToFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ofstream ofs(fileName);
	ofs << this;
	ofs.close(); ///* User needs to implement file storage based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::calibrationC::loadFromFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ifstream ifs(fileName);
	ifs >> this;
	ifs.close(); ///* User needs to implement file load/restore based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::ostream & OPT3101::operator<<(std::ostream & os, const calibrationC * data)
{
	uint8_t c0;
	/// <b>Algorithm of the method is as follows</b>
	os << data->recordLength; ///* Serializes all the members and returns to the stream 
	os << data->registerAddressListSize;
	os << data->internalCrosstalk;
	os << data->EEPROM_connected;
	os << data->extTempSensor_connected;

	for (c0 = 0; c0 < data->recordLength; c0++) {
		os << &data->illumCrosstalk[c0];
		os << &data->phaseOffset[c0];
		os << &data->illumCrosstalkTempCoff[c0];
		os << &data->phaseTempCoff[c0];
		os << &data->phaseAmbientCoff[c0];
	}
	for (c0 = 0; c0 < data->registerAddressListSize; c0++)
		os << &data->registerAddressList[c0];
	return os;
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::istream & OPT3101::operator>>(std::istream & is, calibrationC * data)
{
	uint8_t c0;
	/// <b>Algorithm of the method is as follows</b>
	is >> data->recordLength; ///* Reads from stream and de-serializes all the members  
	is >> data->registerAddressListSize;
	is >> data->internalCrosstalk;
	is >> data->EEPROM_connected;
	is >> data->extTempSensor_connected;

	for (c0 = 0; c0 < data->recordLength; c0++) {
		is >> &data->illumCrosstalk[c0];
		is >> &data->phaseOffset[c0];
		is >> &data->illumCrosstalkTempCoff[c0];
		is >> &data->phaseTempCoff[c0];
		is >> &data->phaseAmbientCoff[c0];
	}
	for (c0 = 0; c0 < data->registerAddressListSize; c0++)
		is >> &data->registerAddressList[c0];
	return is;
}
#endif
