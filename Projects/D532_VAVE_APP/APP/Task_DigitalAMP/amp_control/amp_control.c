/*
 * amp_control.c
 *
 *  Created on: 2019Äê1ÔÂ30ÈÕ
 *      Author: gaojing
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "System_Signal.h"
#include "amp_control_api.h"
#include "amp_control.h"
#include "HardwareVersion.h"
#include "Task_DigitalAMP_Cfg.h"
#include "audio_engineer_conf.h"

#include "tdf853x.h"

#include "amp_i2c.h"

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
#include "trace.h"
#endif

extern amp_LoadDetectionResult DCLoad_result;
extern amp_LoadDetectionResult ACLoad_result;
extern int tdf853x_lc_result_flag;

/* external amp or internal amp */
static uint8_t amp_type_checked = 0;
uint8_t amp_type = 0;

uint8_t amp_startup_diagnostics_complete = 0;

/* amp state */
//int amp_state;

int check_amp_type()
{
	if(amp_type_checked == 0){
		amp_type = getVariantCtrlVerion();
		amp_type_checked = 1;
	}

	return 0;
}

/* section
 *
 *
 *
 */
int amp_init()
{
	int ret;

	if(amp_type == 1){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "external amp\r\n");
#endif
		return 0;
	}else if(amp_type == 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "internal amp\r\n");
#endif
		tdf853x_init();
        amp_startup_diagnostics_complete = 1;
	}else{
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "unknow amp type\r\n");
#endif
		return -1;
	}

	return ret;
}

/* section
 *
 * handle msg
 *
 */
#if (AMP_ENGINEER_CODE_SWITCH == 1)
void handle_set_pwm(void *data)
{
    tdf853x_set_pwm_ssm(data);

    return;
}

void handle_set_channel_phase(void *data)
{
    tdf853x_set_channel_phase(data);

    return;
}
#endif


int amp_handle_ipc_msg(SystemTaskMsgInfoType *msg)
{
	if(amp_type == 1){
		return 0;
	}
    switch(msg->msgId){
        case SYSTEM_SIG_ID_AMP_EXECUTE_LC_FILTER_DIAGNOSTICS:
            tdf853x_lc_filter_diagnostics();
            break;
#if (AMP_ENGINEER_CODE_SWITCH == 1)
        case SYSTEM_SIG_ID_AMP_ENGINEER_SET_PWM:
            handle_set_pwm(msg->parBufPtr);
            break;
        case SYSTEM_SIG_ID_AMP_ENGINEER_SET_CHANNEL_PHASE:
            handle_set_channel_phase(msg->parBufPtr);
            break;
#endif
        default:
            break;
    }

    return 0;
}

/* section
 *
 * api to other module
 *
 */
/* 0-3 dc result 4-7 ac result */
int api_amp_get_diag_result(uint8_t *result)
{
	int i = 0;

	if(amp_startup_diagnostics_complete == 0){
		return -1;
	}

	for(i = 0; i < 4; i++){
		result[i] = DCLoad_result.result[i].LoadCondition;
		result[i + 4] = ACLoad_result.result[i].LoadCondition;
	}

	return 0;
}

int api_amp_on()
{
	SystemTaskMsgInfoType l_message;

	/* TODO: check state */

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AMP_ON;
	l_message.parBufPtr = NULL;
	l_message.size = 0;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "api_amp_on\r\n");
#endif

	/* send msg */
	return System_Task_SendMessage(SYSTEM_TASK_ID_DIGITALAMP, &l_message, false);
}

int api_amp_off()
{
	SystemTaskMsgInfoType l_message;

	/* TODO: check state */

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AMP_OFF;
	l_message.parBufPtr = NULL;
	l_message.size = 0;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "api_amp_off\r\n");
#endif

	/* send msg */
	return System_Task_SendMessage(SYSTEM_TASK_ID_DIGITALAMP, &l_message, false);
}

int api_amp_exec_lc_filter_diagnostics()
{
	SystemTaskMsgInfoType l_message;

	/* TODO: check state */

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AMP_EXECUTE_LC_FILTER_DIAGNOSTICS;
	l_message.parBufPtr = NULL;
	l_message.size = 0;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "api_amp_exec_lc_filter_diagnostics\r\n");
#endif

	/* send msg */
	return System_Task_SendMessage(SYSTEM_TASK_ID_DIGITALAMP, &l_message, false);
}

int api_amp_get_lc_filter_result()
{
    return tdf853x_lc_result_flag;
}

/* AMP engineer mode code */
#if (AMP_ENGINEER_CODE_SWITCH == 1)

void api_amp_engineer_SetPWMFreq_cmd(uint8_t ssmmode)
{
    SystemTaskMsgInfoType l_message;

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AMP_ENGINEER_SET_PWM;
	l_message.parBufPtr = &ssmmode;
	l_message.size = 1;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "api_amp_engineer_SetPWMFreq_cmd\r\n");
#endif

	/* send msg */
	return System_Task_SendMessage(SYSTEM_TASK_ID_DIGITALAMP, &l_message, false);
}

void api_amp_engineer_SetChannelConfig_cmd(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4)
{
    SystemTaskMsgInfoType l_message;

    uint8_t phase[4];

    phase[0] = channel1;
    phase[1] = channel2;
    phase[2] = channel3;
    phase[3] = channel4;    

    /* send msg to audio task */
    l_message.msgId = SYSTEM_SIG_ID_AMP_ENGINEER_SET_CHANNEL_PHASE;
    l_message.parBufPtr = phase;
    l_message.size = 4;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
    TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "api_amp_engineer_SetChannelConfig_cmd\r\n");
#endif

    /* send msg */
    return System_Task_SendMessage(SYSTEM_TASK_ID_DIGITALAMP, &l_message, false);
}
#endif
/* AMP engineer mode code */
