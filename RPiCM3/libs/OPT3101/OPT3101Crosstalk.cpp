/*!
* \file OPT3101Crosstalk.cpp
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
* The file contains class OPT3101::crosstalkC member function definitions
*/

#include "OPT3101Crosstalk.h"
#include "hostController.h"


OPT3101::crosstalkC::crosstalkC(){
	/// <b>Algorithm of the method is as follows</b>
	this->I = 0;
	this->Q = 0;
	this->xtalkScale = 0;
	this->illumScale = 0;
	this->illumDac = 0;
	this->illumDCdac = 0;
	this->tmain = 0;
	this->tillum = 0;
	this->shiftIllumPhase = 0;
	this->commonScale = 0;
	this->illumXtalk=false; ///* sets OPT3101::crosstalkC::illumXtalk to false by default
}

void OPT3101::crosstalkC::readCrosstalkRegisters(OPT3101::device *dev){
	int32_t iRead,qRead;
	uint32_t mRead;
	uint8_t c0;

	/// <b>Algorithm of the method is as follows</b>
	iRead=((int32_t) (dev->reg.iphase_xtalk.read()<<8))>>8; ///* Reads register OPT3101::registers::iphase_xtalk in 24 bit from and converts to signed number
	qRead=((int32_t) (dev->reg.qphase_xtalk.read()<<8))>>8; ///* Reads register OPT3101::registers::qphase_xtalk in 24 bit from and converts to signed number
	mRead=abs(iRead)>abs(qRead)?abs(iRead):abs(qRead); ///* Finds absolute max among the read I and Q register values to determine the OPT3101::crosstalkC::illumXtalk

	this->xtalkScale = 0;
	for (c0 = 15; c0 < 23; c0++)
		if (mRead > (((uint32_t)1) << c0) && mRead <= (((uint32_t)1) << (c0 + 1))){
			this->xtalkScale = c0+1 - 15;
			break;
		}
	///* Determines OPT3101::crosstalkC::illumXtalk and assigns. The algorithm finds the minimum OPT3101::crosstalkC::illumXtalk value for which the raw I and Q registers can be fit to a 16 bit register 
	this->I=(int16_t) (iRead>>this->xtalkScale); ///* Scales down the  24 bit raw register I and Q values with OPT3101::crosstalkC::illumXtalk and assigns to OPT3101::crosstalkC::I and OPT3101::crosstalkC::Q
	this->Q=(int16_t) (qRead>>this->xtalkScale);
}

double OPT3101::crosstalkC::magnitude(){
	double amplitude;
	/// <b>Algorithm of the method is as follows</b>
	amplitude=sqrt(pow(this->I>>(9-this->xtalkScale),2)+pow(this->Q>>(9-this->xtalkScale),2))*1.646; ///* Calculates the magnitude of crosstalk based on a predefined formula. <b>Warning:</b> This uses floating point arithmetic and math.h library for power and sqrt functions
	return amplitude;
}

void OPT3101::crosstalkC::readTemperatureMain(OPT3101::device *dev){
	/// <b>Algorithm of the method is as follows</b>
	this->tmain=dev->reg.tmain.read();  ///* Reads register OPT3101Register::tmain and assigns to OPT3101::crosstalkC::tmain
}
void OPT3101::crosstalkC::readTemperatureIllum(OPT3101::device *dev){
	/// <b>Algorithm of the method is as follows</b>
	this->tillum=dev->reg.tillum.read(); ///* Reads register OPT3101Register::illum and assigns to OPT3101::crosstalkC::illum
}

#ifdef OPT3101_USE_STDIOLIB
void OPT3101::crosstalkC::storeToFile(char *fileName)
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
void OPT3101::crosstalkC::loadFromFile(char * fileName)
{
#ifdef OPT3101_USE_STREAMLIB
	/// <b>Algorithm of the method is as follows</b>
	std::ifstream ifs(fileName);
	ifs >> this;
	ifs.close(); ///* User needs to implement file load/restore based on host. 
#endif
}
#endif

void OPT3101::crosstalkC::report()
{
#ifdef OPT3101_USE_STDIOLIB
	// This is template function to report and analyze the crosstalk value. 
	/// <b>Algorithm of the method is as follows</b>
	printf("----------------------\n");
	printf("Crosstalk Class Report\n");
	printf("----------------------\n"); ///* Prints all the members and values of members on screen. 
	printf("I=%d\n", this->I);
	printf("Q=%d\n", this->Q); ///* User needs to implement file load/restore based on host. 
	printf("xtalkScale=%d\n", this->xtalkScale);
	printf("illumXtalk=%d\n", this->illumXtalk);
	printf("illumScale=%d\n", this->illumScale);
	printf("illumDac=%d\n", this->illumDac);
	printf("tmain=%d\n", this->tmain);
	printf("tillum=%d\n", this->tillum);
	printf("shiftIllumPhase=%d\n", this->shiftIllumPhase);
	printf("commonScale=%d\n", this->commonScale);
	printf("Magnitude=%4.2f\n", this->magnitude());
	printf("----------------------\n");
#endif
}

#ifdef OPT3101_USE_STREAMLIB
std::istream& OPT3101::operator>>(std::istream &is, OPT3101::crosstalkC *data) {
	/// <b>Algorithm of the method is as follows</b>
	is >> data->I; ///* From input stream members are de-serialized and restored 
	is >> data->Q;
	is >> data->xtalkScale;
	is >> data->illumXtalk;
	is >> data->illumScale;
	is >> data->illumDac;
	is >> data->illumDCdac;
	is >> data->tmain;
	is >> data->tillum;
	is >> data->shiftIllumPhase;
	is >> data->commonScale;
	return is;
}
#endif

#ifdef OPT3101_USE_STREAMLIB
std::ostream& OPT3101::operator<<(std::ostream &os, const OPT3101::crosstalkC *data) {
	os << data->I << '\n'; ///* From class serialize the members
	os << data->Q << '\n';
	os << data->xtalkScale << '\n';
	os << data->illumXtalk << '\n';
	os << data->illumScale << '\n';
	os << data->illumDac << '\n';
	os << data->illumDCdac << '\n';
	os << data->tmain << '\n';
	os << data->tillum << '\n';
	os << data->shiftIllumPhase << '\n';
	os << data->commonScale << '\n';
	//printf("Writing the data to file I[%d] Q[%d]\n", xtalk->I, xtalk->Q);
	return os;
}
#endif
