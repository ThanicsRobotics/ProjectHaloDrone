/*!
* \file environmentControl.cpp
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
* The file contains class environmentalController methods and declaration
*/

#include "environmentControl.h"

/** \brief environmentalController declaration
This global variable declaration with name envController of environmentalController is used by class for various environmental setup during calibration steps
*/
environmentalController envController;

environmentalController::environmentalController()
{
	/// <b>Algorithm of the method is as follows</b>
	this->setTargetDistanceInMM = 0; ///* Nulls all the class members
	this->chamberTemperatureInC = 0;
	this->ambientLightInLux = 0;
}

void environmentalController::setTargetDistance(uint16_t targetDistanceInMM)
{
	/// <b>Algorithm of the method is as follows</b>

	this->setTargetDistanceInMM = targetDistanceInMM; ///* Set the class internal member of the call with user input
	///* User implemented function to set the target distance 
}

void environmentalController::setChamberTemperature(int8_t chamberTemperatureInC)
{
	/// <b>Algorithm of the method is as follows</b>
	this->chamberTemperatureInC = chamberTemperatureInC; ///* Set the class internal member of the call with user input
														 ///* User implemented function to set the chamber temperature

}

void environmentalController::setAmbientLight(uint32_t setAmbientLightInLux) 
{
	/// <b>Algorithm of the method is as follows</b>
	this->ambientLightInLux = ambientLightInLux; ///* Set the class internal member of the call with user input 
												 ///* User implemented function to set the ambient light
}

void environmentalController::setTargetToInfinity_OR_coverPhotodiode()
{
	/// <b>Algorithm of the method is as follows</b>
	/// * This is an empty function to convey the measurement setup required. 
	/// * With a cover glass in place, the system is expected to be pointed towards a target with very low reflectivity far beyond the range of the system 
	/// * It is also acceptable to cover the photo diode alone with an optically isolating light blocker. 
}

