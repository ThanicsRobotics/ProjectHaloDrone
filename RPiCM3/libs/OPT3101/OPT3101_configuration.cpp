#include "OPT3101device.h"
/* OPT3101 device initialization Sequence Header file written by OPT3101 Calibration tool - Version 0.8.0
By: a0875461
On: 2018-05-29 23:53:47
Configuration Settings:
{
	"genDeviceI2CMasterTempSensorAddress": 0,
	"superHDRMode": true,
	"genDeviceEEPROMConnection": true,
	"genDeviceI2CMasterConnection": 1,
	"autoHDRMode": true,
	"toolVersion": 0.8.0,
	"superHDRHThreshold": 26000,
	"genDeviceI2CMasterTempSensorMode": tmp102,
	"monoshotMode": false,
	"genDeviceAmbientSupport": 4,
	"crc": 448535433,
	"freqRefClock": 125000,
	"superHDRRatio": 64.0,
	"genDeviceSubFrameCount": 32,
	"user": a0875461,
	"genDeviceAvgFrameCount": 4,
	"timeStamp": 2018-05-29 23:53:25,
	"genDeviceI2CMasterTempSensor": true,
	"freqComp": true,
	"superHDRTXCurrent": 0,
	"superHDRTXDCBias": 0,
}
*/
void OPT3101::device::initialize(){
	// List of registers to initialize OPT3101 device after power-up

	this->reg.tg_ovl_window_start = 7000; // //Overload flab observation window
	this->reg.en_temp_conv = 1; // //Enables the internal

	this->reg.config_tillum_msb = 8; // //Mode select between temp sensors
	this->reg.en_tillum_12b = 1; // //Chooses between 12bit or 8 bit mode
	this->reg.en_tillum_read = 1; // //Enable Ext temp sensor read
	this->reg.temp_avg_illum = 0; // //Sets number of average for ext temp sensor measurement
	this->reg.frame_vd_trig = 1; // //Trigger temp sensor measurement every VD
	this->reg.i2c_en = 1;
	this->reg.i2c_num_tran = 0; // //not required since default is correct
	this->reg.i2c_rw = 1;
	this->reg.i2c_num_bytes_tran1 = 1; ///<

	this->reg.tsens_slave0 = 0x48; // //I2C Address for External Temp sensor for TX0 in super HDR Mode
	this->reg.tsens_slave1 = 0x48; // //I2C Address for External Temp sensor for TX1 in super HDR Mode

	this->reg.clip_mode_fc = 1; // //Enables Clip mode for Frequency correction
	this->reg.clip_mode_temp = 0; // //Disables Clip mode for Temp coff phase correction
	this->reg.clip_mode_offset = 0; // //Disables Clip mode for phase offset correction
	this->reg.iq_read_data_sel = 3; // //Enables 16 bit frame counter
	this->reg.iamb_max_sel = 12; // //Sets maximum ambient support
	this->reg.en_temp_corr = 1; // //Enables Temperature Correction
	this->reg.gpio1_obuf_en=1; // //Enabled output buffer on GPIO1 pin
	this->reg.gpo1_mux_sel=2; 	    // //select dig_gpo_0 on gpio1
	this->reg.dig_gpo_sel0 = 9; 	// //Select Data Ready on dig_gpo_0

	this->reg.en_cont_fcalib=1; // //Enables continuous frequency calibration
	this->reg.start_freq_calib=1; // //Starts the frequency calibration block
	this->reg.en_floop=1; // //Enables the frequency correction loop
	this->reg.en_auto_freq_count=1; // //Enables automatic frequency count
	this->reg.en_freq_corr=1; // //Enables digital frequency correction
	this->reg.sys_clk_divider = 8;  // //Divider for system clock
	this->reg.ref_count_limit = 20480; // //Counter limit
	this->reg.gpio2_obuf_en=0; // //Disables output buffer on GPIO2
	this->reg.gpio2_ibuf_en=1; // //Enables ref clock input of GPIO2

	this->reg.num_sub_frames = 31; // //Sub frames count
	this->reg.num_avg_sub_frames = 31; // //Average frames count
	this->reg.xtalk_filt_time_const = 5; // //Crosstalk filter time constant
	this->reg.tg_seq_int_start 	= 9850; // //Sequence Start
	this->reg.tg_seq_int_end 		= 9858; // //Sequence End
	this->reg.tg_seq_int_mask_start 	= 31; // //Same as AvgFrame Count
	this->reg.tg_seq_int_mask_end 	= 31; // //Same as AvgFrame Count

	this->reg.en_processor_values = 1; // //Enables processor values
	this->reg.en_sequencer = 1; // //Enables the Sequencer
	this->reg.command0 = 0x108; // //Set Channel 1
	this->reg.command1 = 0xB02; // //COMP1.
	this->reg.command2 = 0x100; // //Set Channel 0
	this->reg.command3 = 0xC00; // //COMP1_INV
	this->reg.compare_reg1 = 26000; // //ThresholdH
	this->reg.compare_reg2 = 4312; // //ThresholdH
	this->reg.mux_sel_compin = 0; // //Selects Amplitude for Comparison
	this->reg.en_tx1_on_tx0 = 1; // //Setting TX1 register and connect to TX0

	this->reg.hdr_thr_high = 25500; // //High Threshold
	this->reg.hdr_thr_low = 4812; // //Low Threshold
	this->reg.en_adaptive_hdr = 1; // //Enables adaptive HDR feature

	this->reg.illum_dac_h_tx0 = 2; // //High Current settings [011.2mA:5.6mA X 02]
	this->reg.illum_scale_h_tx0 = 0; // //Illum scale for H [011.2mA:5.6mA X 02]

	this->reg.illum_dac_l_tx0 = 1; // //High Current settings [002.8mA:2.8mA X 01]
	this->reg.illum_scale_l_tx0 = 2; // //Illum scale for H [002.8mA:2.8mA X 01]

	this->reg.illum_dac_h_tx1 = 31; // //High Current settings [173.6mA:5.6mA X 31]
	this->reg.illum_scale_h_tx1 = 0; // //Illum scale for H [173.6mA:5.6mA X 31]

	this->reg.illum_dac_l_tx1 = 31; // //High Current settings [043.4mA:1.4mA X 31]
	this->reg.illum_scale_l_tx1 = 3; // //Illum scale for H [043.4mA:1.4mA X 31]

	this->reg.tg_en = 1; // //Enables Timing Generator

	this->configurationFlags_xtalkFilterTau =  5; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_monoshotMode =  false; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_xtalkSettlingOneTauInMilliSeconds      =  256; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_xtalkSettlingOneTauInDataReadyCounts   =  32; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_frameTimeInMilliSeconds                =  8; // //This is not a register but a settings flag for the SDK
	this->configurationFlags_avgFrameCountExponentOfTwo             =  5; // //This is not a register but a settings flag for the SDK

}

OPT3101::device::device():
configurationFlags_isTXChannelActive{true,true,false},
configurationFlags_isRegisterSetActive{true,true} {this->calibration = new calibrationC[1];}

OPT3101::calibrationC::calibrationC() : calibrationC(true) {
	this->recordLength           = 4; // //This configuration requires 4 crosstalk and other configuration record(s)
	this->internalCrosstalk      = new OPT3101::crosstalkC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->illumCrosstalk         = new OPT3101::crosstalkC[4];   // //This configuration requires 4 internal crosstalk record(s)
	this->phaseOffset            = new OPT3101::phaseOffsetC[4]; // //This configuration requires 4 internal crosstalk record(s)
	this->illumCrosstalkTempCoff = new OPT3101::crosstalkTempCoffC[4]; // //This configuration requires 4 internal crosstalk record(s)
	this->phaseTempCoff          = new OPT3101::phaseTempCoffC[4]; // //This configuration requires 4 internal crosstalk record(s)
	this->phaseAmbientCoff       = new OPT3101::phaseAmbientCoffC[1]; // //This configuration requires 1 internal crosstalk record(s)
	this->registerAddressListSize= 36; // //This configuration requires 36 registers [1152] bits to be stored for calibration
	this->registerAddressList    = new uint8_t[this->registerAddressListSize]; // //Allocation of memory to store register addresses
	this->EEPROM_connected       =  true; // //This configuration helps configure EEPROM
	this->extTempSensor_connected=  false; // //This configuration helps configure Ext temp sensor
	this->registerAddressList[0] = 0x2f; // //Address for register(s) iphase_xtalk_reg_hdr0_tx0,temp_coeff_main_hdr1_tx1
	this->registerAddressList[1] = 0x30; // //Address for register(s) qphase_xtalk_reg_hdr0_tx0,temp_coeff_main_hdr1_tx1
	this->registerAddressList[2] = 0x38; // //Address for register(s) temp_coeff_xtalk_iphase_hdr0_tx0
	this->registerAddressList[3] = 0x39; // //Address for register(s) temp_coeff_xtalk_qphase_hdr0_tx0
	this->registerAddressList[4] = 0x42; // //Address for register(s) phase_offset_hdr0_tx0
	this->registerAddressList[5] = 0x47; // //Address for register(s) tmain_calib_hdr0_tx0,tillum_calib_hdr0_tx0
	this->registerAddressList[6] = 0x45; // //Address for register(s) temp_coeff_main_hdr0_tx0
	this->registerAddressList[7] = 0x46; // //Address for register(s) temp_coeff_illum_hdr0_tx0
	this->registerAddressList[8] = 0x31; // //Address for register(s) iphase_xtalk_reg_hdr1_tx0
	this->registerAddressList[9] = 0x32; // //Address for register(s) qphase_xtalk_reg_hdr1_tx0
	this->registerAddressList[10] = 0x5e; // //Address for register(s) temp_coeff_xtalk_iphase_hdr1_tx0,temp_coeff_xtalk_iphase_hdr0_tx1
	this->registerAddressList[11] = 0x60; // //Address for register(s) temp_coeff_xtalk_qphase_hdr1_tx0,temp_coeff_xtalk_qphase_hdr0_tx1,temp_coeff_xtalk_qphase_hdr1_tx1
	this->registerAddressList[12] = 0x51; // //Address for register(s) phase_offset_hdr1_tx0,temp_coeff_illum_hdr1_tx0
	this->registerAddressList[13] = 0x48; // //Address for register(s) tmain_calib_hdr1_tx0,tillum_calib_hdr1_tx0
	this->registerAddressList[14] = 0x2d; // //Address for register(s) temp_coeff_main_hdr1_tx0,temp_coeff_main_hdr0_tx1
	this->registerAddressList[15] = 0x52; // //Address for register(s) temp_coeff_illum_hdr1_tx0,phase_offset_hdr0_tx1
	this->registerAddressList[16] = 0x33; // //Address for register(s) iphase_xtalk_reg_hdr0_tx1
	this->registerAddressList[17] = 0x34; // //Address for register(s) qphase_xtalk_reg_hdr0_tx1
	this->registerAddressList[18] = 0x49; // //Address for register(s) tmain_calib_hdr0_tx1,tillum_calib_hdr0_tx1
	this->registerAddressList[19] = 0x53; // //Address for register(s) temp_coeff_illum_hdr0_tx1,phase_offset_hdr1_tx1
	this->registerAddressList[20] = 0x54; // //Address for register(s) temp_coeff_illum_hdr0_tx1
	this->registerAddressList[21] = 0x35; // //Address for register(s) iphase_xtalk_reg_hdr1_tx1
	this->registerAddressList[22] = 0x36; // //Address for register(s) qphase_xtalk_reg_hdr1_tx1
	this->registerAddressList[23] = 0x5f; // //Address for register(s) temp_coeff_xtalk_iphase_hdr1_tx1
	this->registerAddressList[24] = 0x41; // //Address for register(s) tmain_calib_hdr1_tx1
	this->registerAddressList[25] = 0x43; // //Address for register(s) tillum_calib_hdr1_tx1,en_phase_corr,en_temp_corr,scale_phase_temp_coeff
	this->registerAddressList[26] = 0x55; // //Address for register(s) temp_coeff_illum_hdr1_tx1
	this->registerAddressList[27] = 0x56; // //Address for register(s) temp_coeff_illum_hdr1_tx1
	this->registerAddressList[28] = 0x2e; // //Address for register(s) illum_xtalk_reg_scale
	this->registerAddressList[29] = 0x3a; // //Address for register(s) scale_temp_coeff_xtalk
	this->registerAddressList[30] = 0x71; // //Address for register(s) shift_illum_phase
	this->registerAddressList[31] = 0xb5; // //Address for register(s) scale_amb_phase_corr_coeff
	this->registerAddressList[32] = 0x0c; // //Address for register(s) amb_phase_corr_pwl_coeff0
	this->registerAddressList[33] = 0xb4; // //Address for register(s) amb_phase_corr_pwl_coeff1,amb_phase_corr_pwl_coeff2,amb_phase_corr_pwl_coeff3
	this->registerAddressList[34] = 0xb8; // //Address for register(s) amb_phase_corr_pwl_x0,amb_phase_corr_pwl_x1
	this->registerAddressList[35] = 0xb9; // //Address for register(s) amb_phase_corr_pwl_x2

}

