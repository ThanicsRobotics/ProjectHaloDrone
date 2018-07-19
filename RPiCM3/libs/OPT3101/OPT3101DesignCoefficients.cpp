/*!
* \file OPT3101DesignCoefficients.cpp
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
* The file contains methods definitions for classes OPT3101::crosstalkTempCoffC OPT3101::calibrationC OPT3101::phaseAmbientCoffC OPT3101::phaseTempCoffC
*/
#include "OPT3101DesignCoefficients.h"

OPT3101::crosstalkTempCoffC::crosstalkTempCoffC()
{
	/// <b>Algorithm of the method is as follows</b>
	this->coffI = 0.0; ///* Initilizes all the members to 0
	this->coffQ = 0.0;
	this->coffIReg = 0;
	this->coffQReg = 0;
	this->commonScale = 0;
}

void OPT3101::crosstalkTempCoffC::calculateCoff(OPT3101::crosstalkC *illumXtalk0, OPT3101::crosstalkC *illumXtalk1)
{
	int32_t I[2], Q[2];
	/// <b>Algorithm of the method is as follows</b>
	I[0] = ((int32_t)illumXtalk0->I) << illumXtalk0->xtalkScale; ///* Scales the 16bit crosstalk registers OPT3101::crosstalkC::I and OPT3101::crosstalkC::Q to 24 bit level using OPT3101::crosstalkC::xtalkScale
	I[1] = ((int32_t)illumXtalk1->I) << illumXtalk1->xtalkScale; 
	Q[0] = ((int32_t)illumXtalk0->Q) << illumXtalk0->xtalkScale;
	Q[1] = ((int32_t)illumXtalk1->Q) << illumXtalk1->xtalkScale;
	///* Makes the data base circular to avoid errorneous high slope measurements
	if (I[0] > 6291456 && I[1] < -6291456) // 3/4 of the 24 bit signed range
		I[1] += 16777216; //Adding 2**24 to this 
	if (I[1] > 6291456 && I[0] < -6291456) // 3/4 of the 24 bit signed range
		I[0] += 16777216; //Adding 2**24 to this 
	if (Q[0] > 6291456 && Q[1] < -6291456) // 3/4 of the 24 bit signed range
		Q[1] += 16777216; //Adding 2**24 to this 
	if (Q[1] > 6291456 && Q[0] < -6291456) // 3/4 of the 24 bit signed range
		Q[0] += 16777216; //Adding 2**24 to this 
						  
						  //printf("Temp diff is [%d]\n", illumXtalk0->tmain - illumXtalk1->tmain);
	if(!((illumXtalk0->tmain - illumXtalk1->tmain)==0)) {
		this->coffI = ((double)(I[0] - I[1])) / (illumXtalk0->tmain - illumXtalk1->tmain) / ((illumXtalk0->magnitude() + illumXtalk1->magnitude())/2.0); 	///* Calculates the double precision OPT3101::crosstalkTempCoffC::coffI and OPT3101::crosstalkTempCoffC::coffQ by finding difference between 24 bit registers divided by delta OPT3101::crosstalkC::tmain divided by OPT3101::crosstalkC::magnitude()
		this->coffQ = ((double)(Q[0] - Q[1])) / (illumXtalk0->tmain - illumXtalk1->tmain) / ((illumXtalk0->magnitude() + illumXtalk1->magnitude())/2.0);
	}
	else {
		this->coffI = 0.0;
		this->coffQ = 0.0;
	}
	///* <b>Warning:</b> Makes the OPT3101::crosstalkTempCoffC::I and OPT3101::crosstalkTempCoffC::Q 0 if the OPT3101::crosstalkC::tmain are same for both the input arguments
}

void OPT3101::crosstalkTempCoffC::report()
{
#ifdef OPT3101_USE_STDIOLIB
	/// <b>Algorithm of the method is as follows</b>
	printf("--------------------------------\n");
	printf("Crosstalk Temp Coff Class Report\n");
	printf("--------------------------------\n"); ///* Prints all the members and values of members on screen. 
	printf("coffI=%4.2f\n",  this->coffI);
	printf("coffQ=%4.2f\n",  this->coffQ);
	printf("coffIReg=%d\n", this->coffIReg);
	printf("coffQReg=%d\n", this->coffQReg);
	printf("commonScale=%d\n", this->commonScale);
	printf("--------------------------------\n"); ///* Prints all the members and values of members on screen. 
#endif
}

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::crosstalkTempCoffC::storeToFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ofstream ofs(fileName);
	ofs << this;
	ofs.close(); ///* User needs to implement file storage based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::crosstalkTempCoffC::loadFromFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ifstream ifs(fileName);
	ifs >> this;
	ifs.close(); ///* User needs to implement file load/restore based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::ostream & OPT3101::operator<<(std::ostream & os, const crosstalkTempCoffC * data)
{
	/// <b>Algorithm of the method is as follows</b>
	os << data->coffI << '\n'; ///* Serializes all the members and returns to the stream 
	os << data->coffQ << '\n';
	os << data->coffIReg << '\n';
	os << data->coffQReg << '\n';
	os << data->commonScale << '\n';
	return os;
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::istream & OPT3101::operator>>(std::istream & is, crosstalkTempCoffC * data)
{
	/// <b>Algorithm of the method is as follows</b>
	is >> data->coffI; ///* Reads from stream and de-serializes all the members 
	is >> data->coffQ;
	is >> data->coffIReg;
	is >> data->coffQReg;
	is >> data->commonScale;
	return is;

}
#endif

OPT3101::phaseTempCoffC::phaseTempCoffC()
{
	/// <b>Algorithm of the method is as follows</b>
	this->coff = 0.0; ///* Initializes all members to 0. Sets OPT3101::phaseTempCoffC::istMainCoff to true
	this->coffReg = 0;
	this->commonScale = 0;
	this->istMainCoff = true;
}

void OPT3101::phaseTempCoffC::calculateCoff(OPT3101::frameData *frameData0, OPT3101::frameData *frameData1, bool istMainCoff)
{
	int32_t phase[2];

	phase[0] = frameData0->phase;
	phase[1] = frameData1->phase;
	/// <b>Algorithm of the method is as follows</b>
	if (phase[0] > 49152 && phase[1] < 16384) // 3/4 of the 16 unsigned
		phase[1] += 65536; //Adding 2**16 to this 
	if (phase[1] > 49152 && phase[0] < 16384) // 3/4 of the 16 unsigned
		phase[0] += 65536; //Adding 2**16 to this 
	///* Makes the data base circular to avoid errorneous high slope measurements
	this->istMainCoff = istMainCoff; ///* Assigns OPT3101::phaseTempCoffC::istMainCoff to the value assigned by argument
	if (istMainCoff)
		if (!(frameData0->tmain - frameData1->tmain) == 0)
			this->coff = ((double)((int32_t)phase[0] - (int32_t)phase[1])) / (frameData0->tmain - frameData1->tmain); ///* Calculates the double precision OPT3101::phaseTempCoffC::coff  by finding difference between OPT3101::frameData::phase divided by delta OPT3101::frameData::tmain or  OPT3101::frameData::tillum based on input flag
		else
			this->coff = 0.0;
	else
		if (!(frameData0->tillum - frameData1->tillum) == 0)
			this->coff = ((double)((int32_t)phase[0] - (int32_t)phase[1])) / (frameData0->tillum - frameData1->tillum);
		else
			this->coff = 0.0;
	///* <b>Warning:</b> Makes the OPT3101::phaseTempCoffC::coff 0 if the OPT3101::frameData::tmain or OPT3101::frameData::tillum are same for both the input arguments

}

void OPT3101::phaseTempCoffC::report()
{
#ifdef OPT3101_USE_STDIOLIB
	/// <b>Algorithm of the method is as follows</b>
	printf("----------------------------\n");
	printf("Phase Temp Coff Class Report\n");
	printf("----------------------------\n"); ///* Prints all the members and values of members on screen. 
	printf("coff=%4.2f\n", this->coff);
	printf("coffReg=%d\n", this->coffReg);
	printf("commonScale=%d\n", this->commonScale);
	printf("----------------------------\n"); ///* Prints all the members and values of members on screen. 
#endif
}

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::phaseTempCoffC::storeToFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ofstream ofs(fileName);
	ofs << this;
	ofs.close(); ///* User needs to implement file storage based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::phaseTempCoffC::loadFromFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ifstream ifs(fileName);
	ifs >> this;
	ifs.close(); ///* User needs to implement file load/restore based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::ostream & OPT3101::operator<<(std::ostream & os, const phaseTempCoffC * data)
{
	/// <b>Algorithm of the method is as follows</b>
	os << data->coff << '\n'; ///* Serializes all the members and returns to the stream 
	os << data->coffReg << '\n';
	os << data->commonScale << '\n';
	os << data->istMainCoff << '\n';
	return os;
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::istream & OPT3101::operator>>(std::istream & is, phaseTempCoffC * data)
{
	/// <b>Algorithm of the method is as follows</b>
	is >> data->coff; ///* Reads from stream and de-serializes all the members 
	is >> data->coffReg;
	is >> data->commonScale;
	is >> data->istMainCoff;
	return is;
}
#endif

OPT3101::phaseAmbientCoffC::phaseAmbientCoffC()
{
	uint8_t c0;
	/// <b>Algorithm of the method is as follows</b>
	for (c0 = 0; c0 < 4; c0++)
		this->coff[c0] = 0; ///* Serializes all the members and returns to the stream 
	for (c0 = 0; c0<4; c0++)
		this->coffReg[c0] = 0;
	for (c0 = 0; c0 < 3; c0++)
		this->splitsReg[c0] = 0;
	this->commonScale = 0;
}

void OPT3101::phaseAmbientCoffC::calculateCoff(uint16_t freqCount)
{
	int8_t c0;
	uint8_t scaleCoff;

	double maxCoff=-1;
	/// <b>Algorithm of the method is as follows</b>
	for(c0=1;c0<4;c0++)
		maxCoff = abs(this->coff[c0]) > maxCoff ? abs(this->coff[c0]) : maxCoff; ///* Identifies max of absolute of OPT3101::phaseAmbientCoffC::coff values 
	
	maxCoff *= 16384 / freqCount; ///* scales the max coefficient with input frequencyCount 
	scaleCoff = 0;
	for (c0 = 0; c0 < 8; c0++) {
		scaleCoff = (maxCoff*(1 << (c0 + 2))) < (1 << 7) ? c0 : scaleCoff; ///* Finds a OPT3101::phaseAmbientCoffC::commonScale which can fit the OPT3101::phaseAmbientCoffC::coff to 8 bit registers
	}
	this->commonScale = scaleCoff;
	for (c0 = 0; c0 < 4; c0++) 
		this->coffReg[c0] = (uint8_t) (16384.0 / freqCount * this->coff[c0]*(1 << (this->commonScale+2)));  ///* Finds OPT3101::phaseAmbientCoffC::coffReg values (8 bit register values) based on OPT3101::phaseAmbientCoffC::coff and OPT3101::phaseAmbientCoffC::commonScale
}

void OPT3101::phaseAmbientCoffC::report()
{
#ifdef OPT3101_USE_STDIOLIB
	uint8_t c0;
	/// <b>Algorithm of the method is as follows</b>
	printf("-------------------------------\n");
	printf("Phase Ambient Coff Class Report\n");
	printf("-------------------------------\n"); ///* Prints all the members and values of members on screen. 
	for(c0=0;c0<4;c0++)
		printf("coff[%d]=%4.2f\n", c0,this->coff[c0]);
	for (c0 = 0; c0<4; c0++)
		printf("coffReg[%d]=%d\n", c0, this->coffReg[c0]);
	for (c0 = 0; c0<3; c0++)
		printf("splitsReg[%d]=%d\n", c0, this->splitsReg[c0]);
	printf("commonScale=%d\n", this->commonScale);
	printf("-------------------------------\n"); ///* Prints all the members and values of members on screen. 
#endif
}

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::phaseAmbientCoffC::storeToFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ofstream ofs(fileName);
	ofs << this;
	ofs.close(); ///* User needs to implement file storage based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::phaseAmbientCoffC::loadFromFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ifstream ifs(fileName);
	ifs >> this;
	ifs.close(); ///* User needs to implement file load/restore based on host. 
#endif
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::ostream & OPT3101::operator<<(std::ostream & os, const phaseAmbientCoffC * data)
{
	uint8_t c0;
	/// <b>Algorithm of the method is as follows</b>
	
	for(c0=0;c0<4;c0++)
		os << data->coff[c0] << '\n'; ///* Serializes all the members and returns to the stream 
	for (c0 = 0; c0<4; c0++)
		os << data->coffReg[c0] << '\n'; 
	for (c0 = 0; c0<3; c0++)
		os << data->splitsReg[c0] << '\n';
	os << data->commonScale << '\n';

	return os;
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::istream & OPT3101::operator>>(std::istream & is, phaseAmbientCoffC * data)
{
	/// <b>Algorithm of the method is as follows</b>
	uint8_t c0;
	for (c0 = 0; c0 < 4; c0++) ///* Reads from stream and de-serializes all the members 
		is >> data->coff[c0];
	for (c0 = 0; c0 < 4; c0++)
		is >> data->coffReg[c0];
	for (c0 = 0; c0 < 3; c0++)
		is >> data->splitsReg[c0];
	is >> data->commonScale;

	return is;

}
#endif

