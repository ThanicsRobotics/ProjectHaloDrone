/*!
* \file OPT3101RegisterDefinition.h
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
* The file contains OPT3101::registers class declaration  
*/

#ifndef OPT3101REGISTERDEFINITION_H_
#define OPT3101REGISTERDEFINITION_H_

#include "register.h"
/*! \class OPT3101::registers
\brief Class that enumerates all registers in OPT3101.

Class that enlists and declares all OPT3101 registers accesible to user. The names of the registers match with names in the <a href="http://www.ti.com/lit/ds/symlink/opt3101.pdf">datasheet</a>
These registers are directly accesible to the user in the program. <br>
For example: OPT3101::registers::hdr_mode = 1 would set the register hdr_mode to 1 <br>
Similarly to read the value of the register users need to call the OPT3101::deviceRegister::read() function like  OPT3101::registers::hdr_mode::read()
*/
namespace OPT3101{
class registers {
	public:
		/*!
		* \brief Constructor for class OPT3101::registers
		* Constructor allocates OPT3101::deviceRegister::size to each OPT3101::deviceRegister instance on construction 
		*/
		registers();
		registers(const hostController& host);
		
		OPT3101::deviceRegister dig_gpo_sel0; ///< dig_gpo_sel0;Register Addresses: 11[3:0];
		OPT3101::deviceRegister dig_gpo_sel1; ///< dig_gpo_sel1;Register Addresses: 11[7:4];
		OPT3101::deviceRegister dig_gpo_sel2; ///< dig_gpo_sel2;Register Addresses: 11[13:10];
		OPT3101::deviceRegister dis_ovl_gating; ///< dis_ovl_gating;Register Addresses: 17[15:15];
		OPT3101::deviceRegister phase_out; ///< phase_out;Register Addresses: 8[15:0];
		OPT3101::deviceRegister phase_overflow; ///< phase_overflow;Register Addresses: 8[16:16];
		OPT3101::deviceRegister hdr_mode; ///< hdr_mode;Register Addresses: 8[17:17];
		OPT3101::deviceRegister tx_channel; ///< tx_channel;Register Addresses: 8[19:18];
		OPT3101::deviceRegister frame_status; ///< frame_status;Register Addresses: 8[20:20];
		OPT3101::deviceRegister mod_freq; ///< mod_freq;Register Addresses: 8[21:21];
		OPT3101::deviceRegister frame_count0; ///< frame_count0;Register Addresses: 8[23:23];
		OPT3101::deviceRegister amp_out; ///< amp_out;Register Addresses: 9[15:0];
		OPT3101::deviceRegister frame_count1; ///< frame_count1;Register Addresses: 9[17:16];
		OPT3101::deviceRegister sig_ovl_flag; ///< sig_ovl_flag;Register Addresses: 9[18:18];
		OPT3101::deviceRegister dealias_bin; ///< dealias_bin;Register Addresses: 9[23:20];
		OPT3101::deviceRegister frame_count2; ///< frame_count2;Register Addresses: 10[1:0];
		OPT3101::deviceRegister amb_data; ///< amb_data;Register Addresses: 10[11:2];
		OPT3101::deviceRegister tmain; ///< tmain;Register Addresses: 10[23:12];
		OPT3101::deviceRegister amplitude_min_thr; ///< amplitude_min_thr;Register Addresses: 16[23:16], 17[23:16];
		OPT3101::deviceRegister amb_ovl_flag; ///< amb_ovl_flag;Register Addresses: 8[22:22];
		OPT3101::deviceRegister phase_overflow_f2; ///< phase_overflow_f2;Register Addresses: 9[19:19];
		OPT3101::deviceRegister ref_count_limit; ///< ref_count_limit;Register Addresses: 15[14:0];this sets the limit of ref-clock count when meth1 is used. By programming this we no longer require frequencies which are multiples of powers of 2.;;The default is calculated for 32.768 Khz.
		OPT3101::deviceRegister start_freq_calib; ///< start_freq_calib;Register Addresses: 15[16:16];starts the freq_calib
		OPT3101::deviceRegister sys_clk_divider; ///< sys_clk_divider;Register Addresses: 15[20:17];The divider can be set according to the ratio b/w ref_clk and tg_clk. The default is 2 which means default ref_clk is assumed at 10 Mhz. ie 40Mhz/4
		OPT3101::deviceRegister freq_count_read_reg; ///< freq_count_read_reg;Register Addresses: 16[14:0];read register which holds the value of freq_loop.
		OPT3101::deviceRegister freq_count_reg; ///< freq_count_reg;Register Addresses: 17[14:0];The register which is used for frequency correction when enable_auto_freq_count = '0'
		OPT3101::deviceRegister en_auto_freq_count; ///< en_auto_freq_count;Register Addresses: 15[21:21];When this is '1' internally computed values is used. Else register value is used.
		OPT3101::deviceRegister en_floop; ///< en_floop;Register Addresses: 15[22:22];Enables the freq_loop block. If this is '0', the clock to the freq_loop is gated.
		OPT3101::deviceRegister en_freq_corr; ///< en_freq_corr;Register Addresses: 15[23:23];This bit applies frequency correction on the phase data either from register or auto_freq. 
		OPT3101::deviceRegister en_cont_fcalib; ///< en_cont_fcalib;Register Addresses: 16[15:15];
		OPT3101::deviceRegister monoshot_bit; ///< monoshot_bit;Register Addresses: 0[23:23];In monoshot mode the register to trigger a measurement.
		OPT3101::deviceRegister monoshot_mode; ///< monoshot_mode;Register Addresses: 39[1:0];LSB: Enters monoshot mode.;;MSB: If this is set monoshot mode shutdown the oscclk. This has to be used together with monoshot_mode.
		OPT3101::deviceRegister powerup_delay; ///< powerup_delay;Register Addresses: 38[23:10];The synchronous counter delay after the ripple counter expires before ungating the clock. About 256*25ns*2^6 ~ 400 us.
		OPT3101::deviceRegister monoshot_numframe; ///< monoshot_numframe;Register Addresses: 39[7:2];The number of frames of TG to be run after a trigger before shutting down the TG. The default is kept as 6 allowing a led cycle.
		OPT3101::deviceRegister monoshot_fz_clkcnt; ///< monoshot_fz_clkcnt;Register Addresses: 39[23:8];The pix_cnt at which a monoshot operation freezes. By default just freezes 100 cycles before a frame boundary.
		OPT3101::deviceRegister en_tx_switch; ///< en_tx_switch;Register Addresses: 42[0:0];Enable switching of led drivers.
		OPT3101::deviceRegister sel_tx_ch; ///< sel_tx_ch;Register Addresses: 42[2:1];choses the fix_reg value when switching is disabled.
		OPT3101::deviceRegister tx_seq_reg; ///< tx_seq_reg;Register Addresses: 42[14:3];Stores the sequence of led switching in this register.;2-1-0-2-1-0. The sequence will come as 0-1-2-0-1-2 
		OPT3101::deviceRegister en_adaptive_hdr; ///< en_adaptive_hdr;Register Addresses: 42[15:15];enable the adaptive hdr. The num_avg_frame in this case should be programmed one more than the normal case.
		OPT3101::deviceRegister sel_hdr_mode; ///< sel_hdr_mode;Register Addresses: 42[16:16];choses which current to use when enable_adaptive_hdr = '0'
		OPT3101::deviceRegister hdr_thr_low; ///< hdr_thr_low;Register Addresses: 44[15:0];The low threshold of the hysterisis loop. Equivalent to ~64 confidence.
		OPT3101::deviceRegister hdr_thr_high; ///< hdr_thr_high;Register Addresses: 43[15:0];the high threshold of the hyserisis loop. Default equivalent to confidence of 256 in 16 bit level.
		OPT3101::deviceRegister illum_scale_l_tx0; ///< illum_scale_l_tx0;Register Addresses: 43[18:16];
		OPT3101::deviceRegister illum_dac_l_tx0; ///< illum_dac_l_tx0;Register Addresses: 41[4:0];
		OPT3101::deviceRegister illum_scale_h_tx0; ///< illum_scale_h_tx0;Register Addresses: 43[21:19];
		OPT3101::deviceRegister illum_dac_h_tx0; ///< illum_dac_h_tx0;Register Addresses: 41[9:5];
		OPT3101::deviceRegister illum_scale_l_tx1; ///< illum_scale_l_tx1;Register Addresses: 44[18:16];
		OPT3101::deviceRegister illum_dac_l_tx1; ///< illum_dac_l_tx1;Register Addresses: 41[14:10];
		OPT3101::deviceRegister illum_scale_h_tx1; ///< illum_scale_h_tx1;Register Addresses: 44[21:19];
		OPT3101::deviceRegister illum_dac_h_tx1; ///< illum_dac_h_tx1;Register Addresses: 41[19:15];
		OPT3101::deviceRegister illum_scale_l_tx2; ///< illum_scale_l_tx2;Register Addresses: 185[20:18];
		OPT3101::deviceRegister illum_dac_l_tx2; ///< illum_dac_l_tx2;Register Addresses: 41[23:20], 42[23:23];
		OPT3101::deviceRegister illum_scale_h_tx2; ///< illum_scale_h_tx2;Register Addresses: 185[23:21];
		OPT3101::deviceRegister illum_dac_h_tx2; ///< illum_dac_h_tx2;Register Addresses: 42[22:18];
		OPT3101::deviceRegister amb_adc_in_tx0; ///< amb_adc_in_tx0;Register Addresses: 185[13:12];
		OPT3101::deviceRegister amb_adc_in_tx1; ///< amb_adc_in_tx1;Register Addresses: 185[15:14];
		OPT3101::deviceRegister amb_adc_in_tx2; ///< amb_adc_in_tx2;Register Addresses: 185[17:16];
		OPT3101::deviceRegister give_dealias_data; ///< give_dealias_data;Register Addresses: 184[20:20];
		OPT3101::deviceRegister en_dealias_meas; ///< en_dealias_meas;Register Addresses: 64[0:0];enables dealias calculation.;Normally with enable_dealiased_measurement set and enable_multi_freq_phase is unset a combined distance and kb is given out. In the normal phase register, phase of the high frequency itself is given.;
		OPT3101::deviceRegister ncr_config; ///< ncr_config;Register Addresses: 64[21:21];option to chose ncr configuration, that is 6/7 (0) or 6/5 (1). Chooses higher frequency by default.
		OPT3101::deviceRegister alpha0_dealias_scale; ///< alpha0_dealias_scale;Register Addresses: 64[14:9];indicates the vector multiplication in intrinsic-xtalk component for the dealias frequency. Default is '1'.
		OPT3101::deviceRegister beta0_dealias_scale; ///< beta0_dealias_scale;Register Addresses: 64[20:15];
		OPT3101::deviceRegister alpha1_dealias_scale; ///< alpha1_dealias_scale;Register Addresses: 65[5:0];indicates the vector multiplication in optical-xtalk component for the dealias frequency. Default is '1'.
		OPT3101::deviceRegister beta1_dealias_scale; ///< beta1_dealias_scale;Register Addresses: 65[11:6];
		OPT3101::deviceRegister en_multi_freq_phase; ///< en_multi_freq_phase;Register Addresses: 64[22:22];With this bit set along with enable_dealiased_measurement, the usual phase register will have both the frequency information. The frequency of the phase will be indicated in one of the status bit.
		OPT3101::deviceRegister temp_avg_main; ///< temp_avg_main;Register Addresses: 3[23:22];
		OPT3101::deviceRegister dis_ovl_for_hdr_meth1; ///< dis_ovl_for_hdr_meth1;Register Addresses: 184[21:21];
		OPT3101::deviceRegister en_ovl_for_hdr_meth2; ///< en_ovl_for_hdr_meth2;Register Addresses: 184[22:22];
		OPT3101::deviceRegister en_tx1_on_tx0; ///< en_tx1_on_tx0;Register Addresses: 185[10:10];
		OPT3101::deviceRegister en_tx2_on_tx0; ///< en_tx2_on_tx0;Register Addresses: 185[11:11];
		OPT3101::deviceRegister clip_mode_fc; ///< clip_mode_fc;Register Addresses: 80[0:0];chooses either rounding off or clipping or wrap around when applying freq-correction. Default is kept as rounding.
		OPT3101::deviceRegister clip_mode_nl; ///< clip_mode_nl;Register Addresses: 80[1:1];chooses either rounding off or clipping or wrap around when applying harmonic correction. Default is kept as rounding.
		OPT3101::deviceRegister clip_mode_temp; ///< clip_mode_temp;Register Addresses: 80[2:2];chooses either rounding off or clipping or wrap around when applying temp correction. Default is kept as rounding.
		OPT3101::deviceRegister clip_mode_offset; ///< clip_mode_offset;Register Addresses: 80[3:3];chooses either rounding off or clipping or wrap around when applying offset. Default is kept as rounding.
		OPT3101::deviceRegister disable_syncing; ///< disable_syncing;Register Addresses: 80[21:21];Normally calc clock and afe_clk are synchronized to avoid reset to reset variation in spur levels. This option is to disable the syncing of dividers (of calc_clk). The default is now changed to '1' because we don't use divided clock for calc-clk from PG3P0 by default. If syncing is used frequency loop will have issues.
		OPT3101::deviceRegister force_en_slave; ///< force_en_slave;Register Addresses: 0[22:22];Enable i2c slave for any address forcefully. That is whether auto_load completed or not.
		OPT3101::deviceRegister force_en_bypass; ///< force_en_bypass;Register Addresses: 0[21:21];This bit allows the slave to write directly to the efuse. This is gated with stop condition at the port level to avoid transition signals at scl/sda.
		OPT3101::deviceRegister override_clkgen_reg; ///< override_clkgen_reg;Register Addresses: 80[22:22];Setting this register '1' allows user to independenly control tm_clkgen(2:1) which controls dealias settings.
		OPT3101::deviceRegister software_reset; ///< software_reset;Register Addresses: 0[0:0];
		OPT3101::deviceRegister dis_tg_aconf; ///< dis_tg_aconf;Register Addresses: 128[23:23];Some of the tg registers are automatically configured such as pdn*_dyn_tg signal, capture_tg_channel etc. if these signals need to be configured by user this bit may be used as an override.
		OPT3101::deviceRegister capture_clk_cnt; ///< capture_clk_cnt;Register Addresses: 160[15:0];This is where early_fvd/svd starts. early_fvd only comes in the frame which is equal num_avg. This is the subframe in which computation results comes up. Programm this to 10600 if planning to use lower frequency.
		OPT3101::deviceRegister tg_en; ///< tg_en;Register Addresses: 128[0:0];gates the tg_clk with this bit.
		OPT3101::deviceRegister num_sub_frames; ///< num_sub_frames;Register Addresses: 159[11:0];The numbef of subframes in a frame. This number should be greater than or equal to num_avg.
		OPT3101::deviceRegister num_avg_sub_frames; ///< num_avg_sub_frames;Register Addresses: 159[23:12];The number of averages for the iq. Used in TG to generate early_fvd and some other TG signals.
		OPT3101::deviceRegister sub_vd_clk_cnt; ///< sub_vd_clk_cnt;Register Addresses: 128[16:1];the number of pixels in a subframe. In PG3P0 the default is changed to support 4ksps. The number is also made a multiple of 32+16.
		OPT3101::deviceRegister tg_illumen_start; ///< tg_illumen_start;Register Addresses: 143[15:0];spare2_tg. This is used for illum_en. Enabled throughout a subframe.
		OPT3101::deviceRegister tg_illumen_end; ///< tg_illumen_end;Register Addresses: 144[15:0];Ending after the 8192+ 250 samples apx.
		OPT3101::deviceRegister tg_illumen_mask_start; ///< tg_illumen_mask_start;Register Addresses: 156[11:0];spare2_mask. By default the mask is programmed till num_avg_iq only.
		OPT3101::deviceRegister tg_illumen_mask_end; ///< tg_illumen_mask_end;Register Addresses: 156[23:12];
		OPT3101::deviceRegister tg_afe_rst_start; ///< tg_afe_rst_start;Register Addresses: 131[15:0];demod_reset. Mask is programmed such that it comes every subframe.
		OPT3101::deviceRegister tg_afe_rst_end; ///< tg_afe_rst_end;Register Addresses: 132[15:0];
		OPT3101::deviceRegister tg_seq_int_start; ///< tg_seq_int_start;Register Addresses: 133[15:0];interrupt. Only happens in first subframe due to the mask programming
		OPT3101::deviceRegister tg_seq_int_end; ///< tg_seq_int_end;Register Addresses: 134[15:0];
		OPT3101::deviceRegister tg_capture_start; ///< tg_capture_start;Register Addresses: 135[15:0];capture_tg_channel. Internal TG signal. This signal need to be changed when you go to slower dealias mode. Program this to 11300 and 11800
		OPT3101::deviceRegister tg_capture_end; ///< tg_capture_end;Register Addresses: 136[15:0];
		OPT3101::deviceRegister tg_ovl_window_start; ///< tg_ovl_window_start;Register Addresses: 137[15:0];ovl_sample. During this time period only ovl is sampled. This exists for only for subframes till the num_avg.
		OPT3101::deviceRegister tg_ovl_window_end; ///< tg_ovl_window_end;Register Addresses: 138[15:0];
		OPT3101::deviceRegister tg_calc_start; ///< tg_calc_start;Register Addresses: 145[15:0];pdn_dyn_tg. This signal exists roughly from early_fvd start till end of the computation. The mask is programmed such that this only comes in the num_avg sub-frame. Programmed such that it will work even if the frequency changes in both direction. 
		OPT3101::deviceRegister tg_calc_end; ///< tg_calc_end;Register Addresses: 146[15:0];
		OPT3101::deviceRegister tg_dynpdn_start; ///< tg_dynpdn_start;Register Addresses: 147[15:0];pdn_dyn1_tg. Used to power down less power intensive digital blocks and analog if tm_frame_vd_sub_cnt greater than num_avg_iq.
		OPT3101::deviceRegister tg_dynpdn_end; ///< tg_dynpdn_end;Register Addresses: 148[15:0];
		OPT3101::deviceRegister tg_seq_int_mask_start; ///< tg_seq_int_mask_start;Register Addresses: 151[11:0];interrupt. Comes only in first (num:0) subframe.
		OPT3101::deviceRegister tg_seq_int_mask_end; ///< tg_seq_int_mask_end;Register Addresses: 151[23:12];
		OPT3101::deviceRegister tg_capture_mask_start; ///< tg_capture_mask_start;Register Addresses: 152[11:0];capture_tg_channel. By default comes only in the num_avg subchannel. This mask is configurable by user only if dis_tg_aconf = '1'.
		OPT3101::deviceRegister tg_capture_mask_end; ///< tg_capture_mask_end;Register Addresses: 152[23:12];
		OPT3101::deviceRegister tg_ovl_window_mask_start; ///< tg_ovl_window_mask_start;Register Addresses: 153[11:0];ovl_sample. This exits till num_avg subframe.
		OPT3101::deviceRegister tg_ovl_window_mask_end; ///< tg_ovl_window_mask_end;Register Addresses: 153[23:12];
		OPT3101::deviceRegister tg_calc_mask_start; ///< tg_calc_mask_start;Register Addresses: 157[11:0];Mask for pdn_dyn_tg. Only enabled during num_avg subframe.
		OPT3101::deviceRegister tg_calc_mask_end; ///< tg_calc_mask_end;Register Addresses: 157[23:12];
		OPT3101::deviceRegister tg_dynpdn_mask_start; ///< tg_dynpdn_mask_start;Register Addresses: 158[11:0];Mask for pdn_dyn1_tg. Used to power down less power intensive digital blocks and analog if tm_frame_vd_sub_cnt greater than num_avg_iq. Enabled till num_avg subframe.
		OPT3101::deviceRegister tg_dynpdn_mask_end; ///< tg_dynpdn_mask_end;Register Addresses: 158[23:12];
		OPT3101::deviceRegister en_sequencer; ///< en_sequencer;Register Addresses: 20[16:16];clock gates the logic for sequencer normally. This bit is used to enable sequencer.
		OPT3101::deviceRegister en_processor_values; ///< en_processor_values;Register Addresses: 20[17:17];Uses processor values instead of register values.
		OPT3101::deviceRegister status_in_reg; ///< status_in_reg;Register Addresses: 20[18:18];the register is used to control the program flow in CPU
		OPT3101::deviceRegister mux_sel_compin; ///< mux_sel_compin;Register Addresses: 19[2:0];choses the value used for comp_a register in cpu.;Following are the choices.;phase_out_fsm;dealiased_kb_fsm;dealiased_distance;confidence
		OPT3101::deviceRegister compare_reg1; ///< compare_reg1;Register Addresses: 19[18:3];
		OPT3101::deviceRegister compare_reg2; ///< compare_reg2;Register Addresses: 20[15:0];
		OPT3101::deviceRegister dis_interrupt; ///< dis_interrupt;Register Addresses: 20[19:19];Disables the interrupt which triggers processor. Does not clock gate processor though.
		OPT3101::deviceRegister command0; ///< command0;Register Addresses: 21[11:0];NOP for 99 cycles
		OPT3101::deviceRegister command1; ///< command1;Register Addresses: 21[23:12];enable intrinsic-xtalk
		OPT3101::deviceRegister command2; ///< command2;Register Addresses: 22[11:0];disable intrinsic xtalk
		OPT3101::deviceRegister command3; ///< command3;Register Addresses: 22[23:12];Direct go to the first line
		OPT3101::deviceRegister command4; ///< command4;Register Addresses: 23[11:0];
		OPT3101::deviceRegister command5; ///< command5;Register Addresses: 23[23:12];
		OPT3101::deviceRegister command6; ///< command6;Register Addresses: 24[11:0];
		OPT3101::deviceRegister command7; ///< command7;Register Addresses: 24[23:12];
		OPT3101::deviceRegister command8; ///< command8;Register Addresses: 25[11:0];
		OPT3101::deviceRegister command9; ///< command9;Register Addresses: 25[23:12];
		OPT3101::deviceRegister command10; ///< command10;Register Addresses: 26[11:0];
		OPT3101::deviceRegister command11; ///< command11;Register Addresses: 26[23:12];
		OPT3101::deviceRegister command12; ///< command12;Register Addresses: 27[11:0];
		OPT3101::deviceRegister command13; ///< command13;Register Addresses: 27[23:12];
		OPT3101::deviceRegister command14; ///< command14;Register Addresses: 28[11:0];
		OPT3101::deviceRegister command15; ///< command15;Register Addresses: 28[23:12];
		OPT3101::deviceRegister command16; ///< command16;Register Addresses: 29[11:0];
		OPT3101::deviceRegister command17; ///< command17;Register Addresses: 29[23:12];
		OPT3101::deviceRegister command18; ///< command18;Register Addresses: 30[11:0];
		OPT3101::deviceRegister command19; ///< command19;Register Addresses: 30[23:12];
		OPT3101::deviceRegister force_scale_val; ///< force_scale_val;Register Addresses: 46[2:0];Uses this scale value if disable_auto_scale is programmed. This scale value is also used during any xtalk calibration even if disable_auto_scale is not applied. Default is '0', which means 24bit demod is taken as it is giving maximum accuracy.
		OPT3101::deviceRegister dis_auto_scale; ///< dis_auto_scale;Register Addresses: 46[3:3];
		OPT3101::deviceRegister disable_conf_rescale; ///< disable_conf_rescale;Register Addresses: 46[13:13];This a mostly a debug register.. When this is set auto_scaled confidence doesn't rescale back. Even when force_scale_val is there, it doesn't rescale. This bit may be set along with force_scale_val to see the effect of confidence scaling.
		OPT3101::deviceRegister int_xtalk_calib; ///< int_xtalk_calib;Register Addresses: 46[4:4];Puts the device into intrinsic calibration mode.
		OPT3101::deviceRegister xtalk_filt_time_const; ///< xtalk_filt_time_const;Register Addresses: 46[23:20];Time constant during crosstalk filtering. Higher the time constant slower the filtering is.
		OPT3101::deviceRegister use_xtalk_filt_int; ///< use_xtalk_filt_int;Register Addresses: 46[5:5];Whehter to use filter or direct sampling for intrinsic crosstalk.
		OPT3101::deviceRegister use_xtalk_reg_int; ///< use_xtalk_reg_int;Register Addresses: 46[6:6];Whether to use register or filter/sample for intrinsic.
		OPT3101::deviceRegister iq_read_data_sel; ///< iq_read_data_sel;Register Addresses: 46[11:9];mux used to chose which of the xtalk register is being read out.;;010 -- raw_i/q;000 -- intrinsic_xtalk;001 -- optical_xtalk
		OPT3101::deviceRegister iphase_xtalk; ///< iphase_xtalk;Register Addresses: 59[23:0];
		OPT3101::deviceRegister qphase_xtalk; ///< qphase_xtalk;Register Addresses: 60[23:0];
		OPT3101::deviceRegister int_xtalk_reg_scale; ///< int_xtalk_reg_scale;Register Addresses: 46[16:14];allows scaling of the meaning of ixtalk register. 0- 2^0, 2^1, 2^2, 2^3 etc.
		OPT3101::deviceRegister iphase_xtalk_int_reg; ///< iphase_xtalk_int_reg;Register Addresses: 61[15:0];inphase  component for intrinsic xtalk
		OPT3101::deviceRegister qphase_xtalk_int_reg; ///< qphase_xtalk_int_reg;Register Addresses: 62[15:0];quadrature  component for intrinsic xtalk
		OPT3101::deviceRegister illum_xtalk_calib; ///< illum_xtalk_calib;Register Addresses: 46[12:12];puts the device into optical calibration mode.
		OPT3101::deviceRegister illum_xtalk_reg_scale; ///< illum_xtalk_reg_scale;Register Addresses: 46[19:17];allows scaling of the meaning of oxtalk register. 0- 2^0, 2^2, 2^4, 2^8.
		OPT3101::deviceRegister use_xtalk_filt_illum; ///< use_xtalk_filt_illum;Register Addresses: 46[7:7];
		OPT3101::deviceRegister use_xtalk_reg_illum; ///< use_xtalk_reg_illum;Register Addresses: 46[8:8];For optical default is to use the register values.
		OPT3101::deviceRegister iphase_xtalk_reg_hdr0_tx0; ///< iphase_xtalk_reg_hdr0_tx0;Register Addresses: 47[15:0];inphase component of the xtalk for hdr0/led0
		OPT3101::deviceRegister qphase_xtalk_reg_hdr0_tx0; ///< qphase_xtalk_reg_hdr0_tx0;Register Addresses: 48[15:0];quadrature component of the xtalk for hdr0/led0
		OPT3101::deviceRegister iphase_xtalk_reg_hdr1_tx0; ///< iphase_xtalk_reg_hdr1_tx0;Register Addresses: 49[15:0];
		OPT3101::deviceRegister qphase_xtalk_reg_hdr1_tx0; ///< qphase_xtalk_reg_hdr1_tx0;Register Addresses: 50[15:0];
		OPT3101::deviceRegister iphase_xtalk_reg_hdr0_tx1; ///< iphase_xtalk_reg_hdr0_tx1;Register Addresses: 51[15:0];
		OPT3101::deviceRegister qphase_xtalk_reg_hdr0_tx1; ///< qphase_xtalk_reg_hdr0_tx1;Register Addresses: 52[15:0];
		OPT3101::deviceRegister iphase_xtalk_reg_hdr1_tx1; ///< iphase_xtalk_reg_hdr1_tx1;Register Addresses: 53[15:0];
		OPT3101::deviceRegister qphase_xtalk_reg_hdr1_tx1; ///< qphase_xtalk_reg_hdr1_tx1;Register Addresses: 54[15:0];
		OPT3101::deviceRegister iphase_xtalk_reg_hdr0_tx2; ///< iphase_xtalk_reg_hdr0_tx2;Register Addresses: 55[15:0];
		OPT3101::deviceRegister qphase_xtalk_reg_hdr0_tx2; ///< qphase_xtalk_reg_hdr0_tx2;Register Addresses: 56[15:0];
		OPT3101::deviceRegister iphase_xtalk_reg_hdr1_tx2; ///< iphase_xtalk_reg_hdr1_tx2;Register Addresses: 57[15:0];
		OPT3101::deviceRegister qphase_xtalk_reg_hdr1_tx2; ///< qphase_xtalk_reg_hdr1_tx2;Register Addresses: 58[15:0];
		OPT3101::deviceRegister en_temp_xtalk_corr; ///< en_temp_xtalk_corr;Register Addresses: 58[16:16];
		OPT3101::deviceRegister scale_temp_coeff_xtalk; ///< scale_temp_coeff_xtalk;Register Addresses: 58[19:17];Allows programmability on the temp_coefficients range and precision.
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr0_tx0; ///< temp_coeff_xtalk_iphase_hdr0_tx0;Register Addresses: 56[23:16];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr0_tx0; ///< temp_coeff_xtalk_qphase_hdr0_tx0;Register Addresses: 57[23:16];
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr1_tx0; ///< temp_coeff_xtalk_iphase_hdr1_tx0;Register Addresses: 94[15:8];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr1_tx0; ///< temp_coeff_xtalk_qphase_hdr1_tx0;Register Addresses: 96[7:0];
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr0_tx1; ///< temp_coeff_xtalk_iphase_hdr0_tx1;Register Addresses: 94[23:16];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr0_tx1; ///< temp_coeff_xtalk_qphase_hdr0_tx1;Register Addresses: 96[15:8];
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr1_tx1; ///< temp_coeff_xtalk_iphase_hdr1_tx1;Register Addresses: 95[7:0];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr1_tx1; ///< temp_coeff_xtalk_qphase_hdr1_tx1;Register Addresses: 96[23:16];
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr0_tx2; ///< temp_coeff_xtalk_iphase_hdr0_tx2;Register Addresses: 95[15:8];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr0_tx2; ///< temp_coeff_xtalk_qphase_hdr0_tx2;Register Addresses: 97[7:0];
		OPT3101::deviceRegister temp_coeff_xtalk_iphase_hdr1_tx2; ///< temp_coeff_xtalk_iphase_hdr1_tx2;Register Addresses: 95[23:16];
		OPT3101::deviceRegister temp_coeff_xtalk_qphase_hdr1_tx2; ///< temp_coeff_xtalk_qphase_hdr1_tx2;Register Addresses: 97[15:8];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr0_tx0; ///< temp_coeff_illum_xtalk_iphase_hdr0_tx0;Register Addresses: 54[23:16];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr0_tx0; ///< temp_coeff_illum_xtalk_qphase_hdr0_tx0;Register Addresses: 55[23:16];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr1_tx0; ///< temp_coeff_illum_xtalk_iphase_hdr1_tx0;Register Addresses: 91[7:0];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr0_tx1; ///< temp_coeff_illum_xtalk_iphase_hdr0_tx1;Register Addresses: 91[15:8];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr1_tx1; ///< temp_coeff_illum_xtalk_iphase_hdr1_tx1;Register Addresses: 91[23:16];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr0_tx2; ///< temp_coeff_illum_xtalk_iphase_hdr0_tx2;Register Addresses: 92[7:0];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_iphase_hdr1_tx2; ///< temp_coeff_illum_xtalk_iphase_hdr1_tx2;Register Addresses: 92[15:8];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr1_tx0; ///< temp_coeff_illum_xtalk_qphase_hdr1_tx0;Register Addresses: 92[23:16];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr0_tx1; ///< temp_coeff_illum_xtalk_qphase_hdr0_tx1;Register Addresses: 93[7:0];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr1_tx1; ///< temp_coeff_illum_xtalk_qphase_hdr1_tx1;Register Addresses: 93[15:8];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr0_tx2; ///< temp_coeff_illum_xtalk_qphase_hdr0_tx2;Register Addresses: 93[23:16];
		OPT3101::deviceRegister temp_coeff_illum_xtalk_qphase_hdr1_tx2; ///< temp_coeff_illum_xtalk_qphase_hdr1_tx2;Register Addresses: 94[7:0];
		OPT3101::deviceRegister amb_xtalk_qphase_coeff; ///< amb_xtalk_qphase_coeff;Register Addresses: 12[15:8];reflects the variation of quad component.
		OPT3101::deviceRegister amb_xtalk_iphase_coeff; ///< amb_xtalk_iphase_coeff;Register Addresses: 12[7:0];reflect the variation of optical crosstalk inphase component.
		OPT3101::deviceRegister scale_amb_coeff_xtalk; ///< scale_amb_coeff_xtalk;Register Addresses: 58[22:20];Ambient xtalk mode. Provides range/precision for ambient correction.
		OPT3101::deviceRegister en_phase_corr; ///< en_phase_corr;Register Addresses: 67[0:0];enables phase correction from the values programmed.
		OPT3101::deviceRegister phase_offset_hdr0_tx0; ///< phase_offset_hdr0_tx0;Register Addresses: 66[15:0];phase_offset for freq1
		OPT3101::deviceRegister phase_offset_hdr1_tx0; ///< phase_offset_hdr1_tx0;Register Addresses: 81[15:0];
		OPT3101::deviceRegister phase_offset_hdr0_tx1; ///< phase_offset_hdr0_tx1;Register Addresses: 82[15:0];
		OPT3101::deviceRegister phase_offset_hdr1_tx1; ///< phase_offset_hdr1_tx1;Register Addresses: 83[15:0];
		OPT3101::deviceRegister phase_offset_hdr0_tx2; ///< phase_offset_hdr0_tx2;Register Addresses: 84[15:0];
		OPT3101::deviceRegister phase_offset_hdr1_tx2; ///< phase_offset_hdr1_tx2;Register Addresses: 85[15:0];
		OPT3101::deviceRegister reverse_phase_before_offset; ///< reverse_phase_before_offset;Register Addresses: 67[9:9];
		OPT3101::deviceRegister phase2_offset_hdr0_tx0; ///< phase2_offset_hdr0_tx0;Register Addresses: 68[15:0];phase offset for freq2
		OPT3101::deviceRegister phase2_offset_hdr1_tx0; ///< phase2_offset_hdr1_tx0;Register Addresses: 86[15:0];
		OPT3101::deviceRegister phase2_offset_hdr0_tx1; ///< phase2_offset_hdr0_tx1;Register Addresses: 87[15:0];
		OPT3101::deviceRegister phase2_offset_hdr1_tx1; ///< phase2_offset_hdr1_tx1;Register Addresses: 88[15:0];
		OPT3101::deviceRegister phase2_offset_hdr0_tx2; ///< phase2_offset_hdr0_tx2;Register Addresses: 89[15:0];
		OPT3101::deviceRegister phase2_offset_hdr1_tx2; ///< phase2_offset_hdr1_tx2;Register Addresses: 90[15:0];
		OPT3101::deviceRegister en_temp_corr; ///< en_temp_corr;Register Addresses: 67[1:1];enables temperature correction for phase
		OPT3101::deviceRegister scale_phase_temp_coeff; ///< scale_phase_temp_coeff;Register Addresses: 67[8:6];changes the meaning of coefficients related to phase correction.
		OPT3101::deviceRegister tmain_calib_hdr0_tx0; ///< tmain_calib_hdr0_tx0;Register Addresses: 71[11:0];calibrated temperature for main temp sensor.Default is 2048 because internally it is treated as offset binary
		OPT3101::deviceRegister temp_coeff_main_hdr0_tx0; ///< temp_coeff_main_hdr0_tx0;Register Addresses: 69[11:0];temperature coefficient for phase correction for main temp. By default means phase change for 64 degrees of temperature.
		OPT3101::deviceRegister tmain_calib_hdr1_tx0; ///< tmain_calib_hdr1_tx0;Register Addresses: 72[11:0];
		OPT3101::deviceRegister temp_coeff_main_hdr1_tx0; ///< temp_coeff_main_hdr1_tx0;Register Addresses: 45[11:0];
		OPT3101::deviceRegister tmain_calib_hdr0_tx1; ///< tmain_calib_hdr0_tx1;Register Addresses: 73[11:0];
		OPT3101::deviceRegister temp_coeff_main_hdr0_tx1; ///< temp_coeff_main_hdr0_tx1;Register Addresses: 45[23:12];
		OPT3101::deviceRegister tmain_calib_hdr1_tx1; ///< tmain_calib_hdr1_tx1;Register Addresses: 65[23:12];
		OPT3101::deviceRegister temp_coeff_main_hdr1_tx1; ///< temp_coeff_main_hdr1_tx1;Register Addresses: 47[23:16], 48[23:20];
		OPT3101::deviceRegister tmain_calib_hdr0_tx2; ///< tmain_calib_hdr0_tx2;Register Addresses: 63[11:0];
		OPT3101::deviceRegister temp_coeff_main_hdr0_tx2; ///< temp_coeff_main_hdr0_tx2;Register Addresses: 49[23:16], 50[23:20];
		OPT3101::deviceRegister tmain_calib_hdr1_tx2; ///< tmain_calib_hdr1_tx2;Register Addresses: 69[23:12];
		OPT3101::deviceRegister temp_coeff_main_hdr1_tx2; ///< temp_coeff_main_hdr1_tx2;Register Addresses: 51[23:16], 52[23:20];
		OPT3101::deviceRegister tillum_calib_hdr0_tx0; ///< tillum_calib_hdr0_tx0;Register Addresses: 71[23:12];calibrated temp for tillum. Default is 2048 because internally it is treated as offset binary
		OPT3101::deviceRegister temp_coeff_illum_hdr0_tx0; ///< temp_coeff_illum_hdr0_tx0;Register Addresses: 70[11:0];temperature coefficient for phase correction for illum temp. By default means phase change for 64 degrees of temperature.
		OPT3101::deviceRegister tillum_calib_hdr1_tx0; ///< tillum_calib_hdr1_tx0;Register Addresses: 72[23:12];
		OPT3101::deviceRegister temp_coeff_illum_hdr1_tx0; ///< temp_coeff_illum_hdr1_tx0;Register Addresses: 81[23:16], 82[23:20];
		OPT3101::deviceRegister tillum_calib_hdr0_tx1; ///< tillum_calib_hdr0_tx1;Register Addresses: 73[23:12];
		OPT3101::deviceRegister temp_coeff_illum_hdr0_tx1; ///< temp_coeff_illum_hdr0_tx1;Register Addresses: 83[23:16], 84[23:20];
		OPT3101::deviceRegister tillum_calib_hdr1_tx1; ///< tillum_calib_hdr1_tx1;Register Addresses: 67[23:12];
		OPT3101::deviceRegister temp_coeff_illum_hdr1_tx1; ///< temp_coeff_illum_hdr1_tx1;Register Addresses: 85[23:16], 86[23:20];
		OPT3101::deviceRegister tillum_calib_hdr0_tx2; ///< tillum_calib_hdr0_tx2;Register Addresses: 63[23:12];
		OPT3101::deviceRegister temp_coeff_illum_hdr0_tx2; ///< temp_coeff_illum_hdr0_tx2;Register Addresses: 87[23:16], 88[23:20];
		OPT3101::deviceRegister tillum_calib_hdr1_tx2; ///< tillum_calib_hdr1_tx2;Register Addresses: 70[23:12];
		OPT3101::deviceRegister temp_coeff_illum_hdr1_tx2; ///< temp_coeff_illum_hdr1_tx2;Register Addresses: 89[23:16], 90[23:20];
		OPT3101::deviceRegister amb_sat_thr; ///< amb_sat_thr;Register Addresses: 13[16:7];the threshold which is used to detect the ambient overload. Default is kept at highest.;Threshold set for 1v on the ambient dac resistor. 512x3/8xIR. 
		OPT3101::deviceRegister amb_calib; ///< amb_calib;Register Addresses: 11[23:14];the ambient at which device is calibrated for optical xtalk/phase offset etc.
		OPT3101::deviceRegister amb_phase_corr_pwl_coeff0; ///< amb_phase_corr_pwl_coeff0;Register Addresses: 12[23:16];Captures the drift in the phase wrto ambient. Can be a negative number to reflect inverse relationship. The number is divided by 2^5 to get the actual value.
		OPT3101::deviceRegister amb_phase_corr_pwl_x0; ///< amb_phase_corr_pwl_x0;Register Addresses: 184[9:0];
		OPT3101::deviceRegister amb_phase_corr_pwl_x1; ///< amb_phase_corr_pwl_x1;Register Addresses: 184[19:10];
		OPT3101::deviceRegister amb_phase_corr_pwl_x2; ///< amb_phase_corr_pwl_x2;Register Addresses: 185[9:0];
		OPT3101::deviceRegister amb_phase_corr_pwl_coeff1; ///< amb_phase_corr_pwl_coeff1;Register Addresses: 180[7:0];The first coefficient is assumed to be the original one 
		OPT3101::deviceRegister amb_phase_corr_pwl_coeff2; ///< amb_phase_corr_pwl_coeff2;Register Addresses: 180[15:8];
		OPT3101::deviceRegister amb_phase_corr_pwl_coeff3; ///< amb_phase_corr_pwl_coeff3;Register Addresses: 180[23:16];
		OPT3101::deviceRegister scale_amb_phase_corr_coeff; ///< scale_amb_phase_corr_coeff;Register Addresses: 181[2:0];Scales the gain/vs accuracy.
		OPT3101::deviceRegister temp_coeff_illum_square_hdr0; ///< temp_coeff_illum_square_hdr0;Register Addresses: 182[7:0];
		OPT3101::deviceRegister temp_coeff_illum_square_hdr1; ///< temp_coeff_illum_square_hdr1;Register Addresses: 182[15:8];
		OPT3101::deviceRegister temp_coeff_main_square_hdr0; ///< temp_coeff_main_square_hdr0;Register Addresses: 183[7:0];
		OPT3101::deviceRegister temp_coeff_main_square_hdr1; ///< temp_coeff_main_square_hdr1;Register Addresses: 183[15:8];
		OPT3101::deviceRegister scale_phase_temp_corr_square; ///< scale_phase_temp_corr_square;Register Addresses: 181[5:3];scales square correction range/accuracy.
		OPT3101::deviceRegister en_nl_corr; ///< en_nl_corr;Register Addresses: 74[0:0];enables harmonic/nonlinear correction for phase.
		OPT3101::deviceRegister a1_coeff_hdr0_tx0; ///< a1_coeff_hdr0_tx0;Register Addresses: 75[15:0];
		OPT3101::deviceRegister a2_coeff_hdr0_tx0; ///< a2_coeff_hdr0_tx0;Register Addresses: 76[15:0];
		OPT3101::deviceRegister a3_coeff_hdr0_tx0; ///< a3_coeff_hdr0_tx0;Register Addresses: 77[15:0];
		OPT3101::deviceRegister a4_coeff_hdr0_tx0; ///< a4_coeff_hdr0_tx0;Register Addresses: 78[15:0];
		OPT3101::deviceRegister scale_nl_corr_coeff; ///< scale_nl_corr_coeff;Register Addresses: 74[19:18];changes the meaning of the nonlinear coefficients.;
		OPT3101::deviceRegister a0_coeff_hdr0_tx0; ///< a0_coeff_hdr0_tx0;Register Addresses: 74[17:2];
		OPT3101::deviceRegister a0_coeff_hdr1_tx0; ///< a0_coeff_hdr1_tx0;Register Addresses: 162[15:0];
		OPT3101::deviceRegister a0_coeff_hdr0_tx1; ///< a0_coeff_hdr0_tx1;Register Addresses: 163[15:0];
		OPT3101::deviceRegister a0_coeff_hdr1_tx1; ///< a0_coeff_hdr1_tx1;Register Addresses: 164[15:0];
		OPT3101::deviceRegister a0_coeff_hdr0_tx2; ///< a0_coeff_hdr0_tx2;Register Addresses: 165[15:0];
		OPT3101::deviceRegister a0_coeff_hdr1_tx2; ///< a0_coeff_hdr1_tx2;Register Addresses: 166[15:0];
		OPT3101::deviceRegister a1_coeff_hdr1_tx0; ///< a1_coeff_hdr1_tx0;Register Addresses: 167[15:0];
		OPT3101::deviceRegister a1_coeff_hdr0_tx1; ///< a1_coeff_hdr0_tx1;Register Addresses: 168[15:0];
		OPT3101::deviceRegister a1_coeff_hdr1_tx1; ///< a1_coeff_hdr1_tx1;Register Addresses: 169[15:0];
		OPT3101::deviceRegister a1_coeff_hdr0_tx2; ///< a1_coeff_hdr0_tx2;Register Addresses: 170[15:0];
		OPT3101::deviceRegister a1_coeff_hdr1_tx2; ///< a1_coeff_hdr1_tx2;Register Addresses: 171[15:0];
		OPT3101::deviceRegister a2_coeff_hdr1_tx0; ///< a2_coeff_hdr1_tx0;Register Addresses: 172[15:0];
		OPT3101::deviceRegister a2_coeff_hdr0_tx1; ///< a2_coeff_hdr0_tx1;Register Addresses: 173[15:0];
		OPT3101::deviceRegister a2_coeff_hdr1_tx1; ///< a2_coeff_hdr1_tx1;Register Addresses: 174[15:0];
		OPT3101::deviceRegister a2_coeff_hdr0_tx2; ///< a2_coeff_hdr0_tx2;Register Addresses: 175[15:0];
		OPT3101::deviceRegister a2_coeff_hdr1_tx2; ///< a2_coeff_hdr1_tx2;Register Addresses: 176[15:0];
		OPT3101::deviceRegister a3_coeff_hdr1_tx0; ///< a3_coeff_hdr1_tx0;Register Addresses: 177[15:0];
		OPT3101::deviceRegister a3_coeff_hdr0_tx1; ///< a3_coeff_hdr0_tx1;Register Addresses: 162[23:16], 163[23:16];
		OPT3101::deviceRegister a3_coeff_hdr1_tx1; ///< a3_coeff_hdr1_tx1;Register Addresses: 164[23:16], 165[23:16];
		OPT3101::deviceRegister a3_coeff_hdr0_tx2; ///< a3_coeff_hdr0_tx2;Register Addresses: 166[23:16], 167[23:16];
		OPT3101::deviceRegister a3_coeff_hdr1_tx2; ///< a3_coeff_hdr1_tx2;Register Addresses: 168[23:16], 169[23:16];
		OPT3101::deviceRegister a4_coeff_hdr1_tx0; ///< a4_coeff_hdr1_tx0;Register Addresses: 170[23:16], 171[23:16];
		OPT3101::deviceRegister a4_coeff_hdr0_tx1; ///< a4_coeff_hdr0_tx1;Register Addresses: 172[23:16], 173[23:16];
		OPT3101::deviceRegister a4_coeff_hdr1_tx1; ///< a4_coeff_hdr1_tx1;Register Addresses: 174[23:16], 175[23:16];
		OPT3101::deviceRegister a4_coeff_hdr0_tx2; ///< a4_coeff_hdr0_tx2;Register Addresses: 176[23:16], 177[23:16];
		OPT3101::deviceRegister a4_coeff_hdr1_tx2; ///< a4_coeff_hdr1_tx2;Register Addresses: 178[15:0];
		OPT3101::deviceRegister tillum; ///< tillum;Register Addresses: 4[19:8];The value of illum-temperature register.
		OPT3101::deviceRegister tsens_slave0; ///< tsens_slave0;Register Addresses: 2[6:0];slave address of the led0 tsensor
		OPT3101::deviceRegister tsens_slave1; ///< tsens_slave1;Register Addresses: 2[13:7];slave address of the led1 tsensor
		OPT3101::deviceRegister tsens_slave2; ///< tsens_slave2;Register Addresses: 2[20:14];slave address of the led2 tsensor
		OPT3101::deviceRegister config_tillum_msb; ///< config_tillum_msb;Register Addresses: 7[23:20];
		OPT3101::deviceRegister en_tillum_12b; ///< en_tillum_12b;Register Addresses: 13[23:23];While interfacing with the TMP02 type tempsensor, this bit needs to be set to swap bytes to allow different read format.
		OPT3101::deviceRegister tillum_unsigned; ///< tillum_unsigned;Register Addresses: 4[23:23];This bit is set '1' when temperature given by tmain/tillum sensor is in unsigned format.
		OPT3101::deviceRegister temp_avg_illum; ///< temp_avg_illum;Register Addresses: 2[23:22];Based on this, temperature is averaged to remove quantization errors on temperature.
		OPT3101::deviceRegister en_tsens_read_fvd; ///< en_tsens_read_fvd;Register Addresses: 3[18:18];If this bit is set tmain temperature is read every frame. Other wise it is read based on a register trigger.
		OPT3101::deviceRegister en_tillum_read; ///< en_tillum_read;Register Addresses: 2[21:21];Enable i2c read of appropriate illum led. If this bit is not set illumination driver temperature is not read.
		OPT3101::deviceRegister eeprom_read_trig; ///< eeprom_read_trig;Register Addresses: 1[0:0];Used to read efuse values into the chain using register trigger
		OPT3101::deviceRegister swap_read_data; ///< swap_read_data;Register Addresses: 1[1:1];swaps/reverse the data read by i2c-host.
		OPT3101::deviceRegister eeprom_start_reg_addr; ///< eeprom_start_reg_addr;Register Addresses: 1[16:9];The first byte written while reading from the efuse. This will typically be 0. At startup there is no way to program this.
		OPT3101::deviceRegister frame_vd_trig; ///< frame_vd_trig;Register Addresses: 1[17:17];when this bit is '1' i2c host is triggered every frame vd. Else it is triggered based on the i2c_trig_reg.
		OPT3101::deviceRegister i2c_trig_reg; ///< i2c_trig_reg;Register Addresses: 1[18:18];The trigger register for i2c transaction.
		OPT3101::deviceRegister i2c_en; ///< i2c_en;Register Addresses: 1[19:19];Enables the i2c host operation. Does not control the init load.
		OPT3101::deviceRegister i2c_rw; ///< i2c_rw;Register Addresses: 1[21:20];Choses the r/w for i2c host operation.;By default it reads temperature sensor.
		OPT3101::deviceRegister i2c_read_data; ///< i2c_read_data;Register Addresses: 3[7:0];The hosts read data.
		OPT3101::deviceRegister i2c_write_data1; ///< i2c_write_data1;Register Addresses: 3[16:9];The address where the read would start. Normally in temperature sensor read this is not required to be programmed.
		OPT3101::deviceRegister i2c_num_tran; ///< i2c_num_tran;Register Addresses: 3[17:17];The number of transactions. Either 1 or 2.
		OPT3101::deviceRegister en_eeprom_read; ///< en_eeprom_read;Register Addresses: 1[23:23];Disables the gating of auto_load clock after init_load_done. Should be used if register triggering has to work.
		OPT3101::deviceRegister init_load_done; ///< init_load_done;Register Addresses: 3[8:8];Can be used to check whether initial auto_load is successful or not.
		OPT3101::deviceRegister addr_slave_eeprom; ///< addr_slave_eeprom;Register Addresses: 1[8:2];
		OPT3101::deviceRegister i2c_num_bytes_tran1; ///< i2c_num_bytes_tran1;Register Addresses: 7[17:16];Number of bytes used in the tran2 of i2c transaction.
		OPT3101::deviceRegister i2c_num_bytes_tran2; ///< i2c_num_bytes_tran2;Register Addresses: 5[23:22];Number of bytes used in the tran2 of i2c transaction.
		OPT3101::deviceRegister i2c_write_data2; ///< i2c_write_data2;Register Addresses: 7[7:0];
		OPT3101::deviceRegister i2c_sel_read_bytes; ///< i2c_sel_read_bytes;Register Addresses: 7[19:18];choses which byte of i2c_read register to be read.
		OPT3101::deviceRegister i2c_cont_rw; ///< i2c_cont_rw;Register Addresses: 0[6:6];
		OPT3101::deviceRegister dis_ovldet; ///< dis_ovldet;Register Addresses: 101[23:23];to disable overload detection
		OPT3101::deviceRegister prog_ovldet_refp; ///< prog_ovldet_refp;Register Addresses: 100[20:18];To program OVL_DET REFP
		OPT3101::deviceRegister prog_ovldet_refm; ///< prog_ovldet_refm;Register Addresses: 100[23:21];To program OVL_DET REFM
		OPT3101::deviceRegister iamb_max_sel; ///< iamb_max_sel;Register Addresses: 114[7:4];selects the value of DAC resistor
		OPT3101::deviceRegister tm_vrefp_diode; ///< tm_vrefp_diode;Register Addresses: 109[2:0];To program the bias voltage INP
		OPT3101::deviceRegister tm_vrefm_diode; ///< tm_vrefm_diode;Register Addresses: 109[5:3];To program the bias voltage INM
		OPT3101::deviceRegister gpo1_mux_sel; ///< gpo1_mux_sel;Register Addresses: 120[8:6];
		OPT3101::deviceRegister gpio1_obuf_en; ///< gpio1_obuf_en;Register Addresses: 120[12:12];
		OPT3101::deviceRegister gpio1_ibuf_en; ///< gpio1_ibuf_en;Register Addresses: 120[13:13];
		OPT3101::deviceRegister gpo2_mux_sel; ///< gpo2_mux_sel;Register Addresses: 120[11:9];
		OPT3101::deviceRegister gpio2_obuf_en; ///< gpio2_obuf_en;Register Addresses: 120[15:15];
		OPT3101::deviceRegister gpio2_ibuf_en; ///< gpio2_ibuf_en;Register Addresses: 120[16:16];
		OPT3101::deviceRegister gpo3_mux_sel; ///< gpo3_mux_sel;Register Addresses: 120[2:0];
		OPT3101::deviceRegister sel_gp3_on_sdam; ///< sel_gp3_on_sdam;Register Addresses: 120[22:22];
		OPT3101::deviceRegister dealias_en; ///< dealias_en;Register Addresses: 113[1:1];To enable Dealias mode to get different Modulation freq close to 40MHz, ;34.3 MHz & 48 MHz.
		OPT3101::deviceRegister dealias_freq; ///< dealias_freq;Register Addresses: 113[2:2];Changes o/p freq in dealias mode;- only when dealais mode is enabled (TM_CLKGENless than2greater than is set to '1').
		OPT3101::deviceRegister shift_illum_phase; ///< shift_illum_phase;Register Addresses: 113[6:3];Shift bits to get different LED_CLK phases in calib mode.;;80M mode:16phases, 22.5 deg separation(360/16)-6.25n separation;;40M mode:16phases, 45 deg separation(360/8)-12.5n separation;;Basically 40M mode MSB bit is unused.
		OPT3101::deviceRegister shut_clocks; ///< shut_clocks;Register Addresses: 113[8:8];to shut down all 20M, 10M clock switching
		OPT3101::deviceRegister invert_tg_clk; ///< invert_tg_clk;Register Addresses: 113[9:9];to invert tg_clk for timing requirements
		OPT3101::deviceRegister invert_afe_clk; ///< invert_afe_clk;Register Addresses: 113[11:11];
		OPT3101::deviceRegister dis_illum_clk_tx; ///< dis_illum_clk_tx;Register Addresses: 113[12:12];Disable LED clk going to transmitter
		OPT3101::deviceRegister en_illum_clk_gpio; ///< en_illum_clk_gpio;Register Addresses: 113[16:16];Disable LED CLK going to GPIO
		OPT3101::deviceRegister illum_clk_gpio_mode; ///< illum_clk_gpio_mode;Register Addresses: 113[15:15];When this bit is '1', illum_en gating led_clk going to GPIO is masked.;
		OPT3101::deviceRegister unmask_illumen_intxtalk; ///< unmask_illumen_intxtalk;Register Addresses: 113[17:17];Mask internal crosstalk signal gating illum_en.
		OPT3101::deviceRegister temp_offset; ///< temp_offset;Register Addresses: 110[16:8];temperature offset
		OPT3101::deviceRegister en_temp_conv; ///< en_temp_conv;Register Addresses: 110[19:19];To enable temperature conversion
		OPT3101::deviceRegister dis_glb_pd_refsys; ///< dis_glb_pd_refsys;Register Addresses: 118[0:0];
		OPT3101::deviceRegister dis_glb_pd_temp_sens; ///< dis_glb_pd_temp_sens;Register Addresses: 118[1:1];
		OPT3101::deviceRegister dis_glb_pd_illum_drv; ///< dis_glb_pd_illum_drv;Register Addresses: 118[2:2];
		OPT3101::deviceRegister dis_glb_pd_afe; ///< dis_glb_pd_afe;Register Addresses: 118[3:3];
		OPT3101::deviceRegister dis_glb_pd_afe_dac; ///< dis_glb_pd_afe_dac;Register Addresses: 118[4:4];
		OPT3101::deviceRegister dis_glb_pd_amb_dac; ///< dis_glb_pd_amb_dac;Register Addresses: 118[5:5];
		OPT3101::deviceRegister dis_glb_pd_amb_adc; ///< dis_glb_pd_amb_adc;Register Addresses: 118[6:6];
		OPT3101::deviceRegister dis_glb_pd_test_curr; ///< dis_glb_pd_test_curr;Register Addresses: 118[7:7];
		OPT3101::deviceRegister dis_glb_pd_osc; ///< dis_glb_pd_osc;Register Addresses: 118[8:8];
		OPT3101::deviceRegister dis_glb_pd_i2chost; ///< dis_glb_pd_i2chost;Register Addresses: 118[9:9];
		OPT3101::deviceRegister pdn_global; ///< pdn_global;Register Addresses: 118[11:11];
		OPT3101::deviceRegister en_dyn_pd_refsys; ///< en_dyn_pd_refsys;Register Addresses: 119[0:0];
		OPT3101::deviceRegister en_dyn_pd_temp_sens; ///< en_dyn_pd_temp_sens;Register Addresses: 119[1:1];
		OPT3101::deviceRegister en_dyn_pd_illum_drv; ///< en_dyn_pd_illum_drv;Register Addresses: 119[2:2];
		OPT3101::deviceRegister en_dyn_pd_afe; ///< en_dyn_pd_afe;Register Addresses: 119[3:3];
		OPT3101::deviceRegister en_dyn_pd_afe_dac; ///< en_dyn_pd_afe_dac;Register Addresses: 119[4:4];
		OPT3101::deviceRegister en_dyn_pd_amb_dac; ///< en_dyn_pd_amb_dac;Register Addresses: 119[5:5];
		OPT3101::deviceRegister en_dyn_pd_amb_adc; ///< en_dyn_pd_amb_adc;Register Addresses: 119[6:6];
		OPT3101::deviceRegister en_dyn_pd_test_curr; ///< en_dyn_pd_test_curr;Register Addresses: 119[7:7];
		OPT3101::deviceRegister en_dyn_pd_osc; ///< en_dyn_pd_osc;Register Addresses: 119[8:8];
		OPT3101::deviceRegister en_dyn_pd_i2chost_osc; ///< en_dyn_pd_i2chost_osc;Register Addresses: 119[9:9];
		OPT3101::deviceRegister TX0_PIN_CONFIG; ///< TX0_PIN_CONFIG;Register Addresses: 122[5:4];
		OPT3101::deviceRegister TX1_PIN_CONFIG; ///< TX1_PIN_CONFIG;Register Addresses: 122[1:0];
		OPT3101::deviceRegister TX2_PIN_CONFIG; ///< TX2_PIN_CONFIG;Register Addresses: 122[3:2];
		OPT3101::deviceRegister EN_TX_CLKB; ///< EN_TX_CLKB;Register Addresses: 121[0:0];
		OPT3101::deviceRegister EN_TX_CLKZ; ///< EN_TX_CLKZ;Register Addresses: 121[2:2];
		OPT3101::deviceRegister sel_illum_tx0_on_tx1; ///< sel_illum_tx0_on_tx1;Register Addresses: 121[3:3];Force ILLUM_EN_0 (TX0) onto ILLUM_EN_1 (TX1). This mode is required to enable static LED drive mode.
		OPT3101::deviceRegister ILLUM_DC_CURR_DAC; ///< ILLUM_DC_CURR_DAC;Register Addresses: 121[11:8];0.5mA*register setting
		OPT3101::deviceRegister PDN_ILLUM_DC_CURR; ///< PDN_ILLUM_DC_CURR;Register Addresses: 121[12:12];
		OPT3101::deviceRegister FEEDBACK_CONT_MODE; ///< FEEDBACK_CONT_MODE;Register Addresses: 121[13:13];
		OPT3101::deviceRegister PDN_ILLUM_DRV; ///< PDN_ILLUM_DRV;Register Addresses: 121[19:19];
		OPT3101::deviceRegister EN_TX_DC_CURR_ALL; ///< EN_TX_DC_CURR_ALL;Register Addresses: 121[4:4];
};
}

#endif /* OPT3101REGISTERDEFINITION_H_ */

