/*
 * audio_control.c
 *
 *  Created on: 2018Äê10ÔÂ19ÈÕ
 *      Author: gaojing
 */

#include "System_Task.h"
#include "System_Signal.h"
#include "stdbool.h"

#include "dsp_global.h"
#include "dsp_driver.h"
#include "dsp_driver_proj.h"

#include "audio_control_api.h"
#include "amp_control_api.h"
#include "audio_engineer_conf.h"

#include "AudioSpiIpcProtocol.h"
#include "audio_ipc_config.h"

#include "Task_Audio_Cfg.h"
#if (STD_ON == TASK_AUDIO_DEBUG_TRACE)
#include "trace.h"
#endif

/*
 *  global variable
 */

/* control debug print, default off */
uint8_t audio_debug_flag = 0;

/* project related */
uint8_t audio_car_model;

/*
 *  global variable end
 */

/*
 *  static variable
 */

/* volume db value */
static int audio_gain = 1;

/* SSV */
static int audio_speed = 0;
static int audio_speed_bonus_gain = 0;
static int audio_sdvc_level = 0;
static UINT8 SDVC_table[256][2];

#define SDVC_TABLE1_NUM 6
static const int sdvc_curves_table1[SDVC_TABLE1_NUM][3] =
{
    {48,    32,     1},
    {76,    60,     2},
    {101,   85,     3},
    {126,   109,    4},
    {150,   134,    5},
    {172,   156,    6},
};

#define SDVC_TABLE2_NUM 7
static const int sdvc_curves_table2[SDVC_TABLE2_NUM][3] =
{
    {48,    32,     1},
    {71,    55,     2},
    {93,    76,     3},
    {114,   97,     4},
    {134,   118,    5},
    {155,   138,    6},
	{172,   156,    7},
};

#define SDVC_TABLE3_NUM 8
static const int sdvc_curves_table3[SDVC_TABLE3_NUM][3] =
{
    {48,    32,     1},
    {66,    50,     2},
    {84,    68,     3},
    {101,   85,     4},
    {119,   103,    5},
    {137,   121,    6},
	{155,   138,    7},
	{172,   156,    8},
};

#define SDVC_TABLE4_NUM 9
static const int sdvc_curves_table4[SDVC_TABLE4_NUM][3] =
{
	{40,    24,     1},
	{55,    39,     2},
	{69,    53,     3},
	{84,    68,     4},
	{98,    82,     5},
	{113,   97,     6},
	{127,   111,    7},
	{142,   126,    8},
	{156,   140,    9},
};

#define SDVC_TABLE5_NUM 9
static const int sdvc_curves_table5[SDVC_TABLE5_NUM][3] =
{
	{32,    16,     1},
	{43,    27,     2},
	{55,    39,     3},
	{66,    50,     4},
	{77,    61,     5},
	{89,    72,     6},
	{100,   84,     7},
	{111,   95,     8},
	{122,   106,    9},
};

/*
 *  static variable end
 */

/*
 *  obsolete
 */
#if 0
/* beep mute status */
static int audio_beep_mute = 0;
#endif
/*
 *  obsolete end
 */

/* api */
UINT8 *beep_cmd_pt = NULL;
UINT8 *clickclack_cmd_pt = NULL;
UINT8 *mute_beep_cmd_pt = NULL;

/*
 * Section: audio control init
 *
 *    init variable to default value
 *
 *
 *
 *
 */
void audio_control_variable_init()
{
	if(beep_cmd_pt == NULL){
		beep_cmd_pt = Mem_Malloc(sizeof(INT32) + sizeof(tChimeCmd_ID));
	}

	if(clickclack_cmd_pt == NULL){
		clickclack_cmd_pt = Mem_Malloc(sizeof(INT32) + sizeof(tClickClackCmd));
	}

	if(mute_beep_cmd_pt == NULL){
		mute_beep_cmd_pt = Mem_Malloc(sizeof(INT32) + sizeof(INT32));
	}
}

void audio_control_variable_uninit()
{
	if(beep_cmd_pt != NULL){
		 Mem_Free(beep_cmd_pt);
		 beep_cmd_pt = NULL;
	}

	if(clickclack_cmd_pt == NULL){
		Mem_Free(clickclack_cmd_pt);
		clickclack_cmd_pt = NULL;
	}

	if(mute_beep_cmd_pt == NULL){
		Mem_Free(mute_beep_cmd_pt);
		mute_beep_cmd_pt = NULL;
	}
}

/*
 * Section: SSV
 *
 *
 *
 *
 *
 *
 */
void initialize_ssv_table(int level)
{
	INT32 (*p)[3];
	INT32 i, j;
	INT32 num;
	INT32 a, b;
	UINT8 gain1 = 0;
	UINT8 gain2 = 0;

	if(level == 1)
	{
		p = sdvc_curves_table1;
		num = SDVC_TABLE1_NUM;
	}else if(level == 2){
		p = sdvc_curves_table2;
		num = SDVC_TABLE2_NUM;
	}else if(level == 3){
		p = sdvc_curves_table3;
		num = SDVC_TABLE3_NUM;
	}else if(level == 4){
		p = sdvc_curves_table4;
		num = SDVC_TABLE4_NUM;
	}else if(level == 5){
		p = sdvc_curves_table5;
		num = SDVC_TABLE5_NUM;
	}else{
		num = 0;
		p = 0;
	}

	j = 0;
	gain1 = 0;
	gain2 = 0;
	for(i = 0; i < num; i++)
	{
		a = p[i][0];
		b = p[i][1];
		gain2 = p[i][2];

		while(j < 256)
		{
			if(j < b)
			{
				SDVC_table[j][0] = gain1;
				SDVC_table[j][1] = gain1;
			}
			else if(j >= b && j < a)
			{
				SDVC_table[j][0] = gain1;
				SDVC_table[j][1] = gain2;
			}
			else
			{
				gain1 = gain2;
				break;
			}
			j++;
		}
	}

	while(j < 256)
	{
		SDVC_table[j][0] = gain1;
		SDVC_table[j][1] = gain1;
		j++;
	}
}

/*
 * Section: handle ipc msg
 *
 *
 *
 *
 *
 *
 */
void audio_handle_set_mic_gain(void *data)
{
	int32_t gain = *((int32_t *)data);

	dsp_driver_ioctl(DSP_SET_MIC_GAIN, data);

	return;
}

void audio_handle_set_loudness(void *data)
{
	if(amp_type == 1){
		return;
	}

	dsp_driver_ioctl(DSP_SET_LOUDNESS, data);

    return;
}

void audio_handle_set_sdvc_level(void *data)
{
    uint32_t level = *((uint32_t *)data);
    tVolCmd vol_data;
    int speed;

    if(audio_sdvc_level == level){
        return;
    }

    audio_sdvc_level = level;
    if(audio_sdvc_level != 0){
        initialize_ssv_table(audio_sdvc_level);
    }

    if(audio_speed > 255){
        speed = 255;
    }else{
        speed = audio_speed;
    }

    if(audio_sdvc_level == 0){
        if(audio_speed_bonus_gain != 0 && audio_gain != 1){
            audio_speed_bonus_gain = 0;
            vol_data.channel = CHANNEL_MAIN_A;
            vol_data.gain = audio_gain;

            dsp_driver_ioctl(DSP_SET_VOL, &vol_data);
        }
    }else{
        audio_speed_bonus_gain = SDVC_table[speed][0];

        if(audio_gain != 1){
            vol_data.channel = CHANNEL_MAIN_A;
            vol_data.gain = audio_gain + audio_speed_bonus_gain * 100;

            dsp_driver_ioctl(DSP_SET_VOL, &vol_data);
        }
    }

    return;
}

void audio_handle_speed(int speed)
{
    tVolCmd vol_data;
    int temp_speed;

    audio_speed = speed;

    if(audio_sdvc_level == 0){
        audio_speed_bonus_gain = 0;
        return;
    }

    if(audio_speed > 255){
        temp_speed = 255;
    }else{
        temp_speed = audio_speed;
    }

    if(audio_speed_bonus_gain == SDVC_table[temp_speed][0]
        || audio_speed_bonus_gain == SDVC_table[temp_speed][1]){
            return;
    }    

    if(audio_speed_bonus_gain <= SDVC_table[temp_speed][0]){
        vol_data.gain = audio_gain + SDVC_table[temp_speed][0] * 100;
        audio_speed_bonus_gain = SDVC_table[temp_speed][0];
    }else if(audio_speed_bonus_gain >= SDVC_table[temp_speed][0]){
        vol_data.gain = audio_gain + SDVC_table[temp_speed][1] * 100;
        audio_speed_bonus_gain = SDVC_table[temp_speed][1];
    }

    if(amp_type == 0){
		if(vol_data.gain > 200){
			vol_data.gain = 200;
		}
	}
    if(amp_type == 1){
		if(vol_data.gain > 0){
			vol_data.gain = 0;
		}
    }

    vol_data.channel = CHANNEL_MAIN_A;

#if (STD_ON == TASK_AUDIO_DEBUG_TRACE)
    TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "speed:%d\r\n", audio_speed);
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "audio_gain:%d\r\n", audio_gain);
    TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "bonus_gain:%d\r\n", audio_speed_bonus_gain);
    TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "total_gain:%d\r\n", vol_data.gain);
#endif    

    if(audio_gain != 1){
        dsp_driver_ioctl(DSP_SET_VOL, &vol_data);
    }

    return;
}

void audio_handle_set_vol(void *data)
{
    tVolCmd *vol_data = (tVolCmd *)data;
    //int speed;

    if(vol_data->channel == CHANNEL_MAIN_A){
    	audio_gain = vol_data->gain;

        vol_data->gain += audio_speed_bonus_gain * 100;

#if 0
        if(audio_speed > 255){
            speed = 255;
        }else{
            speed = audio_speed;
        }

        if(audio_sdvc_level == 0){
            audio_speed_bonus_gain = 0;
        }else{        
            if(audio_speed_bonus_gain <= SDVC_table[speed][0]){
                vol_data->gain += SDVC_table[speed][0];
                audio_speed_bonus_gain = SDVC_table[speed][0];
            }else if(audio_speed_bonus_gain >= SDVC_table[speed][0]){
                vol_data->gain += SDVC_table[speed][1];
                audio_speed_bonus_gain = SDVC_table[speed][1];
            }
        }
#endif

        /* internal VolMax set to 2db */
        if(amp_type == 0){
			if(vol_data->gain > 200){
				vol_data->gain = 200;
			}
        }

        /* external */
        if(amp_type == 1){
        	if(vol_data->gain > 0){
				vol_data->gain = 0;
			}
        }

#if (STD_ON == TASK_AUDIO_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "audio_gain:%d\r\n", audio_gain);
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "bonus_gain:%d\r\n", audio_speed_bonus_gain);
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "total_gain:%d\r\n", vol_data->gain);
#endif    
    }

    dsp_driver_ioctl(DSP_SET_VOL, data);

    return;
}

void audio_handle_set_mux(void *data)
{
    tMuxCmd *mux_data = (tMuxCmd *)data;

    if(amp_type == 1){
        if(mux_data->channel == CHANNEL_MAIN_A && mux_data->gain > -9900){
            mux_data->gain -= 120;
            if(mux_data->gain > 602){
                mux_data->gain = 602;
            }
        }

        if(mux_data->channel == CHANNEL_MONO_A && mux_data->gain > -9900){
            mux_data->gain -= 560;
            if(mux_data->gain > 602){
                mux_data->gain = 602;
            }
        }
    }

    dsp_driver_ioctl(DSP_SET_MUX, data);

    return;
}

void audio_handle_read_register(void *data)
{
	uint8_t data1[20];
	int value = dsp_driver_ioctl(DSP_READ_REGISTER, data);

	data1[AUDIOSPI_INDEX_TYPE] = AUDIOSPI_TYPE_REQ;
	data1[AUDIOSPI_INDEX_FLAG] = 0;
	data1[2] = 0;
	data1[3] = 0;

	audio_ipc_setCmdId(DSP_READ_REGISTER_RESULT, &data1[AUDIOSPI_HEADER_SIZE]);

	data1[AUDIOSPI_HEADER_SIZE + 4] = ((tWriteRegisterCmd_ID *)data)->address & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 5] = (((tWriteRegisterCmd_ID *)data)->address >> 8) & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 6] = (((tWriteRegisterCmd_ID *)data)->address >> 16) & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 7] = (((tWriteRegisterCmd_ID *)data)->address >> 24) & 0xFF;

	data1[AUDIOSPI_HEADER_SIZE + 8] = value & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 9] = (value >> 8) & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 10] = (value >> 16) & 0xFF;
	data1[AUDIOSPI_HEADER_SIZE + 11] = (value >> 24) & 0xFF;

	/* handle msg */
	/* function parameter useless, set to 1 */
	ipc_send_notification(AUDIO_IPC_HW_CHANNEL, AUDIO_IPC_SW_CHANNEL, 1, data1, AUDIOSPI_HEADER_SIZE + 12);
}

void audio_handle_ipc_msg(uint8 *msg, uint32 len)
{
	INT32 cmd_id;
	void *data;

	uint16_t msg_id;
	uint8_t msg_type;
	uint8_t msg_flag;

	/* check ipc protocol */
	msg_type = msg[AUDIOSPI_INDEX_TYPE];
	msg_flag = msg[AUDIOSPI_INDEX_FLAG];
	msg_id = ((msg[3] << 8) & 0xFF00) + msg[2];

	if(audio_debug_flag){
		if((msg_flag & AUDIOSPI_FLAG_ACK) != 0){
			TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "[Audio]:%d Msg need ACK\r\n");
		}else{
			TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "[Audio]:%d Msg wait no ACK\r\n");
		}
	}

	/* check if need send ack */
	if((msg_flag & AUDIOSPI_FLAG_ACK) != 0){
		uint8_t ack_data[4];
		ack_data[msg_type] = AUDIOSPI_TYPE_ACK;
		ack_data[AUDIOSPI_INDEX_FLAG] = 0;
		ack_data[2] = msg_id & 0xFF;
		ack_data[3] = (msg_id >> 8) & 0xFF;
		/* function parameter useless, set to 1 */
		ipc_send_notification(AUDIO_IPC_HW_CHANNEL, AUDIO_IPC_SW_CHANNEL, 1, ack_data, AUDIOSPI_ACK_MSG_SIZE);
		if(audio_debug_flag){
			TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "[Audio]:%d Msg return ACK\r\n");
		}
	}
	/* skip ipc header */
	msg += AUDIOSPI_HEADER_SIZE;

	/* */
	cmd_id = *((INT32 *)msg);
	data = (void *)(msg + 4);

	//TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "audio_handle_ipc_msg %d\r\n", msg_id);

	/* AMP engineer mode code */
	if(cmd_id == AMP_ENGINEER_SET_PWM){
#if (AMP_ENGINEER_CODE_SWITCH == 1)
		api_amp_engineer_SetPWMFreq_cmd(*((uint8_t *)data));
#endif
		return;
	}

    if(cmd_id == AMP_ENGINEER_SET_CHANNEL_CONFIG){
#if (AMP_ENGINEER_CODE_SWITCH == 1)
		uint8_t *pt = (uint8_t *)data;
		api_amp_engineer_SetChannelConfig_cmd(*pt, *(pt + 1), *(pt + 2), *(pt + 3));
#endif
		return;
	}
	/* AMP engineer mode code */

    switch(cmd_id){
        case AMP_EXECUTE_LC_FILTER_DIAGNOSTICS:
            api_amp_exec_lc_filter_diagnostics();
            break;
        case DSP_SET_VOL:
            audio_handle_set_vol(data);
            break;
        case DSP_SET_MUX:
            audio_handle_set_mux(data);
            break;
        case DSP_SET_SDVC_LEVEL:
            audio_handle_set_sdvc_level(data);
        	break;
        case DSP_SET_LOUDNESS:
        	audio_handle_set_loudness(data);
        	break;
        case DSP_SET_MIC_GAIN:
        	audio_handle_set_mic_gain(data);
        	break;
        case DSP_READ_REGISTER:
        	audio_handle_read_register(data);
        	break;
        default:
            dsp_driver_ioctl(cmd_id, data);
            break;
    }

    return;
}

/*
 * Section: ioc audio api
 *
 *
 *
 *
 *
 *
 */

tChimeCmd_ID beep_cmd;
tPDCCmd_ID beep_pdc_cmd;

#if 1
int api_audio_beep_on_by_id(int id)
{
	SystemTaskMsgInfoType l_message;

	if(id >= BEEP_SONAR_LV1){
		beep_pdc_cmd.action = PDC_ACTION_START;
		beep_pdc_cmd.id = id - BEEP_SONAR_LV1;
	}else{
		beep_cmd.action = CHIME_ACTION_START;
		beep_cmd.id = id;
		beep_cmd.type = CHIME_TYPE_CONTINOUS;
	}

	/* send msg to audio task */
	if(id >= BEEP_SONAR_LV1){
		l_message.msgId = SYSTEM_SIG_ID_AUDIO_BEEP_PDC;
		l_message.parBufPtr = &beep_pdc_cmd;
		l_message.size = sizeof(beep_pdc_cmd);
	}else{
		l_message.msgId = SYSTEM_SIG_ID_AUDIO_BEEP;
		l_message.parBufPtr = &beep_cmd;
		l_message.size = sizeof(beep_cmd);
	}

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);

	return 0;
}
#endif

#if 0
int api_audio_beep_on_by_id(int id)
{
	SystemTaskMsgInfoType l_message;
	tChimeCmd_ID *chimdCmd;

	*((INT32 *)beep_cmd_pt) = DSP_SET_CHIME_BY_ID;
	chimdCmd = (tChimeCmd_ID *)(beep_cmd_pt + 4);

	chimdCmd->action = CHIME_ACTION_START;
	chimdCmd->id = id;
	if(id >= BEEP_SONAR_LV4){
		chimdCmd->type = CHIME_TYPE_CONTINOUS_TONE;
	}else{
		chimdCmd->type = CHIME_TYPE_CONTINOUS;
	}

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AUDIO_SETTING;
	l_message.parBufPtr = beep_cmd_pt;
	l_message.size = sizeof(INT32) + sizeof(tChimeCmd_ID);

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);

	return 0;
}
#endif

int api_audio_beep_off_by_id(int id)
{
	SystemTaskMsgInfoType l_message;

	if(id >= BEEP_SONAR_LV1){
		beep_pdc_cmd.action = PDC_ACTION_STOP;
		beep_pdc_cmd.id = id - BEEP_SONAR_LV1;
	}else{
		beep_cmd.action = CHIME_ACTION_STOP;
		beep_cmd.id = id;
		beep_cmd.type = CHIME_TYPE_CONTINOUS;
	}

	/* send msg to audio task */
	if(id >= BEEP_SONAR_LV1){
		l_message.msgId = SYSTEM_SIG_ID_AUDIO_BEEP_PDC;
		l_message.parBufPtr = &beep_pdc_cmd;
		l_message.size = sizeof(beep_pdc_cmd);
	}else{
		l_message.msgId = SYSTEM_SIG_ID_AUDIO_BEEP;
		l_message.parBufPtr = &beep_cmd;
		l_message.size = sizeof(beep_cmd);
	}

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);

	return 0;
}

tClickClackCmd clickclack_cmd;
int api_audio_clickclack(int action)
{
	SystemTaskMsgInfoType l_message;

	clickclack_cmd.action = action;

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AUDIO_CLICKCLACK;
	l_message.parBufPtr = &clickclack_cmd;
	l_message.size = sizeof(clickclack_cmd);

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);

	return 0;
}



int api_audio_return_lc_result(uint8_t *kvalues)
{
	SystemTaskMsgInfoType l_message;

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AUDIO_EXECUTE_LC_FILTER_DIAGNOSTICS;
	l_message.parBufPtr = kvalues;
	l_message.size = 5;

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);
}

int api_audio_enter_ecall()
{
	uint8_t action;

	action = 1;

	SystemTaskMsgInfoType l_message;

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AUDIO_DO_ECALL;
	l_message.parBufPtr = &action;
	l_message.size = 1;

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);
}

int api_audio_leave_ecall()
{
	uint8_t action;

	action = 0;

	SystemTaskMsgInfoType l_message;

	/* send msg to audio task */
	l_message.msgId = SYSTEM_SIG_ID_AUDIO_DO_ECALL;
	l_message.parBufPtr = &action;
	l_message.size = 1;

	/* send msg */
	System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);
}

/*
 *  obsolete
 */
#if 0
INT32 beep_mute_cmd;
int api_audio_mute_beep(int mute)
{
	SystemTaskMsgInfoType l_message;

	if(audio_beep_mute == mute){
		return 0;
	}

	beep_mute_cmd = mute;
	if(mute == 1){
		/* send msg to audio task */
		l_message.msgId = SYSTEM_SIG_ID_AUDIO_MUTE_BEEP;
		l_message.parBufPtr = &beep_mute_cmd;
		l_message.size = sizeof(beep_mute_cmd);

		/* send msg */
		System_Task_SendMessage(SYSTEM_TASK_ID_AUDIO, &l_message, false);
	}
	if(mute == 0){
		/* TODO */
	}

	audio_beep_mute = mute;
}
#endif
/*
 *  obsolete end
 */
