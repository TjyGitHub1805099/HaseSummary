/*
 * amp_control_api.h
 *
 *  Created on: 2019Äê1ÔÂ30ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_DIGITALAMP_AMP_CONTROL_AMP_CONTROL_API_H_
#define TASK_DIGITALAMP_AMP_CONTROL_AMP_CONTROL_API_H_

#include "audio_engineer_conf.h"

/* need to according */
#define DCLoadCondition_Unknow 0x00
#define DCLoadCondition_ShortLoad 0x01
#define DCLoadCondition_ShortSupply 0x02
#define DCLoadCondition_ShortExternalVoltage 0x03
#define DCLoadCondition_ShortGround 0x04
#define DCLoadCondition_NormalLoad 0x05
#define DCLoadCondition_OpenLoad 0x06

#define ACLoadCondition_Unknow 0x00
#define ACLoadCondition_NormalLoad 0x01
#define ACLoadCondition_OpenLoad 0x02

int api_amp_get_diag_result(uint8_t *result);
int api_amp_on();
int api_amp_off();
int api_amp_exec_lc_filter_diagnostics();
int api_amp_get_lc_filter_result();

/* AMP engineer mode code */
#if (AMP_ENGINEER_CODE_SWITCH == 1)
void api_amp_engineer_SetPWMFreq_cmd(uint8_t ssmmode);
void api_amp_engineer_SetChannelConfig_cmd(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4);
#endif
/* AMP engineer mode code */

/* init amp(internal or external) */
int amp_init();
int check_amp_type();

extern uint8_t amp_type;

#endif /* TASK_DIGITALAMP_AMP_CONTROL_AMP_CONTROL_API_H_ */
