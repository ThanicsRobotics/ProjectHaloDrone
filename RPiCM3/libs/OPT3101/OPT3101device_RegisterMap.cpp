/*!
* \file OPT3101device_RegisterMap.cpp
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
* The file contains OPT3101::registers class constructor and assignment
*/

#include "OPT3101RegisterDefinition.h"

OPT3101::registers::registers()
{
	
}

OPT3101::registers::registers(int i2cFd) :
dig_gpo_sel0(i2cFd, 1),			///< dig_gpo_sel0;Register Addresses: 11[3:0];
dig_gpo_sel1(i2cFd, 1),			///< dig_gpo_sel1;Register Addresses: 11[7:4];
dig_gpo_sel2(i2cFd, 1),			///< dig_gpo_sel2;Register Addresses: 11[13:10];
dis_ovl_gating(i2cFd, 1),			///< dis_ovl_gating;Register Addresses: 17[15:15];
phase_out(i2cFd, 1),			///< phase_out;Register Addresses: 8[15:0];
phase_overflow(i2cFd, 1),			///< phase_overflow;Register Addresses: 8[16:16];
hdr_mode(i2cFd, 1),			///< hdr_mode;Register Addresses: 8[17:17];
tx_channel(i2cFd, 1),			///< tx_channel;Register Addresses: 8[19:18];
frame_status(i2cFd, 1),			///< frame_status;Register Addresses: 8[20:20];
mod_freq(i2cFd, 1),			///< mod_freq;Register Addresses: 8[21:21];
frame_count0(i2cFd, 1),			///< frame_count0;Register Addresses: 8[23:23];
amp_out(i2cFd, 1),			///< amp_out;Register Addresses: 9[15:0];
frame_count1(i2cFd, 1),			///< frame_count1;Register Addresses: 9[17:16];
sig_ovl_flag(i2cFd, 1),			///< sig_ovl_flag;Register Addresses: 9[18:18];
dealias_bin(i2cFd, 1),			///< dealias_bin;Register Addresses: 9[23:20];
frame_count2(i2cFd, 1),			///< frame_count2;Register Addresses: 10[1:0];
amb_data(i2cFd, 1),			///< amb_data;Register Addresses: 10[11:2];
tmain(i2cFd, 1),			///< tmain;Register Addresses: 10[23:12];
amplitude_min_thr(i2cFd, 2),			///< amplitude_min_thr;Register Addresses: 16[23:16], 17[23:16];
amb_ovl_flag(i2cFd, 1),			///< amb_ovl_flag;Register Addresses: 8[22:22];
phase_overflow_f2(i2cFd, 1),			///< phase_overflow_f2;Register Addresses: 9[19:19];
ref_count_limit(i2cFd, 1),			///< ref_count_limit;Register Addresses: 15[14:0];this sets the limit of ref-clock count when meth1 is used. By programming this we no longer require frequencies which are multiples of powers of 2.;;The default is calculated for 32.768 Khz.
start_freq_calib(i2cFd, 1),			///< start_freq_calib;Register Addresses: 15[16:16];starts the freq_calib
sys_clk_divider(i2cFd, 1),			///< sys_clk_divider;Register Addresses: 15[20:17];The divider can be set according to the ratio b/w ref_clk and tg_clk. The default is 2 which means default ref_clk is assumed at 10 Mhz. ie 40Mhz/4
freq_count_read_reg(i2cFd, 1),			///< freq_count_read_reg;Register Addresses: 16[14:0];read register which holds the value of freq_loop.
freq_count_reg(i2cFd, 1),			///< freq_count_reg;Register Addresses: 17[14:0];The register which is used for frequency correction when enable_auto_freq_count = '0'
en_auto_freq_count(i2cFd, 1),			///< en_auto_freq_count;Register Addresses: 15[21:21];When this is '1' internally computed values is used. Else register value is used.
en_floop(i2cFd, 1),			///< en_floop;Register Addresses: 15[22:22];Enables the freq_loop block. If this is '0', the clock to the freq_loop is gated.
en_freq_corr(i2cFd, 1),			///< en_freq_corr;Register Addresses: 15[23:23];This bit applies frequency correction on the phase data either from register or auto_freq. 
en_cont_fcalib(i2cFd, 1),			///< en_cont_fcalib;Register Addresses: 16[15:15];
monoshot_bit(i2cFd, 1),			///< monoshot_bit;Register Addresses: 0[23:23];In monoshot mode the register to trigger a measurement.
monoshot_mode(i2cFd, 1),			///< monoshot_mode;Register Addresses: 39[1:0];LSB: Enters monoshot mode.;;MSB: If this is set monoshot mode shutdown the oscclk. This has to be used together with monoshot_mode.
powerup_delay(i2cFd, 1),			///< powerup_delay;Register Addresses: 38[23:10];The synchronous counter delay after the ripple counter expires before ungating the clock. About 256*25ns*2^6 ~ 400 us.
monoshot_numframe(i2cFd, 1),			///< monoshot_numframe;Register Addresses: 39[7:2];The number of frames of TG to be run after a trigger before shutting down the TG. The default is kept as 6 allowing a led cycle.
monoshot_fz_clkcnt(i2cFd, 1),			///< monoshot_fz_clkcnt;Register Addresses: 39[23:8];The pix_cnt at which a monoshot operation freezes. By default just freezes 100 cycles before a frame boundary.
en_tx_switch(i2cFd, 1),			///< en_tx_switch;Register Addresses: 42[0:0];Enable switching of led drivers.
sel_tx_ch(i2cFd, 1),			///< sel_tx_ch;Register Addresses: 42[2:1];choses the fix_reg value when switching is disabled.
tx_seq_reg(i2cFd, 1),			///< tx_seq_reg;Register Addresses: 42[14:3];Stores the sequence of led switching in this register.;2-1-0-2-1-0. The sequence will come as 0-1-2-0-1-2 
en_adaptive_hdr(i2cFd, 1),			///< en_adaptive_hdr;Register Addresses: 42[15:15];enable the adaptive hdr. The num_avg_frame in this case should be programmed one more than the normal case.
sel_hdr_mode(i2cFd, 1),			///< sel_hdr_mode;Register Addresses: 42[16:16];choses which current to use when enable_adaptive_hdr = '0'
hdr_thr_low(i2cFd, 1),			///< hdr_thr_low;Register Addresses: 44[15:0];The low threshold of the hysterisis loop. Equivalent to ~64 confidence.
hdr_thr_high(i2cFd, 1),			///< hdr_thr_high;Register Addresses: 43[15:0];the high threshold of the hyserisis loop. Default equivalent to confidence of 256 in 16 bit level.
illum_scale_l_tx0(i2cFd, 1),			///< illum_scale_l_tx0;Register Addresses: 43[18:16];
illum_dac_l_tx0(i2cFd, 1),			///< illum_dac_l_tx0;Register Addresses: 41[4:0];
illum_scale_h_tx0(i2cFd, 1),			///< illum_scale_h_tx0;Register Addresses: 43[21:19];
illum_dac_h_tx0(i2cFd, 1),			///< illum_dac_h_tx0;Register Addresses: 41[9:5];
illum_scale_l_tx1(i2cFd, 1),			///< illum_scale_l_tx1;Register Addresses: 44[18:16];
illum_dac_l_tx1(i2cFd, 1),			///< illum_dac_l_tx1;Register Addresses: 41[14:10];
illum_scale_h_tx1(i2cFd, 1),			///< illum_scale_h_tx1;Register Addresses: 44[21:19];
illum_dac_h_tx1(i2cFd, 1),			///< illum_dac_h_tx1;Register Addresses: 41[19:15];
illum_scale_l_tx2(i2cFd, 1),			///< illum_scale_l_tx2;Register Addresses: 185[20:18];
illum_dac_l_tx2(i2cFd, 2),			///< illum_dac_l_tx2;Register Addresses: 41[23:20], 42[23:23];
illum_scale_h_tx2(i2cFd, 1),			///< illum_scale_h_tx2;Register Addresses: 185[23:21];
illum_dac_h_tx2(i2cFd, 1),			///< illum_dac_h_tx2;Register Addresses: 42[22:18];
amb_adc_in_tx0(i2cFd, 1),			///< amb_adc_in_tx0;Register Addresses: 185[13:12];
amb_adc_in_tx1(i2cFd, 1),			///< amb_adc_in_tx1;Register Addresses: 185[15:14];
amb_adc_in_tx2(i2cFd, 1),			///< amb_adc_in_tx2;Register Addresses: 185[17:16];
give_dealias_data(i2cFd, 1),			///< give_dealias_data;Register Addresses: 184[20:20];
en_dealias_meas(i2cFd, 1),			///< en_dealias_meas;Register Addresses: 64[0:0];enables dealias calculation.;Normally with enable_dealiased_measurement set and enable_multi_freq_phase is unset a combined distance and kb is given out. In the normal phase register, phase of the high frequency itself is given.;
ncr_config(i2cFd, 1),			///< ncr_config;Register Addresses: 64[21:21];option to chose ncr configuration, that is 6/7 (0) or 6/5 (1). Chooses higher frequency by default.
alpha0_dealias_scale(i2cFd, 1),			///< alpha0_dealias_scale;Register Addresses: 64[14:9];indicates the vector multiplication in intrinsic-xtalk component for the dealias frequency. Default is '1'.
beta0_dealias_scale(i2cFd, 1),			///< beta0_dealias_scale;Register Addresses: 64[20:15];
alpha1_dealias_scale(i2cFd, 1),			///< alpha1_dealias_scale;Register Addresses: 65[5:0];indicates the vector multiplication in optical-xtalk component for the dealias frequency. Default is '1'.
beta1_dealias_scale(i2cFd, 1),			///< beta1_dealias_scale;Register Addresses: 65[11:6];
en_multi_freq_phase(i2cFd, 1),			///< en_multi_freq_phase;Register Addresses: 64[22:22];With this bit set along with enable_dealiased_measurement, the usual phase register will have both the frequency information. The frequency of the phase will be indicated in one of the status bit.
temp_avg_main(i2cFd, 1),			///< temp_avg_main;Register Addresses: 3[23:22];
dis_ovl_for_hdr_meth1(i2cFd, 1),			///< dis_ovl_for_hdr_meth1;Register Addresses: 184[21:21];
en_ovl_for_hdr_meth2(i2cFd, 1),			///< en_ovl_for_hdr_meth2;Register Addresses: 184[22:22];
en_tx1_on_tx0(i2cFd, 1),			///< en_tx1_on_tx0;Register Addresses: 185[10:10];
en_tx2_on_tx0(i2cFd, 1),			///< en_tx2_on_tx0;Register Addresses: 185[11:11];
clip_mode_fc(i2cFd, 1),			///< clip_mode_fc;Register Addresses: 80[0:0];chooses either rounding off or clipping or wrap around when applying freq-correction. Default is kept as rounding.
clip_mode_nl(i2cFd, 1),			///< clip_mode_nl;Register Addresses: 80[1:1];chooses either rounding off or clipping or wrap around when applying harmonic correction. Default is kept as rounding.
clip_mode_temp(i2cFd, 1),			///< clip_mode_temp;Register Addresses: 80[2:2];chooses either rounding off or clipping or wrap around when applying temp correction. Default is kept as rounding.
clip_mode_offset(i2cFd, 1),			///< clip_mode_offset;Register Addresses: 80[3:3];chooses either rounding off or clipping or wrap around when applying offset. Default is kept as rounding.
disable_syncing(i2cFd, 1),			///< disable_syncing;Register Addresses: 80[21:21];Normally calc clock and afe_clk are synchronized to avoid reset to reset variation in spur levels. This option is to disable the syncing of dividers (of calc_clk). The default is now changed to '1' because we don't use divided clock for calc-clk from PG3P0 by default. If syncing is used frequency loop will have issues.
force_en_slave(i2cFd, 1),			///< force_en_slave;Register Addresses: 0[22:22];Enable i2c slave for any address forcefully. That is whether auto_load completed or not.
force_en_bypass(i2cFd, 1),			///< force_en_bypass;Register Addresses: 0[21:21];This bit allows the slave to write directly to the efuse. This is gated with stop condition at the port level to avoid transition signals at scl/sda.
override_clkgen_reg(i2cFd, 1),			///< override_clkgen_reg;Register Addresses: 80[22:22];Setting this register '1' allows user to independenly control tm_clkgen(2:1) which controls dealias settings.
software_reset(i2cFd, 1),			///< software_reset;Register Addresses: 0[0:0];
dis_tg_aconf(i2cFd, 1),			///< dis_tg_aconf;Register Addresses: 128[23:23];Some of the tg registers are automatically configured such as pdn*_dyn_tg signal, capture_tg_channel etc. if these signals need to be configured by user this bit may be used as an override.
capture_clk_cnt(i2cFd, 1),			///< capture_clk_cnt;Register Addresses: 160[15:0];This is where early_fvd/svd starts. early_fvd only comes in the frame which is equal num_avg. This is the subframe in which computation results comes up. Programm this to 10600 if planning to use lower frequency.
tg_en(i2cFd, 1),			///< tg_en;Register Addresses: 128[0:0];gates the tg_clk with this bit.
num_sub_frames(i2cFd, 1),			///< num_sub_frames;Register Addresses: 159[11:0];The numbef of subframes in a frame. This number should be greater than or equal to num_avg.
num_avg_sub_frames(i2cFd, 1),			///< num_avg_sub_frames;Register Addresses: 159[23:12];The number of averages for the iq. Used in TG to generate early_fvd and some other TG signals.
sub_vd_clk_cnt(i2cFd, 1),			///< sub_vd_clk_cnt;Register Addresses: 128[16:1];the number of pixels in a subframe. In PG3P0 the default is changed to support 4ksps. The number is also made a multiple of 32+16.
tg_illumen_start(i2cFd, 1),			///< tg_illumen_start;Register Addresses: 143[15:0];spare2_tg. This is used for illum_en. Enabled throughout a subframe.
tg_illumen_end(i2cFd, 1),			///< tg_illumen_end;Register Addresses: 144[15:0];Ending after the 8192+ 250 samples apx.
tg_illumen_mask_start(i2cFd, 1),			///< tg_illumen_mask_start;Register Addresses: 156[11:0];spare2_mask. By default the mask is programmed till num_avg_iq only.
tg_illumen_mask_end(i2cFd, 1),			///< tg_illumen_mask_end;Register Addresses: 156[23:12];
tg_afe_rst_start(i2cFd, 1),			///< tg_afe_rst_start;Register Addresses: 131[15:0];demod_reset. Mask is programmed such that it comes every subframe.
tg_afe_rst_end(i2cFd, 1),			///< tg_afe_rst_end;Register Addresses: 132[15:0];
tg_seq_int_start(i2cFd, 1),			///< tg_seq_int_start;Register Addresses: 133[15:0];interrupt. Only happens in first subframe due to the mask programming
tg_seq_int_end(i2cFd, 1),			///< tg_seq_int_end;Register Addresses: 134[15:0];
tg_capture_start(i2cFd, 1),			///< tg_capture_start;Register Addresses: 135[15:0];capture_tg_channel. Internal TG signal. This signal need to be changed when you go to slower dealias mode. Program this to 11300 and 11800
tg_capture_end(i2cFd, 1),			///< tg_capture_end;Register Addresses: 136[15:0];
tg_ovl_window_start(i2cFd, 1),			///< tg_ovl_window_start;Register Addresses: 137[15:0];ovl_sample. During this time period only ovl is sampled. This exists for only for subframes till the num_avg.
tg_ovl_window_end(i2cFd, 1),			///< tg_ovl_window_end;Register Addresses: 138[15:0];
tg_calc_start(i2cFd, 1),			///< tg_calc_start;Register Addresses: 145[15:0];pdn_dyn_tg. This signal exists roughly from early_fvd start till end of the computation. The mask is programmed such that this only comes in the num_avg sub-frame. Programmed such that it will work even if the frequency changes in both direction. 
tg_calc_end(i2cFd, 1),			///< tg_calc_end;Register Addresses: 146[15:0];
tg_dynpdn_start(i2cFd, 1),			///< tg_dynpdn_start;Register Addresses: 147[15:0];pdn_dyn1_tg. Used to power down less power intensive digital blocks and analog if tm_frame_vd_sub_cnt greater than num_avg_iq.
tg_dynpdn_end(i2cFd, 1),			///< tg_dynpdn_end;Register Addresses: 148[15:0];
tg_seq_int_mask_start(i2cFd, 1),			///< tg_seq_int_mask_start;Register Addresses: 151[11:0];interrupt. Comes only in first (num:0) subframe.
tg_seq_int_mask_end(i2cFd, 1),			///< tg_seq_int_mask_end;Register Addresses: 151[23:12];
tg_capture_mask_start(i2cFd, 1),			///< tg_capture_mask_start;Register Addresses: 152[11:0];capture_tg_channel. By default comes only in the num_avg subchannel. This mask is configurable by user only if dis_tg_aconf = '1'.
tg_capture_mask_end(i2cFd, 1),			///< tg_capture_mask_end;Register Addresses: 152[23:12];
tg_ovl_window_mask_start(i2cFd, 1),			///< tg_ovl_window_mask_start;Register Addresses: 153[11:0];ovl_sample. This exits till num_avg subframe.
tg_ovl_window_mask_end(i2cFd, 1),			///< tg_ovl_window_mask_end;Register Addresses: 153[23:12];
tg_calc_mask_start(i2cFd, 1),			///< tg_calc_mask_start;Register Addresses: 157[11:0];Mask for pdn_dyn_tg. Only enabled during num_avg subframe.
tg_calc_mask_end(i2cFd, 1),			///< tg_calc_mask_end;Register Addresses: 157[23:12];
tg_dynpdn_mask_start(i2cFd, 1),			///< tg_dynpdn_mask_start;Register Addresses: 158[11:0];Mask for pdn_dyn1_tg. Used to power down less power intensive digital blocks and analog if tm_frame_vd_sub_cnt greater than num_avg_iq. Enabled till num_avg subframe.
tg_dynpdn_mask_end(i2cFd, 1),			///< tg_dynpdn_mask_end;Register Addresses: 158[23:12];
en_sequencer(i2cFd, 1),			///< en_sequencer;Register Addresses: 20[16:16];clock gates the logic for sequencer normally. This bit is used to enable sequencer.
en_processor_values(i2cFd, 1),			///< en_processor_values;Register Addresses: 20[17:17];Uses processor values instead of register values.
status_in_reg(i2cFd, 1),			///< status_in_reg;Register Addresses: 20[18:18];the register is used to control the program flow in CPU
mux_sel_compin(i2cFd, 1),			///< mux_sel_compin;Register Addresses: 19[2:0];choses the value used for comp_a register in cpu.;Following are the choices.;phase_out_fsm;dealiased_kb_fsm;dealiased_distance;confidence
compare_reg1(i2cFd, 1),			///< compare_reg1;Register Addresses: 19[18:3];
compare_reg2(i2cFd, 1),			///< compare_reg2;Register Addresses: 20[15:0];
dis_interrupt(i2cFd, 1),			///< dis_interrupt;Register Addresses: 20[19:19];Disables the interrupt which triggers processor. Does not clock gate processor though.
command0(i2cFd, 1),			///< command0;Register Addresses: 21[11:0];NOP for 99 cycles
command1(i2cFd, 1),			///< command1;Register Addresses: 21[23:12];enable intrinsic-xtalk
command2(i2cFd, 1),			///< command2;Register Addresses: 22[11:0];disable intrinsic xtalk
command3(i2cFd, 1),			///< command3;Register Addresses: 22[23:12];Direct go to the first line
command4(i2cFd, 1),			///< command4;Register Addresses: 23[11:0];
command5(i2cFd, 1),			///< command5;Register Addresses: 23[23:12];
command6(i2cFd, 1),			///< command6;Register Addresses: 24[11:0];
command7(i2cFd, 1),			///< command7;Register Addresses: 24[23:12];
command8(i2cFd, 1),			///< command8;Register Addresses: 25[11:0];
command9(i2cFd, 1),			///< command9;Register Addresses: 25[23:12];
command10(i2cFd, 1),			///< command10;Register Addresses: 26[11:0];
command11(i2cFd, 1),			///< command11;Register Addresses: 26[23:12];
command12(i2cFd, 1),			///< command12;Register Addresses: 27[11:0];
command13(i2cFd, 1),			///< command13;Register Addresses: 27[23:12];
command14(i2cFd, 1),			///< command14;Register Addresses: 28[11:0];
command15(i2cFd, 1),			///< command15;Register Addresses: 28[23:12];
command16(i2cFd, 1),			///< command16;Register Addresses: 29[11:0];
command17(i2cFd, 1),			///< command17;Register Addresses: 29[23:12];
command18(i2cFd, 1),			///< command18;Register Addresses: 30[11:0];
command19(i2cFd, 1),			///< command19;Register Addresses: 30[23:12];
force_scale_val(i2cFd, 1),			///< force_scale_val;Register Addresses: 46[2:0];Uses this scale value if disable_auto_scale is programmed. This scale value is also used during any xtalk calibration even if disable_auto_scale is not applied. Default is '0', which means 24bit demod is taken as it is giving maximum accuracy.
dis_auto_scale(i2cFd, 1),			///< dis_auto_scale;Register Addresses: 46[3:3];
disable_conf_rescale(i2cFd, 1),			///< disable_conf_rescale;Register Addresses: 46[13:13];This a mostly a debug register.. When this is set auto_scaled confidence doesn't rescale back. Even when force_scale_val is there, it doesn't rescale. This bit may be set along with force_scale_val to see the effect of confidence scaling.
int_xtalk_calib(i2cFd, 1),			///< int_xtalk_calib;Register Addresses: 46[4:4];Puts the device into intrinsic calibration mode.
xtalk_filt_time_const(i2cFd, 1),			///< xtalk_filt_time_const;Register Addresses: 46[23:20];Time constant during crosstalk filtering. Higher the time constant slower the filtering is.
use_xtalk_filt_int(i2cFd, 1),			///< use_xtalk_filt_int;Register Addresses: 46[5:5];Whehter to use filter or direct sampling for intrinsic crosstalk.
use_xtalk_reg_int(i2cFd, 1),			///< use_xtalk_reg_int;Register Addresses: 46[6:6];Whether to use register or filter/sample for intrinsic.
iq_read_data_sel(i2cFd, 1),			///< iq_read_data_sel;Register Addresses: 46[11:9];mux used to chose which of the xtalk register is being read out.;;010 -- raw_i/q;000 -- intrinsic_xtalk;001 -- optical_xtalk
iphase_xtalk(i2cFd, 1),			///< iphase_xtalk;Register Addresses: 59[23:0];
qphase_xtalk(i2cFd, 1),			///< qphase_xtalk;Register Addresses: 60[23:0];
int_xtalk_reg_scale(i2cFd, 1),			///< int_xtalk_reg_scale;Register Addresses: 46[16:14];allows scaling of the meaning of ixtalk register. 0- 2^0, 2^1, 2^2, 2^3 etc.
iphase_xtalk_int_reg(i2cFd, 1),			///< iphase_xtalk_int_reg;Register Addresses: 61[15:0];inphase  component for intrinsic xtalk
qphase_xtalk_int_reg(i2cFd, 1),			///< qphase_xtalk_int_reg;Register Addresses: 62[15:0];quadrature  component for intrinsic xtalk
illum_xtalk_calib(i2cFd, 1),			///< illum_xtalk_calib;Register Addresses: 46[12:12];puts the device into optical calibration mode.
illum_xtalk_reg_scale(i2cFd, 1),			///< illum_xtalk_reg_scale;Register Addresses: 46[19:17];allows scaling of the meaning of oxtalk register. 0- 2^0, 2^2, 2^4, 2^8.
use_xtalk_filt_illum(i2cFd, 1),			///< use_xtalk_filt_illum;Register Addresses: 46[7:7];
use_xtalk_reg_illum(i2cFd, 1),			///< use_xtalk_reg_illum;Register Addresses: 46[8:8];For optical default is to use the register values.
iphase_xtalk_reg_hdr0_tx0(i2cFd, 1),			///< iphase_xtalk_reg_hdr0_tx0;Register Addresses: 47[15:0];inphase component of the xtalk for hdr0/led0
qphase_xtalk_reg_hdr0_tx0(i2cFd, 1),			///< qphase_xtalk_reg_hdr0_tx0;Register Addresses: 48[15:0];quadrature component of the xtalk for hdr0/led0
iphase_xtalk_reg_hdr1_tx0(i2cFd, 1),			///< iphase_xtalk_reg_hdr1_tx0;Register Addresses: 49[15:0];
qphase_xtalk_reg_hdr1_tx0(i2cFd, 1),			///< qphase_xtalk_reg_hdr1_tx0;Register Addresses: 50[15:0];
iphase_xtalk_reg_hdr0_tx1(i2cFd, 1),			///< iphase_xtalk_reg_hdr0_tx1;Register Addresses: 51[15:0];
qphase_xtalk_reg_hdr0_tx1(i2cFd, 1),			///< qphase_xtalk_reg_hdr0_tx1;Register Addresses: 52[15:0];
iphase_xtalk_reg_hdr1_tx1(i2cFd, 1),			///< iphase_xtalk_reg_hdr1_tx1;Register Addresses: 53[15:0];
qphase_xtalk_reg_hdr1_tx1(i2cFd, 1),			///< qphase_xtalk_reg_hdr1_tx1;Register Addresses: 54[15:0];
iphase_xtalk_reg_hdr0_tx2(i2cFd, 1),			///< iphase_xtalk_reg_hdr0_tx2;Register Addresses: 55[15:0];
qphase_xtalk_reg_hdr0_tx2(i2cFd, 1),			///< qphase_xtalk_reg_hdr0_tx2;Register Addresses: 56[15:0];
iphase_xtalk_reg_hdr1_tx2(i2cFd, 1),			///< iphase_xtalk_reg_hdr1_tx2;Register Addresses: 57[15:0];
qphase_xtalk_reg_hdr1_tx2(i2cFd, 1),			///< qphase_xtalk_reg_hdr1_tx2;Register Addresses: 58[15:0];
en_temp_xtalk_corr(i2cFd, 1),			///< en_temp_xtalk_corr;Register Addresses: 58[16:16];
scale_temp_coeff_xtalk(i2cFd, 1),			///< scale_temp_coeff_xtalk;Register Addresses: 58[19:17];Allows programmability on the temp_coefficients range and precision.
temp_coeff_xtalk_iphase_hdr0_tx0(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr0_tx0;Register Addresses: 56[23:16];
temp_coeff_xtalk_qphase_hdr0_tx0(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr0_tx0;Register Addresses: 57[23:16];
temp_coeff_xtalk_iphase_hdr1_tx0(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr1_tx0;Register Addresses: 94[15:8];
temp_coeff_xtalk_qphase_hdr1_tx0(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr1_tx0;Register Addresses: 96[7:0];
temp_coeff_xtalk_iphase_hdr0_tx1(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr0_tx1;Register Addresses: 94[23:16];
temp_coeff_xtalk_qphase_hdr0_tx1(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr0_tx1;Register Addresses: 96[15:8];
temp_coeff_xtalk_iphase_hdr1_tx1(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr1_tx1;Register Addresses: 95[7:0];
temp_coeff_xtalk_qphase_hdr1_tx1(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr1_tx1;Register Addresses: 96[23:16];
temp_coeff_xtalk_iphase_hdr0_tx2(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr0_tx2;Register Addresses: 95[15:8];
temp_coeff_xtalk_qphase_hdr0_tx2(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr0_tx2;Register Addresses: 97[7:0];
temp_coeff_xtalk_iphase_hdr1_tx2(i2cFd, 1),			///< temp_coeff_xtalk_iphase_hdr1_tx2;Register Addresses: 95[23:16];
temp_coeff_xtalk_qphase_hdr1_tx2(i2cFd, 1),			///< temp_coeff_xtalk_qphase_hdr1_tx2;Register Addresses: 97[15:8];
temp_coeff_illum_xtalk_iphase_hdr0_tx0(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr0_tx0;Register Addresses: 54[23:16];
temp_coeff_illum_xtalk_qphase_hdr0_tx0(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr0_tx0;Register Addresses: 55[23:16];
temp_coeff_illum_xtalk_iphase_hdr1_tx0(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr1_tx0;Register Addresses: 91[7:0];
temp_coeff_illum_xtalk_iphase_hdr0_tx1(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr0_tx1;Register Addresses: 91[15:8];
temp_coeff_illum_xtalk_iphase_hdr1_tx1(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr1_tx1;Register Addresses: 91[23:16];
temp_coeff_illum_xtalk_iphase_hdr0_tx2(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr0_tx2;Register Addresses: 92[7:0];
temp_coeff_illum_xtalk_iphase_hdr1_tx2(i2cFd, 1),			///< temp_coeff_illum_xtalk_iphase_hdr1_tx2;Register Addresses: 92[15:8];
temp_coeff_illum_xtalk_qphase_hdr1_tx0(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr1_tx0;Register Addresses: 92[23:16];
temp_coeff_illum_xtalk_qphase_hdr0_tx1(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr0_tx1;Register Addresses: 93[7:0];
temp_coeff_illum_xtalk_qphase_hdr1_tx1(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr1_tx1;Register Addresses: 93[15:8];
temp_coeff_illum_xtalk_qphase_hdr0_tx2(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr0_tx2;Register Addresses: 93[23:16];
temp_coeff_illum_xtalk_qphase_hdr1_tx2(i2cFd, 1),			///< temp_coeff_illum_xtalk_qphase_hdr1_tx2;Register Addresses: 94[7:0];
amb_xtalk_qphase_coeff(i2cFd, 1),			///< amb_xtalk_qphase_coeff;Register Addresses: 12[15:8];reflects the variation of quad component.
amb_xtalk_iphase_coeff(i2cFd, 1),			///< amb_xtalk_iphase_coeff;Register Addresses: 12[7:0];reflect the variation of optical crosstalk inphase component.
scale_amb_coeff_xtalk(i2cFd, 1),			///< scale_amb_coeff_xtalk;Register Addresses: 58[22:20];Ambient xtalk mode. Provides range/precision for ambient correction.
en_phase_corr(i2cFd, 1),			///< en_phase_corr;Register Addresses: 67[0:0];enables phase correction from the values programmed.
phase_offset_hdr0_tx0(i2cFd, 1),			///< phase_offset_hdr0_tx0;Register Addresses: 66[15:0];phase_offset for freq1
phase_offset_hdr1_tx0(i2cFd, 1),			///< phase_offset_hdr1_tx0;Register Addresses: 81[15:0];
phase_offset_hdr0_tx1(i2cFd, 1),			///< phase_offset_hdr0_tx1;Register Addresses: 82[15:0];
phase_offset_hdr1_tx1(i2cFd, 1),			///< phase_offset_hdr1_tx1;Register Addresses: 83[15:0];
phase_offset_hdr0_tx2(i2cFd, 1),			///< phase_offset_hdr0_tx2;Register Addresses: 84[15:0];
phase_offset_hdr1_tx2(i2cFd, 1),			///< phase_offset_hdr1_tx2;Register Addresses: 85[15:0];
reverse_phase_before_offset(i2cFd, 1),			///< reverse_phase_before_offset;Register Addresses: 67[9:9];
phase2_offset_hdr0_tx0(i2cFd, 1),			///< phase2_offset_hdr0_tx0;Register Addresses: 68[15:0];phase offset for freq2
phase2_offset_hdr1_tx0(i2cFd, 1),			///< phase2_offset_hdr1_tx0;Register Addresses: 86[15:0];
phase2_offset_hdr0_tx1(i2cFd, 1),			///< phase2_offset_hdr0_tx1;Register Addresses: 87[15:0];
phase2_offset_hdr1_tx1(i2cFd, 1),			///< phase2_offset_hdr1_tx1;Register Addresses: 88[15:0];
phase2_offset_hdr0_tx2(i2cFd, 1),			///< phase2_offset_hdr0_tx2;Register Addresses: 89[15:0];
phase2_offset_hdr1_tx2(i2cFd, 1),			///< phase2_offset_hdr1_tx2;Register Addresses: 90[15:0];
en_temp_corr(i2cFd, 1),			///< en_temp_corr;Register Addresses: 67[1:1];enables temperature correction for phase
scale_phase_temp_coeff(i2cFd, 1),			///< scale_phase_temp_coeff;Register Addresses: 67[8:6];changes the meaning of coefficients related to phase correction.
tmain_calib_hdr0_tx0(i2cFd, 1),			///< tmain_calib_hdr0_tx0;Register Addresses: 71[11:0];calibrated temperature for main temp sensor.Default is 2048 because internally it is treated as offset binary
temp_coeff_main_hdr0_tx0(i2cFd, 1),			///< temp_coeff_main_hdr0_tx0;Register Addresses: 69[11:0];temperature coefficient for phase correction for main temp. By default means phase change for 64 degrees of temperature.
tmain_calib_hdr1_tx0(i2cFd, 1),			///< tmain_calib_hdr1_tx0;Register Addresses: 72[11:0];
temp_coeff_main_hdr1_tx0(i2cFd, 1),			///< temp_coeff_main_hdr1_tx0;Register Addresses: 45[11:0];
tmain_calib_hdr0_tx1(i2cFd, 1),			///< tmain_calib_hdr0_tx1;Register Addresses: 73[11:0];
temp_coeff_main_hdr0_tx1(i2cFd, 1),			///< temp_coeff_main_hdr0_tx1;Register Addresses: 45[23:12];
tmain_calib_hdr1_tx1(i2cFd, 1),			///< tmain_calib_hdr1_tx1;Register Addresses: 65[23:12];
temp_coeff_main_hdr1_tx1(i2cFd, 2),			///< temp_coeff_main_hdr1_tx1;Register Addresses: 47[23:16], 48[23:20];
tmain_calib_hdr0_tx2(i2cFd, 1),			///< tmain_calib_hdr0_tx2;Register Addresses: 63[11:0];
temp_coeff_main_hdr0_tx2(i2cFd, 2),			///< temp_coeff_main_hdr0_tx2;Register Addresses: 49[23:16], 50[23:20];
tmain_calib_hdr1_tx2(i2cFd, 1),			///< tmain_calib_hdr1_tx2;Register Addresses: 69[23:12];
temp_coeff_main_hdr1_tx2(i2cFd, 2),			///< temp_coeff_main_hdr1_tx2;Register Addresses: 51[23:16], 52[23:20];
tillum_calib_hdr0_tx0(i2cFd, 1),			///< tillum_calib_hdr0_tx0;Register Addresses: 71[23:12];calibrated temp for tillum. Default is 2048 because internally it is treated as offset binary
temp_coeff_illum_hdr0_tx0(i2cFd, 1),			///< temp_coeff_illum_hdr0_tx0;Register Addresses: 70[11:0];temperature coefficient for phase correction for illum temp. By default means phase change for 64 degrees of temperature.
tillum_calib_hdr1_tx0(i2cFd, 1),			///< tillum_calib_hdr1_tx0;Register Addresses: 72[23:12];
temp_coeff_illum_hdr1_tx0(i2cFd, 2),			///< temp_coeff_illum_hdr1_tx0;Register Addresses: 81[23:16], 82[23:20];
tillum_calib_hdr0_tx1(i2cFd, 1),			///< tillum_calib_hdr0_tx1;Register Addresses: 73[23:12];
temp_coeff_illum_hdr0_tx1(i2cFd, 2),			///< temp_coeff_illum_hdr0_tx1;Register Addresses: 83[23:16], 84[23:20];
tillum_calib_hdr1_tx1(i2cFd, 1),			///< tillum_calib_hdr1_tx1;Register Addresses: 67[23:12];
temp_coeff_illum_hdr1_tx1(i2cFd, 2),			///< temp_coeff_illum_hdr1_tx1;Register Addresses: 85[23:16], 86[23:20];
tillum_calib_hdr0_tx2(i2cFd, 1),			///< tillum_calib_hdr0_tx2;Register Addresses: 63[23:12];
temp_coeff_illum_hdr0_tx2(i2cFd, 2),			///< temp_coeff_illum_hdr0_tx2;Register Addresses: 87[23:16], 88[23:20];
tillum_calib_hdr1_tx2(i2cFd, 1),			///< tillum_calib_hdr1_tx2;Register Addresses: 70[23:12];
temp_coeff_illum_hdr1_tx2(i2cFd, 2),			///< temp_coeff_illum_hdr1_tx2;Register Addresses: 89[23:16], 90[23:20];
amb_sat_thr(i2cFd, 1),			///< amb_sat_thr;Register Addresses: 13[16:7];the threshold which is used to detect the ambient overload. Default is kept at highest.;Threshold set for 1v on the ambient dac resistor. 512x3/8xIR. 
amb_calib(i2cFd, 1),			///< amb_calib;Register Addresses: 11[23:14];the ambient at which device is calibrated for optical xtalk/phase offset etc.
amb_phase_corr_pwl_coeff0(i2cFd, 1),			///< amb_phase_corr_pwl_coeff0;Register Addresses: 12[23:16];Captures the drift in the phase wrto ambient. Can be a negative number to reflect inverse relationship. The number is divided by 2^5 to get the actual value.
amb_phase_corr_pwl_x0(i2cFd, 1),			///< amb_phase_corr_pwl_x0;Register Addresses: 184[9:0];
amb_phase_corr_pwl_x1(i2cFd, 1),			///< amb_phase_corr_pwl_x1;Register Addresses: 184[19:10];
amb_phase_corr_pwl_x2(i2cFd, 1),			///< amb_phase_corr_pwl_x2;Register Addresses: 185[9:0];
amb_phase_corr_pwl_coeff1(i2cFd, 1),			///< amb_phase_corr_pwl_coeff1;Register Addresses: 180[7:0];The first coefficient is assumed to be the original one 
amb_phase_corr_pwl_coeff2(i2cFd, 1),			///< amb_phase_corr_pwl_coeff2;Register Addresses: 180[15:8];
amb_phase_corr_pwl_coeff3(i2cFd, 1),			///< amb_phase_corr_pwl_coeff3;Register Addresses: 180[23:16];
scale_amb_phase_corr_coeff(i2cFd, 1),			///< scale_amb_phase_corr_coeff;Register Addresses: 181[2:0];Scales the gain/vs accuracy.
temp_coeff_illum_square_hdr0(i2cFd, 1),			///< temp_coeff_illum_square_hdr0;Register Addresses: 182[7:0];
temp_coeff_illum_square_hdr1(i2cFd, 1),			///< temp_coeff_illum_square_hdr1;Register Addresses: 182[15:8];
temp_coeff_main_square_hdr0(i2cFd, 1),			///< temp_coeff_main_square_hdr0;Register Addresses: 183[7:0];
temp_coeff_main_square_hdr1(i2cFd, 1),			///< temp_coeff_main_square_hdr1;Register Addresses: 183[15:8];
scale_phase_temp_corr_square(i2cFd, 1),			///< scale_phase_temp_corr_square;Register Addresses: 181[5:3];scales square correction range/accuracy.
en_nl_corr(i2cFd, 1),			///< en_nl_corr;Register Addresses: 74[0:0];enables harmonic/nonlinear correction for phase.
a1_coeff_hdr0_tx0(i2cFd, 1),			///< a1_coeff_hdr0_tx0;Register Addresses: 75[15:0];
a2_coeff_hdr0_tx0(i2cFd, 1),			///< a2_coeff_hdr0_tx0;Register Addresses: 76[15:0];
a3_coeff_hdr0_tx0(i2cFd, 1),			///< a3_coeff_hdr0_tx0;Register Addresses: 77[15:0];
a4_coeff_hdr0_tx0(i2cFd, 1),			///< a4_coeff_hdr0_tx0;Register Addresses: 78[15:0];
scale_nl_corr_coeff(i2cFd, 1),			///< scale_nl_corr_coeff;Register Addresses: 74[19:18];changes the meaning of the nonlinear coefficients.;
a0_coeff_hdr0_tx0(i2cFd, 1),			///< a0_coeff_hdr0_tx0;Register Addresses: 74[17:2];
a0_coeff_hdr1_tx0(i2cFd, 1),			///< a0_coeff_hdr1_tx0;Register Addresses: 162[15:0];
a0_coeff_hdr0_tx1(i2cFd, 1),			///< a0_coeff_hdr0_tx1;Register Addresses: 163[15:0];
a0_coeff_hdr1_tx1(i2cFd, 1),			///< a0_coeff_hdr1_tx1;Register Addresses: 164[15:0];
a0_coeff_hdr0_tx2(i2cFd, 1),			///< a0_coeff_hdr0_tx2;Register Addresses: 165[15:0];
a0_coeff_hdr1_tx2(i2cFd, 1),			///< a0_coeff_hdr1_tx2;Register Addresses: 166[15:0];
a1_coeff_hdr1_tx0(i2cFd, 1),			///< a1_coeff_hdr1_tx0;Register Addresses: 167[15:0];
a1_coeff_hdr0_tx1(i2cFd, 1),			///< a1_coeff_hdr0_tx1;Register Addresses: 168[15:0];
a1_coeff_hdr1_tx1(i2cFd, 1),			///< a1_coeff_hdr1_tx1;Register Addresses: 169[15:0];
a1_coeff_hdr0_tx2(i2cFd, 1),			///< a1_coeff_hdr0_tx2;Register Addresses: 170[15:0];
a1_coeff_hdr1_tx2(i2cFd, 1),			///< a1_coeff_hdr1_tx2;Register Addresses: 171[15:0];
a2_coeff_hdr1_tx0(i2cFd, 1),			///< a2_coeff_hdr1_tx0;Register Addresses: 172[15:0];
a2_coeff_hdr0_tx1(i2cFd, 1),			///< a2_coeff_hdr0_tx1;Register Addresses: 173[15:0];
a2_coeff_hdr1_tx1(i2cFd, 1),			///< a2_coeff_hdr1_tx1;Register Addresses: 174[15:0];
a2_coeff_hdr0_tx2(i2cFd, 1),			///< a2_coeff_hdr0_tx2;Register Addresses: 175[15:0];
a2_coeff_hdr1_tx2(i2cFd, 1),			///< a2_coeff_hdr1_tx2;Register Addresses: 176[15:0];
a3_coeff_hdr1_tx0(i2cFd, 1),			///< a3_coeff_hdr1_tx0;Register Addresses: 177[15:0];
a3_coeff_hdr0_tx1(i2cFd, 2),			///< a3_coeff_hdr0_tx1;Register Addresses: 162[23:16], 163[23:16];
a3_coeff_hdr1_tx1(i2cFd, 2),			///< a3_coeff_hdr1_tx1;Register Addresses: 164[23:16], 165[23:16];
a3_coeff_hdr0_tx2(i2cFd, 2),			///< a3_coeff_hdr0_tx2;Register Addresses: 166[23:16], 167[23:16];
a3_coeff_hdr1_tx2(i2cFd, 2),			///< a3_coeff_hdr1_tx2;Register Addresses: 168[23:16], 169[23:16];
a4_coeff_hdr1_tx0(i2cFd, 2),			///< a4_coeff_hdr1_tx0;Register Addresses: 170[23:16], 171[23:16];
a4_coeff_hdr0_tx1(i2cFd, 2),			///< a4_coeff_hdr0_tx1;Register Addresses: 172[23:16], 173[23:16];
a4_coeff_hdr1_tx1(i2cFd, 2),			///< a4_coeff_hdr1_tx1;Register Addresses: 174[23:16], 175[23:16];
a4_coeff_hdr0_tx2(i2cFd, 2),			///< a4_coeff_hdr0_tx2;Register Addresses: 176[23:16], 177[23:16];
a4_coeff_hdr1_tx2(i2cFd, 1),			///< a4_coeff_hdr1_tx2;Register Addresses: 178[15:0];
tillum(i2cFd, 1),			///< tillum;Register Addresses: 4[19:8];The value of illum-temperature register.
tsens_slave0(i2cFd, 1),			///< tsens_slave0;Register Addresses: 2[6:0];slave address of the led0 tsensor
tsens_slave1(i2cFd, 1),			///< tsens_slave1;Register Addresses: 2[13:7];slave address of the led1 tsensor
tsens_slave2(i2cFd, 1),			///< tsens_slave2;Register Addresses: 2[20:14];slave address of the led2 tsensor
config_tillum_msb(i2cFd, 1),			///< config_tillum_msb;Register Addresses: 7[23:20];
en_tillum_12b(i2cFd, 1),			///< en_tillum_12b;Register Addresses: 13[23:23];While interfacing with the TMP02 type tempsensor, this bit needs to be set to swap bytes to allow different read format.
tillum_unsigned(i2cFd, 1),			///< tillum_unsigned;Register Addresses: 4[23:23];This bit is set '1' when temperature given by tmain/tillum sensor is in unsigned format.
temp_avg_illum(i2cFd, 1),			///< temp_avg_illum;Register Addresses: 2[23:22];Based on this, temperature is averaged to remove quantization errors on temperature.
en_tsens_read_fvd(i2cFd, 1),			///< en_tsens_read_fvd;Register Addresses: 3[18:18];If this bit is set tmain temperature is read every frame. Other wise it is read based on a register trigger.
en_tillum_read(i2cFd, 1),			///< en_tillum_read;Register Addresses: 2[21:21];Enable i2c read of appropriate illum led. If this bit is not set illumination driver temperature is not read.
eeprom_read_trig(i2cFd, 1),			///< eeprom_read_trig;Register Addresses: 1[0:0];Used to read efuse values into the chain using register trigger
swap_read_data(i2cFd, 1),			///< swap_read_data;Register Addresses: 1[1:1];swaps/reverse the data read by i2c-host.
eeprom_start_reg_addr(i2cFd, 1),			///< eeprom_start_reg_addr;Register Addresses: 1[16:9];The first byte written while reading from the efuse. This will typically be 0. At startup there is no way to program this.
frame_vd_trig(i2cFd, 1),			///< frame_vd_trig;Register Addresses: 1[17:17];when this bit is '1' i2c host is triggered every frame vd. Else it is triggered based on the i2c_trig_reg.
i2c_trig_reg(i2cFd, 1),			///< i2c_trig_reg;Register Addresses: 1[18:18];The trigger register for i2c transaction.
i2c_en(i2cFd, 1),			///< i2c_en;Register Addresses: 1[19:19];Enables the i2c host operation. Does not control the init load.
i2c_rw(i2cFd, 1),			///< i2c_rw;Register Addresses: 1[21:20];Choses the r/w for i2c host operation.;By default it reads temperature sensor.
i2c_read_data(i2cFd, 1),			///< i2c_read_data;Register Addresses: 3[7:0];The hosts read data.
i2c_write_data1(i2cFd, 1),			///< i2c_write_data1;Register Addresses: 3[16:9];The address where the read would start. Normally in temperature sensor read this is not required to be programmed.
i2c_num_tran(i2cFd, 1),			///< i2c_num_tran;Register Addresses: 3[17:17];The number of transactions. Either 1 or 2.
en_eeprom_read(i2cFd, 1),			///< en_eeprom_read;Register Addresses: 1[23:23];Disables the gating of auto_load clock after init_load_done. Should be used if register triggering has to work.
init_load_done(i2cFd, 1),			///< init_load_done;Register Addresses: 3[8:8];Can be used to check whether initial auto_load is successful or not.
addr_slave_eeprom(i2cFd, 1),			///< addr_slave_eeprom;Register Addresses: 1[8:2];
i2c_num_bytes_tran1(i2cFd, 1),			///< i2c_num_bytes_tran1;Register Addresses: 7[17:16];Number of bytes used in the tran2 of i2c transaction.
i2c_num_bytes_tran2(i2cFd, 1),			///< i2c_num_bytes_tran2;Register Addresses: 5[23:22];Number of bytes used in the tran2 of i2c transaction.
i2c_write_data2(i2cFd, 1),			///< i2c_write_data2;Register Addresses: 7[7:0];
i2c_sel_read_bytes(i2cFd, 1),			///< i2c_sel_read_bytes;Register Addresses: 7[19:18];choses which byte of i2c_read register to be read.
i2c_cont_rw(i2cFd, 1),			///< i2c_cont_rw;Register Addresses: 0[6:6];
dis_ovldet(i2cFd, 1),			///< dis_ovldet;Register Addresses: 101[23:23];to disable overload detection
prog_ovldet_refp(i2cFd, 1),			///< prog_ovldet_refp;Register Addresses: 100[20:18];To program OVL_DET REFP
prog_ovldet_refm(i2cFd, 1),			///< prog_ovldet_refm;Register Addresses: 100[23:21];To program OVL_DET REFM
iamb_max_sel(i2cFd, 1),			///< iamb_max_sel;Register Addresses: 114[7:4];selects the value of DAC resistor
tm_vrefp_diode(i2cFd, 1),			///< tm_vrefp_diode;Register Addresses: 109[2:0];To program the bias voltage INP
tm_vrefm_diode(i2cFd, 1),			///< tm_vrefm_diode;Register Addresses: 109[5:3];To program the bias voltage INM
gpo1_mux_sel(i2cFd, 1),			///< gpo1_mux_sel;Register Addresses: 120[8:6];
gpio1_obuf_en(i2cFd, 1),			///< gpio1_obuf_en;Register Addresses: 120[12:12];
gpio1_ibuf_en(i2cFd, 1),			///< gpio1_ibuf_en;Register Addresses: 120[13:13];
gpo2_mux_sel(i2cFd, 1),			///< gpo2_mux_sel;Register Addresses: 120[11:9];
gpio2_obuf_en(i2cFd, 1),			///< gpio2_obuf_en;Register Addresses: 120[15:15];
gpio2_ibuf_en(i2cFd, 1),			///< gpio2_ibuf_en;Register Addresses: 120[16:16];
gpo3_mux_sel(i2cFd, 1),			///< gpo3_mux_sel;Register Addresses: 120[2:0];
sel_gp3_on_sdam(i2cFd, 1),			///< sel_gp3_on_sdam;Register Addresses: 120[22:22];
dealias_en(i2cFd, 1),			///< dealias_en;Register Addresses: 113[1:1];To enable Dealias mode to get different Modulation freq close to 40MHz, ;34.3 MHz & 48 MHz.
dealias_freq(i2cFd, 1),			///< dealias_freq;Register Addresses: 113[2:2];Changes o/p freq in dealias mode;- only when dealais mode is enabled (TM_CLKGENless than2greater than is set to '1').
shift_illum_phase(i2cFd, 1),			///< shift_illum_phase;Register Addresses: 113[6:3];Shift bits to get different LED_CLK phases in calib mode.;;80M mode:16phases, 22.5 deg separation(360/16)-6.25n separation;;40M mode:16phases, 45 deg separation(360/8)-12.5n separation;;Basically 40M mode MSB bit is unused.
shut_clocks(i2cFd, 1),			///< shut_clocks;Register Addresses: 113[8:8];to shut down all 20M, 10M clock switching
invert_tg_clk(i2cFd, 1),			///< invert_tg_clk;Register Addresses: 113[9:9];to invert tg_clk for timing requirements
invert_afe_clk(i2cFd, 1),			///< invert_afe_clk;Register Addresses: 113[11:11];
dis_illum_clk_tx(i2cFd, 1),			///< dis_illum_clk_tx;Register Addresses: 113[12:12];Disable LED clk going to transmitter
en_illum_clk_gpio(i2cFd, 1),			///< en_illum_clk_gpio;Register Addresses: 113[16:16];Disable LED CLK going to GPIO
illum_clk_gpio_mode(i2cFd, 1),			///< illum_clk_gpio_mode;Register Addresses: 113[15:15];When this bit is '1', illum_en gating led_clk going to GPIO is masked.;
unmask_illumen_intxtalk(i2cFd, 1),			///< unmask_illumen_intxtalk;Register Addresses: 113[17:17];Mask internal crosstalk signal gating illum_en.
temp_offset(i2cFd, 1),			///< temp_offset;Register Addresses: 110[16:8];temperature offset
en_temp_conv(i2cFd, 1),			///< en_temp_conv;Register Addresses: 110[19:19];To enable temperature conversion
dis_glb_pd_refsys(i2cFd, 1),			///< dis_glb_pd_refsys;Register Addresses: 118[0:0];
dis_glb_pd_temp_sens(i2cFd, 1),			///< dis_glb_pd_temp_sens;Register Addresses: 118[1:1];
dis_glb_pd_illum_drv(i2cFd, 1),			///< dis_glb_pd_illum_drv;Register Addresses: 118[2:2];
dis_glb_pd_afe(i2cFd, 1),			///< dis_glb_pd_afe;Register Addresses: 118[3:3];
dis_glb_pd_afe_dac(i2cFd, 1),			///< dis_glb_pd_afe_dac;Register Addresses: 118[4:4];
dis_glb_pd_amb_dac(i2cFd, 1),			///< dis_glb_pd_amb_dac;Register Addresses: 118[5:5];
dis_glb_pd_amb_adc(i2cFd, 1),			///< dis_glb_pd_amb_adc;Register Addresses: 118[6:6];
dis_glb_pd_test_curr(i2cFd, 1),			///< dis_glb_pd_test_curr;Register Addresses: 118[7:7];
dis_glb_pd_osc(i2cFd, 1),			///< dis_glb_pd_osc;Register Addresses: 118[8:8];
dis_glb_pd_i2chost(i2cFd, 1),			///< dis_glb_pd_i2chost;Register Addresses: 118[9:9];
pdn_global(i2cFd, 1),			///< pdn_global;Register Addresses: 118[11:11];
en_dyn_pd_refsys(i2cFd, 1),			///< en_dyn_pd_refsys;Register Addresses: 119[0:0];
en_dyn_pd_temp_sens(i2cFd, 1),			///< en_dyn_pd_temp_sens;Register Addresses: 119[1:1];
en_dyn_pd_illum_drv(i2cFd, 1),			///< en_dyn_pd_illum_drv;Register Addresses: 119[2:2];
en_dyn_pd_afe(i2cFd, 1),			///< en_dyn_pd_afe;Register Addresses: 119[3:3];
en_dyn_pd_afe_dac(i2cFd, 1),			///< en_dyn_pd_afe_dac;Register Addresses: 119[4:4];
en_dyn_pd_amb_dac(i2cFd, 1),			///< en_dyn_pd_amb_dac;Register Addresses: 119[5:5];
en_dyn_pd_amb_adc(i2cFd, 1),			///< en_dyn_pd_amb_adc;Register Addresses: 119[6:6];
en_dyn_pd_test_curr(i2cFd, 1),			///< en_dyn_pd_test_curr;Register Addresses: 119[7:7];
en_dyn_pd_osc(i2cFd, 1),			///< en_dyn_pd_osc;Register Addresses: 119[8:8];
en_dyn_pd_i2chost_osc(i2cFd, 1),			///< en_dyn_pd_i2chost_osc;Register Addresses: 119[9:9];
TX0_PIN_CONFIG(i2cFd, 1),			///< TX0_PIN_CONFIG;Register Addresses: 122[5:4];
TX1_PIN_CONFIG(i2cFd, 1),			///< TX1_PIN_CONFIG;Register Addresses: 122[1:0];
TX2_PIN_CONFIG(i2cFd, 1),			///< TX2_PIN_CONFIG;Register Addresses: 122[3:2];
EN_TX_CLKB(i2cFd, 1),			///< EN_TX_CLKB;Register Addresses: 121[0:0];
EN_TX_CLKZ(i2cFd, 1),			///< EN_TX_CLKZ;Register Addresses: 121[2:2];
sel_illum_tx0_on_tx1(i2cFd, 1),			///< sel_illum_tx0_on_tx1;Register Addresses: 121[3:3];Force ILLUM_EN_0 (TX0) onto ILLUM_EN_1 (TX1). This mode is required to enable static LED drive mode.
ILLUM_DC_CURR_DAC(i2cFd, 1),			///< ILLUM_DC_CURR_DAC;Register Addresses: 121[11:8];0.5mA*register setting
PDN_ILLUM_DC_CURR(i2cFd, 1),			///< PDN_ILLUM_DC_CURR;Register Addresses: 121[12:12];
FEEDBACK_CONT_MODE(i2cFd, 1),			///< FEEDBACK_CONT_MODE;Register Addresses: 121[13:13];
PDN_ILLUM_DRV(i2cFd, 1),			///< PDN_ILLUM_DRV;Register Addresses: 121[19:19];
EN_TX_DC_CURR_ALL(i2cFd, 1)			///< EN_TX_DC_CURR_ALL;Register Addresses: 121[4:4];
{
	//Initialization for register dig_gpo_sel0
	this->dig_gpo_sel0.address[0] = 0x0b; ///< Address=11
	this->dig_gpo_sel0.msb[0] = 3;
	this->dig_gpo_sel0.lsb[0] = 0;

	//Initialization for register dig_gpo_sel1
	this->dig_gpo_sel1.address[0] = 0x0b; ///< Address=11
	this->dig_gpo_sel1.msb[0] = 7;
	this->dig_gpo_sel1.lsb[0] = 4;

	//Initialization for register dig_gpo_sel2
	this->dig_gpo_sel2.address[0] = 0x0b; ///< Address=11
	this->dig_gpo_sel2.msb[0] = 13;
	this->dig_gpo_sel2.lsb[0] = 10;

	//Initialization for register dis_ovl_gating
	this->dis_ovl_gating.address[0] = 0x11; ///< Address=17
	this->dis_ovl_gating.msb[0] = 15;
	this->dis_ovl_gating.lsb[0] = 15;

	//Initialization for register phase_out
	this->phase_out.address[0] = 0x08; ///< Address=8
	this->phase_out.msb[0] = 15;
	this->phase_out.lsb[0] = 0;

	//Initialization for register phase_overflow
	this->phase_overflow.address[0] = 0x08; ///< Address=8
	this->phase_overflow.msb[0] = 16;
	this->phase_overflow.lsb[0] = 16;

	//Initialization for register hdr_mode
	this->hdr_mode.address[0] = 0x08; ///< Address=8
	this->hdr_mode.msb[0] = 17;
	this->hdr_mode.lsb[0] = 17;

	//Initialization for register tx_channel
	this->tx_channel.address[0] = 0x08; ///< Address=8
	this->tx_channel.msb[0] = 19;
	this->tx_channel.lsb[0] = 18;

	//Initialization for register frame_status
	this->frame_status.address[0] = 0x08; ///< Address=8
	this->frame_status.msb[0] = 20;
	this->frame_status.lsb[0] = 20;

	//Initialization for register mod_freq
	this->mod_freq.address[0] = 0x08; ///< Address=8
	this->mod_freq.msb[0] = 21;
	this->mod_freq.lsb[0] = 21;

	//Initialization for register frame_count0
	this->frame_count0.address[0] = 0x08; ///< Address=8
	this->frame_count0.msb[0] = 23;
	this->frame_count0.lsb[0] = 23;

	//Initialization for register amp_out
	this->amp_out.address[0] = 0x09; ///< Address=9
	this->amp_out.msb[0] = 15;
	this->amp_out.lsb[0] = 0;

	//Initialization for register frame_count1
	this->frame_count1.address[0] = 0x09; ///< Address=9
	this->frame_count1.msb[0] = 17;
	this->frame_count1.lsb[0] = 16;

	//Initialization for register sig_ovl_flag
	this->sig_ovl_flag.address[0] = 0x09; ///< Address=9
	this->sig_ovl_flag.msb[0] = 18;
	this->sig_ovl_flag.lsb[0] = 18;

	//Initialization for register dealias_bin
	this->dealias_bin.address[0] = 0x09; ///< Address=9
	this->dealias_bin.msb[0] = 23;
	this->dealias_bin.lsb[0] = 20;

	//Initialization for register frame_count2
	this->frame_count2.address[0] = 0x0a; ///< Address=10
	this->frame_count2.msb[0] = 1;
	this->frame_count2.lsb[0] = 0;

	//Initialization for register amb_data
	this->amb_data.address[0] = 0x0a; ///< Address=10
	this->amb_data.msb[0] = 11;
	this->amb_data.lsb[0] = 2;

	//Initialization for register tmain
	this->tmain.address[0] = 0x0a; ///< Address=10
	this->tmain.msb[0] = 23;
	this->tmain.lsb[0] = 12;

	//Initialization for register amplitude_min_thr
	this->amplitude_min_thr.address[1] = 0x10; ///< Address=16
	this->amplitude_min_thr.msb[1] = 23;
	this->amplitude_min_thr.lsb[1] = 16;
	this->amplitude_min_thr.address[0] = 0x11; ///< Address=17
	this->amplitude_min_thr.msb[0] = 23;
	this->amplitude_min_thr.lsb[0] = 16;

	//Initialization for register amb_ovl_flag
	this->amb_ovl_flag.address[0] = 0x08; ///< Address=8
	this->amb_ovl_flag.msb[0] = 22;
	this->amb_ovl_flag.lsb[0] = 22;

	//Initialization for register phase_overflow_f2
	this->phase_overflow_f2.address[0] = 0x09; ///< Address=9
	this->phase_overflow_f2.msb[0] = 19;
	this->phase_overflow_f2.lsb[0] = 19;

	//Initialization for register ref_count_limit
	this->ref_count_limit.address[0] = 0x0f; ///< Address=15
	this->ref_count_limit.msb[0] = 14;
	this->ref_count_limit.lsb[0] = 0;

	//Initialization for register start_freq_calib
	this->start_freq_calib.address[0] = 0x0f; ///< Address=15
	this->start_freq_calib.msb[0] = 16;
	this->start_freq_calib.lsb[0] = 16;

	//Initialization for register sys_clk_divider
	this->sys_clk_divider.address[0] = 0x0f; ///< Address=15
	this->sys_clk_divider.msb[0] = 20;
	this->sys_clk_divider.lsb[0] = 17;

	//Initialization for register freq_count_read_reg
	this->freq_count_read_reg.address[0] = 0x10; ///< Address=16
	this->freq_count_read_reg.msb[0] = 14;
	this->freq_count_read_reg.lsb[0] = 0;

	//Initialization for register freq_count_reg
	this->freq_count_reg.address[0] = 0x11; ///< Address=17
	this->freq_count_reg.msb[0] = 14;
	this->freq_count_reg.lsb[0] = 0;

	//Initialization for register en_auto_freq_count
	this->en_auto_freq_count.address[0] = 0x0f; ///< Address=15
	this->en_auto_freq_count.msb[0] = 21;
	this->en_auto_freq_count.lsb[0] = 21;

	//Initialization for register en_floop
	this->en_floop.address[0] = 0x0f; ///< Address=15
	this->en_floop.msb[0] = 22;
	this->en_floop.lsb[0] = 22;

	//Initialization for register en_freq_corr
	this->en_freq_corr.address[0] = 0x0f; ///< Address=15
	this->en_freq_corr.msb[0] = 23;
	this->en_freq_corr.lsb[0] = 23;

	//Initialization for register en_cont_fcalib
	this->en_cont_fcalib.address[0] = 0x10; ///< Address=16
	this->en_cont_fcalib.msb[0] = 15;
	this->en_cont_fcalib.lsb[0] = 15;

	//Initialization for register monoshot_bit
	this->monoshot_bit.address[0] = 0x00; ///< Address=0
	this->monoshot_bit.msb[0] = 23;
	this->monoshot_bit.lsb[0] = 23;

	//Initialization for register monoshot_mode
	this->monoshot_mode.address[0] = 0x27; ///< Address=39
	this->monoshot_mode.msb[0] = 1;
	this->monoshot_mode.lsb[0] = 0;

	//Initialization for register powerup_delay
	this->powerup_delay.address[0] = 0x26; ///< Address=38
	this->powerup_delay.msb[0] = 23;
	this->powerup_delay.lsb[0] = 10;

	//Initialization for register monoshot_numframe
	this->monoshot_numframe.address[0] = 0x27; ///< Address=39
	this->monoshot_numframe.msb[0] = 7;
	this->monoshot_numframe.lsb[0] = 2;

	//Initialization for register monoshot_fz_clkcnt
	this->monoshot_fz_clkcnt.address[0] = 0x27; ///< Address=39
	this->monoshot_fz_clkcnt.msb[0] = 23;
	this->monoshot_fz_clkcnt.lsb[0] = 8;

	//Initialization for register en_tx_switch
	this->en_tx_switch.address[0] = 0x2a; ///< Address=42
	this->en_tx_switch.msb[0] = 0;
	this->en_tx_switch.lsb[0] = 0;

	//Initialization for register sel_tx_ch
	this->sel_tx_ch.address[0] = 0x2a; ///< Address=42
	this->sel_tx_ch.msb[0] = 2;
	this->sel_tx_ch.lsb[0] = 1;

	//Initialization for register tx_seq_reg
	this->tx_seq_reg.address[0] = 0x2a; ///< Address=42
	this->tx_seq_reg.msb[0] = 14;
	this->tx_seq_reg.lsb[0] = 3;

	//Initialization for register en_adaptive_hdr
	this->en_adaptive_hdr.address[0] = 0x2a; ///< Address=42
	this->en_adaptive_hdr.msb[0] = 15;
	this->en_adaptive_hdr.lsb[0] = 15;

	//Initialization for register sel_hdr_mode
	this->sel_hdr_mode.address[0] = 0x2a; ///< Address=42
	this->sel_hdr_mode.msb[0] = 16;
	this->sel_hdr_mode.lsb[0] = 16;

	//Initialization for register hdr_thr_low
	this->hdr_thr_low.address[0] = 0x2c; ///< Address=44
	this->hdr_thr_low.msb[0] = 15;
	this->hdr_thr_low.lsb[0] = 0;

	//Initialization for register hdr_thr_high
	this->hdr_thr_high.address[0] = 0x2b; ///< Address=43
	this->hdr_thr_high.msb[0] = 15;
	this->hdr_thr_high.lsb[0] = 0;

	//Initialization for register illum_scale_l_tx0
	this->illum_scale_l_tx0.address[0] = 0x2b; ///< Address=43
	this->illum_scale_l_tx0.msb[0] = 18;
	this->illum_scale_l_tx0.lsb[0] = 16;

	//Initialization for register illum_dac_l_tx0
	this->illum_dac_l_tx0.address[0] = 0x29; ///< Address=41
	this->illum_dac_l_tx0.msb[0] = 4;
	this->illum_dac_l_tx0.lsb[0] = 0;

	//Initialization for register illum_scale_h_tx0
	this->illum_scale_h_tx0.address[0] = 0x2b; ///< Address=43
	this->illum_scale_h_tx0.msb[0] = 21;
	this->illum_scale_h_tx0.lsb[0] = 19;

	//Initialization for register illum_dac_h_tx0
	this->illum_dac_h_tx0.address[0] = 0x29; ///< Address=41
	this->illum_dac_h_tx0.msb[0] = 9;
	this->illum_dac_h_tx0.lsb[0] = 5;

	//Initialization for register illum_scale_l_tx1
	this->illum_scale_l_tx1.address[0] = 0x2c; ///< Address=44
	this->illum_scale_l_tx1.msb[0] = 18;
	this->illum_scale_l_tx1.lsb[0] = 16;

	//Initialization for register illum_dac_l_tx1
	this->illum_dac_l_tx1.address[0] = 0x29; ///< Address=41
	this->illum_dac_l_tx1.msb[0] = 14;
	this->illum_dac_l_tx1.lsb[0] = 10;

	//Initialization for register illum_scale_h_tx1
	this->illum_scale_h_tx1.address[0] = 0x2c; ///< Address=44
	this->illum_scale_h_tx1.msb[0] = 21;
	this->illum_scale_h_tx1.lsb[0] = 19;

	//Initialization for register illum_dac_h_tx1
	this->illum_dac_h_tx1.address[0] = 0x29; ///< Address=41
	this->illum_dac_h_tx1.msb[0] = 19;
	this->illum_dac_h_tx1.lsb[0] = 15;

	//Initialization for register illum_scale_l_tx2
	this->illum_scale_l_tx2.address[0] = 0xb9; ///< Address=185
	this->illum_scale_l_tx2.msb[0] = 20;
	this->illum_scale_l_tx2.lsb[0] = 18;

	//Initialization for register illum_dac_l_tx2
	this->illum_dac_l_tx2.address[1] = 0x29; ///< Address=41
	this->illum_dac_l_tx2.msb[1] = 23;
	this->illum_dac_l_tx2.lsb[1] = 20;
	this->illum_dac_l_tx2.address[0] = 0x2a; ///< Address=42
	this->illum_dac_l_tx2.msb[0] = 23;
	this->illum_dac_l_tx2.lsb[0] = 23;

	//Initialization for register illum_scale_h_tx2
	this->illum_scale_h_tx2.address[0] = 0xb9; ///< Address=185
	this->illum_scale_h_tx2.msb[0] = 23;
	this->illum_scale_h_tx2.lsb[0] = 21;

	//Initialization for register illum_dac_h_tx2
	this->illum_dac_h_tx2.address[0] = 0x2a; ///< Address=42
	this->illum_dac_h_tx2.msb[0] = 22;
	this->illum_dac_h_tx2.lsb[0] = 18;

	//Initialization for register amb_adc_in_tx0
	this->amb_adc_in_tx0.address[0] = 0xb9; ///< Address=185
	this->amb_adc_in_tx0.msb[0] = 13;
	this->amb_adc_in_tx0.lsb[0] = 12;

	//Initialization for register amb_adc_in_tx1
	this->amb_adc_in_tx1.address[0] = 0xb9; ///< Address=185
	this->amb_adc_in_tx1.msb[0] = 15;
	this->amb_adc_in_tx1.lsb[0] = 14;

	//Initialization for register amb_adc_in_tx2
	this->amb_adc_in_tx2.address[0] = 0xb9; ///< Address=185
	this->amb_adc_in_tx2.msb[0] = 17;
	this->amb_adc_in_tx2.lsb[0] = 16;

	//Initialization for register give_dealias_data
	this->give_dealias_data.address[0] = 0xb8; ///< Address=184
	this->give_dealias_data.msb[0] = 20;
	this->give_dealias_data.lsb[0] = 20;

	//Initialization for register en_dealias_meas
	this->en_dealias_meas.address[0] = 0x40; ///< Address=64
	this->en_dealias_meas.msb[0] = 0;
	this->en_dealias_meas.lsb[0] = 0;

	//Initialization for register ncr_config
	this->ncr_config.address[0] = 0x40; ///< Address=64
	this->ncr_config.msb[0] = 21;
	this->ncr_config.lsb[0] = 21;

	//Initialization for register alpha0_dealias_scale
	this->alpha0_dealias_scale.address[0] = 0x40; ///< Address=64
	this->alpha0_dealias_scale.msb[0] = 14;
	this->alpha0_dealias_scale.lsb[0] = 9;

	//Initialization for register beta0_dealias_scale
	this->beta0_dealias_scale.address[0] = 0x40; ///< Address=64
	this->beta0_dealias_scale.msb[0] = 20;
	this->beta0_dealias_scale.lsb[0] = 15;

	//Initialization for register alpha1_dealias_scale
	this->alpha1_dealias_scale.address[0] = 0x41; ///< Address=65
	this->alpha1_dealias_scale.msb[0] = 5;
	this->alpha1_dealias_scale.lsb[0] = 0;

	//Initialization for register beta1_dealias_scale
	this->beta1_dealias_scale.address[0] = 0x41; ///< Address=65
	this->beta1_dealias_scale.msb[0] = 11;
	this->beta1_dealias_scale.lsb[0] = 6;

	//Initialization for register en_multi_freq_phase
	this->en_multi_freq_phase.address[0] = 0x40; ///< Address=64
	this->en_multi_freq_phase.msb[0] = 22;
	this->en_multi_freq_phase.lsb[0] = 22;

	//Initialization for register temp_avg_main
	this->temp_avg_main.address[0] = 0x03; ///< Address=3
	this->temp_avg_main.msb[0] = 23;
	this->temp_avg_main.lsb[0] = 22;

	//Initialization for register dis_ovl_for_hdr_meth1
	this->dis_ovl_for_hdr_meth1.address[0] = 0xb8; ///< Address=184
	this->dis_ovl_for_hdr_meth1.msb[0] = 21;
	this->dis_ovl_for_hdr_meth1.lsb[0] = 21;

	//Initialization for register en_ovl_for_hdr_meth2
	this->en_ovl_for_hdr_meth2.address[0] = 0xb8; ///< Address=184
	this->en_ovl_for_hdr_meth2.msb[0] = 22;
	this->en_ovl_for_hdr_meth2.lsb[0] = 22;

	//Initialization for register en_tx1_on_tx0
	this->en_tx1_on_tx0.address[0] = 0xb9; ///< Address=185
	this->en_tx1_on_tx0.msb[0] = 10;
	this->en_tx1_on_tx0.lsb[0] = 10;

	//Initialization for register en_tx2_on_tx0
	this->en_tx2_on_tx0.address[0] = 0xb9; ///< Address=185
	this->en_tx2_on_tx0.msb[0] = 11;
	this->en_tx2_on_tx0.lsb[0] = 11;

	//Initialization for register clip_mode_fc
	this->clip_mode_fc.address[0] = 0x50; ///< Address=80
	this->clip_mode_fc.msb[0] = 0;
	this->clip_mode_fc.lsb[0] = 0;

	//Initialization for register clip_mode_nl
	this->clip_mode_nl.address[0] = 0x50; ///< Address=80
	this->clip_mode_nl.msb[0] = 1;
	this->clip_mode_nl.lsb[0] = 1;

	//Initialization for register clip_mode_temp
	this->clip_mode_temp.address[0] = 0x50; ///< Address=80
	this->clip_mode_temp.msb[0] = 2;
	this->clip_mode_temp.lsb[0] = 2;

	//Initialization for register clip_mode_offset
	this->clip_mode_offset.address[0] = 0x50; ///< Address=80
	this->clip_mode_offset.msb[0] = 3;
	this->clip_mode_offset.lsb[0] = 3;

	//Initialization for register disable_syncing
	this->disable_syncing.address[0] = 0x50; ///< Address=80
	this->disable_syncing.msb[0] = 21;
	this->disable_syncing.lsb[0] = 21;

	//Initialization for register force_en_slave
	this->force_en_slave.address[0] = 0x00; ///< Address=0
	this->force_en_slave.msb[0] = 22;
	this->force_en_slave.lsb[0] = 22;

	//Initialization for register force_en_bypass
	this->force_en_bypass.address[0] = 0x00; ///< Address=0
	this->force_en_bypass.msb[0] = 21;
	this->force_en_bypass.lsb[0] = 21;

	//Initialization for register override_clkgen_reg
	this->override_clkgen_reg.address[0] = 0x50; ///< Address=80
	this->override_clkgen_reg.msb[0] = 22;
	this->override_clkgen_reg.lsb[0] = 22;

	//Initialization for register software_reset
	this->software_reset.address[0] = 0x00; ///< Address=0
	this->software_reset.msb[0] = 0;
	this->software_reset.lsb[0] = 0;

	//Initialization for register dis_tg_aconf
	this->dis_tg_aconf.address[0] = 0x80; ///< Address=128
	this->dis_tg_aconf.msb[0] = 23;
	this->dis_tg_aconf.lsb[0] = 23;

	//Initialization for register capture_clk_cnt
	this->capture_clk_cnt.address[0] = 0xa0; ///< Address=160
	this->capture_clk_cnt.msb[0] = 15;
	this->capture_clk_cnt.lsb[0] = 0;

	//Initialization for register tg_en
	this->tg_en.address[0] = 0x80; ///< Address=128
	this->tg_en.msb[0] = 0;
	this->tg_en.lsb[0] = 0;

	//Initialization for register num_sub_frames
	this->num_sub_frames.address[0] = 0x9f; ///< Address=159
	this->num_sub_frames.msb[0] = 11;
	this->num_sub_frames.lsb[0] = 0;

	//Initialization for register num_avg_sub_frames
	this->num_avg_sub_frames.address[0] = 0x9f; ///< Address=159
	this->num_avg_sub_frames.msb[0] = 23;
	this->num_avg_sub_frames.lsb[0] = 12;

	//Initialization for register sub_vd_clk_cnt
	this->sub_vd_clk_cnt.address[0] = 0x80; ///< Address=128
	this->sub_vd_clk_cnt.msb[0] = 16;
	this->sub_vd_clk_cnt.lsb[0] = 1;

	//Initialization for register tg_illumen_start
	this->tg_illumen_start.address[0] = 0x8f; ///< Address=143
	this->tg_illumen_start.msb[0] = 15;
	this->tg_illumen_start.lsb[0] = 0;

	//Initialization for register tg_illumen_end
	this->tg_illumen_end.address[0] = 0x90; ///< Address=144
	this->tg_illumen_end.msb[0] = 15;
	this->tg_illumen_end.lsb[0] = 0;

	//Initialization for register tg_illumen_mask_start
	this->tg_illumen_mask_start.address[0] = 0x9c; ///< Address=156
	this->tg_illumen_mask_start.msb[0] = 11;
	this->tg_illumen_mask_start.lsb[0] = 0;

	//Initialization for register tg_illumen_mask_end
	this->tg_illumen_mask_end.address[0] = 0x9c; ///< Address=156
	this->tg_illumen_mask_end.msb[0] = 23;
	this->tg_illumen_mask_end.lsb[0] = 12;

	//Initialization for register tg_afe_rst_start
	this->tg_afe_rst_start.address[0] = 0x83; ///< Address=131
	this->tg_afe_rst_start.msb[0] = 15;
	this->tg_afe_rst_start.lsb[0] = 0;

	//Initialization for register tg_afe_rst_end
	this->tg_afe_rst_end.address[0] = 0x84; ///< Address=132
	this->tg_afe_rst_end.msb[0] = 15;
	this->tg_afe_rst_end.lsb[0] = 0;

	//Initialization for register tg_seq_int_start
	this->tg_seq_int_start.address[0] = 0x85; ///< Address=133
	this->tg_seq_int_start.msb[0] = 15;
	this->tg_seq_int_start.lsb[0] = 0;

	//Initialization for register tg_seq_int_end
	this->tg_seq_int_end.address[0] = 0x86; ///< Address=134
	this->tg_seq_int_end.msb[0] = 15;
	this->tg_seq_int_end.lsb[0] = 0;

	//Initialization for register tg_capture_start
	this->tg_capture_start.address[0] = 0x87; ///< Address=135
	this->tg_capture_start.msb[0] = 15;
	this->tg_capture_start.lsb[0] = 0;

	//Initialization for register tg_capture_end
	this->tg_capture_end.address[0] = 0x88; ///< Address=136
	this->tg_capture_end.msb[0] = 15;
	this->tg_capture_end.lsb[0] = 0;

	//Initialization for register tg_ovl_window_start
	this->tg_ovl_window_start.address[0] = 0x89; ///< Address=137
	this->tg_ovl_window_start.msb[0] = 15;
	this->tg_ovl_window_start.lsb[0] = 0;

	//Initialization for register tg_ovl_window_end
	this->tg_ovl_window_end.address[0] = 0x8a; ///< Address=138
	this->tg_ovl_window_end.msb[0] = 15;
	this->tg_ovl_window_end.lsb[0] = 0;

	//Initialization for register tg_calc_start
	this->tg_calc_start.address[0] = 0x91; ///< Address=145
	this->tg_calc_start.msb[0] = 15;
	this->tg_calc_start.lsb[0] = 0;

	//Initialization for register tg_calc_end
	this->tg_calc_end.address[0] = 0x92; ///< Address=146
	this->tg_calc_end.msb[0] = 15;
	this->tg_calc_end.lsb[0] = 0;

	//Initialization for register tg_dynpdn_start
	this->tg_dynpdn_start.address[0] = 0x93; ///< Address=147
	this->tg_dynpdn_start.msb[0] = 15;
	this->tg_dynpdn_start.lsb[0] = 0;

	//Initialization for register tg_dynpdn_end
	this->tg_dynpdn_end.address[0] = 0x94; ///< Address=148
	this->tg_dynpdn_end.msb[0] = 15;
	this->tg_dynpdn_end.lsb[0] = 0;

	//Initialization for register tg_seq_int_mask_start
	this->tg_seq_int_mask_start.address[0] = 0x97; ///< Address=151
	this->tg_seq_int_mask_start.msb[0] = 11;
	this->tg_seq_int_mask_start.lsb[0] = 0;

	//Initialization for register tg_seq_int_mask_end
	this->tg_seq_int_mask_end.address[0] = 0x97; ///< Address=151
	this->tg_seq_int_mask_end.msb[0] = 23;
	this->tg_seq_int_mask_end.lsb[0] = 12;

	//Initialization for register tg_capture_mask_start
	this->tg_capture_mask_start.address[0] = 0x98; ///< Address=152
	this->tg_capture_mask_start.msb[0] = 11;
	this->tg_capture_mask_start.lsb[0] = 0;

	//Initialization for register tg_capture_mask_end
	this->tg_capture_mask_end.address[0] = 0x98; ///< Address=152
	this->tg_capture_mask_end.msb[0] = 23;
	this->tg_capture_mask_end.lsb[0] = 12;

	//Initialization for register tg_ovl_window_mask_start
	this->tg_ovl_window_mask_start.address[0] = 0x99; ///< Address=153
	this->tg_ovl_window_mask_start.msb[0] = 11;
	this->tg_ovl_window_mask_start.lsb[0] = 0;

	//Initialization for register tg_ovl_window_mask_end
	this->tg_ovl_window_mask_end.address[0] = 0x99; ///< Address=153
	this->tg_ovl_window_mask_end.msb[0] = 23;
	this->tg_ovl_window_mask_end.lsb[0] = 12;

	//Initialization for register tg_calc_mask_start
	this->tg_calc_mask_start.address[0] = 0x9d; ///< Address=157
	this->tg_calc_mask_start.msb[0] = 11;
	this->tg_calc_mask_start.lsb[0] = 0;

	//Initialization for register tg_calc_mask_end
	this->tg_calc_mask_end.address[0] = 0x9d; ///< Address=157
	this->tg_calc_mask_end.msb[0] = 23;
	this->tg_calc_mask_end.lsb[0] = 12;

	//Initialization for register tg_dynpdn_mask_start
	this->tg_dynpdn_mask_start.address[0] = 0x9e; ///< Address=158
	this->tg_dynpdn_mask_start.msb[0] = 11;
	this->tg_dynpdn_mask_start.lsb[0] = 0;

	//Initialization for register tg_dynpdn_mask_end
	this->tg_dynpdn_mask_end.address[0] = 0x9e; ///< Address=158
	this->tg_dynpdn_mask_end.msb[0] = 23;
	this->tg_dynpdn_mask_end.lsb[0] = 12;

	//Initialization for register en_sequencer
	this->en_sequencer.address[0] = 0x14; ///< Address=20
	this->en_sequencer.msb[0] = 16;
	this->en_sequencer.lsb[0] = 16;

	//Initialization for register en_processor_values
	this->en_processor_values.address[0] = 0x14; ///< Address=20
	this->en_processor_values.msb[0] = 17;
	this->en_processor_values.lsb[0] = 17;

	//Initialization for register status_in_reg
	this->status_in_reg.address[0] = 0x14; ///< Address=20
	this->status_in_reg.msb[0] = 18;
	this->status_in_reg.lsb[0] = 18;

	//Initialization for register mux_sel_compin
	this->mux_sel_compin.address[0] = 0x13; ///< Address=19
	this->mux_sel_compin.msb[0] = 2;
	this->mux_sel_compin.lsb[0] = 0;

	//Initialization for register compare_reg1
	this->compare_reg1.address[0] = 0x13; ///< Address=19
	this->compare_reg1.msb[0] = 18;
	this->compare_reg1.lsb[0] = 3;

	//Initialization for register compare_reg2
	this->compare_reg2.address[0] = 0x14; ///< Address=20
	this->compare_reg2.msb[0] = 15;
	this->compare_reg2.lsb[0] = 0;

	//Initialization for register dis_interrupt
	this->dis_interrupt.address[0] = 0x14; ///< Address=20
	this->dis_interrupt.msb[0] = 19;
	this->dis_interrupt.lsb[0] = 19;

	//Initialization for register command0
	this->command0.address[0] = 0x15; ///< Address=21
	this->command0.msb[0] = 11;
	this->command0.lsb[0] = 0;

	//Initialization for register command1
	this->command1.address[0] = 0x15; ///< Address=21
	this->command1.msb[0] = 23;
	this->command1.lsb[0] = 12;

	//Initialization for register command2
	this->command2.address[0] = 0x16; ///< Address=22
	this->command2.msb[0] = 11;
	this->command2.lsb[0] = 0;

	//Initialization for register command3
	this->command3.address[0] = 0x16; ///< Address=22
	this->command3.msb[0] = 23;
	this->command3.lsb[0] = 12;

	//Initialization for register command4
	this->command4.address[0] = 0x17; ///< Address=23
	this->command4.msb[0] = 11;
	this->command4.lsb[0] = 0;

	//Initialization for register command5
	this->command5.address[0] = 0x17; ///< Address=23
	this->command5.msb[0] = 23;
	this->command5.lsb[0] = 12;

	//Initialization for register command6
	this->command6.address[0] = 0x18; ///< Address=24
	this->command6.msb[0] = 11;
	this->command6.lsb[0] = 0;

	//Initialization for register command7
	this->command7.address[0] = 0x18; ///< Address=24
	this->command7.msb[0] = 23;
	this->command7.lsb[0] = 12;

	//Initialization for register command8
	this->command8.address[0] = 0x19; ///< Address=25
	this->command8.msb[0] = 11;
	this->command8.lsb[0] = 0;

	//Initialization for register command9
	this->command9.address[0] = 0x19; ///< Address=25
	this->command9.msb[0] = 23;
	this->command9.lsb[0] = 12;

	//Initialization for register command10
	this->command10.address[0] = 0x1a; ///< Address=26
	this->command10.msb[0] = 11;
	this->command10.lsb[0] = 0;

	//Initialization for register command11
	this->command11.address[0] = 0x1a; ///< Address=26
	this->command11.msb[0] = 23;
	this->command11.lsb[0] = 12;

	//Initialization for register command12
	this->command12.address[0] = 0x1b; ///< Address=27
	this->command12.msb[0] = 11;
	this->command12.lsb[0] = 0;

	//Initialization for register command13
	this->command13.address[0] = 0x1b; ///< Address=27
	this->command13.msb[0] = 23;
	this->command13.lsb[0] = 12;

	//Initialization for register command14
	this->command14.address[0] = 0x1c; ///< Address=28
	this->command14.msb[0] = 11;
	this->command14.lsb[0] = 0;

	//Initialization for register command15
	this->command15.address[0] = 0x1c; ///< Address=28
	this->command15.msb[0] = 23;
	this->command15.lsb[0] = 12;

	//Initialization for register command16
	this->command16.address[0] = 0x1d; ///< Address=29
	this->command16.msb[0] = 11;
	this->command16.lsb[0] = 0;

	//Initialization for register command17
	this->command17.address[0] = 0x1d; ///< Address=29
	this->command17.msb[0] = 23;
	this->command17.lsb[0] = 12;

	//Initialization for register command18
	this->command18.address[0] = 0x1e; ///< Address=30
	this->command18.msb[0] = 11;
	this->command18.lsb[0] = 0;

	//Initialization for register command19
	this->command19.address[0] = 0x1e; ///< Address=30
	this->command19.msb[0] = 23;
	this->command19.lsb[0] = 12;

	//Initialization for register force_scale_val
	this->force_scale_val.address[0] = 0x2e; ///< Address=46
	this->force_scale_val.msb[0] = 2;
	this->force_scale_val.lsb[0] = 0;

	//Initialization for register dis_auto_scale
	this->dis_auto_scale.address[0] = 0x2e; ///< Address=46
	this->dis_auto_scale.msb[0] = 3;
	this->dis_auto_scale.lsb[0] = 3;

	//Initialization for register disable_conf_rescale
	this->disable_conf_rescale.address[0] = 0x2e; ///< Address=46
	this->disable_conf_rescale.msb[0] = 13;
	this->disable_conf_rescale.lsb[0] = 13;

	//Initialization for register int_xtalk_calib
	this->int_xtalk_calib.address[0] = 0x2e; ///< Address=46
	this->int_xtalk_calib.msb[0] = 4;
	this->int_xtalk_calib.lsb[0] = 4;

	//Initialization for register xtalk_filt_time_const
	this->xtalk_filt_time_const.address[0] = 0x2e; ///< Address=46
	this->xtalk_filt_time_const.msb[0] = 23;
	this->xtalk_filt_time_const.lsb[0] = 20;

	//Initialization for register use_xtalk_filt_int
	this->use_xtalk_filt_int.address[0] = 0x2e; ///< Address=46
	this->use_xtalk_filt_int.msb[0] = 5;
	this->use_xtalk_filt_int.lsb[0] = 5;

	//Initialization for register use_xtalk_reg_int
	this->use_xtalk_reg_int.address[0] = 0x2e; ///< Address=46
	this->use_xtalk_reg_int.msb[0] = 6;
	this->use_xtalk_reg_int.lsb[0] = 6;

	//Initialization for register iq_read_data_sel
	this->iq_read_data_sel.address[0] = 0x2e; ///< Address=46
	this->iq_read_data_sel.msb[0] = 11;
	this->iq_read_data_sel.lsb[0] = 9;

	//Initialization for register iphase_xtalk
	this->iphase_xtalk.address[0] = 0x3b; ///< Address=59
	this->iphase_xtalk.msb[0] = 23;
	this->iphase_xtalk.lsb[0] = 0;

	//Initialization for register qphase_xtalk
	this->qphase_xtalk.address[0] = 0x3c; ///< Address=60
	this->qphase_xtalk.msb[0] = 23;
	this->qphase_xtalk.lsb[0] = 0;

	//Initialization for register int_xtalk_reg_scale
	this->int_xtalk_reg_scale.address[0] = 0x2e; ///< Address=46
	this->int_xtalk_reg_scale.msb[0] = 16;
	this->int_xtalk_reg_scale.lsb[0] = 14;

	//Initialization for register iphase_xtalk_int_reg
	this->iphase_xtalk_int_reg.address[0] = 0x3d; ///< Address=61
	this->iphase_xtalk_int_reg.msb[0] = 15;
	this->iphase_xtalk_int_reg.lsb[0] = 0;

	//Initialization for register qphase_xtalk_int_reg
	this->qphase_xtalk_int_reg.address[0] = 0x3e; ///< Address=62
	this->qphase_xtalk_int_reg.msb[0] = 15;
	this->qphase_xtalk_int_reg.lsb[0] = 0;

	//Initialization for register illum_xtalk_calib
	this->illum_xtalk_calib.address[0] = 0x2e; ///< Address=46
	this->illum_xtalk_calib.msb[0] = 12;
	this->illum_xtalk_calib.lsb[0] = 12;

	//Initialization for register illum_xtalk_reg_scale
	this->illum_xtalk_reg_scale.address[0] = 0x2e; ///< Address=46
	this->illum_xtalk_reg_scale.msb[0] = 19;
	this->illum_xtalk_reg_scale.lsb[0] = 17;

	//Initialization for register use_xtalk_filt_illum
	this->use_xtalk_filt_illum.address[0] = 0x2e; ///< Address=46
	this->use_xtalk_filt_illum.msb[0] = 7;
	this->use_xtalk_filt_illum.lsb[0] = 7;

	//Initialization for register use_xtalk_reg_illum
	this->use_xtalk_reg_illum.address[0] = 0x2e; ///< Address=46
	this->use_xtalk_reg_illum.msb[0] = 8;
	this->use_xtalk_reg_illum.lsb[0] = 8;

	//Initialization for register iphase_xtalk_reg_hdr0_tx0
	this->iphase_xtalk_reg_hdr0_tx0.address[0] = 0x2f; ///< Address=47
	this->iphase_xtalk_reg_hdr0_tx0.msb[0] = 15;
	this->iphase_xtalk_reg_hdr0_tx0.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr0_tx0
	this->qphase_xtalk_reg_hdr0_tx0.address[0] = 0x30; ///< Address=48
	this->qphase_xtalk_reg_hdr0_tx0.msb[0] = 15;
	this->qphase_xtalk_reg_hdr0_tx0.lsb[0] = 0;

	//Initialization for register iphase_xtalk_reg_hdr1_tx0
	this->iphase_xtalk_reg_hdr1_tx0.address[0] = 0x31; ///< Address=49
	this->iphase_xtalk_reg_hdr1_tx0.msb[0] = 15;
	this->iphase_xtalk_reg_hdr1_tx0.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr1_tx0
	this->qphase_xtalk_reg_hdr1_tx0.address[0] = 0x32; ///< Address=50
	this->qphase_xtalk_reg_hdr1_tx0.msb[0] = 15;
	this->qphase_xtalk_reg_hdr1_tx0.lsb[0] = 0;

	//Initialization for register iphase_xtalk_reg_hdr0_tx1
	this->iphase_xtalk_reg_hdr0_tx1.address[0] = 0x33; ///< Address=51
	this->iphase_xtalk_reg_hdr0_tx1.msb[0] = 15;
	this->iphase_xtalk_reg_hdr0_tx1.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr0_tx1
	this->qphase_xtalk_reg_hdr0_tx1.address[0] = 0x34; ///< Address=52
	this->qphase_xtalk_reg_hdr0_tx1.msb[0] = 15;
	this->qphase_xtalk_reg_hdr0_tx1.lsb[0] = 0;

	//Initialization for register iphase_xtalk_reg_hdr1_tx1
	this->iphase_xtalk_reg_hdr1_tx1.address[0] = 0x35; ///< Address=53
	this->iphase_xtalk_reg_hdr1_tx1.msb[0] = 15;
	this->iphase_xtalk_reg_hdr1_tx1.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr1_tx1
	this->qphase_xtalk_reg_hdr1_tx1.address[0] = 0x36; ///< Address=54
	this->qphase_xtalk_reg_hdr1_tx1.msb[0] = 15;
	this->qphase_xtalk_reg_hdr1_tx1.lsb[0] = 0;

	//Initialization for register iphase_xtalk_reg_hdr0_tx2
	this->iphase_xtalk_reg_hdr0_tx2.address[0] = 0x37; ///< Address=55
	this->iphase_xtalk_reg_hdr0_tx2.msb[0] = 15;
	this->iphase_xtalk_reg_hdr0_tx2.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr0_tx2
	this->qphase_xtalk_reg_hdr0_tx2.address[0] = 0x38; ///< Address=56
	this->qphase_xtalk_reg_hdr0_tx2.msb[0] = 15;
	this->qphase_xtalk_reg_hdr0_tx2.lsb[0] = 0;

	//Initialization for register iphase_xtalk_reg_hdr1_tx2
	this->iphase_xtalk_reg_hdr1_tx2.address[0] = 0x39; ///< Address=57
	this->iphase_xtalk_reg_hdr1_tx2.msb[0] = 15;
	this->iphase_xtalk_reg_hdr1_tx2.lsb[0] = 0;

	//Initialization for register qphase_xtalk_reg_hdr1_tx2
	this->qphase_xtalk_reg_hdr1_tx2.address[0] = 0x3a; ///< Address=58
	this->qphase_xtalk_reg_hdr1_tx2.msb[0] = 15;
	this->qphase_xtalk_reg_hdr1_tx2.lsb[0] = 0;

	//Initialization for register en_temp_xtalk_corr
	this->en_temp_xtalk_corr.address[0] = 0x3a; ///< Address=58
	this->en_temp_xtalk_corr.msb[0] = 16;
	this->en_temp_xtalk_corr.lsb[0] = 16;

	//Initialization for register scale_temp_coeff_xtalk
	this->scale_temp_coeff_xtalk.address[0] = 0x3a; ///< Address=58
	this->scale_temp_coeff_xtalk.msb[0] = 19;
	this->scale_temp_coeff_xtalk.lsb[0] = 17;

	//Initialization for register temp_coeff_xtalk_iphase_hdr0_tx0
	this->temp_coeff_xtalk_iphase_hdr0_tx0.address[0] = 0x38; ///< Address=56
	this->temp_coeff_xtalk_iphase_hdr0_tx0.msb[0] = 23;
	this->temp_coeff_xtalk_iphase_hdr0_tx0.lsb[0] = 16;

	//Initialization for register temp_coeff_xtalk_qphase_hdr0_tx0
	this->temp_coeff_xtalk_qphase_hdr0_tx0.address[0] = 0x39; ///< Address=57
	this->temp_coeff_xtalk_qphase_hdr0_tx0.msb[0] = 23;
	this->temp_coeff_xtalk_qphase_hdr0_tx0.lsb[0] = 16;

	//Initialization for register temp_coeff_xtalk_iphase_hdr1_tx0
	this->temp_coeff_xtalk_iphase_hdr1_tx0.address[0] = 0x5e; ///< Address=94
	this->temp_coeff_xtalk_iphase_hdr1_tx0.msb[0] = 15;
	this->temp_coeff_xtalk_iphase_hdr1_tx0.lsb[0] = 8;

	//Initialization for register temp_coeff_xtalk_qphase_hdr1_tx0
	this->temp_coeff_xtalk_qphase_hdr1_tx0.address[0] = 0x60; ///< Address=96
	this->temp_coeff_xtalk_qphase_hdr1_tx0.msb[0] = 7;
	this->temp_coeff_xtalk_qphase_hdr1_tx0.lsb[0] = 0;

	//Initialization for register temp_coeff_xtalk_iphase_hdr0_tx1
	this->temp_coeff_xtalk_iphase_hdr0_tx1.address[0] = 0x5e; ///< Address=94
	this->temp_coeff_xtalk_iphase_hdr0_tx1.msb[0] = 23;
	this->temp_coeff_xtalk_iphase_hdr0_tx1.lsb[0] = 16;

	//Initialization for register temp_coeff_xtalk_qphase_hdr0_tx1
	this->temp_coeff_xtalk_qphase_hdr0_tx1.address[0] = 0x60; ///< Address=96
	this->temp_coeff_xtalk_qphase_hdr0_tx1.msb[0] = 15;
	this->temp_coeff_xtalk_qphase_hdr0_tx1.lsb[0] = 8;

	//Initialization for register temp_coeff_xtalk_iphase_hdr1_tx1
	this->temp_coeff_xtalk_iphase_hdr1_tx1.address[0] = 0x5f; ///< Address=95
	this->temp_coeff_xtalk_iphase_hdr1_tx1.msb[0] = 7;
	this->temp_coeff_xtalk_iphase_hdr1_tx1.lsb[0] = 0;

	//Initialization for register temp_coeff_xtalk_qphase_hdr1_tx1
	this->temp_coeff_xtalk_qphase_hdr1_tx1.address[0] = 0x60; ///< Address=96
	this->temp_coeff_xtalk_qphase_hdr1_tx1.msb[0] = 23;
	this->temp_coeff_xtalk_qphase_hdr1_tx1.lsb[0] = 16;

	//Initialization for register temp_coeff_xtalk_iphase_hdr0_tx2
	this->temp_coeff_xtalk_iphase_hdr0_tx2.address[0] = 0x5f; ///< Address=95
	this->temp_coeff_xtalk_iphase_hdr0_tx2.msb[0] = 15;
	this->temp_coeff_xtalk_iphase_hdr0_tx2.lsb[0] = 8;

	//Initialization for register temp_coeff_xtalk_qphase_hdr0_tx2
	this->temp_coeff_xtalk_qphase_hdr0_tx2.address[0] = 0x61; ///< Address=97
	this->temp_coeff_xtalk_qphase_hdr0_tx2.msb[0] = 7;
	this->temp_coeff_xtalk_qphase_hdr0_tx2.lsb[0] = 0;

	//Initialization for register temp_coeff_xtalk_iphase_hdr1_tx2
	this->temp_coeff_xtalk_iphase_hdr1_tx2.address[0] = 0x5f; ///< Address=95
	this->temp_coeff_xtalk_iphase_hdr1_tx2.msb[0] = 23;
	this->temp_coeff_xtalk_iphase_hdr1_tx2.lsb[0] = 16;

	//Initialization for register temp_coeff_xtalk_qphase_hdr1_tx2
	this->temp_coeff_xtalk_qphase_hdr1_tx2.address[0] = 0x61; ///< Address=97
	this->temp_coeff_xtalk_qphase_hdr1_tx2.msb[0] = 15;
	this->temp_coeff_xtalk_qphase_hdr1_tx2.lsb[0] = 8;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr0_tx0
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx0.address[0] = 0x36; ///< Address=54
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx0.msb[0] = 23;
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx0.lsb[0] = 16;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr0_tx0
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx0.address[0] = 0x37; ///< Address=55
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx0.msb[0] = 23;
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx0.lsb[0] = 16;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr1_tx0
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx0.address[0] = 0x5b; ///< Address=91
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx0.msb[0] = 7;
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx0.lsb[0] = 0;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr0_tx1
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx1.address[0] = 0x5b; ///< Address=91
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx1.msb[0] = 15;
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx1.lsb[0] = 8;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr1_tx1
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx1.address[0] = 0x5b; ///< Address=91
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx1.msb[0] = 23;
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx1.lsb[0] = 16;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr0_tx2
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx2.address[0] = 0x5c; ///< Address=92
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx2.msb[0] = 7;
	this->temp_coeff_illum_xtalk_iphase_hdr0_tx2.lsb[0] = 0;

	//Initialization for register temp_coeff_illum_xtalk_iphase_hdr1_tx2
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx2.address[0] = 0x5c; ///< Address=92
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx2.msb[0] = 15;
	this->temp_coeff_illum_xtalk_iphase_hdr1_tx2.lsb[0] = 8;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr1_tx0
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx0.address[0] = 0x5c; ///< Address=92
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx0.msb[0] = 23;
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx0.lsb[0] = 16;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr0_tx1
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx1.address[0] = 0x5d; ///< Address=93
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx1.msb[0] = 7;
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx1.lsb[0] = 0;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr1_tx1
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx1.address[0] = 0x5d; ///< Address=93
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx1.msb[0] = 15;
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx1.lsb[0] = 8;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr0_tx2
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx2.address[0] = 0x5d; ///< Address=93
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx2.msb[0] = 23;
	this->temp_coeff_illum_xtalk_qphase_hdr0_tx2.lsb[0] = 16;

	//Initialization for register temp_coeff_illum_xtalk_qphase_hdr1_tx2
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx2.address[0] = 0x5e; ///< Address=94
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx2.msb[0] = 7;
	this->temp_coeff_illum_xtalk_qphase_hdr1_tx2.lsb[0] = 0;

	//Initialization for register amb_xtalk_qphase_coeff
	this->amb_xtalk_qphase_coeff.address[0] = 0x0c; ///< Address=12
	this->amb_xtalk_qphase_coeff.msb[0] = 15;
	this->amb_xtalk_qphase_coeff.lsb[0] = 8;

	//Initialization for register amb_xtalk_iphase_coeff
	this->amb_xtalk_iphase_coeff.address[0] = 0x0c; ///< Address=12
	this->amb_xtalk_iphase_coeff.msb[0] = 7;
	this->amb_xtalk_iphase_coeff.lsb[0] = 0;

	//Initialization for register scale_amb_coeff_xtalk
	this->scale_amb_coeff_xtalk.address[0] = 0x3a; ///< Address=58
	this->scale_amb_coeff_xtalk.msb[0] = 22;
	this->scale_amb_coeff_xtalk.lsb[0] = 20;

	//Initialization for register en_phase_corr
	this->en_phase_corr.address[0] = 0x43; ///< Address=67
	this->en_phase_corr.msb[0] = 0;
	this->en_phase_corr.lsb[0] = 0;

	//Initialization for register phase_offset_hdr0_tx0
	this->phase_offset_hdr0_tx0.address[0] = 0x42; ///< Address=66
	this->phase_offset_hdr0_tx0.msb[0] = 15;
	this->phase_offset_hdr0_tx0.lsb[0] = 0;

	//Initialization for register phase_offset_hdr1_tx0
	this->phase_offset_hdr1_tx0.address[0] = 0x51; ///< Address=81
	this->phase_offset_hdr1_tx0.msb[0] = 15;
	this->phase_offset_hdr1_tx0.lsb[0] = 0;

	//Initialization for register phase_offset_hdr0_tx1
	this->phase_offset_hdr0_tx1.address[0] = 0x52; ///< Address=82
	this->phase_offset_hdr0_tx1.msb[0] = 15;
	this->phase_offset_hdr0_tx1.lsb[0] = 0;

	//Initialization for register phase_offset_hdr1_tx1
	this->phase_offset_hdr1_tx1.address[0] = 0x53; ///< Address=83
	this->phase_offset_hdr1_tx1.msb[0] = 15;
	this->phase_offset_hdr1_tx1.lsb[0] = 0;

	//Initialization for register phase_offset_hdr0_tx2
	this->phase_offset_hdr0_tx2.address[0] = 0x54; ///< Address=84
	this->phase_offset_hdr0_tx2.msb[0] = 15;
	this->phase_offset_hdr0_tx2.lsb[0] = 0;

	//Initialization for register phase_offset_hdr1_tx2
	this->phase_offset_hdr1_tx2.address[0] = 0x55; ///< Address=85
	this->phase_offset_hdr1_tx2.msb[0] = 15;
	this->phase_offset_hdr1_tx2.lsb[0] = 0;

	//Initialization for register reverse_phase_before_offset
	this->reverse_phase_before_offset.address[0] = 0x43; ///< Address=67
	this->reverse_phase_before_offset.msb[0] = 9;
	this->reverse_phase_before_offset.lsb[0] = 9;

	//Initialization for register phase2_offset_hdr0_tx0
	this->phase2_offset_hdr0_tx0.address[0] = 0x44; ///< Address=68
	this->phase2_offset_hdr0_tx0.msb[0] = 15;
	this->phase2_offset_hdr0_tx0.lsb[0] = 0;

	//Initialization for register phase2_offset_hdr1_tx0
	this->phase2_offset_hdr1_tx0.address[0] = 0x56; ///< Address=86
	this->phase2_offset_hdr1_tx0.msb[0] = 15;
	this->phase2_offset_hdr1_tx0.lsb[0] = 0;

	//Initialization for register phase2_offset_hdr0_tx1
	this->phase2_offset_hdr0_tx1.address[0] = 0x57; ///< Address=87
	this->phase2_offset_hdr0_tx1.msb[0] = 15;
	this->phase2_offset_hdr0_tx1.lsb[0] = 0;

	//Initialization for register phase2_offset_hdr1_tx1
	this->phase2_offset_hdr1_tx1.address[0] = 0x58; ///< Address=88
	this->phase2_offset_hdr1_tx1.msb[0] = 15;
	this->phase2_offset_hdr1_tx1.lsb[0] = 0;

	//Initialization for register phase2_offset_hdr0_tx2
	this->phase2_offset_hdr0_tx2.address[0] = 0x59; ///< Address=89
	this->phase2_offset_hdr0_tx2.msb[0] = 15;
	this->phase2_offset_hdr0_tx2.lsb[0] = 0;

	//Initialization for register phase2_offset_hdr1_tx2
	this->phase2_offset_hdr1_tx2.address[0] = 0x5a; ///< Address=90
	this->phase2_offset_hdr1_tx2.msb[0] = 15;
	this->phase2_offset_hdr1_tx2.lsb[0] = 0;

	//Initialization for register en_temp_corr
	this->en_temp_corr.address[0] = 0x43; ///< Address=67
	this->en_temp_corr.msb[0] = 1;
	this->en_temp_corr.lsb[0] = 1;

	//Initialization for register scale_phase_temp_coeff
	this->scale_phase_temp_coeff.address[0] = 0x43; ///< Address=67
	this->scale_phase_temp_coeff.msb[0] = 8;
	this->scale_phase_temp_coeff.lsb[0] = 6;

	//Initialization for register tmain_calib_hdr0_tx0
	this->tmain_calib_hdr0_tx0.address[0] = 0x47; ///< Address=71
	this->tmain_calib_hdr0_tx0.msb[0] = 11;
	this->tmain_calib_hdr0_tx0.lsb[0] = 0;

	//Initialization for register temp_coeff_main_hdr0_tx0
	this->temp_coeff_main_hdr0_tx0.address[0] = 0x45; ///< Address=69
	this->temp_coeff_main_hdr0_tx0.msb[0] = 11;
	this->temp_coeff_main_hdr0_tx0.lsb[0] = 0;

	//Initialization for register tmain_calib_hdr1_tx0
	this->tmain_calib_hdr1_tx0.address[0] = 0x48; ///< Address=72
	this->tmain_calib_hdr1_tx0.msb[0] = 11;
	this->tmain_calib_hdr1_tx0.lsb[0] = 0;

	//Initialization for register temp_coeff_main_hdr1_tx0
	this->temp_coeff_main_hdr1_tx0.address[0] = 0x2d; ///< Address=45
	this->temp_coeff_main_hdr1_tx0.msb[0] = 11;
	this->temp_coeff_main_hdr1_tx0.lsb[0] = 0;

	//Initialization for register tmain_calib_hdr0_tx1
	this->tmain_calib_hdr0_tx1.address[0] = 0x49; ///< Address=73
	this->tmain_calib_hdr0_tx1.msb[0] = 11;
	this->tmain_calib_hdr0_tx1.lsb[0] = 0;

	//Initialization for register temp_coeff_main_hdr0_tx1
	this->temp_coeff_main_hdr0_tx1.address[0] = 0x2d; ///< Address=45
	this->temp_coeff_main_hdr0_tx1.msb[0] = 23;
	this->temp_coeff_main_hdr0_tx1.lsb[0] = 12;

	//Initialization for register tmain_calib_hdr1_tx1
	this->tmain_calib_hdr1_tx1.address[0] = 0x41; ///< Address=65
	this->tmain_calib_hdr1_tx1.msb[0] = 23;
	this->tmain_calib_hdr1_tx1.lsb[0] = 12;

	//Initialization for register temp_coeff_main_hdr1_tx1
	this->temp_coeff_main_hdr1_tx1.address[1] = 0x2f; ///< Address=47
	this->temp_coeff_main_hdr1_tx1.msb[1] = 23;
	this->temp_coeff_main_hdr1_tx1.lsb[1] = 16;
	this->temp_coeff_main_hdr1_tx1.address[0] = 0x30; ///< Address=48
	this->temp_coeff_main_hdr1_tx1.msb[0] = 23;
	this->temp_coeff_main_hdr1_tx1.lsb[0] = 20;

	//Initialization for register tmain_calib_hdr0_tx2
	this->tmain_calib_hdr0_tx2.address[0] = 0x3f; ///< Address=63
	this->tmain_calib_hdr0_tx2.msb[0] = 11;
	this->tmain_calib_hdr0_tx2.lsb[0] = 0;

	//Initialization for register temp_coeff_main_hdr0_tx2
	this->temp_coeff_main_hdr0_tx2.address[1] = 0x31; ///< Address=49
	this->temp_coeff_main_hdr0_tx2.msb[1] = 23;
	this->temp_coeff_main_hdr0_tx2.lsb[1] = 16;
	this->temp_coeff_main_hdr0_tx2.address[0] = 0x32; ///< Address=50
	this->temp_coeff_main_hdr0_tx2.msb[0] = 23;
	this->temp_coeff_main_hdr0_tx2.lsb[0] = 20;

	//Initialization for register tmain_calib_hdr1_tx2
	this->tmain_calib_hdr1_tx2.address[0] = 0x45; ///< Address=69
	this->tmain_calib_hdr1_tx2.msb[0] = 23;
	this->tmain_calib_hdr1_tx2.lsb[0] = 12;

	//Initialization for register temp_coeff_main_hdr1_tx2
	this->temp_coeff_main_hdr1_tx2.address[1] = 0x33; ///< Address=51
	this->temp_coeff_main_hdr1_tx2.msb[1] = 23;
	this->temp_coeff_main_hdr1_tx2.lsb[1] = 16;
	this->temp_coeff_main_hdr1_tx2.address[0] = 0x34; ///< Address=52
	this->temp_coeff_main_hdr1_tx2.msb[0] = 23;
	this->temp_coeff_main_hdr1_tx2.lsb[0] = 20;

	//Initialization for register tillum_calib_hdr0_tx0
	this->tillum_calib_hdr0_tx0.address[0] = 0x47; ///< Address=71
	this->tillum_calib_hdr0_tx0.msb[0] = 23;
	this->tillum_calib_hdr0_tx0.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr0_tx0
	this->temp_coeff_illum_hdr0_tx0.address[0] = 0x46; ///< Address=70
	this->temp_coeff_illum_hdr0_tx0.msb[0] = 11;
	this->temp_coeff_illum_hdr0_tx0.lsb[0] = 0;

	//Initialization for register tillum_calib_hdr1_tx0
	this->tillum_calib_hdr1_tx0.address[0] = 0x48; ///< Address=72
	this->tillum_calib_hdr1_tx0.msb[0] = 23;
	this->tillum_calib_hdr1_tx0.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr1_tx0
	this->temp_coeff_illum_hdr1_tx0.address[1] = 0x51; ///< Address=81
	this->temp_coeff_illum_hdr1_tx0.msb[1] = 23;
	this->temp_coeff_illum_hdr1_tx0.lsb[1] = 16;
	this->temp_coeff_illum_hdr1_tx0.address[0] = 0x52; ///< Address=82
	this->temp_coeff_illum_hdr1_tx0.msb[0] = 23;
	this->temp_coeff_illum_hdr1_tx0.lsb[0] = 20;

	//Initialization for register tillum_calib_hdr0_tx1
	this->tillum_calib_hdr0_tx1.address[0] = 0x49; ///< Address=73
	this->tillum_calib_hdr0_tx1.msb[0] = 23;
	this->tillum_calib_hdr0_tx1.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr0_tx1
	this->temp_coeff_illum_hdr0_tx1.address[1] = 0x53; ///< Address=83
	this->temp_coeff_illum_hdr0_tx1.msb[1] = 23;
	this->temp_coeff_illum_hdr0_tx1.lsb[1] = 16;
	this->temp_coeff_illum_hdr0_tx1.address[0] = 0x54; ///< Address=84
	this->temp_coeff_illum_hdr0_tx1.msb[0] = 23;
	this->temp_coeff_illum_hdr0_tx1.lsb[0] = 20;

	//Initialization for register tillum_calib_hdr1_tx1
	this->tillum_calib_hdr1_tx1.address[0] = 0x43; ///< Address=67
	this->tillum_calib_hdr1_tx1.msb[0] = 23;
	this->tillum_calib_hdr1_tx1.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr1_tx1
	this->temp_coeff_illum_hdr1_tx1.address[1] = 0x55; ///< Address=85
	this->temp_coeff_illum_hdr1_tx1.msb[1] = 23;
	this->temp_coeff_illum_hdr1_tx1.lsb[1] = 16;
	this->temp_coeff_illum_hdr1_tx1.address[0] = 0x56; ///< Address=86
	this->temp_coeff_illum_hdr1_tx1.msb[0] = 23;
	this->temp_coeff_illum_hdr1_tx1.lsb[0] = 20;

	//Initialization for register tillum_calib_hdr0_tx2
	this->tillum_calib_hdr0_tx2.address[0] = 0x3f; ///< Address=63
	this->tillum_calib_hdr0_tx2.msb[0] = 23;
	this->tillum_calib_hdr0_tx2.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr0_tx2
	this->temp_coeff_illum_hdr0_tx2.address[1] = 0x57; ///< Address=87
	this->temp_coeff_illum_hdr0_tx2.msb[1] = 23;
	this->temp_coeff_illum_hdr0_tx2.lsb[1] = 16;
	this->temp_coeff_illum_hdr0_tx2.address[0] = 0x58; ///< Address=88
	this->temp_coeff_illum_hdr0_tx2.msb[0] = 23;
	this->temp_coeff_illum_hdr0_tx2.lsb[0] = 20;

	//Initialization for register tillum_calib_hdr1_tx2
	this->tillum_calib_hdr1_tx2.address[0] = 0x46; ///< Address=70
	this->tillum_calib_hdr1_tx2.msb[0] = 23;
	this->tillum_calib_hdr1_tx2.lsb[0] = 12;

	//Initialization for register temp_coeff_illum_hdr1_tx2
	this->temp_coeff_illum_hdr1_tx2.address[1] = 0x59; ///< Address=89
	this->temp_coeff_illum_hdr1_tx2.msb[1] = 23;
	this->temp_coeff_illum_hdr1_tx2.lsb[1] = 16;
	this->temp_coeff_illum_hdr1_tx2.address[0] = 0x5a; ///< Address=90
	this->temp_coeff_illum_hdr1_tx2.msb[0] = 23;
	this->temp_coeff_illum_hdr1_tx2.lsb[0] = 20;

	//Initialization for register amb_sat_thr
	this->amb_sat_thr.address[0] = 0x0d; ///< Address=13
	this->amb_sat_thr.msb[0] = 16;
	this->amb_sat_thr.lsb[0] = 7;

	//Initialization for register amb_calib
	this->amb_calib.address[0] = 0x0b; ///< Address=11
	this->amb_calib.msb[0] = 23;
	this->amb_calib.lsb[0] = 14;

	//Initialization for register amb_phase_corr_pwl_coeff0
	this->amb_phase_corr_pwl_coeff0.address[0] = 0x0c; ///< Address=12
	this->amb_phase_corr_pwl_coeff0.msb[0] = 23;
	this->amb_phase_corr_pwl_coeff0.lsb[0] = 16;

	//Initialization for register amb_phase_corr_pwl_x0
	this->amb_phase_corr_pwl_x0.address[0] = 0xb8; ///< Address=184
	this->amb_phase_corr_pwl_x0.msb[0] = 9;
	this->amb_phase_corr_pwl_x0.lsb[0] = 0;

	//Initialization for register amb_phase_corr_pwl_x1
	this->amb_phase_corr_pwl_x1.address[0] = 0xb8; ///< Address=184
	this->amb_phase_corr_pwl_x1.msb[0] = 19;
	this->amb_phase_corr_pwl_x1.lsb[0] = 10;

	//Initialization for register amb_phase_corr_pwl_x2
	this->amb_phase_corr_pwl_x2.address[0] = 0xb9; ///< Address=185
	this->amb_phase_corr_pwl_x2.msb[0] = 9;
	this->amb_phase_corr_pwl_x2.lsb[0] = 0;

	//Initialization for register amb_phase_corr_pwl_coeff1
	this->amb_phase_corr_pwl_coeff1.address[0] = 0xb4; ///< Address=180
	this->amb_phase_corr_pwl_coeff1.msb[0] = 7;
	this->amb_phase_corr_pwl_coeff1.lsb[0] = 0;

	//Initialization for register amb_phase_corr_pwl_coeff2
	this->amb_phase_corr_pwl_coeff2.address[0] = 0xb4; ///< Address=180
	this->amb_phase_corr_pwl_coeff2.msb[0] = 15;
	this->amb_phase_corr_pwl_coeff2.lsb[0] = 8;

	//Initialization for register amb_phase_corr_pwl_coeff3
	this->amb_phase_corr_pwl_coeff3.address[0] = 0xb4; ///< Address=180
	this->amb_phase_corr_pwl_coeff3.msb[0] = 23;
	this->amb_phase_corr_pwl_coeff3.lsb[0] = 16;

	//Initialization for register scale_amb_phase_corr_coeff
	this->scale_amb_phase_corr_coeff.address[0] = 0xb5; ///< Address=181
	this->scale_amb_phase_corr_coeff.msb[0] = 2;
	this->scale_amb_phase_corr_coeff.lsb[0] = 0;

	//Initialization for register temp_coeff_illum_square_hdr0
	this->temp_coeff_illum_square_hdr0.address[0] = 0xb6; ///< Address=182
	this->temp_coeff_illum_square_hdr0.msb[0] = 7;
	this->temp_coeff_illum_square_hdr0.lsb[0] = 0;

	//Initialization for register temp_coeff_illum_square_hdr1
	this->temp_coeff_illum_square_hdr1.address[0] = 0xb6; ///< Address=182
	this->temp_coeff_illum_square_hdr1.msb[0] = 15;
	this->temp_coeff_illum_square_hdr1.lsb[0] = 8;

	//Initialization for register temp_coeff_main_square_hdr0
	this->temp_coeff_main_square_hdr0.address[0] = 0xb7; ///< Address=183
	this->temp_coeff_main_square_hdr0.msb[0] = 7;
	this->temp_coeff_main_square_hdr0.lsb[0] = 0;

	//Initialization for register temp_coeff_main_square_hdr1
	this->temp_coeff_main_square_hdr1.address[0] = 0xb7; ///< Address=183
	this->temp_coeff_main_square_hdr1.msb[0] = 15;
	this->temp_coeff_main_square_hdr1.lsb[0] = 8;

	//Initialization for register scale_phase_temp_corr_square
	this->scale_phase_temp_corr_square.address[0] = 0xb5; ///< Address=181
	this->scale_phase_temp_corr_square.msb[0] = 5;
	this->scale_phase_temp_corr_square.lsb[0] = 3;

	//Initialization for register en_nl_corr
	this->en_nl_corr.address[0] = 0x4a; ///< Address=74
	this->en_nl_corr.msb[0] = 0;
	this->en_nl_corr.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr0_tx0
	this->a1_coeff_hdr0_tx0.address[0] = 0x4b; ///< Address=75
	this->a1_coeff_hdr0_tx0.msb[0] = 15;
	this->a1_coeff_hdr0_tx0.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr0_tx0
	this->a2_coeff_hdr0_tx0.address[0] = 0x4c; ///< Address=76
	this->a2_coeff_hdr0_tx0.msb[0] = 15;
	this->a2_coeff_hdr0_tx0.lsb[0] = 0;

	//Initialization for register a3_coeff_hdr0_tx0
	this->a3_coeff_hdr0_tx0.address[0] = 0x4d; ///< Address=77
	this->a3_coeff_hdr0_tx0.msb[0] = 15;
	this->a3_coeff_hdr0_tx0.lsb[0] = 0;

	//Initialization for register a4_coeff_hdr0_tx0
	this->a4_coeff_hdr0_tx0.address[0] = 0x4e; ///< Address=78
	this->a4_coeff_hdr0_tx0.msb[0] = 15;
	this->a4_coeff_hdr0_tx0.lsb[0] = 0;

	//Initialization for register scale_nl_corr_coeff
	this->scale_nl_corr_coeff.address[0] = 0x4a; ///< Address=74
	this->scale_nl_corr_coeff.msb[0] = 19;
	this->scale_nl_corr_coeff.lsb[0] = 18;

	//Initialization for register a0_coeff_hdr0_tx0
	this->a0_coeff_hdr0_tx0.address[0] = 0x4a; ///< Address=74
	this->a0_coeff_hdr0_tx0.msb[0] = 17;
	this->a0_coeff_hdr0_tx0.lsb[0] = 2;

	//Initialization for register a0_coeff_hdr1_tx0
	this->a0_coeff_hdr1_tx0.address[0] = 0xa2; ///< Address=162
	this->a0_coeff_hdr1_tx0.msb[0] = 15;
	this->a0_coeff_hdr1_tx0.lsb[0] = 0;

	//Initialization for register a0_coeff_hdr0_tx1
	this->a0_coeff_hdr0_tx1.address[0] = 0xa3; ///< Address=163
	this->a0_coeff_hdr0_tx1.msb[0] = 15;
	this->a0_coeff_hdr0_tx1.lsb[0] = 0;

	//Initialization for register a0_coeff_hdr1_tx1
	this->a0_coeff_hdr1_tx1.address[0] = 0xa4; ///< Address=164
	this->a0_coeff_hdr1_tx1.msb[0] = 15;
	this->a0_coeff_hdr1_tx1.lsb[0] = 0;

	//Initialization for register a0_coeff_hdr0_tx2
	this->a0_coeff_hdr0_tx2.address[0] = 0xa5; ///< Address=165
	this->a0_coeff_hdr0_tx2.msb[0] = 15;
	this->a0_coeff_hdr0_tx2.lsb[0] = 0;

	//Initialization for register a0_coeff_hdr1_tx2
	this->a0_coeff_hdr1_tx2.address[0] = 0xa6; ///< Address=166
	this->a0_coeff_hdr1_tx2.msb[0] = 15;
	this->a0_coeff_hdr1_tx2.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr1_tx0
	this->a1_coeff_hdr1_tx0.address[0] = 0xa7; ///< Address=167
	this->a1_coeff_hdr1_tx0.msb[0] = 15;
	this->a1_coeff_hdr1_tx0.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr0_tx1
	this->a1_coeff_hdr0_tx1.address[0] = 0xa8; ///< Address=168
	this->a1_coeff_hdr0_tx1.msb[0] = 15;
	this->a1_coeff_hdr0_tx1.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr1_tx1
	this->a1_coeff_hdr1_tx1.address[0] = 0xa9; ///< Address=169
	this->a1_coeff_hdr1_tx1.msb[0] = 15;
	this->a1_coeff_hdr1_tx1.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr0_tx2
	this->a1_coeff_hdr0_tx2.address[0] = 0xaa; ///< Address=170
	this->a1_coeff_hdr0_tx2.msb[0] = 15;
	this->a1_coeff_hdr0_tx2.lsb[0] = 0;

	//Initialization for register a1_coeff_hdr1_tx2
	this->a1_coeff_hdr1_tx2.address[0] = 0xab; ///< Address=171
	this->a1_coeff_hdr1_tx2.msb[0] = 15;
	this->a1_coeff_hdr1_tx2.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr1_tx0
	this->a2_coeff_hdr1_tx0.address[0] = 0xac; ///< Address=172
	this->a2_coeff_hdr1_tx0.msb[0] = 15;
	this->a2_coeff_hdr1_tx0.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr0_tx1
	this->a2_coeff_hdr0_tx1.address[0] = 0xad; ///< Address=173
	this->a2_coeff_hdr0_tx1.msb[0] = 15;
	this->a2_coeff_hdr0_tx1.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr1_tx1
	this->a2_coeff_hdr1_tx1.address[0] = 0xae; ///< Address=174
	this->a2_coeff_hdr1_tx1.msb[0] = 15;
	this->a2_coeff_hdr1_tx1.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr0_tx2
	this->a2_coeff_hdr0_tx2.address[0] = 0xaf; ///< Address=175
	this->a2_coeff_hdr0_tx2.msb[0] = 15;
	this->a2_coeff_hdr0_tx2.lsb[0] = 0;

	//Initialization for register a2_coeff_hdr1_tx2
	this->a2_coeff_hdr1_tx2.address[0] = 0xb0; ///< Address=176
	this->a2_coeff_hdr1_tx2.msb[0] = 15;
	this->a2_coeff_hdr1_tx2.lsb[0] = 0;

	//Initialization for register a3_coeff_hdr1_tx0
	this->a3_coeff_hdr1_tx0.address[0] = 0xb1; ///< Address=177
	this->a3_coeff_hdr1_tx0.msb[0] = 15;
	this->a3_coeff_hdr1_tx0.lsb[0] = 0;

	//Initialization for register a3_coeff_hdr0_tx1
	this->a3_coeff_hdr0_tx1.address[1] = 0xa2; ///< Address=162
	this->a3_coeff_hdr0_tx1.msb[1] = 23;
	this->a3_coeff_hdr0_tx1.lsb[1] = 16;
	this->a3_coeff_hdr0_tx1.address[0] = 0xa3; ///< Address=163
	this->a3_coeff_hdr0_tx1.msb[0] = 23;
	this->a3_coeff_hdr0_tx1.lsb[0] = 16;

	//Initialization for register a3_coeff_hdr1_tx1
	this->a3_coeff_hdr1_tx1.address[1] = 0xa4; ///< Address=164
	this->a3_coeff_hdr1_tx1.msb[1] = 23;
	this->a3_coeff_hdr1_tx1.lsb[1] = 16;
	this->a3_coeff_hdr1_tx1.address[0] = 0xa5; ///< Address=165
	this->a3_coeff_hdr1_tx1.msb[0] = 23;
	this->a3_coeff_hdr1_tx1.lsb[0] = 16;

	//Initialization for register a3_coeff_hdr0_tx2
	this->a3_coeff_hdr0_tx2.address[1] = 0xa6; ///< Address=166
	this->a3_coeff_hdr0_tx2.msb[1] = 23;
	this->a3_coeff_hdr0_tx2.lsb[1] = 16;
	this->a3_coeff_hdr0_tx2.address[0] = 0xa7; ///< Address=167
	this->a3_coeff_hdr0_tx2.msb[0] = 23;
	this->a3_coeff_hdr0_tx2.lsb[0] = 16;

	//Initialization for register a3_coeff_hdr1_tx2
	this->a3_coeff_hdr1_tx2.address[1] = 0xa8; ///< Address=168
	this->a3_coeff_hdr1_tx2.msb[1] = 23;
	this->a3_coeff_hdr1_tx2.lsb[1] = 16;
	this->a3_coeff_hdr1_tx2.address[0] = 0xa9; ///< Address=169
	this->a3_coeff_hdr1_tx2.msb[0] = 23;
	this->a3_coeff_hdr1_tx2.lsb[0] = 16;

	//Initialization for register a4_coeff_hdr1_tx0
	this->a4_coeff_hdr1_tx0.address[1] = 0xaa; ///< Address=170
	this->a4_coeff_hdr1_tx0.msb[1] = 23;
	this->a4_coeff_hdr1_tx0.lsb[1] = 16;
	this->a4_coeff_hdr1_tx0.address[0] = 0xab; ///< Address=171
	this->a4_coeff_hdr1_tx0.msb[0] = 23;
	this->a4_coeff_hdr1_tx0.lsb[0] = 16;

	//Initialization for register a4_coeff_hdr0_tx1
	this->a4_coeff_hdr0_tx1.address[1] = 0xac; ///< Address=172
	this->a4_coeff_hdr0_tx1.msb[1] = 23;
	this->a4_coeff_hdr0_tx1.lsb[1] = 16;
	this->a4_coeff_hdr0_tx1.address[0] = 0xad; ///< Address=173
	this->a4_coeff_hdr0_tx1.msb[0] = 23;
	this->a4_coeff_hdr0_tx1.lsb[0] = 16;

	//Initialization for register a4_coeff_hdr1_tx1
	this->a4_coeff_hdr1_tx1.address[1] = 0xae; ///< Address=174
	this->a4_coeff_hdr1_tx1.msb[1] = 23;
	this->a4_coeff_hdr1_tx1.lsb[1] = 16;
	this->a4_coeff_hdr1_tx1.address[0] = 0xaf; ///< Address=175
	this->a4_coeff_hdr1_tx1.msb[0] = 23;
	this->a4_coeff_hdr1_tx1.lsb[0] = 16;

	//Initialization for register a4_coeff_hdr0_tx2
	this->a4_coeff_hdr0_tx2.address[1] = 0xb0; ///< Address=176
	this->a4_coeff_hdr0_tx2.msb[1] = 23;
	this->a4_coeff_hdr0_tx2.lsb[1] = 16;
	this->a4_coeff_hdr0_tx2.address[0] = 0xb1; ///< Address=177
	this->a4_coeff_hdr0_tx2.msb[0] = 23;
	this->a4_coeff_hdr0_tx2.lsb[0] = 16;

	//Initialization for register a4_coeff_hdr1_tx2
	this->a4_coeff_hdr1_tx2.address[0] = 0xb2; ///< Address=178
	this->a4_coeff_hdr1_tx2.msb[0] = 15;
	this->a4_coeff_hdr1_tx2.lsb[0] = 0;

	//Initialization for register tillum
	this->tillum.address[0] = 0x04; ///< Address=4
	this->tillum.msb[0] = 19;
	this->tillum.lsb[0] = 8;

	//Initialization for register tsens_slave0
	this->tsens_slave0.address[0] = 0x02; ///< Address=2
	this->tsens_slave0.msb[0] = 6;
	this->tsens_slave0.lsb[0] = 0;

	//Initialization for register tsens_slave1
	this->tsens_slave1.address[0] = 0x02; ///< Address=2
	this->tsens_slave1.msb[0] = 13;
	this->tsens_slave1.lsb[0] = 7;

	//Initialization for register tsens_slave2
	this->tsens_slave2.address[0] = 0x02; ///< Address=2
	this->tsens_slave2.msb[0] = 20;
	this->tsens_slave2.lsb[0] = 14;

	//Initialization for register config_tillum_msb
	this->config_tillum_msb.address[0] = 0x07; ///< Address=7
	this->config_tillum_msb.msb[0] = 23;
	this->config_tillum_msb.lsb[0] = 20;

	//Initialization for register en_tillum_12b
	this->en_tillum_12b.address[0] = 0x0d; ///< Address=13
	this->en_tillum_12b.msb[0] = 23;
	this->en_tillum_12b.lsb[0] = 23;

	//Initialization for register tillum_unsigned
	this->tillum_unsigned.address[0] = 0x04; ///< Address=4
	this->tillum_unsigned.msb[0] = 23;
	this->tillum_unsigned.lsb[0] = 23;

	//Initialization for register temp_avg_illum
	this->temp_avg_illum.address[0] = 0x02; ///< Address=2
	this->temp_avg_illum.msb[0] = 23;
	this->temp_avg_illum.lsb[0] = 22;

	//Initialization for register en_tsens_read_fvd
	this->en_tsens_read_fvd.address[0] = 0x03; ///< Address=3
	this->en_tsens_read_fvd.msb[0] = 18;
	this->en_tsens_read_fvd.lsb[0] = 18;

	//Initialization for register en_tillum_read
	this->en_tillum_read.address[0] = 0x02; ///< Address=2
	this->en_tillum_read.msb[0] = 21;
	this->en_tillum_read.lsb[0] = 21;

	//Initialization for register eeprom_read_trig
	this->eeprom_read_trig.address[0] = 0x01; ///< Address=1
	this->eeprom_read_trig.msb[0] = 0;
	this->eeprom_read_trig.lsb[0] = 0;

	//Initialization for register swap_read_data
	this->swap_read_data.address[0] = 0x01; ///< Address=1
	this->swap_read_data.msb[0] = 1;
	this->swap_read_data.lsb[0] = 1;

	//Initialization for register eeprom_start_reg_addr
	this->eeprom_start_reg_addr.address[0] = 0x01; ///< Address=1
	this->eeprom_start_reg_addr.msb[0] = 16;
	this->eeprom_start_reg_addr.lsb[0] = 9;

	//Initialization for register frame_vd_trig
	this->frame_vd_trig.address[0] = 0x01; ///< Address=1
	this->frame_vd_trig.msb[0] = 17;
	this->frame_vd_trig.lsb[0] = 17;

	//Initialization for register i2c_trig_reg
	this->i2c_trig_reg.address[0] = 0x01; ///< Address=1
	this->i2c_trig_reg.msb[0] = 18;
	this->i2c_trig_reg.lsb[0] = 18;

	//Initialization for register i2c_en
	this->i2c_en.address[0] = 0x01; ///< Address=1
	this->i2c_en.msb[0] = 19;
	this->i2c_en.lsb[0] = 19;

	//Initialization for register i2c_rw
	this->i2c_rw.address[0] = 0x01; ///< Address=1
	this->i2c_rw.msb[0] = 21;
	this->i2c_rw.lsb[0] = 20;

	//Initialization for register i2c_read_data
	this->i2c_read_data.address[0] = 0x03; ///< Address=3
	this->i2c_read_data.msb[0] = 7;
	this->i2c_read_data.lsb[0] = 0;

	//Initialization for register i2c_write_data1
	this->i2c_write_data1.address[0] = 0x03; ///< Address=3
	this->i2c_write_data1.msb[0] = 16;
	this->i2c_write_data1.lsb[0] = 9;

	//Initialization for register i2c_num_tran
	this->i2c_num_tran.address[0] = 0x03; ///< Address=3
	this->i2c_num_tran.msb[0] = 17;
	this->i2c_num_tran.lsb[0] = 17;

	//Initialization for register en_eeprom_read
	this->en_eeprom_read.address[0] = 0x01; ///< Address=1
	this->en_eeprom_read.msb[0] = 23;
	this->en_eeprom_read.lsb[0] = 23;

	//Initialization for register init_load_done
	this->init_load_done.address[0] = 0x03; ///< Address=3
	this->init_load_done.msb[0] = 8;
	this->init_load_done.lsb[0] = 8;

	//Initialization for register addr_slave_eeprom
	this->addr_slave_eeprom.address[0] = 0x01; ///< Address=1
	this->addr_slave_eeprom.msb[0] = 8;
	this->addr_slave_eeprom.lsb[0] = 2;

	//Initialization for register i2c_num_bytes_tran1
	this->i2c_num_bytes_tran1.address[0] = 0x07; ///< Address=7
	this->i2c_num_bytes_tran1.msb[0] = 17;
	this->i2c_num_bytes_tran1.lsb[0] = 16;

	//Initialization for register i2c_num_bytes_tran2
	this->i2c_num_bytes_tran2.address[0] = 0x05; ///< Address=5
	this->i2c_num_bytes_tran2.msb[0] = 23;
	this->i2c_num_bytes_tran2.lsb[0] = 22;

	//Initialization for register i2c_write_data2
	this->i2c_write_data2.address[0] = 0x07; ///< Address=7
	this->i2c_write_data2.msb[0] = 7;
	this->i2c_write_data2.lsb[0] = 0;

	//Initialization for register i2c_sel_read_bytes
	this->i2c_sel_read_bytes.address[0] = 0x07; ///< Address=7
	this->i2c_sel_read_bytes.msb[0] = 19;
	this->i2c_sel_read_bytes.lsb[0] = 18;

	//Initialization for register i2c_cont_rw
	this->i2c_cont_rw.address[0] = 0x00; ///< Address=0
	this->i2c_cont_rw.msb[0] = 6;
	this->i2c_cont_rw.lsb[0] = 6;

	//Initialization for register dis_ovldet
	this->dis_ovldet.address[0] = 0x65; ///< Address=101
	this->dis_ovldet.msb[0] = 23;
	this->dis_ovldet.lsb[0] = 23;

	//Initialization for register prog_ovldet_refp
	this->prog_ovldet_refp.address[0] = 0x64; ///< Address=100
	this->prog_ovldet_refp.msb[0] = 20;
	this->prog_ovldet_refp.lsb[0] = 18;

	//Initialization for register prog_ovldet_refm
	this->prog_ovldet_refm.address[0] = 0x64; ///< Address=100
	this->prog_ovldet_refm.msb[0] = 23;
	this->prog_ovldet_refm.lsb[0] = 21;

	//Initialization for register iamb_max_sel
	this->iamb_max_sel.address[0] = 0x72; ///< Address=114
	this->iamb_max_sel.msb[0] = 7;
	this->iamb_max_sel.lsb[0] = 4;

	//Initialization for register tm_vrefp_diode
	this->tm_vrefp_diode.address[0] = 0x6d; ///< Address=109
	this->tm_vrefp_diode.msb[0] = 2;
	this->tm_vrefp_diode.lsb[0] = 0;

	//Initialization for register tm_vrefm_diode
	this->tm_vrefm_diode.address[0] = 0x6d; ///< Address=109
	this->tm_vrefm_diode.msb[0] = 5;
	this->tm_vrefm_diode.lsb[0] = 3;

	//Initialization for register gpo1_mux_sel
	this->gpo1_mux_sel.address[0] = 0x78; ///< Address=120
	this->gpo1_mux_sel.msb[0] = 8;
	this->gpo1_mux_sel.lsb[0] = 6;

	//Initialization for register gpio1_obuf_en
	this->gpio1_obuf_en.address[0] = 0x78; ///< Address=120
	this->gpio1_obuf_en.msb[0] = 12;
	this->gpio1_obuf_en.lsb[0] = 12;

	//Initialization for register gpio1_ibuf_en
	this->gpio1_ibuf_en.address[0] = 0x78; ///< Address=120
	this->gpio1_ibuf_en.msb[0] = 13;
	this->gpio1_ibuf_en.lsb[0] = 13;

	//Initialization for register gpo2_mux_sel
	this->gpo2_mux_sel.address[0] = 0x78; ///< Address=120
	this->gpo2_mux_sel.msb[0] = 11;
	this->gpo2_mux_sel.lsb[0] = 9;

	//Initialization for register gpio2_obuf_en
	this->gpio2_obuf_en.address[0] = 0x78; ///< Address=120
	this->gpio2_obuf_en.msb[0] = 15;
	this->gpio2_obuf_en.lsb[0] = 15;

	//Initialization for register gpio2_ibuf_en
	this->gpio2_ibuf_en.address[0] = 0x78; ///< Address=120
	this->gpio2_ibuf_en.msb[0] = 16;
	this->gpio2_ibuf_en.lsb[0] = 16;

	//Initialization for register gpo3_mux_sel
	this->gpo3_mux_sel.address[0] = 0x78; ///< Address=120
	this->gpo3_mux_sel.msb[0] = 2;
	this->gpo3_mux_sel.lsb[0] = 0;

	//Initialization for register sel_gp3_on_sdam
	this->sel_gp3_on_sdam.address[0] = 0x78; ///< Address=120
	this->sel_gp3_on_sdam.msb[0] = 22;
	this->sel_gp3_on_sdam.lsb[0] = 22;

	//Initialization for register dealias_en
	this->dealias_en.address[0] = 0x71; ///< Address=113
	this->dealias_en.msb[0] = 1;
	this->dealias_en.lsb[0] = 1;

	//Initialization for register dealias_freq
	this->dealias_freq.address[0] = 0x71; ///< Address=113
	this->dealias_freq.msb[0] = 2;
	this->dealias_freq.lsb[0] = 2;

	//Initialization for register shift_illum_phase
	this->shift_illum_phase.address[0] = 0x71; ///< Address=113
	this->shift_illum_phase.msb[0] = 6;
	this->shift_illum_phase.lsb[0] = 3;

	//Initialization for register shut_clocks
	this->shut_clocks.address[0] = 0x71; ///< Address=113
	this->shut_clocks.msb[0] = 8;
	this->shut_clocks.lsb[0] = 8;

	//Initialization for register invert_tg_clk
	this->invert_tg_clk.address[0] = 0x71; ///< Address=113
	this->invert_tg_clk.msb[0] = 9;
	this->invert_tg_clk.lsb[0] = 9;

	//Initialization for register invert_afe_clk
	this->invert_afe_clk.address[0] = 0x71; ///< Address=113
	this->invert_afe_clk.msb[0] = 11;
	this->invert_afe_clk.lsb[0] = 11;

	//Initialization for register dis_illum_clk_tx
	this->dis_illum_clk_tx.address[0] = 0x71; ///< Address=113
	this->dis_illum_clk_tx.msb[0] = 12;
	this->dis_illum_clk_tx.lsb[0] = 12;

	//Initialization for register en_illum_clk_gpio
	this->en_illum_clk_gpio.address[0] = 0x71; ///< Address=113
	this->en_illum_clk_gpio.msb[0] = 16;
	this->en_illum_clk_gpio.lsb[0] = 16;

	//Initialization for register illum_clk_gpio_mode
	this->illum_clk_gpio_mode.address[0] = 0x71; ///< Address=113
	this->illum_clk_gpio_mode.msb[0] = 15;
	this->illum_clk_gpio_mode.lsb[0] = 15;

	//Initialization for register unmask_illumen_intxtalk
	this->unmask_illumen_intxtalk.address[0] = 0x71; ///< Address=113
	this->unmask_illumen_intxtalk.msb[0] = 17;
	this->unmask_illumen_intxtalk.lsb[0] = 17;

	//Initialization for register temp_offset
	this->temp_offset.address[0] = 0x6e; ///< Address=110
	this->temp_offset.msb[0] = 16;
	this->temp_offset.lsb[0] = 8;

	//Initialization for register en_temp_conv
	this->en_temp_conv.address[0] = 0x6e; ///< Address=110
	this->en_temp_conv.msb[0] = 19;
	this->en_temp_conv.lsb[0] = 19;

	//Initialization for register dis_glb_pd_refsys
	this->dis_glb_pd_refsys.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_refsys.msb[0] = 0;
	this->dis_glb_pd_refsys.lsb[0] = 0;

	//Initialization for register dis_glb_pd_temp_sens
	this->dis_glb_pd_temp_sens.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_temp_sens.msb[0] = 1;
	this->dis_glb_pd_temp_sens.lsb[0] = 1;

	//Initialization for register dis_glb_pd_illum_drv
	this->dis_glb_pd_illum_drv.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_illum_drv.msb[0] = 2;
	this->dis_glb_pd_illum_drv.lsb[0] = 2;

	//Initialization for register dis_glb_pd_afe
	this->dis_glb_pd_afe.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_afe.msb[0] = 3;
	this->dis_glb_pd_afe.lsb[0] = 3;

	//Initialization for register dis_glb_pd_afe_dac
	this->dis_glb_pd_afe_dac.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_afe_dac.msb[0] = 4;
	this->dis_glb_pd_afe_dac.lsb[0] = 4;

	//Initialization for register dis_glb_pd_amb_dac
	this->dis_glb_pd_amb_dac.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_amb_dac.msb[0] = 5;
	this->dis_glb_pd_amb_dac.lsb[0] = 5;

	//Initialization for register dis_glb_pd_amb_adc
	this->dis_glb_pd_amb_adc.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_amb_adc.msb[0] = 6;
	this->dis_glb_pd_amb_adc.lsb[0] = 6;

	//Initialization for register dis_glb_pd_test_curr
	this->dis_glb_pd_test_curr.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_test_curr.msb[0] = 7;
	this->dis_glb_pd_test_curr.lsb[0] = 7;

	//Initialization for register dis_glb_pd_osc
	this->dis_glb_pd_osc.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_osc.msb[0] = 8;
	this->dis_glb_pd_osc.lsb[0] = 8;

	//Initialization for register dis_glb_pd_i2chost
	this->dis_glb_pd_i2chost.address[0] = 0x76; ///< Address=118
	this->dis_glb_pd_i2chost.msb[0] = 9;
	this->dis_glb_pd_i2chost.lsb[0] = 9;

	//Initialization for register pdn_global
	this->pdn_global.address[0] = 0x76; ///< Address=118
	this->pdn_global.msb[0] = 11;
	this->pdn_global.lsb[0] = 11;

	//Initialization for register en_dyn_pd_refsys
	this->en_dyn_pd_refsys.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_refsys.msb[0] = 0;
	this->en_dyn_pd_refsys.lsb[0] = 0;

	//Initialization for register en_dyn_pd_temp_sens
	this->en_dyn_pd_temp_sens.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_temp_sens.msb[0] = 1;
	this->en_dyn_pd_temp_sens.lsb[0] = 1;

	//Initialization for register en_dyn_pd_illum_drv
	this->en_dyn_pd_illum_drv.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_illum_drv.msb[0] = 2;
	this->en_dyn_pd_illum_drv.lsb[0] = 2;

	//Initialization for register en_dyn_pd_afe
	this->en_dyn_pd_afe.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_afe.msb[0] = 3;
	this->en_dyn_pd_afe.lsb[0] = 3;

	//Initialization for register en_dyn_pd_afe_dac
	this->en_dyn_pd_afe_dac.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_afe_dac.msb[0] = 4;
	this->en_dyn_pd_afe_dac.lsb[0] = 4;

	//Initialization for register en_dyn_pd_amb_dac
	this->en_dyn_pd_amb_dac.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_amb_dac.msb[0] = 5;
	this->en_dyn_pd_amb_dac.lsb[0] = 5;

	//Initialization for register en_dyn_pd_amb_adc
	this->en_dyn_pd_amb_adc.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_amb_adc.msb[0] = 6;
	this->en_dyn_pd_amb_adc.lsb[0] = 6;

	//Initialization for register en_dyn_pd_test_curr
	this->en_dyn_pd_test_curr.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_test_curr.msb[0] = 7;
	this->en_dyn_pd_test_curr.lsb[0] = 7;

	//Initialization for register en_dyn_pd_osc
	this->en_dyn_pd_osc.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_osc.msb[0] = 8;
	this->en_dyn_pd_osc.lsb[0] = 8;

	//Initialization for register en_dyn_pd_i2chost_osc
	this->en_dyn_pd_i2chost_osc.address[0] = 0x77; ///< Address=119
	this->en_dyn_pd_i2chost_osc.msb[0] = 9;
	this->en_dyn_pd_i2chost_osc.lsb[0] = 9;

	//Initialization for register TX0_PIN_CONFIG
	this->TX0_PIN_CONFIG.address[0] = 0x7a; ///< Address=122
	this->TX0_PIN_CONFIG.msb[0] = 5;
	this->TX0_PIN_CONFIG.lsb[0] = 4;

	//Initialization for register TX1_PIN_CONFIG
	this->TX1_PIN_CONFIG.address[0] = 0x7a; ///< Address=122
	this->TX1_PIN_CONFIG.msb[0] = 1;
	this->TX1_PIN_CONFIG.lsb[0] = 0;

	//Initialization for register TX2_PIN_CONFIG
	this->TX2_PIN_CONFIG.address[0] = 0x7a; ///< Address=122
	this->TX2_PIN_CONFIG.msb[0] = 3;
	this->TX2_PIN_CONFIG.lsb[0] = 2;

	//Initialization for register EN_TX_CLKB
	this->EN_TX_CLKB.address[0] = 0x79; ///< Address=121
	this->EN_TX_CLKB.msb[0] = 0;
	this->EN_TX_CLKB.lsb[0] = 0;

	//Initialization for register EN_TX_CLKZ
	this->EN_TX_CLKZ.address[0] = 0x79; ///< Address=121
	this->EN_TX_CLKZ.msb[0] = 2;
	this->EN_TX_CLKZ.lsb[0] = 2;

	//Initialization for register sel_illum_tx0_on_tx1
	this->sel_illum_tx0_on_tx1.address[0] = 0x79; ///< Address=121
	this->sel_illum_tx0_on_tx1.msb[0] = 3;
	this->sel_illum_tx0_on_tx1.lsb[0] = 3;

	//Initialization for register ILLUM_DC_CURR_DAC
	this->ILLUM_DC_CURR_DAC.address[0] = 0x79; ///< Address=121
	this->ILLUM_DC_CURR_DAC.msb[0] = 11;
	this->ILLUM_DC_CURR_DAC.lsb[0] = 8;

	//Initialization for register PDN_ILLUM_DC_CURR
	this->PDN_ILLUM_DC_CURR.address[0] = 0x79; ///< Address=121
	this->PDN_ILLUM_DC_CURR.msb[0] = 12;
	this->PDN_ILLUM_DC_CURR.lsb[0] = 12;

	//Initialization for register FEEDBACK_CONT_MODE
	this->FEEDBACK_CONT_MODE.address[0] = 0x79; ///< Address=121
	this->FEEDBACK_CONT_MODE.msb[0] = 13;
	this->FEEDBACK_CONT_MODE.lsb[0] = 13;

	//Initialization for register PDN_ILLUM_DRV
	this->PDN_ILLUM_DRV.address[0] = 0x79; ///< Address=121
	this->PDN_ILLUM_DRV.msb[0] = 19;
	this->PDN_ILLUM_DRV.lsb[0] = 19;

	//Initialization for register EN_TX_DC_CURR_ALL
	this->EN_TX_DC_CURR_ALL.address[0] = 0x79; ///< Address=121
	this->EN_TX_DC_CURR_ALL.msb[0] = 4;

}

