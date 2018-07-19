/*!
* \file OPT3101Crosstalk.h
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
* This file contains the OPT3101::crosstalkC class declaration 
*/

#ifndef OPT3101CROSSTALK_H_
#define OPT3101CROSSTALK_H_

#include <stdint.h>
#include <cmath>

#include "OPT3101device.h"
#include "hostController.h"

#ifdef OPT3101_USE_STREAMLIB
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#endif

//#include <windows.h>


namespace OPT3101 {
	/** \brief OPT3101::device declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class device;

	/*! \class OPT3101::crosstalkC
	\brief Class that holds crosstalk related registers and values.

	This is a class container that can hold crosstalk values.  OPT3101 device has different crosstalk phenomenon which needs to be calibrated and compensated<br>
	There are primarily 2 types of crosstalk phenomenon that need to be compensated or calibrated out to achieve good system performance. <br>
	-# Internal crosstalk
		-#	This is defined as the amount of signal measured by AFE when there the TX channels are turned OFF. Internal crosstalk phenomenon occurs due to coupling from power supply and ground.<br>
		-#  This phenomenon is independent of TX channel or configuration since the TX channels are off.
	-# Illum crosstalk
		-#	This is defined as the amount of signal measured by AFE when the TX channels are turned ON and the system is pointing to infinity or with photo diode covered.
		-#	This phenomenon is highly dependent on TX channel and the current settings on each TX.

	OPT3101 device has internal registers to correct and compensate for different crosstalk phenomenon. This class acts as a temporary storage of crosstalk values measured.
	Calculations have to be made on the crosstalk values to transform them to register settings. Besides that other calibrationC coefficients like OPT3101::crosstalkTempCoffC will be calculated.
	*/
	class crosstalkC {
	public:
		int16_t I; ///< 16bit register setting for In phase component of the crosstalk 
		int16_t Q; ///< 16bit register setting for Quadrature phase component of the crosstalk 
		uint8_t  xtalkScale; ///< Crosstalk registers in OPT3101 device are 24 bit where as compensation registers are 16 bit. The scale parameter captures the scaling of the 24bit register to fit in to 16 bits
		bool illumXtalk; ///< flag to capture if the crosstalk values belong to internal crosstalk (false) or illum crosstalk (true)
		uint8_t illumScale; ///< Member that captures the snapshot of illumination current multiplier scale register  during crosstalk measurement for analysis 
		uint8_t illumDac; ///< Member that captures the snapshot of illumination current setting register  during crosstalk measurement for analysis 
		uint8_t illumDCdac; ///< Member that captures the snapshot of illumination DC current setting register  during crosstalk measurement for analysis 
		uint16_t tmain; ///< Member that captures the snapshot of temperature of main temp sensor during crosstalk measurement
		uint16_t tillum; ///< Member that captures the snapshot of temperature of external temp sensor during crosstalk measurement
		uint8_t shiftIllumPhase; ///< Member that captures the snapshot of shift illum phase during crosstalk measurement
		uint8_t commonScale; ///< Member that captures the common scale to be applied to all the crosstalk values to be applied OPT3101 device during facotry calibrationC. This value is calculated as a common value based on a bunch of OPT3101::crosstalkC instances
		/*!
		* \brief Constructor for class OPT3101::crosstalkC
		*  Constructor initializes the OPT3101::crosstalkC::illumXtalk parameter to a default false
		*/
		crosstalkC();
		/*!
		* \brief calculates magnitude
		*  This method calculates the magnitude of crosstalk represented in codes. Typical and good values for internal crosstalk is around 30. Typical and good values for illum crosstalk for highest current (170mA) < 300 codes.
		* \returns magnitude of crosstalk;
		*/
		double magnitude();
		/*!
		* \brief reads the device 24 bit crosstalk registers
		*  This method reads the 24 bit crosstalk registers OPT3101::registers::iphase_xtalk and OPT3101::registers::qphase_xtalk. Translates them to 16 bit values and captures to OPT3101::crosstalkC::I OPT3101::crosstalkC::Q and OPT3101::crosstalkC::xtalkScale
		* \param[in] dev; The dev is pointer to class OPT3101::device . This is required to realize I2C transactions and register map
		* \returns Nothing;
		*/
		void readCrosstalkRegisters(device *dev);
		/*!
		* \brief reads the temperature reading from main temp sensor
		*  This method reads the main temp sensor  register OPT3101Register::tmain and captures to  OPT3101::crosstalkC::tmain
		* \param[in] dev; The dev is pointer to class OPT3101::device . This is required to realize I2C transactions and register map
		* \returns Nothing;
		*/
		void readTemperatureMain(device *dev);
		/*!
		* \brief reads the temperature reading from external temp sensor
		*  This method reads the main temp sensor  register OPT3101Register::tillum and captures to  OPT3101::crosstalkC::tillum
		* \param[in] dev; The dev is pointer to class OPT3101::device . This is required to realize I2C transactions and register map
		* \returns Nothing;
		*/
		void readTemperatureIllum(device *dev);
#ifdef OPT3101_USE_STDIOLIB		
		/*!
		* \brief store crosstalk values to file
		*  This methods stores the crosstalk values to a non-volatile memory. Storage to non-volatile memory is important to be able to calculate temperature coefficient OPT3101::crosstalkTempCoffC
		* \param[in] fileName; Path and name of the file to capture the crosstalk values to
		* \returns Nothing;
		*/
		void storeToFile(char *fileName);
		/*!
		* \brief load crosstalk values from file
		*  This methods loads the crosstalk values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important to be able to calculate temperature coefficient OPT3101::crosstalkTempCoffC
		* \param[in] fileName; Path and name of the file from where the crosstalk values are loaded
		* \returns Nothing;
		*/
		void loadFromFile(char *fileName);
#endif
		/*!
		* \brief reports members of the instance
		*  Print the members of the class instance on screen 
		* \returns Nothing;
		*/
		void report();
#ifdef OPT3101_USE_STREAMLIB
		/*!
		* \brief Operator overload to store class contents to a file 
		*  Casts all the class members for file storage
		* \param[out] os; os is data stream to serialize data
		* \param[in] data; data is pointer to the class to be serialzied and stored
		* \returns std::ostream; Serialized std::ostream to be written to a file 
		*/
		friend std::ostream& operator<<(std::ostream& os, const crosstalkC *data);
		/*!
		* \brief Operator overload to load class contents to from a file
		*  Retreives all the class members from a stored file 
		* \param[in] is; is input stream from where the data is loaded
		* \param[out] data; data is pointer to the class to be restored
		* \returns std::istream; Serialized Input stream loaded from file 
		*/
		friend std::istream& operator>>(std::istream& is,       crosstalkC *data);
#endif
	};
}

#endif /* OPT3101CROSSTALK_H_ */
