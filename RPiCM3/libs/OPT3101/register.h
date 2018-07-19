/*!
* \file register.h
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
* The file contains class descriptions and methods for register control for OPT3101 device
*/

#ifndef REGISTER_H_
#define REGISTER_H_

#include <stdint.h>
#include "hostController.h"


/*! \class OPT3101::deviceRegister
\brief Class that contains positional information for registers in the register map  

Class that contains positional information for a register in OPT3101 device.
OPT3101 has 256 registers each 24 bits wide (OPT3101::deviceRegister::msb of 23 and OPT3101::deviceRegister::lsb of 0 with OPT3101::deviceRegister::address varying from 0 to 255)
Bits are groups together and are addressed with a name. Registers have names, placed at different segments of the register map.
For eg: tmain (temperature of main temp sensor) of OPT3101 occupies location with address 10 from msb of 23 bits to 12 bits
in other words OPT3101::deviceRegister::address=10 OPT3101::deviceRegister::msb=23 OPT3101::deviceRegister::lsb=12
This class helps resolve programmers to read and write I2C to OPT3101 device by writing program code with higher level of abstraction with register names rather than dealing with positions of registers
OPT3101::deviceRegister class helps resolve MSB, LSB and ADDRESS fields for a register by its name.  
Using this class gives user a functional name for each register making the register read and write more readable and meaningful. 
In case of OPT3101 device there are some registers which span across 2 address positions, hence the OPT3101::deviceRegister::address, OPT3101::deviceRegister::msb and OPT3101::deviceRegister::lsb have 2 allocations each. In most register cases 
they may not end up using both the allocations. The actual usage can be found using OPT3101::deviceRegister::size member <br>
Example of register which has more than 1 address is OPT3101::registers::amplitude_min_thr which spans across register 0x10 from bits 23:16 and 0x11 from bites 23:16 
*/
namespace OPT3101{
class deviceRegister{
	private:
		/*!
		* \brief Function to resolve actual data to be written before calling writeI2C function
		*
		* \param[in] value; This resolves the value to number of physical i2c address and register writes based on the MSP, LSB and ADDRESS members.
		* \return Nothing
		*/
		void write(int32_t registerData);
		/*!
		* \brief Function read I2C from the specified address index
		* Intended as internal function to perform I2C read for a particular address index for the register
		* \param[in] addressIndex 
		* \return Nothing
		* 
		*/
		uint32_t read(uint8_t addressIndex);
		/*!
		* \brief Function that invokes I2C read transaction for the register given the address
		* Function that invokes I2C transaction for the register given the address
		* \param[in] address
		* \return value read for the address specified 
		* 
		*/
		uint32_t readI2C(uint8_t address);
		/*!
		* \brief Function that invokes I2C write transaction for the register given the address and the data 
		* Function that invokes I2C transaction for the register given the address and data
		* \param[in] address
		* \param[in] data
		* \return Nothing
		* 
		*/
		void writeI2C(uint8_t address, uint32_t data);
	public:
		uint8_t msb[2]; ///< This is the MSB position of this register. The register occupies the OPT3101::deviceRegister::address from OPT3101::deviceRegister::msb to OPT3101::deviceRegister::lsb
		uint8_t lsb[2]; ///< This is the LSB position of this register. The register occupies the OPT3101::deviceRegister::address from OPT3101::deviceRegister::msb to OPT3101::deviceRegister::lsb
		uint8_t address[2]; ///< This is the ADDRESS of this register. The register occupies the OPT3101::deviceRegister::address from OPT3101::deviceRegister::msb to OPT3101::deviceRegister::lsb
		uint8_t size; ///< This specifies how many ADDRESS does this register span across. For eg: There are registers which span multiple address locations in chunks. 
		/*!
		* \brief Constructor for class OPT3101::deviceRegister
		*  Constructor allocated size to each register instance on construction
		* \param[in] size; size (typically 1 or 2 bytes) determines the number of segments that the register is divided in to.
		*
		*/
		deviceRegister(uint8_t size);
		/*!
		* \brief Operator overload for '='.
		*
		* This makes calling this class simpler. dev.register=value will resolve the register address and value and invoke hostController::writeI2C method 
		* With proper implementation of the hostController::writeI2C methods the h/w would receive resolved I2C WRITE commands 
		* The write operations are read modify writes to make the system robust
		*  A single call of this method could invoke up to 2 I2C READ and 2 I2C WRITE transaction depending on the register
		* \param[in] value; value to be set to register
		* \return Nothing
		* 
		*/
		void operator=(int32_t value);
		/*!
		* \brief Function called to read the value of register 
		*
		*  This method provides an abstraction for register to be used seamlessly with code for read register operations. 
		*  hostController::readI2C method is invoked with OPT3101::deviceRegister::address fields and the resulting register value is combined as per the register positional information and reported as a uint32_t number
		*  A single call of this method could invoke up to 2 I2C READ transaction depending on the register
		* \return value; value is reading of register from OPT3101 device
		*/
		uint32_t read();
};
}
#endif /* REGISTER_HPP_ */
