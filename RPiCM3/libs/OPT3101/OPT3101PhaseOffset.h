/*!
* \file OPT3101PhaseOffset.h
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
* This file contains the OPT3101::phaseOffsetC class declaration
*/

#ifndef OPT3101PHASEOFFSET_H_
#define OPT3101PHASEOFFSET_H_

#include <stdint.h>
#include "OPT3101device.h"
#include "OPT3101frameData.h"

#ifdef OPT3101_USE_STREAMLIB
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#endif


namespace OPT3101{
	/*! \class OPT3101::phaseOffsetC
	\brief Class that holds phase offset register data 

	The class contains the phase offset calibration registers and related snapshot from the device settings. 
	Phase offset calibration is an important factory calibration that needs to be performed on every unit. The calibration is important to remove all the offsets and delays and read accurate distance or phase. 
	The calibration involves having a target at a known distance and setting up offset register so that the device reads distance as expected. 
	This class besides capturing the register settings also holds critical snapshot from device for analysis and debug. 
	*/
	class phaseOffsetC {
		public:
			uint16_t phaseOffset; ///< This it the 16 bit phase offset register value to be written to the device registers for correcting the delays and offsets. 
			uint16_t referenceDistanceInCodes; ///< Captures a snapshot of the distances measured in OPT3101::frameData::phase codes used during calibration 
			uint8_t illumScale; ///< Captures the illumination scale used during phase offset measurement
			uint8_t illumDac; ///< Captures the illumination current setting used during phase offset measurement
			uint8_t illumDCdac; ///< Captures the illumination DC current setting used during phase offset measurement
			uint16_t freqCount; ///< Captures the frequency Count register OPT3101::registers::freq_count_read_reg during phase offset measurement
			uint8_t shiftIllumPhase; ///< Captures the register OPT3101::registers::shift_illum_phase during phase offset measurement
			frameData data; ///< Captures the OPT3101::frameData used to calculate the OPT3101::phaseOffsetC::phaseOffset register
			/*!
			* \brief initializes the members to 0
			*
			* \returns Nothing;
			*/
			phaseOffsetC();
			/*!
			* \brief reports members of the instance
			*  Print the members of the class instance on screen
			* \returns Nothing;
			*/
			void report();
#ifdef OPT3101_USE_STDIOLIB
			/*!
			* \brief store phase offset values to file
			*  This methods stores the phase offset values to a non-volatile memory. Storage to non-volatile memory is important to be able to calculate temperature coefficient OPT3101::crosstalkTempCoffC
			* \param[in] fileName; Path and name of the file to capture the crosstalk values to
			* \returns Nothing;
			*/
			void storeToFile(char *fileName);
			/*!
			* \brief load phase offset values from file
			*  This methods loads the phase offset values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important to be able to calculate temperature coefficient OPT3101::crosstalkTempCoffC
			* \param[in] fileName; Path and name of the file from where the crosstalk values are loaded
			* \returns Nothing;
			*/
			void loadFromFile(char *fileName);
#endif
#ifdef OPT3101_USE_STREAMLIB
			/*!
			* \brief Operator overload to store class contents to a file
			*  Casts all the class members for file storage
			* \param[out] os; os is data stream to serialize data
			* \param[in] data; data is pointer to the class to be serialized and stored
			* \returns std::ostream; Serialized std::ostream to be written to a file
			*/
			friend std::ostream& operator<<(std::ostream& os, const phaseOffsetC *data);
			/*!
			* \brief Operator overload to load class contents to from a file
			*  Retreives all the class members from a stored file
			* \param[in] is; is input stream from where the data is loaded
			* \param[out] data; data is pointer to the class to be restored
			* \returns std::istream; Serialized Input stream loaded from file
			*/
			friend std::istream& operator>>(std::istream& is, phaseOffsetC *data);
#endif
	};
}
#endif /* OPT3101PHASEOFFSET_H_ */
