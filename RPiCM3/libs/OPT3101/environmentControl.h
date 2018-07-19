/*!
* \file environmentControl.h
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
* The file contains class descriptions and methods for environment control
*/

#ifndef ENVIRONMENTCONTROL_H_
#define ENVIRONMENTCONTROL_H_

#include <stdint.h>

/*! \class environmentalController
\brief Generic implementation for environment controller

As part of OPT3101SDK there are several environment specific functions that need to be performed. Examples of such functions are setting up targets at particular distance, setting up chamber temperature. 
These are essential functions to perform the calibration of OPT3101 based system. The setting up of these environment conditions have to be done at specific steps during calibration. 
The template class environmentalController makes this process simple and scalable. Appropriate methods of this class are called at relevant steps in calibration.
Users using this SDK are expected to implement these functions based on the environment setup. <br><br>
The names of the methods reflect the conditions for the environment. For eg: environmentalController::setTargetToInfinity_OR_coverPhotodiode method as the name suggest is a condition
where the OPT3101 system needs to be pointed to infinity (far away from detectable range of system) or the photo diode needs to be covered so no light leaks directly between the transmitter(s) and photo diode.  
These implementations are very specific to the host on which the SDK is ported and run. <br>
This abstract class provides a template with self explanatory method names which the users can override and implement their own functions. <br>
*/
class environmentalController {
	private:
		uint16_t setTargetDistanceInMM; ///< Remembers the target distance in mm
		int8_t chamberTemperatureInC; ///< Remembers the chamber temperature in C
		uint32_t ambientLightInLux; ///< Remembers the ambient light in lux   
	public:
		/*!
		* \brief Constructor to initialize variables to 0
		*
		* \return Nothing
		*/
		environmentalController();
		/*!
		* \brief Sets target distance
		*
		* \param[in] targetDistanceInMM; targetDistanceInMM is the known distance where the target needs to be placed. This is expressed in mm
		* \return Nothing
		*/
		void setTargetDistance(uint16_t targetDistanceInMM);
		/*!
		* \brief Sets chamber temperature
		*
		* \param[in] chamberTemperatureInC; chamberTemperatureInC is the known temperature the chamber needs to be set to and settled. This is critical for temperature related calibration steps
		* \return Nothing
		*/
		void setChamberTemperature(int8_t chamberTemperatureInC);
		/*!
		* \brief Sets ambient light condition
		*
		* \param[in] ambientLightInLux; ambientLightInLux is the intensity (in lux) of ambient light to be applied to the system to perform OPT3101::phaseAmbientCoffC measurements
		* \return Nothing
		*/
		void setAmbientLight(uint32_t ambientLightInLux);
		/*!
		* \brief Sets target such that the amplitude of system is very low
		* This is a crucial step to determine OPT3101::crosstalkC values for the OPT3101 system. The environmentController is expected to target in such a way that the amplitude of the system reads 
		* a low enough value that this can be ignored as crosstalk value. Either setting target to far enough position (or low enough reflectivity) or covering the photo diode would help achieve this
		* \return Nothing
		*/
		void setTargetToInfinity_OR_coverPhotodiode();
};

#endif /* ENVIRONMENTCONTROL_H_ */
