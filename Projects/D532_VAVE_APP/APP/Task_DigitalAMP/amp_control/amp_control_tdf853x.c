/*
 * amp_control_tdf853x.c
 *
 *  Created on: 2019Äê2ÔÂ28ÈÕ
 *      Author: gaojing
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "System_Task.h"
#include "System_Signal.h"
#include "amp_i2c.h"
#include "Task_DigitalAMP_Cfg.h"
#include "tdf853x.h"
#include "tdf853x_config.h"
#include "project_n591_data.h"
#include "audio_engineer_conf.h"

#include "HardwareVersion.h"

#include "Eeprom.h"


#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
#include "trace.h"
#endif

#define tdf853x_lc_flag_e2p_addr 12288

/* amp identification */
static amp_id_t amp_identification;

amp_LoadDetectionResult DCLoad_result;
amp_LoadDetectionResult ACLoad_result;

uint8_t tdf853x_lc_diagnostics_result[5];
int tdf853x_lc_result_flag = -1;

#if (AMP_ENGINEER_CODE_SWITCH == 1)
uint8_t amp_pwmFreq_config_eng[] = {
	0x02,           /* MsgType  */
	0x00,           /* PacketId */
	0x05,           /* Length   */
	0x80,           /* ModuleId */
	0x16,           /* CmdId    */
	0x04,
	0x00,
	0x00,
};

uint8_t amp_channel_config_eng[] = {
	0x02,           /* MsgType  */
	0x00,           /* PacketId */
	0x2F,           /* Length   */
	0x80,           /* ModuleId */
	0x24,           /* CmdId    */
	0x04,

	0x01,
	0x32,
	0x01,
	0x00,
	0x02,
	0xA5,
	0xC5,
	0x02,
	0x00,
	0x24,
	0x00,

	0x02,
	0x32,
	0x01,
	0x00,
	0x02,
	0xA5,
	0xC5,
	0x02,
	0x00,
	0x24,
	0x00,

	0x03,
	0x32,
	0x01,
	0x00,
	0x02,
	0xA5,
	0xC5,
	0x02,
	0x00,
	0x24,
	0x00,

	0x04,
	0x32,
	0x01,
	0x00,
	0x02,
	0xA5,
	0xC5,
	0x02,
	0x00,
	0x24,
	0x00,
};
#endif

/* check command error */
int tdf853x_checkMsgError(uint8_t lastCmdId)
{
	int ret;
	int retry = 0;
	uint8_t data[2];

	while(retry < 3){
		ret = tdf853x_read(GetMsgErrorStatus_req, &amp_i2c, NULL, data);
		if(ret == 0){
			break;
		}
		retry++;
		System_Task_Delay(1);
	}

	if(retry >= 3){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_checkMsgError read failed\r\n");
#endif
		return -1;
	}

	if(lastCmdId != data[0] || data[1] != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_checkMsgError failed\r\n");
        TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "id:%x\r\n", data[0]);
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "lastCmdId:%x, e_code:%x\r\n", lastCmdId, data[1]);
#endif
		return -1;
	}

	return 0;
}

int boot_tdf853x()
{
	int retry = 0;

	/* TODO: ENABLE pin */

	/* should delay more than 20ms */
	System_Task_Delay(25);

	/* read identification */
	while(retry < 10){
		if(tdf853x_read(GetIdentification_req, &amp_i2c, NULL, &amp_identification) == 0){
			/* TODO: check chip hardware and software version */
			break;
		}
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "boot_tdf853x read failed\r\n");
#endif
		retry++;
        System_Task_Delay(10);
	}
	if(retry >=10){
		return -1;
	}
	System_Task_Delay(1);

	/* patch */

	/* keycode */

	return 0;
}

#define tdf853x_checkMsgError_macro(cmdId) \
		if(tdf853x_checkMsgError((cmdId)) < 0){ \
			return -1; \
		}

inline void configure_tdf853x_print_error(uint8_t cmdId, int ret)
{
	if((ret) != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "configure_tdf853x_error\r\n");
        TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "cmdId:%d\r\n", cmdId);
#endif
	}
}

#define configure_tdf853x_error(cmdId, ret) \
	configure_tdf853x_print_error((cmdId), (ret)); \
	if((ret) != 0){ \
		return (ret); \
	}

int configure_tdf853x()
{
	int ret;
	uint8_t SetClockState_command;

	CircuitVersion circuit_version;
	circuit_version = getCircuitVerion();

    if(circuit_version != VERSION_1 || circuit_version != VERSION_2){
        //LC configure
        ret = tdf853x_write_array(&amp_i2c, amp_lc_config, sizeof(amp_lc_config));
        configure_tdf853x_error(SetLCFilterConfig_cmd, ret);
        tdf853x_checkMsgError_macro(SetLCFilterConfig_cmd);
        System_Task_Delay(1);
    }

	/* Optional: SetDiagPin */
	ret = tdf853x_write_array(&amp_i2c, SetDiagPin_cmd_data, sizeof(SetDiagPin_cmd_data));
	configure_tdf853x_error(SetDiagPin_cmd, ret);
	tdf853x_checkMsgError_macro(SetDiagPin_cmd);
	System_Task_Delay(1);

	/* disable all channel */
	ret = tdf853x_write(SetChannelDisable_cmd, &amp_i2c, NULL);
	configure_tdf853x_error(SetChannelDisable_cmd, ret);
	tdf853x_checkMsgError_macro(SetChannelDisable_cmd);
	System_Task_Delay(1);

	/* Optional: SetDeviceIOConfig_cmd */

	/* SetAudioConfig_cmd */
	ret = tdf853x_write(SetAudioConfig_cmd, &amp_i2c, &amp_audio_config);
	configure_tdf853x_error(SetAudioConfig_cmd, ret);
	tdf853x_checkMsgError_macro(SetAudioConfig_cmd);
	System_Task_Delay(1);

	/* SetClockState_cmd */
	SetClockState_command = SetClockState_Connect;
	ret = tdf853x_write(SetClockState_cmd, &amp_i2c, &SetClockState_command);
	configure_tdf853x_error(SetClockState_cmd, ret);
	tdf853x_checkMsgError_macro(SetClockState_cmd);
	System_Task_Delay(1);

	/* Optional: SetAudioRoutingConfig_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_audioRouting_config, sizeof(amp_audioRouting_config));
	configure_tdf853x_error(SetAudioRoutingConfig_cmd, ret);
	tdf853x_checkMsgError_macro(SetAudioRoutingConfig_cmd);
	System_Task_Delay(1);

	/* Optional: SetPWMFreq_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_pwmFreq_config, sizeof(amp_pwmFreq_config));
	configure_tdf853x_error(SetPWMFreq_cmd, ret);
	tdf853x_checkMsgError_macro(SetPWMFreq_cmd);
	System_Task_Delay(1);

	/* Optional: SetModSchemeConfig_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_modscheme_config, sizeof(amp_modscheme_config));
	configure_tdf853x_error(SetModSchemeConfig_cmd, ret);
	tdf853x_checkMsgError_macro(SetModSchemeConfig_cmd);
	System_Task_Delay(1);

	/* Optional: SetHPFConfig_cmd */

	/* Optional: SetMuteConfig_cmd */

	/* Optional: SetDeviceConfig_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_device_config, sizeof(amp_device_config));
	configure_tdf853x_error(SetDeviceConfig_cmd, ret);
	tdf853x_checkMsgError_macro(SetDeviceConfig_cmd);
	System_Task_Delay(1);

	/* Optional: SetChannelConfig_cmd */
	if(circuit_version == VERSION_1 || circuit_version == VERSION_2){
        if(tdf853x_lc_diagnostics_result[0] == 1){
			amp_channel_config[7] = tdf853x_lc_diagnostics_result[1];
			amp_channel_config[18] = tdf853x_lc_diagnostics_result[2];
			amp_channel_config[29] = tdf853x_lc_diagnostics_result[3];
			amp_channel_config[40] = tdf853x_lc_diagnostics_result[4];
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
    		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x lc already diagnostics\r\n");
            TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "kvalue:%d %d %d %d\r\n",
                tdf853x_lc_diagnostics_result[1], tdf853x_lc_diagnostics_result[2], tdf853x_lc_diagnostics_result[3],
                tdf853x_lc_diagnostics_result[4]);
#endif
		}
		ret = tdf853x_write_array(&amp_i2c, amp_channel_config, sizeof(amp_channel_config));
        configure_tdf853x_error(SetChannelConfig_cmd, ret);
        tdf853x_checkMsgError_macro(SetChannelConfig_cmd);
		System_Task_Delay(1);
	}else{
		if(tdf853x_lc_diagnostics_result[0] == 1){
			amp_channel_config_hardware_version_3[7] = tdf853x_lc_diagnostics_result[1];
			amp_channel_config_hardware_version_3[18] = tdf853x_lc_diagnostics_result[2];
			amp_channel_config_hardware_version_3[29] = tdf853x_lc_diagnostics_result[3];
			amp_channel_config_hardware_version_3[40] = tdf853x_lc_diagnostics_result[4];
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
    		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x lc already diagnostics\r\n");
            TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "kvalue:%d %d %d %d\r\n",
                tdf853x_lc_diagnostics_result[1], tdf853x_lc_diagnostics_result[2], tdf853x_lc_diagnostics_result[3],
                tdf853x_lc_diagnostics_result[4]);
#endif
		}
		ret = tdf853x_write_array(&amp_i2c, amp_channel_config_hardware_version_3, sizeof(amp_channel_config_hardware_version_3));
        configure_tdf853x_error(SetChannelConfig_cmd, ret);
        tdf853x_checkMsgError_macro(SetChannelConfig_cmd);
		System_Task_Delay(1);
	}

    System_Task_Delay(5);

	return 0;
}

inline void execute_startup_diagnostics_print_error(uint8_t cmdId, int ret)
{
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "execute_startup_diagnostics\r\n");
        TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "cmdId:%d\r\n", cmdId);
#endif
	}
}

#define execute_startup_diagnostics_error(cmdId, ret) \
	execute_startup_diagnostics_print_error((cmdId), (ret)); \
	if((ret) != 0){ \
		return (ret); \
	}

int execute_startup_diagnostics(int doAc)
{
	int ret;

	amp_StartupDiagStatus diagStatus;

	/* SetDCLoadDetectionConfig_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_dcLoadDetection_config, sizeof(amp_dcLoadDetection_config));
	execute_startup_diagnostics_error(SetDCLoadDetectionConfig_cmd, ret);
	tdf853x_checkMsgError_macro(SetDCLoadDetectionConfig_cmd);
	System_Task_Delay(1);

	/* SetStartupDiag_cmd */
    if(doAc == 1){
    	ret = tdf853x_write_array(&amp_i2c, amp_startUpDiag_config, sizeof(amp_startUpDiag_config));
    	execute_startup_diagnostics_error(SetStartupDiag_cmd, ret);
    	tdf853x_checkMsgError_macro(SetStartupDiag_cmd);
    }else{
        ret = tdf853x_write_array(&amp_i2c, amp_startUpDiag_config_noac, sizeof(amp_startUpDiag_config_noac));
    	execute_startup_diagnostics_error(SetStartupDiag_cmd, ret);
    	tdf853x_checkMsgError_macro(SetStartupDiag_cmd);
    }
	System_Task_Delay(320);

	/* GetStartupDiagStatus */
	ret = tdf853x_read(GetStartupDiagStatus_req, &amp_i2c, NULL, &diagStatus);

	if(diagStatus.StartupDiagnosticStatus != GetStartupDiagStatus_repl_Diagnostic_COMPLETE){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "GetStartupDiagStatus error\r\n");
#endif
		return -1;
	}
	System_Task_Delay(1);

	/* GetDCLoadDetectionResult */
	ret = tdf853x_read(GetDCLoadDetectionResult_req, &amp_i2c, NULL, &DCLoad_result);
	execute_startup_diagnostics_error(GetDCLoadDetectionResult_req, ret);
	System_Task_Delay(1);

	/* GetACLoadDetectionResult */
    if(doAc == 1){
    	ret = tdf853x_read(GetACLoadDetectionResult_req, &amp_i2c, NULL, &ACLoad_result);
    	execute_startup_diagnostics_error(GetACLoadDetectionResult_req, ret);
    	System_Task_Delay(1);
    }else{

    }

	return 0;
}

int tdf853x_init()
{
	int ret;

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_init\r\n");
#endif

    /* read flag and kvalue */
    Eep_Read(tdf853x_lc_flag_e2p_addr, tdf853x_lc_diagnostics_result, 5);

	ret = boot_tdf853x();
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "boot_tdf853x error\r\n");
#endif
		return -1;
	}

	ret = configure_tdf853x();
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "configure_tdf853x error\r\n");
#endif
		return -1;
	}

	ret = execute_startup_diagnostics(0);
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "execute_startup_diagnostics error\r\n");
#endif
		return -1;
	}

	ret = tdf853x_write(SetChannelEnable_cmd, &amp_i2c, NULL);
	System_Task_Delay(100);
    if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
        TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_init enable channel error\r\n");
#endif
        return -1;
    }
	tdf853x_checkMsgError_macro(SetChannelEnable_cmd);

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
	TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_init end\r\n");
#endif

	return 0;
}

int tdf853x_disable_channels()
{
	int ret;

	/* disable all channel */
	ret = tdf853x_write(SetChannelDisable_cmd, &amp_i2c, NULL);
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_disable_channels error\r\n");
#endif
		return ret;
	}
    tdf853x_checkMsgError_macro(SetChannelDisable_cmd);
	System_Task_Delay(150);

	return 0;
}

int tdf853x_enable_channels()
{
	int ret;

	/* enable all channel */
	ret = tdf853x_write(SetChannelEnable_cmd, &amp_i2c, NULL);
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_enable_channels error\r\n");
#endif
		return ret;
	}
    tdf853x_checkMsgError_macro(SetChannelEnable_cmd);
	System_Task_Delay(100);

	return 0;
}

int tdf853x_shutdown()
{
	int ret;

	/* disable all channel */
	ret = tdf853x_write(SetChannelDisable_cmd, &amp_i2c, NULL);
    if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_disable_channels error\r\n");
#endif
		return ret;
	}
    tdf853x_checkMsgError_macro(SetChannelDisable_cmd);
	System_Task_Delay(150);    

	/* TODO: set enable pin to low */

	/* TODO: disable supply */

	return 0;
}

inline void lc_filter_tdf853x_print_error(uint8_t cmdId, int ret)
{
	if(ret != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "lc_filter_tdf853x_print_error\r\n");
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "cmdId %x\r\n", cmdId);
#endif
	}
}

#define lc_filter_tdf853x_error_notification(ret, kvalues) \
	if((ret) != 0){ \
		api_audio_return_lc_result((kvalues)); \
		return -1; \
	}


#define lc_filter_tdf853x_error(cmdId, ret) \
	lc_filter_tdf853x_print_error((cmdId), (ret)); \
	if((ret) != 0){ \
		return (ret); \
	}

int tdf853x_check_faultstatus()
{
	int ret;
	uint8_t data[2];

	ret = tdf853x_read(GetFaultStatus_req, &amp_i2c, NULL, data);
	lc_filter_tdf853x_error(GetFaultStatus_req, ret);

	if(data[0] != 0 || ((data[1] & 0xC0) != 0)){
		return -1;
	}

	return 0;
}

int tdf853x_check_lc_diagnostic(uint8_t *data)
{
	int i;
	int ret;
	double temperature;
	int supply_voltage;
	int device_state;
	amp_ChannelStatus channel_status;

	/* get dc load result */
	ret = tdf853x_read(GetDCLoadDetectionResult_req, &amp_i2c, NULL, &DCLoad_result);
	lc_filter_tdf853x_error(GetDCLoadDetectionResult_req, ret);
	System_Task_Delay(1);

	/* check all channel dc open load */
	for(i = 0; i < DCLoad_result.NrOfChannels; i++){
		if(DCLoad_result.result[i].LoadCondition != GetDCLoadDetectionResult_repl_DCLoadCondition_OpenLoad){
            *data = LC_FILTER_DIAGNOSTICS_ERROR_NOT_OPEN_LOAD;
			return -1;
		}
	}

	/* get ac load result */
	ret = tdf853x_read(GetACLoadDetectionResult_req, &amp_i2c, NULL, &ACLoad_result);
	lc_filter_tdf853x_error(GetACLoadDetectionResult_req, ret);
	System_Task_Delay(1);

	/* check all channel ac open load */
	for(i = 0; i < ACLoad_result.NrOfChannels; i++){
		if(ACLoad_result.result[i].LoadCondition != GetACLoadDetectionResult_repl_ACLoadCondition_OpenLoad){
            *data = LC_FILTER_DIAGNOSTICS_ERROR_NOT_OPEN_LOAD;
			return -1;
		}
	}

	/* temperature */
	ret = tdf853x_read(GetTemperature_req, &amp_i2c, NULL, &temperature);
	lc_filter_tdf853x_error(GetTemperature_req, ret);
	System_Task_Delay(1);

	if(temperature < 0 || temperature > 70){
        *data = LC_FILTER_DIAGNOSTICS_ERROR_TEMPERATURE;
		return -1;
	}

	/* supply voltage */
	ret = tdf853x_read(GetSupplyVoltage_req, &amp_i2c, NULL, &supply_voltage);
	lc_filter_tdf853x_error(GetSupplyVoltage_req, ret);
	System_Task_Delay(1);

	if(supply_voltage < 12000 || supply_voltage > 16000){
        *data = LC_FILTER_DIAGNOSTICS_ERROR_VOLTAGE;
		return -1;
	}

	/* device state */
	ret = tdf853x_read(GetDeviceState_req, &amp_i2c, NULL, &device_state);
	lc_filter_tdf853x_error(GetDeviceState_req, ret);
	System_Task_Delay(1);

	if(device_state != GetDeviceState_repl_State_Playing){
		return -1;
	}

	/* check fault status */
	if(tdf853x_check_faultstatus() != 0){
		return -1;
	}

	/* channel status */
	ret = tdf853x_read(GetChannelStatus_req, &amp_i2c, NULL, &channel_status);
	lc_filter_tdf853x_error(GetChannelStatus_req, ret);
	System_Task_Delay(1);

	for(i = 0; i < channel_status.NrOfChannels; i++){
		if(channel_status.result[i].error_byte != 0){
			return -1;
		}
	}

	return 0;
}

int tdf853x_lc_filter_diagnostics()
{
    int i;
	int ret;
    
	uint8_t data[2];
	uint8_t kvalues[5];
	uint8_t SetClockState_command;
	amp_FilterDiagnosticResult filter_diagnostic_result;

	tdf853x_lc_result_flag = -1;

    CircuitVersion circuit_version;
	circuit_version = getCircuitVerion();

	kvalues[0] = 0;
    kvalues[1] = LC_FILTER_DIAGNOSTICS_ERROR_OTHER;

	/* SetChannelDisable_cmd */
	ret = tdf853x_write(SetChannelDisable_cmd, &amp_i2c, NULL);
	//lc_filter_tdf853x_print_error(SetChannelDisable_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(150);
	if(tdf853x_checkMsgError(SetChannelDisable_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	/* SetClockState_cmd */
	SetClockState_command = SetClockState_Disconnect;
	ret = tdf853x_write(SetClockState_cmd, &amp_i2c, &SetClockState_command);
	//lc_filter_tdf853x_print_error(SetClockState_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(100);
	if(tdf853x_checkMsgError(SetClockState_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	/* SetChannelEnable_cmd */
	ret = tdf853x_write(SetChannelEnable_cmd, &amp_i2c, NULL);
	//lc_filter_tdf853x_print_error(SetChannelEnable_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(10);
	if(tdf853x_checkMsgError(SetChannelEnable_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	/* SetChannelMute_cmd */
	ret = tdf853x_write(SetChannelMute_cmd, &amp_i2c, NULL);
	//lc_filter_tdf853x_print_error(SetChannelMute_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(10);
	if(tdf853x_checkMsgError(SetChannelMute_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	/* SetDCLoadDetectionConfig_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_dcLoadDetection_config, sizeof(amp_dcLoadDetection_config));
	//lc_filter_tdf853x_print_error(SetDCLoadDetectionConfig_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(10);
	if(tdf853x_checkMsgError(SetDCLoadDetectionConfig_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

    /* SetStartupDiag_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_startUpDiag_config, sizeof(amp_startUpDiag_config));
	//lc_filter_tdf853x_print_error(SetStartupDiag_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(10);
	if(tdf853x_checkMsgError(SetStartupDiag_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

    /* SetClockState_cmd */
	SetClockState_command = SetClockState_Connect;
	ret = tdf853x_write(SetClockState_cmd, &amp_i2c, &SetClockState_command);
	//lc_filter_tdf853x_print_error(SetClockState_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(10);
	if(tdf853x_checkMsgError(SetClockState_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	/* GetFaultStatus */
	ret = tdf853x_read(GetFaultStatus_req, &amp_i2c, NULL, data);
	//lc_filter_tdf853x_print_error(GetFaultStatus_req, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(520);

	/* SetFilterDiagnostic_cmd */
	ret = tdf853x_write_array(&amp_i2c, amp_SetFilterDiagnostic_cmd_data, sizeof(amp_SetFilterDiagnostic_cmd_data));
	//lc_filter_tdf853x_print_error(SetFilterDiagnostic_cmd, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	System_Task_Delay(520);
	if(tdf853x_checkMsgError(SetFilterDiagnostic_cmd) < 0){
		goto LC_FILTER_ERROR;
	}
	System_Task_Delay(10);

	if(tdf853x_check_lc_diagnostic(&kvalues[1]) != 0){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
        TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "lc diagnostic condition error\r\n");
#endif
		goto LC_FILTER_ERROR;
	}

	/* Get filter diagnostic result */
	ret = tdf853x_read(GetFilterDiagnosticResult_req, &amp_i2c, NULL, &filter_diagnostic_result);
	//lc_filter_tdf853x_print_error(GetFilterDiagnosticResult_req, ret);
    if(ret != 0){
        goto LC_FILTER_ERROR;
    }
	for(i = 0; i < filter_diagnostic_result.NrOfChannels; i++){
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
		TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "kmax:%d, kmin:%d, kvalue:%d\r\n",
				filter_diagnostic_result.result[i].kmax, filter_diagnostic_result.result[i].kmin, filter_diagnostic_result.result[i].kvalue);
#endif
	}

	/* calculate kvalue */
	tdf853x_lc_diagnostics_result[0] = 1;
	kvalues[0] = 1;
	for(i = 0; i < filter_diagnostic_result.NrOfChannels; i++){
		if((filter_diagnostic_result.result[i].kmax - 15 - filter_diagnostic_result.result[i].kmin) <= 12){
			tdf853x_lc_diagnostics_result[i + 1] = filter_diagnostic_result.result[i].kmin + 4;
		}else if((filter_diagnostic_result.result[i].kmax - 15 - filter_diagnostic_result.result[i].kmin) <= 45){
			tdf853x_lc_diagnostics_result[i + 1] = filter_diagnostic_result.result[i].kmin + round((filter_diagnostic_result.result[i].kmax - 15 - filter_diagnostic_result.result[i].kmin) / 3);
		}else{
			tdf853x_lc_diagnostics_result[i + 1] = filter_diagnostic_result.result[i].kmin + 15;
		}
		kvalues[i + 1] = tdf853x_lc_diagnostics_result[i + 1];
	}

	/* write E2P ROM */
	ret = Eep_Write(tdf853x_lc_flag_e2p_addr, tdf853x_lc_diagnostics_result, 5);

    if(ret == 0){
    	tdf853x_lc_result_flag = 0;
        //api_audio_return_lc_result(kvalues);
    }else{
        kvalues[0] = 0;
        kvalues[1] = LC_FILTER_DIAGNOSTICS_ERROR_WRITE_E2P;
        //api_audio_return_lc_result(kvalues);
        goto LC_FILTER_ERROR;
    }	

LC_FILTER_ERROR:
    api_audio_return_lc_result((kvalues));

    /* connect clock */
    SetClockState_command = SetClockState_Connect;
	ret = tdf853x_write(SetClockState_cmd, &amp_i2c, &SetClockState_command);

    /* disable filter diagnostic */
	ret = tdf853x_write_array(&amp_i2c, amp_SetFilterDiagnostic_cmd_stop_data, sizeof(amp_SetFilterDiagnostic_cmd_stop_data));

    /* Set channel disable */
	tdf853x_write(SetChannelDisable_cmd, &amp_i2c, NULL);
    System_Task_Delay(150);

    /* channel unmute */
	ret = tdf853x_write(SetChannelUnMute_cmd, &amp_i2c, NULL);

    if(circuit_version == VERSION_1 || circuit_version == VERSION_2){
        if(tdf853x_lc_diagnostics_result[0] == 1){
            amp_channel_config[7] = tdf853x_lc_diagnostics_result[1];
            amp_channel_config[18] = tdf853x_lc_diagnostics_result[2];
            amp_channel_config[29] = tdf853x_lc_diagnostics_result[3];
            amp_channel_config[40] = tdf853x_lc_diagnostics_result[4];
        }
        ret = tdf853x_write_array(&amp_i2c, amp_channel_config, sizeof(amp_channel_config));
        System_Task_Delay(1);
    }else{
        if(tdf853x_lc_diagnostics_result[0] == 1){
            amp_channel_config_hardware_version_3[7] = tdf853x_lc_diagnostics_result[1];
            amp_channel_config_hardware_version_3[18] = tdf853x_lc_diagnostics_result[2];
            amp_channel_config_hardware_version_3[29] = tdf853x_lc_diagnostics_result[3];
            amp_channel_config_hardware_version_3[40] = tdf853x_lc_diagnostics_result[4];
        }
        ret = tdf853x_write_array(&amp_i2c, amp_channel_config_hardware_version_3, sizeof(amp_channel_config_hardware_version_3));
        System_Task_Delay(1);
    }

    /* Set channel enable */
	tdf853x_write(SetChannelEnable_cmd, &amp_i2c, NULL);
    System_Task_Delay(150);

	return 0;
}

int tdf853x_execute_dignostics()
{
    if(tdf853x_disable_channels() == -1){
        tdf853x_enable_channels();
        return -1;
    }

    execute_startup_diagnostics(1);
    System_Task_Delay(1);

    if(tdf853x_enable_channels() == -1){
        return -1;
    }

    return 0;

}

#if (AMP_ENGINEER_CODE_SWITCH == 1)
int tdf853x_set_pwm_ssm(void *data)
{
    uint8_t ssmode = *((uint8_t *)data);

    amp_pwmFreq_config_eng[6] = ssmode;

	if(tdf853x_disable_channels() == -1){
		return -1;
	}

	tdf853x_write_array(&amp_i2c, amp_pwmFreq_config_eng, sizeof(amp_pwmFreq_config_eng));
    System_Task_Delay(1);
	tdf853x_checkMsgError_macro(SetPWMFreq_cmd);

	if(tdf853x_enable_channels() == -1){
		return -1;
	}

	return 0;
}

void tdf853x_set_channel_phase(void *data)
{
    uint8_t *phase = (uint8_t *)data;
    
    amp_channel_config_eng[9] = *phase;
    phase++;
    amp_channel_config_eng[20] = *phase;
    phase++;
    amp_channel_config_eng[31] = *phase;
    phase++;
    amp_channel_config_eng[42] = *phase;
    phase++;

    if(tdf853x_disable_channels() == -1){
		return -1;
	}

	tdf853x_write_array(&amp_i2c, amp_channel_config_eng, sizeof(amp_channel_config_eng));
    System_Task_Delay(1);
	tdf853x_checkMsgError_macro(SetChannelConfig_cmd);

	if(tdf853x_enable_channels() == -1){
		return -1;
	}
    
    return 0;
}
#endif
