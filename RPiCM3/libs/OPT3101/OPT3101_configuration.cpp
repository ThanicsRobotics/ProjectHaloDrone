/*
Copyright (c) 2018 Texas Instruments Incorporated
All rights reserved not granted herein.
Limited License.  
Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (TI Devices).  No hardware patent is licensed hereunder.
Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution
Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:
*	No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.
*	any redistribution and use are licensed by TI for use only with TI Devices.
*	Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:
*	any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.
*	any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.
Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.
DISCLAIMER.
THIS SOFTWARE IS PROVIDED BY TI AND TI's LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "OPT3101device.h"


/* OPT3101 device initialization Sequence Header file written by OPT3101 Calibration tool - Version 0.8.0
By: andre
On: 2018-07-25 21:31:35
Configuration Settings:
{
	"TXDCCurrent": 0,
	"superHDRMode": false,
	"genDeviceEEPROMConnection": false,
	"genDeviceI2CMasterConnection": 0,
	"autoHDRMode": false,
	"toolVersion": 0.8.0,
	"multiLEDMode": true,
	"monoshotMode": false,
	"genDeviceAmbientSupport": 4,
	"TX0HCurrent": 0,
	"crc": 1306148662,
	"genDeviceSubFrameCount": 128,
	"user": andre,
	"genDeviceAvgFrameCount": 7,
	"timeStamp": 2018-07-25 21:31:29,
	"freqComp": false,
	"LEDOrder": 131586,
}
*/
void OPT3101::device::initialize(void){
	// List of registers to initialize OPT3101 device after power-up

	this->reg.force_en_slave = 1; // //Since I2C Master bus is floating this register needs to be set to enable device to respond

	this->reg.tg_ovl_window_start = 7000; // //Overload flab observation window
	this->reg.en_temp_conv = 1; // //Enables the internal

	this->reg.clip_mode_fc = 1; // //Enables Clip mode for Frequency correction
	this->reg.clip_mode_temp = 0; // //Disables Clip mode for Temp coff phase correction
	this->reg.clip_mode_offset = 0; // //Disables Clip mode for phase offset correction
	this->reg.iq_read_data_sel = 3; // //Enables 16 bit frame counter
	this->reg.iamb_max_sel = 12; // //Sets maximum ambient support
	this->reg.en_temp_corr = 1; // //Enables Temperature Correction
	this->reg.gpio1_obuf_en=1; // //Enabled output buffer on GPIO1 pin
	this->reg.gpo1_mux_sel=2; 	    // //select dig_gpo_0 on gpio1
	this->reg.dig_gpo_sel0 = 9; 	// //Select Data Ready on dig_gpo_0

	this->reg.num_sub_frames = 127; // //Sub frames count
	this->reg.num_avg_sub_frames = 127; // //Average frames count
	this->reg.xtalk_filt_time_const = 3; // //Crosstalk filter time constant
	this->reg.tg_seq_int_start 	= 9850; // //Sequence Start
	this->reg.tg_seq_int_end 		= 9858; // //Sequence End
	this->reg.tg_seq_int_mask_start 	= 127; // //Same as AvgFrame Count
	this->reg.tg_seq_int_mask_end 	= 127; // //Same as AvgFrame Count

	this->reg.illum_dac_h_tx0 = 31; // //High Current settings [173.6mA:5.6mA X 31]
	this->reg.illum_scale_h_tx0 = 0; // //Illum scale for H [173.6mA:5.6mA X 31]

	this->reg.sel_hdr_mode = 1; // //Selecting H Channel by default

	this->reg.tx_seq_reg   = 2184; // //Setting TX Switching order
	this->reg.en_tx_switch = 1 ; // //Enable TX Switching order

	this->reg.tg_en = 1; // //Enables Timing Generator

	this->configurationFlags_xtalkFilterTau =  3; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_monoshotMode =  false; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_xtalkSettlingOneTauInMilliSeconds      =  256; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_xtalkSettlingOneTauInDataReadyCounts   =  8; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_frameTimeInMilliSeconds                =  32; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_avgFrameCountExponentOfTwo             =  7; // //This is not a register but a settings flag for the SDK

}

OPT3101::device::device(void):
configurationFlags_isTXChannelActive{true,false,false},
configurationFlags_isRegisterSetActive{false,true} {this->calibration = new calibrationC[1];}

OPT3101::calibrationC::calibrationC(void) : calibrationC(true) {
	this->recordLength           = 1; // //This configuration requires 1 crosstalk and other configuration record(s)
	this->internalCrosstalk      = new OPT3101::crosstalkC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->illumCrosstalk         = new OPT3101::crosstalkC[1];   // //This configuration requires 1 internal crosstalk record(s)
	this->phaseOffset            = new OPT3101::phaseOffsetC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->illumCrosstalkTempCoff = new OPT3101::crosstalkTempCoffC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->phaseTempCoff          = new OPT3101::phaseTempCoffC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->phaseAmbientCoff       = new OPT3101::phaseAmbientCoffC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->registerAddressListSize= 17; // //This configuration requires 17 registers [544] bits to be stored for calibration
	this->registerAddressList    = new uint8_t[this->registerAddressListSize]; // //Allocation of memory to store register addresses
	this->EEPROM_connected       =  false; // //This configuration helps configure EEPROM
	this->extTempSensor_connected=  true; // //This configuration helps configure Ext temp sensor
	this->registerAddressList[0] = 0x31; // //Address for register(s) iphase_xtalk_reg_hdr1_tx0
	this->registerAddressList[1] = 0x32; // //Address for register(s) qphase_xtalk_reg_hdr1_tx0
	this->registerAddressList[2] = 0x5e; // //Address for register(s) temp_coeff_xtalk_iphase_hdr1_tx0
	this->registerAddressList[3] = 0x60; // //Address for register(s) temp_coeff_xtalk_qphase_hdr1_tx0
	this->registerAddressList[4] = 0x51; // //Address for register(s) phase_offset_hdr1_tx0,temp_coeff_illum_hdr1_tx0
	this->registerAddressList[5] = 0x48; // //Address for register(s) tmain_calib_hdr1_tx0,tillum_calib_hdr1_tx0
	this->registerAddressList[6] = 0x2d; // //Address for register(s) temp_coeff_main_hdr1_tx0
	this->registerAddressList[7] = 0x52; // //Address for register(s) temp_coeff_illum_hdr1_tx0
	this->registerAddressList[8] = 0x2e; // //Address for register(s) illum_xtalk_reg_scale
	this->registerAddressList[9] = 0x3a; // //Address for register(s) scale_temp_coeff_xtalk
	this->registerAddressList[10] = 0x71; // //Address for register(s) shift_illum_phase
	this->registerAddressList[11] = 0x43; // //Address for register(s) en_phase_corr,en_temp_corr,scale_phase_temp_coeff
	this->registerAddressList[12] = 0xb5; // //Address for register(s) scale_amb_phase_corr_coeff
	this->registerAddressList[13] = 0x0c; // //Address for register(s) amb_phase_corr_pwl_coeff0
	this->registerAddressList[14] = 0xb4; // //Address for register(s) amb_phase_corr_pwl_coeff1,amb_phase_corr_pwl_coeff2,amb_phase_corr_pwl_coeff3
	this->registerAddressList[15] = 0xb8; // //Address for register(s) amb_phase_corr_pwl_x0,amb_phase_corr_pwl_x1
	this->registerAddressList[16] = 0xb9; // //Address for register(s) amb_phase_corr_pwl_x2

}

