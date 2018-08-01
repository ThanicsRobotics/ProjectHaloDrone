/*
* \file register.cpp
* \author  Karthik Rajagopal <krthik@ti.com>
* \version 0.8
*
* \section COPYRIGHT
* TEXAS INSTRUMENTS TEXT FILE LICENSE
* Copyright (c) 2018 – 2018 Texas Instruments Incorporated
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
* The file contains class descriptions and methods for register control for OPT3101 device
*/


#include "register.h"

/** \brief hostController declaration
This global variable declaration with name host of hostController is used by various methods to send specific control commands to the host
*/
OPT3101::deviceRegister::deviceRegister()
{
	
}
OPT3101::deviceRegister::deviceRegister(int i2cFd, uint8_t size)
	: size(size), mI2cFd(i2cFd)
{
	/// <b>Algorithm of the method is as follows</b>
	//this->size=size; /// * Assigns size from input argument to the class member OPT3101::deviceRegister::size 
}
void OPT3101::deviceRegister::write(int32_t value){
	int8_t i;
	uint32_t mask;
	uint32_t readValue;
	uint32_t writeValue;
	uint32_t regmask;
	uint8_t size;
	/// <b>Algorithm of the method is as follows</b>
	value=(uint32_t) value; /// * Converts the input value to uint32_t 
	for(i=0;i<this->size;i++){ /// * Loops though the number of OPT3101::deviceRegister::address fields based on OPT3101::deviceRegister::size
		readValue=this->read(i); /// * Invokes hostController::readI2C() method to get register value from h/w
		size=this->msb[i]+1-this->lsb[i];
		mask=~(((uint32_t) -1)<<(size));
		writeValue=value&mask; 
		value=value>>size;
		regmask=(~((((uint32_t) -1)<<(this->msb[i]+1))^(((uint32_t) -1)<<this->lsb[i])))&0xFFFFFF; /// * Masks the bits for the new value to be written 
		writeValue=(writeValue<<this->lsb[i])|(readValue&regmask);/// * Creates a new register value to be written to h/w based on created mask and data to be written
		if(readValue!=writeValue)
			this->writeI2C(this->address[i],writeValue); /// * Initiates hostController::writeI2C function with address and new register value
	}
}

void OPT3101::deviceRegister::operator=(int32_t value){
	this->write(value);
}

uint32_t OPT3101::deviceRegister::read(){
	int8_t i;
	uint32_t readValue;
	uint32_t regmask;
	uint8_t size;
	uint8_t bitPos;
	uint32_t returnValue;

	returnValue=0;
	bitPos=0;
	/// <b>Algorithm of the method is as follows</b>
	for(i=0;i<this->size;i++){ /// * Loops though the number of OPT3101::deviceRegister::address fields based on  OPT3101::deviceRegister::size
		readValue=this->read(i); ///	* Invokes hostController::readI2C() method to get register value from h/w
		size=this->msb[i]+1-this->lsb[i];
		regmask=(((((uint32_t) -1)<<(this->msb[i]+1))^(((uint32_t) -1)<<this->lsb[i])))&0xFFFFFF; /// * Masks the bits for the register value to be reported based on register positional information
		//printf("Read mask for register 0x%02x is 0x%08x\n",this->address[i],regmask);
		returnValue|=((readValue&regmask)>>this->lsb[i])<<bitPos; /// * Assembles the value of the register
		bitPos+=size;
	}
	//printf("Resolved Value is %d",returnValue);
	return returnValue; /// * Returns the value read from the h/w for the register name specified
}

uint32_t OPT3101::deviceRegister::read(uint8_t index){
	/// <b>Algorithm of the method is as follows</b>
	return OPT3101::deviceRegister::readI2C(this->address[index]); /// * Returns the value read from the h/w for the register name specified
}

uint32_t OPT3101::deviceRegister::readI2C(uint8_t address){
	/// <b>Algorithm of the method is as follows</b>
	return host.readI2C(mI2cFd, address); /// * Invokes hostController::readI2C and returns the value read from the h/w d
}

void OPT3101::deviceRegister::writeI2C(uint8_t address,uint32_t data){
	/// <b>Algorithm of the method is as follows</b>
	host.writeI2C(mI2cFd, address, data); /// * Invokes hostController::writeI2C with address and data specified as arguments
}

//printf("Dummy I2C read  from OPT3101 register addr:0x%02x data:0x%06x\n",address,i2cReadValue);
//printf("Dummy I2C write to   OPT3101 register addr:0x%02x data:0x%06x\n",address,data);
