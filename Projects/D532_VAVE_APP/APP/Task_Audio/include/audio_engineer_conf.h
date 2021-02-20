/*
 * Audio_engineer_conf.h
 *
 *  Created on: 2019年3月29日
 *      Author: gaojing
 */

#ifndef __AUDIO_ENGINEER_CONF_H__
#define __AUDIO_ENGINEER_CONF_H__

//#define DSP_PROJECT_CODE 0 // 591
#define DSP_PROJECT_CODE 1 // 532

#define DSP_ENGINEER_CODE_SWITCH 1
#define AMP_ENGINEER_CODE_SWITCH 1

// 0: no ecnr
// 1: 1mic ecnr
// 2: 2mic ecnr
// 4: noecnr for s05 and pre
#define DSP_ECNR_MODE 4
// can be 1 only when DSP_ECNR_MODE is 2
#define DSP_ECNR_RECORD_2MIC 0

// #define DSP_ECNR_RECORD 0
// #define DSP_ENABLE_ECNR 0
// 0 for 1mic 1 for 2mic
// #define DSP_FIRMWARE 0
#define DSP_SET_PHONE_CHANNEL_FILTER 0
#define DSP_DISABLE_TUNING 0
#define DSP_DISABLE_LIMITER 0

#endif /* __AUDIO_ENGINEER_CONF_H__ */
