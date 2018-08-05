/*!
* \file OPT3101frameData.h
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
* This file contains the OPT3101::frameData class declaraction
*/

#ifndef OPT3101FRAMEDATA_H_
#define OPT3101FRAMEDATA_H_

#include <stdint.h>
#include "hostController.h"
#include "OPT3101device.h"

#ifdef OPT3101_USE_STREAMLIB
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#endif


namespace OPT3101 {
	/** \brief OPT3101::device declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class device;
	
	/*! \class OPT3101::frameData
	\brief Class the data output from OPT3101 device 

	This is a class that holds output data from OPT3101 AFE. 
	Measurements by AFE OPT3101 device are stores in registers with address 0x08, x09 & 0x0A. Difference sections of these registers hold different measurements like OPT3101::frameData::phase, OPT3101::frameData::amplitude <br>
	This class provides an abstraction where the registers are read, relevant portions of the registers are extracted and assigned to members with meaningful names.
	This provides a functional abstraction to the user. 
	*/

	class frameData {
		public:
			uint32_t phase; ///< phase value measured by the AFE. Actually 16 bit unsigned value upscaled to 17 bits with 1 bit of frameData::phaseovl flag added to the MSB. Same as OPT3101::registers::phase_out
			uint32_t distance;
			bool phaseovl; ///< phase overload flag, specifying if phase value detected exceeds 16 bits. (greater than 65535). OPT3101::registers::.phase_overflow
			bool phaseovlf2; ///< phase overload flag for frequency 2, specifying if phase value detected exceeds 16 bits. (greater than 65535). Same as OPT3101::registers::phase_overflow_f2
			bool ambovl; ///< ambient over load flag which flags if the AFE is saturated due to very high ambient condition. Same as OPT3101::registers::amb_ovl_flag
			bool illumDac; ///< flag that specifies which of the auto-hdr register config from where the measurement is made. Same as OPT3101::registers::hdr_mode 
			uint8_t ledChannel; ///< value that specifies from which channel the measurement is made. Same as OPT3101::registers::tx_channel
			bool frameStatus; ///< flag that specifies if the measurement is a valid data. Same as OPT3101::registers::frame_status
			bool dealiasFreq; ///< flag that specifies if the measurement is done with fundamental frequency of de-alias frequency. Same as OPT3101::registers::mod_freq
			uint8_t frameCounter; ///< Frame counter which counts every data ready. Same as combination of OPT3101::register::frame_count0 , OPT3101::registers::frame_count1 and OPT3101::registers::frame_count2
			uint16_t amplitude; ///< amplitude value measured by AFE. 15 bit representation of the signal. Same as OPT3101::registers::amp_out
			bool sigovl; ///< flag that specifies if the AFE is saturated due to signal. Same as OPT3101::registers::sig_ovl_flag
			uint8_t dealiasBin; ///< value that specifies the de-aliased bin number. Same as OPT3101::registers::dealias_bin
			uint16_t ambient; ///< value specifies the ambient light measured by the ambient cancellation block. Same as OPT3101::registers::amb_data
			uint16_t temp; ///< value of internal temp sensor read from register 0x0A. Same as OPT3101::registers::tmain . This is updated every data ready.
			uint16_t tmain; ///< value of internal temp sensor read from register OPT3101::registers::tmain
			uint16_t tillum; ///< value of external temp sensor read using the I2C Master. Same as OPT3101::registers::tillum
			/*!
			* \brief initializes the members to 0
			*
			* \returns Nothing;
			*/
			frameData();
			/*!
			* \brief captures measurement data from the device 
			*  This method invokes I2C transactions to capture data from the OPT3101 device registers 0x08, 0x09 and 0x0A. Extracts and assigns members their respective values based on their positions in the registers. 
			* \param[in] host; Pointer to the class hostController used to directly invoke I2C transactions bypassing OPT3101::registers or OPT3101::device class
			* \param[in] dev; Pointer to the class OPT3101::device used to read register(s) from device
			* \returns Nothing;
			*/
			void capture(hostController *host, device *dev);
			/*!
			* \brief reports members of the instance
			*  Print the members of the class instance on screen
			* \returns Nothing;
			*/
			void report();
			/*!
			* \brief calculates mean and assigns to self
			*  Given a set on input OPT3101::frameData the method calculates the mean frame and assigns to self
			* \param[in] inputData; Pointer to the class OPT3101::frameData instance with multiple frame data
			* \param[in] nFrames; Number of measurements to step though in the inputData to calculate mean 
			* \returns Nothing;
			*/
			void populateWithMean(frameData *inputData, uint16_t nFrames);
#ifdef OPT3101_USE_STDIOLIB			
			/*!
			* \brief saves phase temp coff values to file
			*  This methods saves the phase temp coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
			* \param[in] fileName; Path and name of the file to store
			* \returns Nothing;
			*/
			void storeToFile(char *fileName);
			/*!
			* \brief load phase temp coff values from file
			*  This methods loads the phase temp coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
			* \param[in] fileName; Path and name of the file from where to load
			* \returns Nothing;
			*/
			void loadFromFile(char *fileName);
#endif
#ifdef OPT3101_USE_STREAMLIB
			/*!
			* \brief Operator overload to store class contents to a file
			*  Casts all the class members for file storage
			* \param[out] os; os is data stream to serialize data
			* \param[in] data; data is pointer to the class to be serlialized and stored
			* \returns std::ostream; Serialized std::ostream to be written to a file
			*/
			friend std::ostream& operator<<(std::ostream& os, const frameData *data);
			/*!
			* \brief Operator overload to load class contents to from a file
			*  Retreives all the class members from a stored file
			* \param[in] is; is input stream from where the data is loaded
			* \param[out] data; data is pointer to the class to be restored
			* \returns std::istream; Serialized Input stream loaded from file
			*/
			friend std::istream& operator>>(std::istream& is, frameData *data);
#endif

		};
}
#endif /* OPT3101FRAMEDATA_H_ */
