/*!
* \file OPT3101device.h
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
* This file contains the OPT3101::device class declaration. This is the master class that integrates all the components of the SDK
*/


#ifndef OPT3101DEVICE_H_
#define OPT3101DEVICE_H_

#include "OPT3101RegisterDefinition.h"
#include "OPT3101Calibration.h"
#include "OPT3101DesignCoefficients.h"
#include "OPT3101PhaseOffset.h"
#include "OPT3101frameData.h"

/*! \namespace OPT3101
\brief OPT3101 name space is used to classify and segregate all OPT3101SDK related methods and class definitions. 

OPT3101SDK is intended to be used on various different host configurations. Having a segregated name space helps isolate and manage the structure of the classes and methods better. 
*/
namespace OPT3101 {

	/** \brief OPT3101::crosstalkC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class crosstalkC;
	/** \brief OPT3101::phaseOffsetC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class phaseOffsetC;
	/** \brief OPT3101::crosstalkTempCoffC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class crosstalkTempCoffC;
	/** \brief OPT3101::phaseTempCoffC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class phaseTempCoffC;
	/** \brief OPT3101::phaseAmbientCoffC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class phaseAmbientCoffC;
	/** \brief OPT3101::calibrationC declaration
	This is declared here to avoid cyclic reference of classes
	*/
	class calibrationC;

	/*! \class OPT3101::device
	\brief This is the master class that integrates all the OPT3101SDK functions

	OPT3101::device is at the apex of the OPT3101SDK providing all the methods and functionality required for a full system bring up calibration and debug<br>
	The register control for the device, calibration routines are instantiated in this call for user assess. 
	*/
	class device {
		private:
			
			void writeDataToEEPROM(uint8_t location, uint8_t data);
			void measurePhaseOffset(phaseOffsetC *phaseOffset, uint16_t refDistanceInCodes);
			void measureIllumCrosstalk(crosstalkC *illumXtalk);
			uint8_t determineConfigCount();
		public:
			int i2cFd;
			/*!
			* \brief Constructor for class OPT3101::device
			* Constructor definition for which is generated by OPT3101 configurator tool. Mainly the OPT3101::device::configurationFlags_isTXChannelActive and OPT3101::device::configurationFlags_isRegisterSetActive are set in the constructor
			* \returns Nothing;
			*/
			device(int deviceAddress);
			~device();
			registers reg; ///< Instance of the register class OPT3101::registers control for the device dev.reg.{registerName}=value initiates I2C register writes. 
			calibrationC *calibration; ///< Instance of the calibration class OPT3101::calibrationC. This instance acts a staging area to load, save and calculate calibration coefficients during initial bring up, debug and factory calibration steps. 
			/*!
			* \brief resets device
			* Template function which involves hostController::resetDevice method 
			* \returns Nothing;
			*/
			void reset();
			/*!
			* \brief initializes the device 
			* Definition for the method is generated by the OPT3101 configuration generator tool. This method consists for all the register writes and sequences required to bring the device to desired operating condition
			* \returns Nothing;
			*/
			void initialize();

			/*!
			* \brief Measures and corrects internal crosstalk 
			* Method abstracts the internal crosstalk measurement. There are bunch of steps and register read/writes involved to measure and correct internal crosstalk which this method handles. This method is recommended to be run every time after power-up
			* \param[out] internalXtalk; internalXtalk in pointer to the instance of class OPT3101::crosstalkC where the internal crosstalk values gets loaded. This is used to understand and analyze system performance. 
			* \returns Nothing;
			*/
			void measureAndCorrectInternalCrosstalk(crosstalkC *internalXtalk);
			/*!
			* \brief Measures illum crosstalk for a given channel and register set
			* Method abstracts the illum crosstalk measurement.  The measurement of illum crosstalk for a given channel and register set involves configuring the device with bunch of register writes and reads. This method abstracts the procedure to make it simple for users to adopt<br>
			* This calibration procedure demands a very specific target/environmental requirement. This method invokes the appropriate template method from the envController class. Users needs to implement the method which achieves the specific requirement
			* \param[out] illumXtalk; illumXtalk in pointer to the instance of class OPT3101::crosstalkC where the illum crosstalk values gets loaded. This is used to understand and analyze system performance besides being used for temp coff estimation and factory calibration.
			* \param[in] txChannel; txChannel is the number denoting the channel for which the crosstalk measurement has to be performed. For eg: illumXtalk=0 does measurement for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which the illum crosstalk measurement needs to be performed. For eg: registerSet='h' would perform measurement on registers related to illum_dac_h_tx*
			* \param[in] shiftIllumPhase; shiftIllumPhase is the register value to set the OPT3101::registers::shift_illum_phase. This modifies the relative phase between the TX channels and the AFE helping perform non-linear calibration without moving the target.
			* \returns Nothing;
			*/
			void measureIllumCrosstalk(crosstalkC *illumXtalk, uint8_t txChannel, char registerSet, uint8_t shiftIllumPhase = 0);
			/*!
			* \brief Measures phase offset for a given channel and register set
			* Method abstracts the phase offset measurement.  The measurement of phase offset is for a given channel and register set involves configuring the device with bunch of register writes and reads. This method abstracts the procedure to make it simple for users to adopt<br>
			* This calibration procedure demands a very specific target/environmental requirement. This method invokes the appropriate template method from the envController class. Users needs to implement the method which achieves the specific requirement
			* \param[out] phaseOffset; phaseOffset in pointer to the instance of class OPT3101::phaseOffsetC where the phase offset values gets loaded. This is used to understand and analyze system performance besides being used for temp coff estimation and factory calibration.
			* \param[in] txChannel; txChannel is the number denoting the channel for which the measurement has to be performed. For eg: illumXtalk=0 does measurement for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which the measurement needs to be performed. For eg: registerSet='h' would perform measurement on registers related to illum_dac_h_tx*
			* \param[in] refDistanceInCodes; refDistanceInCodes is reference distance of the target specified in codes OPT3101::frameData::phase codes. The conversion for the distance in mm to codes documented in the method OPT3101::device::calibrationSession_perDesignCalibrationPhaseTemp
			* \param[in] shiftIllumPhase; shiftIllumPhase is the register value to set the OPT3101::registers::shift_illum_phase. This modifies the relative phase between the TX channels and the AFE helping perform non-linear calibration without moving the target.
			* \returns Nothing;
			*/
			void measurePhaseOffset(phaseOffsetC *phaseOffset, uint8_t txChannel, char registerSet, uint16_t refDistanceInCodes, uint8_t shiftIllumPhase = 0);
			/*!
			* \brief load illum crosstalk from OPT3101::crosstalkC class to the h/w 
			* Method loads the registers from OPT3101::crosstalkC instance specified to the h/w to specified txChannel and registerSet
			* \param[out] illumXtalk; illumXtalk is pointer to the instance of OPT3101::crosstalkC class. The register values form the instance are loaded to the hardware to specified txChannel and registerSet
			* \param[in] txChannel; txChannel is the number denoting the channel to which the registers need to be loaded For eg: illumXtalk=0 loads for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which register needs to be loaded. For eg: registerSet='h' would load on to registers related to illum_dac_h_tx*
			* \returns Nothing;
			*/
			void loadIllumCrosstalk(crosstalkC *illumXtalk, uint8_t txChannel, char registerSet);
			/*!
			* \brief load phase offset from OPT3101::phaseOffsetC class to the h/w
			* Method loads the registers from OPT3101::phaseOffsetC instance specified to the h/w to specified txChannel and registerSet
			* \param[in] phaseOffset; phaseOffset is pointer to the instance of OPT3101::phaseOffsetC class. The register values form the instance are loaded to the hardware to specified txChannel and registerSet
			* \param[in] txChannel; txChannel is the number denoting the channel to which the registers need to be loaded For eg: illumXtalk=0 loads for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which register needs to be loaded. For eg: registerSet='h' would load on to registers related to illum_dac_h_tx*
			* \returns Nothing;
			*/
			void loadPhaseOffset(phaseOffsetC *phaseOffset, uint8_t txChannel, char registerSet);
			/*!
			* \brief load crosstalk temperature coefficients from OPT3101::crosstalkTempCoffC class to the h/w
			* Method loads the registers from OPT3101::crosstalkTempCoffC instance specified to the h/w to specified txChannel and registerSet
			* \param[in] illumXtalkTempCoff; illumXtalkTempCoff is pointer to the instance of OPT3101::crosstalkTempCoffC class. The register values form the instance are loaded to the hardware to specified txChannel and registerSet
			* \param[in] txChannel; txChannel is the number denoting the channel to which the registers need to be loaded For eg: illumXtalk=0 loads for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which register needs to be loaded. For eg: registerSet='h' would load on to registers related to illum_dac_h_tx*
			* \returns Nothing;
			*/
			void loadIllumCrosstalkTempCoff(crosstalkTempCoffC *illumXtalkTempCoff, uint8_t txChannel, char registerSet);
			/*!
			* \brief load phase offset temperature coefficients from OPT3101::phaseTempCoffC class to the h/w
			* Method loads the registers from OPT3101::phaseTempCoffC instance specified to the h/w to specified txChannel and registerSet
			* \param[in] phaseTempCoff; phaseTempCoff is pointer to the instance of OPT3101::phaseTempCoffC class. The register values form the instance are loaded to the hardware to specified txChannel and registerSet
			* \param[in] txChannel; txChannel is the number denoting the channel to which the registers need to be loaded For eg: illumXtalk=0 loads for TX0 channel
			* \param[in] registerSet; registerSet is a char 'h' or 'l' specifying register set on which register needs to be loaded. For eg: registerSet='h' would load on to registers related to illum_dac_h_tx*
			* \returns Nothing;
			*/
			void loadPhaseOffsetTempCoff(phaseTempCoffC *phaseTempCoff, uint8_t txChannel, char registerSet);
			/*!
			* \brief load phase ambient coefficients from OPT3101::phaseAmbientCoffC class to the h/w
			* Method loads the registers from OPT3101::phaseAmbientCoff instance specified to the h/w 
			* \param[in] phaseAmbientCoff; phaseAmbientCoff is pointer to the instance of OPT3101::phaseAmbientCoffC class. The register values form the instance are loaded to the hardware
			* \returns Nothing;
			*/
			void loadPhaseAmbientCoff(phaseAmbientCoffC *phaseAmbientCoff);

			/*!
			* \brief loads all illum crosstalk set specified by OPT3101 configuration tool  to h/w
			* Method loads all the illum crosstalk registers from OPT3101::calibrationC::illumCrosstalk to the h/w to their specific TX channels and register sets. The order in which data is stored and indexed is preset and loaded accordingly. 
			* The method provides a higher level of abstraction for users to just load the illumCrosstalk calibration portion of the calibration data from the self contained instance of OPT3101::calibrationC to the h/w
			* \param[in] loadFromFile; loadFromFile is a flag which specifies whether to load the calibration coff from file or from the OPT3101::device::calibration member of the OPT3101::calibrationC class
			* \returns Nothing;
			*/
			void loadIllumCrosstalkSet(bool loadFromFile = true);
			/*!
			* \brief loads all illum crosstalk temp coefficients set specified by OPT3101 configuration tool to h/w
			* Method loads all the illum crosstalk temp coff registers from OPT3101::calibrationC::illumCrosstalkTempCoff to the h/w to their specific TX channels and register sets. The order in which data is stored and indexed is preset and loaded accordingly.
			* The method provides a higher level of abstraction for users to just load the illumCrosstalk temp coff calibration portion of the calibration data from the self contained instance of OPT3101::calibrationC to the h/w
			* \param[in] loadFromFile; loadFromFile is a flag which specifies whether to load the calibration coff from file or from the OPT3101::device::calibration member of the OPT3101::calibrationC class
			* \returns Nothing;
			*/
			void loadIllumCrosstalkTempCoffSet(bool loadFromFile = true);
			/*!
			* \brief loads all phase offset set specified by OPT3101 configuration tool to h/w
			* Method loads all the phase offset registers from OPT3101::calibration::phaseOffset to the h/w to their specific TX channels and register sets. The order in which data is stored and indexed is preset and loaded accordingly.
			* The method provides a higher level of abstraction for users to just load the phase offset calibration portion of the calibration data from the self contained instance of OPT3101::calibrationC to the h/w
			* \returns Nothing;
			*/
			void loadPhaseOffsetSet();
			/*!
			* \brief loads all phase temp coff set specified by OPT3101 configuration tool to h/w
			* Method loads all the phase temp coff registers from OPT3101::calibration::phaseTempCoff to the h/w to their specific TX channels and register sets. The order in which data is stored and indexed is preset and loaded accordingly.
			* The method provides a higher level of abstraction for users to just load the phase temp coff calibration portion of the calibration data from the self contained instance of OPT3101::calibrationC to the h/w
			* \param[in] loadFromFile; loadFromFile is a flag which specifies whether to load the calibration coff from file or from the OPT3101::device::calibration member of the OPT3101::calibrationC class
			* \returns Nothing;
			*/
			void loadPhaseOffsetTempCoffSet(bool loadFromFile = true);
			/*!
			* \brief loads phase ambient coff set to h/w
			* Method loads all the phase ambient coff registers from OPT3101::calibrationC::phaseAmbientCoff to the h/w.
			* The method provides a higher level of abstraction for users to just load the phase ambient coff calibration portion of the calibration data from the self contained instance of OPT3101::calibrationC to the h/w
			* \param[in] loadFromFile; loadFromFile is a flag which specifies whether to load the calibration coff from file or from the OPT3101::device::calibration member of the OPT3101::calibrationC class
			* \returns Nothing;
			*/
			void loadPhaseAmbientCoffSet(bool loadFromFile = true);

			/*!
			* \brief method for calibration session run the first time the board is brought up
			* Method is a sequence of steps recommended when the system is powered up for the first time.
			* This this is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_firstTimeBringUp();
			/*!
			* \brief method for calibration of crosstalk temperature coefficients 
			* Method is a sequence of steps recommended to perform crosstalk temperature coefficients 
			* This this is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_perDesignCalibrationCrosstalkTemp();
			/*!
			* \brief method for calibration of phase temperature coefficients
			* This is recommended to be run on a few units per design. Coefficients calculated by this can be used for all the units in production
			* Method is a sequence of steps recommended to perform phase temperature coefficients
			* This this is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_perDesignCalibrationPhaseTemp();
			/*!
			* \brief method for calibration of phase ambient coefficients
			* This is recommended to be run on a few units per design. Coefficients calculated by this can be used for all the units in production
			* Method is a sequence of steps recommended to perform phase ambient coefficients
			* This this is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_perDesignCalibrationPhaseAmbient();
			/*!
			* \brief method for calibration of phase ambient coefficients after manually curve fitting 
			* This is recommended to be run on a few units per design. Coefficients calculated by this can be used for all the units in production
			* Method is a expected to be run after a PWL curve fit has been done to the ambient data after running OPT3101::device::calibrationSession_perDesignCalibrationPhaseAmbient and analyzing and curve fitting the data
			* This method doesn't do anything to the device except to update the coff and store it to file
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibration_perDesignCalibrationPhaseAmbientSetCoffAfterCurveFit();
			/*!
			* \brief method for calibration during factory for each unit of the system
			* This is recommended to be run on a each and every units through production.
			* Method is a sequence of steps recommended to perform all necessary calibration per unit
			* This is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_perUnitFactoryCalibration();
			/*!
			* \brief method to store the calibration to EERPOM or some other from on non-volatile memory to be loaded on power-up every time
			* This is recommended to be run on a each and every units through production after OPT3101::device::calibrationSession_perUnitFactoryCalibration
			* Method is a sequence of steps recommended to store the calibration data to a non-volatile memory for restoration during actual system operation
			* In case if the configuration flag OPT3101::device::EEPROM_connected is set to true, this method uses the OPT3101 I2C slave to write data to the connected EEPROM 
			* This is a session, this is a standalone session includes reset of device and initialization
			* Details of the steps performed are documented in the detailed section
			* \returns Nothing;
			*/
			void calibrationSession_perUnitFactoryCalibrationWriteRegisterDataToNonVolatileMemory();

			// Configuration flags to be set by configurator tool 
			bool configurationFlags_monoshotMode; ///< Configuration flag to specify if the device is operating in mono shot mode which is set by OPT3101 configurator tool 
			uint8_t configurationFlags_xtalkFilterTau;  ///< Configuration value of crosstalk filter time constant which is set by OPT3101 configurator tool
			uint16_t configurationFlags_xtalkSettlingOneTauInMilliSeconds;  ///< Configuration value specifying the time constant in milliseconds which is set by OPT3101 configurator tool
			uint16_t configurationFlags_xtalkSettlingOneTauInDataReadyCounts;  ///< Configuration value specifying the time constant in data ready counts which is set by OPT3101 configurator tool
			uint8_t configurationFlags_avgFrameCountExponentOfTwo; ///< Configuration value specifying exponent component of average frame count which is set by OPT3101 configurator tool
			uint32_t configurationFlags_frameTimeInMilliSeconds; ///< Configuration value specifying frame time in milli-seconds which is set by OPT3101 configurator tool
			bool configurationFlags_isTXChannelActive[3]; ///< Configuration value specifying the active channels set by OPT3101 congifigurator tool 
			bool configurationFlags_isRegisterSetActive[2]; ///< Configuration value specifying the active register set by OPT3101 congifigurator tool 

		};
}

#endif /* OPT3101DEVICE_H_ */
