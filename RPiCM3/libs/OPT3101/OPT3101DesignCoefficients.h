/*!
* \file OPT3101DesignCoefficients.h
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
* This file contains class declarations OPT3101::crosstalkTempCoffC OPT3101::phaseTempCoffC OPT3101::phaseAmbientCoffC . These are design coefficients required for various calibration steps. 
*/

#ifndef OPT3101DESIGNCOEFFICIENTS_H_
#define OPT3101DESIGNCOEFFICIENTS_H_

#include <stdint.h>
#include "OPT3101device.h"
#include "OPT3101Crosstalk.h"

#ifdef OPT3101_USE_STREAMLIB
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#endif 

/** \brief OPT3101::device declaration
This is declared here to avoid cyclic reference of classes
*/
//class OPT3101::;


namespace OPT3101 {
	/** \brief OPT3101::frameData declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class device;
	/** \brief OPT3101::device declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class frameData;	
	/** \brief OPT3101::crosstalkC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class crosstalkC;
	/*! \class OPT3101::crosstalkTempCoffC
	\brief Class that holds crosstalk temperature coefficient related registers and values.

	This is a class container that can hold crosstalk temperature coefficients and related register values.  OPT3101 device has different crosstalk phenomenon which needs to be calibrated and compensated<br>
	Refer to documentation for OPT3101::crosstalkC class documentation<br>
	Crosstalk varies with temperature which needs to be calibration and compensated to improve the system performance. 
	Illum crosstalk temperature variation needs to be evaluated and a compensation coefficients have to be calculated.
	OPT3101 device has internal registers to correct and compensate for illum crosstalk temperature variation. This class acts as a temporary storage of crosstalk temp coefficients and related registers.
	Calculations have to be made on the crosstalk values, coefficients to transform them to register settings. Instances of OPT3101::crosstalkC taken at different temperate will be used to calculate these coefficients and registers. 
	*/
	class crosstalkTempCoffC {
		public:
			double coffI; ///< Coefficient for I Component with floating point precision.Units for this measurement is crosstalk OPT3101::crosstalkC::I (24 bit level) codes change per unit of temperature sensor codes (OPT3101::registers::tmain) per OPT3101::crosstalkC::magnitude() of crosstalk 
			double coffQ; ///< Coefficient for Q Component with floating point precision.Units for this measurement is crosstalk OPT3101::crosstalkC::Q (24 bit level) codes change per unit of temperature sensor codes (OPT3101::registers::tmain) per OPT3101::crosstalkC::magnitude() of crosstalk 
			uint8_t coffIReg; ///< Value of OPT3101::crosstalkTempCoffC::coffI represented as a 8 bit number to be written to illum crosstalk temp compensation registers 
			uint8_t coffQReg; ///< Value of OPT3101::crosstalkTempCoffC::coffQ represented as a 8 bit number to be written to illum crosstalk temp compensation registers 
			uint8_t commonScale; ///< Value of common scale for register coefficients achieved using different instances of OPT3101::crosstalkTempCoffC. Calculated by OPT3101::calibration::findCrosstalkTempRegisterValues during production calibration. Used while loading a set of TX and current configurations to the OPT3101 device during production calibration
			/*!
			* \brief initializes the members to 0
			*
			* \returns Nothing;
			*/
			crosstalkTempCoffC();
			/*!
			* \brief calculates the crosstalk temp coff
			*  
			* \param[in] illumXtalk0; Pointer to illum crosstalk OPT3101::crosstalkC instance measured at a lower  temperature 
			* \param[in] illumXtalk1; Pointer to illum crosstalk OPT3101::crosstalkC instance measured at a higher temperature 
			* \returns Nothing;
			*/
			void calculateCoff(crosstalkC *illumXtalk0,crosstalkC *illumXtalk1);
			/*!
			* \brief reports members of the instance
			*  Print the members of the class instance on screen
			* \returns Nothing;
			*/
			void report();
#ifdef OPT3101_USE_STDIOLIB
			/*!
			* \brief saves crosstalk temp coff values to file
			*  This methods saves the crosstalk temp coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
			* \param[in] fileName; Path and name of the file to store
			* \returns Nothing;
			*/
			void storeToFile(char *fileName);
			/*!
			* \brief load crosstalk temp coff values from file
			*  This methods loads the crosstalk temp coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
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
			* \param[in] data; data is pointer to the class to be serialized and stored
			* \returns std::ostream; Serialized std::ostream to be written to a file
			*/
			friend std::ostream& operator<<(std::ostream& os, const crosstalkTempCoffC *data);
			/*!
			* \brief Operator overload to load class contents to from a file
			*  Retrieves all the class members from a stored file
			* \param[in] is; is input stream from where the data is loaded
			* \param[out] data; data is pointer to the class to be restored
			* \returns std::istream; Serialized Input stream loaded from file
			*/
			friend std::istream& operator>>(std::istream& is, crosstalkTempCoffC *data);
#endif
	};
	/*! \class OPT3101::phaseTempCoffC
	\brief Class that holds phase temperature coefficient related registers and values.

	This is a class container that can hold phase/phase offset temperature coefficients and related register values.  OPT3101 AFE phase measurement drifts over temperature. This drift is primarily due to change in delay in the path of the TX channels.
	The drift will show up in OPT3101::frameData::phase measurement which will translate to error in measurement.<br>
	OPT3101 has mechanism to compensate the output for this drift, for which phase temperature coefficient is required. 
	Either internal temp sensor or external temperature sensor can be used to compensate for this. 
	This class acts as a temporary storage of phase temp coefficients and related registers.
	Calculations have to be made on the phase offsets and frame data, coefficients to transform them to register settings. Instances of OPT3101::frameData taken at different temperature will be used to calculate these coefficients and registers.
	*/
	class phaseTempCoffC {
		public: 
			double coff; ///< Coefficient stores in floating point precision.Units for this measurement is  OPT3101::frameData::phase codes (16 bit level) codes change per unit of temperature sensor codes (OPT3101::registers::tmain or OPT3101::registers::tillum based on OPT3101::phaseTempCoffC::istMainCoff
			uint16_t coffReg; ///< Coefficient value of OPT3101::phaseTempCoffC::coff represented as 16bit register setting
			uint8_t commonScale; ///< Common scale of OPT3101::phaseTempCoffC::coffReg calculated based on various OPT3101::phaseTempCoffC class instances. This is calculated by OPT3101::calibrationC::findPhaseTempRegisterValues during production calibration
			bool istMainCoff; ///< Flag which determines if the coefficient was obtained using OPT3101::registers::tmain difference or OPT3101::registers::tillum 
			/*!
			* \brief initializes the members to 0
			*
			* \returns Nothing;
			*/
			phaseTempCoffC();
			/*!
			* \brief calculates the phase temp coff
			*
			* \param[in] frameData0; Pointer to frame data OPT3101::frameData instance measured at a lower  temperature
			* \param[in] frameData1; Pointer to frame data OPT3101::frameData instance measured at a higher temperature
			* \param[in] istMainCoff; istMainCoff specifies if the calculation should be done with Tmain Coff 
			* \returns Nothing;
			*/
			void calculateCoff(frameData *frameData0,frameData *frameData1,bool istMainCoff=true);
			/*!
			* \brief reports members of the instance
			*  Print the members of the class instance on screen
			* \returns Nothing;
			*/
			void report();
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
			/*!
			* \brief Operator overload to store class contents to a file
			*  Casts all the class members for file storage
			* \param[out] os; os is data stream to serialize data
			* \param[in] data; data is pointer to the class to be serialized and stored
			* \returns std::ostream; Serialized std::ostream to be written to a file
			*/
#endif
#ifdef OPT3101_USE_STREAMLIB
			friend std::ostream& operator<<(std::ostream& os, const phaseTempCoffC *data);
			/*!
			* \brief Operator overload to load class contents to from a file
			*  Retrieves all the class members from a stored file
			* \param[in] is; is input stream from where the data is loaded
			* \param[out] data; data is pointer to the class to be restored
			* \returns std::istream; Serialized Input stream loaded from file
			*/
			friend std::istream& operator>>(std::istream& is, phaseTempCoffC *data);
#endif
	};
	/*! \class OPT3101::phaseAmbientCoffC
	\brief Class that holds ambient phase  coefficient related registers and values.

	This is a class container that can hold phase offset ambient coefficients and related registers. 
	Photo diodes has a phenomenon where the phase delay of modulated current generated by photo diode is dependent on the amount of ambient light falling on the photo diode. 
	The effect observed on the system is that the phase reported by the system will be dependent on the amount of ambient light of the environment. <br>
	OPT3101 has compensation block for the same, which needs coefficients to be  programmed. OPT3101 has a look up table based correction since the phenomenon in non-linear. 
	The look-up table is a 4 way split PWL correction with 4 independent coefficients. The PWL correlation is between ambient ADC output and phase measured<br>
	This class has members to contain the coefficients and the split points. 
	*/
	class phaseAmbientCoffC {
		public:
			double coff[4]; ///< Coefficients stored in floating point precision. Since there are 4 sections the OPT3101::phaseAmbientCoffC::coff is array of 4 elements. Units for this is OPT3101::frameData::phase codes per unit OPT3101::frameData::ambient codes 
			uint8_t coffReg[4]; ///< Coefficients value of  OPT3101::phaseAmbientCoffC::coff represented as 8 bit register values calculated by  OPT3101::phaseAmbientCoffC::calculateCoff
			uint16_t splitsReg[3]; ///< Register entries for the split point for the PWL. Since there are 4 sections where are 3 split points to be set. Units for this is OPT3101::frameData::ambient codes
			uint8_t commonScale; ///< Since there are 4 OPT3101::phaseAmbientCoffC::coff to represent them as 8 bit register values a common scaling factor is required. This is calculated by method  OPT3101::phaseAmbientCoffC::calculateCoff
			/*!
			* \brief initializes the members to 0
			*
			* \returns Nothing;
			*/
			phaseAmbientCoffC();
			/*!
			* \brief Calculates the register settings for the phase ambient coefficient
			* Calculates OPT3101::phaseAmbientCoffC::coffReg and OPT3101::phaseAmbientCoffC::commonScale based on OPT3101::phaseAmbientCoffC::coff 
			* \param[in] freqCounter; freqCounter is the count which represents the frequency from the Frequency correction block. This is read from register OPT3101::registers::freq_count_read_reg
			* \returns Nothing;
			*/
			void calculateCoff(uint16_t freqCounter);
			/*!
			* \brief reports members of the instance
			*  Print the members of the class instance on screen
			* \returns Nothing;
			*/
			void report();
#ifdef OPT3101_USE_STDIOLIB
			/*!
			* \brief saves phase ambient coff values to file
			*  This methods saves the phase ambient coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
			* \param[in] fileName; Path and name of the file to store
			* \returns Nothing;
			*/
			void storeToFile(char *fileName);
			/*!
			* \brief load phase ambient coff values from file
			*  This methods loads the phase ambient coff values to a non-volatile memory. Storage/Restoration to/from non-volatile memory is important for factory calibration
			* \param[in] fileName; Path and name of the file from where to load
			* \returns Nothing;
			*/
			void loadFromFile(char *fileName);
			/*!
			* \brief Operator overload to store class contents to a file
			*  Casts all the class members for file storage
			* \param[out] os; os is data stream to serialize data
			* \param[in] data; data is pointer to the class to be serialized and stored
			* \returns std::ostream; Serialized std::ostream to be written to a file
			*/
#endif
#ifdef OPT3101_USE_STREAMLIB
			friend std::ostream& operator<<(std::ostream& os, const phaseAmbientCoffC *data);
			/*!
			* \brief Operator overload to load class contents to from a file
			*  Retrieves all the class members from a stored file
			* \param[in] is; is input stream from where the data is loaded
			* \param[out] data; data is pointer to the class to be restored
			* \returns std::istream; Serialized Input stream loaded from file
			*/
			friend std::istream& operator>>(std::istream& is, phaseAmbientCoffC *data);
#endif
	};
}

#endif /* OPT3101DESIGNCOEFFICIENTS_H_ */
