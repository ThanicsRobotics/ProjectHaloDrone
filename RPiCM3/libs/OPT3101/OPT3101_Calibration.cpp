/*!
* \file OPT3101device_Calibration.cpp
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
* The file contains methods definitions for top level calibration methods for OPT3101::device 
*/

#include "OPT3101device.h"


void OPT3101::device::calibrationSession_firstTimeBringUp() {
	uint8_t c0, c1, indx;
#ifdef OPT3101_USE_STDIOLIB
	char fileName[FILENAME_LENGTH];
#endif
	/// <b>Algorithm of the method is as follows</b>
	this->reset(); ///* Resets the device calling OPT3101::device::reset method
	this->initialize(); ///* Initializes the OPT3101 device by calling OPT3101::device::initialize method
	this->measureAndCorrectInternalCrosstalk(&this->calibration->internalCrosstalk[0]); ///* Calls method OPT3101::device::measureAndCorrectInternalCrosstalk with argument OPT3101::calibrationC::internalCrosstalk
	this->calibration->internalCrosstalk[0].report(); ///* Calls the method OPT3101::crosstalkC::report for debug and data analysis
													  // The folowing step is to perfrom illum crosstalk measurements
	envController.setTargetToInfinity_OR_coverPhotodiode(); ///* Calls the method environmentalController::setTargetToInfinity_OR_coverPhotodiode , which is expected to set the environment so as to be able to measure illumination crosstalk

	indx = 0;
	// Loop to iterate over all TX channels
	for (c0 = 0; c0 < 3; c0++) {  ///* Loops though all the valid TX channel and register set configurations
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					this->measureIllumCrosstalk(&this->calibration->illumCrosstalk[indx], c0, c1 ? 'l' : 'h');  ///* Calls method OPT3101::device::measureIllumCrosstalk with argument OPT3101::calibrationC::illumCrosstalk
					this->calibration->illumCrosstalk[indx].report(); ///* Calls the method OPT3101::crosstalkC::report for debug and data analysis
																	  // Provide file name if save to File is implemented eg: illumCrosstalkTX{txChannel}Reg{regSet}_roomTemp
#ifdef OPT3101_USE_STDIOLIB
					sprintf(fileName, "%s/illumCrosstalk_TX%d_%c.txt", filePath, c0, c1 ? 'l' : 'h'); ///* Creates filename  illumCrosstalk_TX{channel}_{h/l}.txt in path filePath
					this->calibration->illumCrosstalk[indx].storeToFile(fileName);  ///* Calls method OPT3101::crosstalkC::storeToFile to store the data
#endif
					indx++;
				}
			}
		}
	}
}

void OPT3101::device::calibrationSession_perUnitFactoryCalibrationWriteRegisterDataToNonVolatileMemory()
{
	uint16_t c0;
	uint8_t regStore[5], c1;
	uint32_t data;
	/// <b>Algorithm of the method is as follows</b>
	if (!this->calibration->EEPROM_connected) ///* Method returns without doing any operation when the device configuration flag OPT3101::device::EEPROM_connected is false. User needs to implement their own function if no EEPROM is present
		return;
	regStore[0] = this->reg.tsens_slave0.read();
	regStore[1] = this->reg.i2c_en.read();
	regStore[2] = this->reg.i2c_num_tran.read();
	regStore[3] = this->reg.i2c_rw.read();
	regStore[4] = this->reg.i2c_num_bytes_tran1.read(); ///* Critical registers which are modified in this method are read from the h/w and temporarily bufferd to local variables.

	this->reg.tsens_slave0 = 0x50;
	this->reg.i2c_en = 1;
	this->reg.i2c_num_tran = 0;
	this->reg.i2c_rw = 0;
	this->reg.i2c_num_bytes_tran1 = 1; ///* OPT3101 device is configured to write desired data though the SDA_M/SCL_M lines to the connected external EEPROM

									   // This portions clears all the element in EEPROM to 0xFF
	for (c0 = 0; c0 < 256; c0++) {
		this->writeDataToEEPROM((uint8_t)c0, 0xFF); ///* Erases the EEPROM with  0xFF data in all lcoations.
	}

	for (c0 = 0; c0 < this->calibration->registerAddressListSize; c0++) {
		this->writeDataToEEPROM(c0 << 2, this->calibration->registerAddressList[c0]);
		data = host.readI2C(this->calibration->registerAddressList[c0]); ///* Reads all the registers from the list OPT3101::calibrationC::registerAddressList from h/w and writes the address and data to the connected external EEPROM
		for (c1 = 0; c1<3; c1++)
			this->writeDataToEEPROM((c0 << 2) + c1 + 1, (data >> ((c1 << 3)) & 0xFF));
	}
	this->reg.tsens_slave0 = regStore[0];
	this->reg.i2c_en = regStore[1];
	this->reg.i2c_num_tran = regStore[2];
	this->reg.i2c_rw = regStore[3];
	this->reg.i2c_num_bytes_tran1 = regStore[4]; ///* Restores the device state to the same state as before entering this method from the buffered local variables
}

void OPT3101::device::calibrationSession_perDesignCalibrationCrosstalkTemp() {
	uint8_t c0, c1, indx, c2;
#ifdef OPT3101_USE_STDIOLIB
	char fileName[FILENAME_LENGTH];
#endif
	/// <b>Algorithm of the method is as follows</b>
	OPT3101::crosstalkC illumXtalk[2][6]; ///* Declares temporary variable of OPT3101::crosstalkC class to hold internal crosstalk data across temperature, TX channel and register settings
	uint8_t chamberTemperature;
	this->reset(); ///* Resets the device calling OPT3101::device::reset method
	this->initialize(); ///* Initializes the OPT3101 device by calling OPT3101::device::initialize method
	this->measureAndCorrectInternalCrosstalk(&this->calibration->internalCrosstalk[0]); ///* Calls method OPT3101::device::measureAndCorrectInternalCrosstalk with argument OPT3101::calibrationC::internalCrosstalk
	envController.setTargetToInfinity_OR_coverPhotodiode(); ///* Calls the method environmentalController::setTargetToInfinity_OR_coverPhotodiode , which is expected to set the environment so as to be able to measure illumination crosstalk

	c2 = 0;
	for (chamberTemperature = 25; chamberTemperature <= 70; chamberTemperature += 45) { ///* Loops through chamber temperature settings to get temp coff
		envController.setChamberTemperature(chamberTemperature); ///* Calls the method environmentalController::setChamberTemperature , which is expected to set the chamber to desired temperature
		indx = 0;
		// Loop to iterate over all TX channels
		for (c0 = 0; c0 < 3; c0++) {  ///* Loops though all the valid TX channel and register set configurations
			if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
				for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
					if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
						this->measureIllumCrosstalk(&illumXtalk[c2][indx], c0, c1 ? 'l' : 'h'); ///* Calls method OPT3101::device::measureIllumCrosstalk with temporary variable of OPT3101::crosstalkC class 
#ifdef OPT3101_USE_STDIOLIB
						sprintf(fileName, "%s/illumCrosstalk_%dC_TX%d_%c.txt", filePath, chamberTemperature, c0, c1 ? 'l' : 'h'); ///* Creates filename  illumCrosstalk_{chamberTemp}C_TX{channel}_{h/l}.txt in path filePath
						illumXtalk[c2][indx].storeToFile(fileName); ///* Calls method OPT3101::crosstalkC::storeToFile to store the data
#endif
						illumXtalk[c2][indx].report(); ///* Calls OPT3101::crosstalkC::report method to report the crosstalk on screen
						indx++;
					}
				}
			}
		}
		c2++;
	}

	envController.setChamberTemperature(25); // Set temperature back to room temperature
	indx = 0;
	for (c0 = 0; c0 < 3; c0++) { // Loop to iterate over all TX channels
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					this->calibration->illumCrosstalkTempCoff[indx].calculateCoff(&illumXtalk[0][indx], &illumXtalk[1][indx]); ///* For each TX channel and register setting calls the OPT3101::calibrationC::illumCrosstalkTempCoff method with appropriate illum crosstalk values stored in temporary variable which stores calibration coeff to OPT3101::calibration::illumCrosstalkTempCoff
#ifdef OPT3101_USE_STDIOLIB
					sprintf(fileName, "%s/illumCrosstalkTempCoff_TX%d_%c.txt", filePath, c0, c1 ? 'l' : 'h');  ///* Creates filename  illumCrosstalkTempCoff_TX{channel}_{h/l}.txt in path filePath
					this->calibration->illumCrosstalkTempCoff[indx].storeToFile(fileName); ///* Calls method OPT3101::crosstalkTempCoffC::storeToFile to store the data
#endif
					this->calibration->illumCrosstalkTempCoff[indx].report(); ///* Calls OPT3101::crosstalkTempCoffC::report to report the crosstalk temp coff on screen
					indx++;
				}
			}
		}
	}
}

void OPT3101::device::calibrationSession_perDesignCalibrationPhaseTemp() {
	/// <b>Algorithm of the method is as follows</b>
	uint8_t c0, c1, c2, indx, chamberTemperature;
	uint16_t refDistanceInCodes;
	OPT3101::phaseOffsetC phaseOffset[2]; ///* Declares temporary variable of OPT3101::phaseOffsetC class to hold phase offset data across temperature, TX channel and register settings
	uint32_t refDistanceInMM;
#ifdef OPT3101_USE_STDIOLIB
	char fileName[FILENAME_LENGTH];
#endif
	this->reset(); ///* Resets the device calling OPT3101::device::reset method
	this->initialize(); ///* Initializes the OPT3101 device by calling OPT3101::device::initialize method
	this->measureAndCorrectInternalCrosstalk(&this->calibration->internalCrosstalk[0]); ///* Calls method OPT3101::device::measureAndCorrectInternalCrosstalk with argument OPT3101::calibrationC::internalCrosstalk
	this->loadIllumCrosstalkSet(); ///* Calls method OPT3101::device::loadIllumCrosstalkSet to load all the illum crosstalk compensation registers from files

	indx = 0;
	// Loop to iterate over all TX channels
	for (c0 = 0; c0 < 3; c0++) { ///* Loops though all the valid TX channel and register set configurations
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					refDistanceInMM = 0; ///* <b>Warning</b> User is expected to select and set reference distance so that the amplitude of the system for this particular TX and register set configurations measures between 16K and 24K. Default is set to 0 in the SDK
					envController.setTargetDistance(refDistanceInMM); ///* Calls environmentalController::setTargetDistance method with the specified distance
					refDistanceInCodes = (refDistanceInMM * 4477) >> 10; ///* Converts the reference distance specified in codes related to OPT3101::frameData::phase ADC codes
					c2 = 0;
					for (chamberTemperature = 25; chamberTemperature <= 70; chamberTemperature += 45) {  ///* Loops through chamber temperature settings to get temp coff
						envController.setChamberTemperature(chamberTemperature);  ///* Calls the method environmentalController::setChamberTemperature , which is expected to set the chamber to desired temperature
																				  //printf("Waiting for board to heat up... press key after heated up...");
																				  //host.pause();
						this->measurePhaseOffset(&phaseOffset[c2++], c0, c1 ? 'h' : 'l', refDistanceInCodes); ///* Measured phase offset by calling method OPT3101::device::measurePhaseOffset with specified reference distance
					}
					this->calibration->phaseTempCoff[indx].calculateCoff(&phaseOffset[0].data, &phaseOffset[1].data, this->calibration->extTempSensor_connected); ///* Calls method OPT3101::phaseTempCoffC::calculateCoff with temperature phase offset and assigns data to OPT3101::calibrationC::phaseTempCoff
#ifdef OPT3101_USE_STDIOLIB
					sprintf(fileName, "%s/phaseTempCoff_TX%d_%c.txt", filePath, c0, c1 ? 'l' : 'h'); ///* Creates filename phaseTempCoff_TX{channel}_{h/l}.txt in path filePath
					this->calibration->phaseTempCoff[indx].storeToFile(fileName);  ///* Calls method OPT3101::phaseTempCoffC::storeToFile to store the data 
#endif
					this->calibration->phaseTempCoff[indx].report();///* Calls OPT3101::phaseTempCoffC::report to report the phase temp coff on screen
					indx++;
				}
			}
		}
	}
	envController.setChamberTemperature(25); // Set temperature back to room temperature
}

void OPT3101::device::calibration_perDesignCalibrationPhaseAmbientSetCoffAfterCurveFit() {
	///* <b>Warning:</b> User is expected to curve fit the phase ambient data with PWL curve fit and provide the floating point precision coefficients and split points 
#ifdef OPT3101_USE_STDIOLIB
	char fileName[FILENAME_LENGTH];
	sprintf(fileName, "%s/phaseAmbCoff.txt", filePath); ///* Creates filename phaseAmbCoff.txt in path filePath
#endif
	this->calibration->phaseAmbientCoff[0].coff[0] = 0.0; ///* Users is expected to enter all the OPT3101::calibration::phaseAmbientCoff::coff values manually after computation outside based on data generated by OPT3101::device::calibrationSession_perDesignCalibrationPhaseAmbient
	this->calibration->phaseAmbientCoff[0].coff[1] = 0.0; // Set this based on PWL curveFit data
	this->calibration->phaseAmbientCoff[0].coff[2] = 0.0; // Set this based on PWL curveFit data
	this->calibration->phaseAmbientCoff[0].coff[3] = 0.0; // Set this based on PWL curveFit data
	this->calibration->phaseAmbientCoff[0].splitsReg[0] = 0; // Set this based on PWL curveFit data
	this->calibration->phaseAmbientCoff[0].splitsReg[1] = 0; // Set this based on PWL curveFit data
	this->calibration->phaseAmbientCoff[0].splitsReg[2] = 0; ///* Users is expected to enter all the OPT3101::calibration::phaseAmbientCoff::splitsReg values manually after computation outside based on data generated by OPT3101::device::calibrationSession_perDesignCalibrationPhaseAmbient
#ifdef OPT3101_USE_STDIOLIB
	this->calibration->phaseAmbientCoff[0].storeToFile(fileName); ///* Calls OPT3101::calibration::phaseAmbientCoff::storeToFile to store to file name phaseAmbCoff.txt in the path filePath
#endif
}

void OPT3101::device::calibrationSession_perDesignCalibrationPhaseAmbient() {
	uint8_t c0, c1, indx;
	uint16_t refDistanceInCodes;
	uint32_t ambientLightInLux;
	OPT3101::phaseOffsetC phaseOffset[16];
	uint32_t refDistanceInMM;
	bool breakFlag;
#ifdef OPT3101_USE_STDIOLIB
	char fileName[FILENAME_LENGTH];
#endif

	/// <b>Algorithm of the method is as follows</b>

	this->reset(); ///* Resets the device calling OPT3101::device::reset method
	this->initialize(); ///* Initializes the OPT3101 device by calling OPT3101::device::initialize method
	this->measureAndCorrectInternalCrosstalk(&this->calibration->internalCrosstalk[0]); ///* Calls method OPT3101::device::measureAndCorrectInternalCrosstalk with argument OPT3101::calibrationC::internalCrosstalk
	this->loadIllumCrosstalkSet();///* Calls method OPT3101::device::loadIllumCrosstalkSet to load all the illum crosstalk compensation registers

	indx = 0;
	breakFlag = false;
	// Loop to iterate over all TX channels
	for (c0 = 0; c0 < 3; c0++) { ///* Loops though all the valid TX channel and register set configurations
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					refDistanceInMM = 0; ///* <b>Warning</b> User is expected to select and set reference distance so that the amplitude of the system for this particular TX and register set configurations measures between 16K and 24K. Default is set to 0 in the SDK
					envController.setTargetDistance(refDistanceInMM); ///* Calls environmentalController::setTargetDistance method with the specified distance
					refDistanceInCodes = (refDistanceInMM * 4477) >> 10; ///* Converts the reference distance specified in codes related to OPT3101::frameData::phase ADC codes
					indx = 0;
					for (ambientLightInLux = 0; ambientLightInLux < 160000; ambientLightInLux += 10000) {  ///* Loops through user desired ambient lux conditions to get phase Ambient coff
						envController.setAmbientLight(ambientLightInLux); ///* Calls the method environmentalController::setAmbientLight , which is expected to set the environment to have the specified ambient conditions
						this->measurePhaseOffset(&phaseOffset[indx++], c0, c1 ? 'h' : 'l', refDistanceInCodes); ///* Measures phase offset by calling method OPT3101::device::measurePhaseOffset with specified reference distance
#ifdef OPT3101_USE_STDIOLIB
						sprintf(fileName, "%s/phaseAmbCoff_rawData_%06dlux.txt", filePath, ambientLightInLux); ///* Creates filename phaseAmbCoff_rawData_{ambientinLux}lux.txt in path filePath
						phaseOffset->storeToFile(fileName); ///* Calls OPT3101::phaseOffsetC::storeToFile to archive the data for analysis and curve fitting
#endif
						phaseOffset->report(); ///* Calls OPT3101::phaseOffsetC::report to display values on screen
					}
					breakFlag = true; ///* Since Phase ambient coff is required to be done only for one TX configuration the the method breaks from the loop after 1 ambient sweep
					break; // This is required only for one TX and 1 of the registers since this is property of photo diode. 
						   // Measure and populate Ambient Coefficients 
				}
				if (breakFlag)
					break;
			}
			if (breakFlag)
				break;
		}
		if (breakFlag)
			break;
	}
	envController.setAmbientLight(0);

}

void OPT3101::device::calibrationSession_perUnitFactoryCalibration()
{
	uint8_t c0, c1, indx;
	uint16_t refDistanceInCodes;
	uint32_t *refDistanceInMM;
	refDistanceInMM = new uint32_t[this->calibration->recordLength];

	for(c0=0;c0<this->calibration->recordLength;c0++)
		refDistanceInMM[c0] = 0; // Users are expected to set this distance in MM to desired value 
							/// <b>Algorithm of the method is as follows</b>

	this->reset(); ///* Resets the device calling OPT3101::device::reset method
	this->initialize(); ///* Initializes the OPT3101 device by calling OPT3101::device::initialize method
	this->measureAndCorrectInternalCrosstalk(&this->calibration->internalCrosstalk[0]); ///* Calls method OPT3101::device::measureAndCorrectInternalCrosstalk with argument OPT3101::calibrationC::internalCrosstalk

	envController.setTargetToInfinity_OR_coverPhotodiode(); ///* Calls the method environmentalController::setTargetToInfinity_OR_coverPhotodiode , which is expected to set the environment so as to be able to measure illumination crosstalk
	indx = 0;
	// Loop to iterate over all TX channels
	for (c0 = 0; c0 < 3; c0++) { ///* Loops though all the valid TX channel and register set configurations
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					this->measureIllumCrosstalk(&this->calibration->illumCrosstalk[indx], c0, c1 ? 'h' : 'l'); ///* Calls method OPT3101::device::measureIllumCrosstalk to capture the illumcrosstalk to variable OPT3101::calibrationC::illumCrosstalk for all TX config and register set
					this->calibration->illumCrosstalk[indx].report(); ///* Calls OPT3101::crosstalkC::report for each TX channel and register set to report crosstalk values on screen
					indx++;
				}
			}
		}
	}

	this->loadIllumCrosstalkSet(false); ///* Calls the OPT3101::device::loadIllumCrosstalkSet with false argument so that method to load all illum crosstalk settings from the OPT3101::device::crosstalk::illumCrosstalk member
	this->loadIllumCrosstalkTempCoffSet(); ///* Calls the OPT3101::device::loadIllumCrosstalkTempCoffSet to load all illum crosstalk temp coff settings from file

	indx = 0;
	// Loop to iterate over all TX channels
	for (c0 = 0; c0 < 3; c0++) { ///* Loops though all the valid TX channel and register set configurations
		if (this->configurationFlags_isTXChannelActive[c0]) { // Checking is TX channel is active for this profile 
			for (c1 = 0; c1 < 2; c1++) { // Loop to iterate over the H/L registers 
				if (this->configurationFlags_isRegisterSetActive[c1]) { // Checking if registers are active for this profile
					refDistanceInMM[indx] = refDistanceInMM[indx];///* <b>Warning</b> User is expected to select and set reference distance so that the amplitude of the system for this particular TX and register set configurations measures between 16K and 24K. Default is set to 0 in the SDK
					envController.setTargetDistance(refDistanceInMM[indx]); ///* Calls environmentalController::setTargetDistance method with the specified distance
					refDistanceInCodes = (refDistanceInMM[indx] * 4477) >> 10; ///* Converts the reference distance specified in codes related to OPT3101::frameData::phase ADC codes
					this->measurePhaseOffset(&this->calibration->phaseOffset[indx++], c0, c1 ? 'h' : 'l', refDistanceInCodes); ///* Measures phase offset by calling method OPT3101::device::measurePhaseOffset with specified reference distance and OPT3101::calibrationC::phaseOffset as argument to store the phase offset values
				}
			}
		}
	}
	//this->loadPhaseOffsetSet(); ///* Calls the OPT3101::device::loadPhaseOffsetSet method with false argument so as to load all the phase offset registers from the OPT3101::device::calibration::phaseOffset instance instead of files
	this->loadPhaseOffsetTempCoffSet(); ///* Calls the OPT3101::device::loadPhaseOffsetTempCoffSet method to load all the phase offset temp coff. This method already calls OPT3101::loadPhaseOffsetSet internally
	this->loadPhaseAmbientCoffSet(); ///* Calls the OPT3101::device::loadPhaseAmbientCoffSet method to load all the phase offset temp coff
	this->calibrationSession_perUnitFactoryCalibrationWriteRegisterDataToNonVolatileMemory(); ///* Calls the OPT3101::device::calibrationSession_perUnitFactoryCalibrationWriteRegisterDataToNonVolatileMemory to store the calibration data to a non-volatile memory
}
