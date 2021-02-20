/*
 * tdf853x.c
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: gaojing
 */
#include "Std_Types.h"
#include "tdf853x_instance.h"

#include "tdf853x.h"

typedef int (*prepare_buffer)(uint8_t* target_buf, int* length, void *context_var);
typedef int (*process_result)(uint8_t* buf, int* length, void *context_var, void *result);

typedef struct{
	int cmd_id;
	prepare_buffer prepare_buffer_func;
	process_result process_result_func;
}tdf853x_read_command_t;

typedef struct{
	int cmd_id;
	prepare_buffer prepare_buffer_func;
}tdf853x_write_command_t;

/*
 * section: Configuration commands
 */

/* SetAudioConfig_cmd */
static int tdf853x_SetAudioConfig_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	amp_audio_config_t *config = (amp_audio_config_t *)context_var;

	int tmp_length;
	uint8_t buf[6];

	tmp_length = 0;

	*length = 0;

	/*  */
	buf[0] = 0x00;
	buf[1] = config->samplingFrequency;
	buf[2] = config->frameSize;
	buf[3] = config->slotSize;
	buf[4] = config->TDMFrameSync;
	buf[5] = config->bitClockDelay;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetAudioConfig_cmd, buf, 6, target_buf);
	/* no parameters */
	tmp_length += 2 + 6;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}

/*
 * section: Control commands
 */

/* SetClockState_cmd */
static int tdf853x_SetClockState_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/*  */
	buf[0] = *((uint8_t *)context_var);

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetClockState_cmd, buf, 1, target_buf);
	/* no parameters */
	tmp_length += 2 + 1;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}

/* SetChannelEnable_cmd */
static int tdf853x_SetChannelEnable_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/* disable all channel */
	buf[0] = SetChannelEnable_ENABLE << CHANNEL1_ENABLE_BIT_OFFSET |
			SetChannelEnable_ENABLE << CHANNEL2_ENABLE_BIT_OFFSET |
			SetChannelEnable_ENABLE << CHANNEL3_ENABLE_BIT_OFFSET |
			SetChannelEnable_ENABLE << CHANNEL4_ENABLE_BIT_OFFSET;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetChannelEnable_cmd, buf, 1, target_buf);
	/* no parameters */
	tmp_length += 2 + 1;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}

/* SetChannelDisable_cmd */
static int tdf853x_SetChannelDisable_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/* disable all channel */
	buf[0] = SetChannelDisable_DISABLE << CHANNEL1_DISABLE_BIT_OFFSET |
			SetChannelDisable_DISABLE << CHANNEL2_DISABLE_BIT_OFFSET |
			SetChannelDisable_DISABLE << CHANNEL3_DISABLE_BIT_OFFSET |
			SetChannelDisable_DISABLE << CHANNEL4_DISABLE_BIT_OFFSET;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetChannelDisable_cmd, buf, 1, target_buf);
	/* no parameters */
	tmp_length += 2 + 1;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}

/* SetChannelMute_cmd */
static int tdf853x_SetChannelMute_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/* mute all channel */
	buf[0] = 0x0F;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetChannelMute_cmd, buf, 1, target_buf);
	/* no parameters */
	tmp_length += 2 + 1;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}

/* SetChannelUnMute_cmd */
static int tdf853x_SetChannelUnMute_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/* mute all channel */
	buf[0] = 0x0F;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, SetChannelUnMute_cmd, buf, 1, target_buf);
	/* no parameters */
	tmp_length += 2 + 1;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}


/* Reboot_cmd */
static int tdf853x_Reboot_cmd(uint8_t* target_buf, int* length, void *context_var)
{
	int tmp_length;
	uint8_t buf[1];

	tmp_length = 0;

	*length = 0;

	/* <moduleId><CmdId><Parameters> */
	tdf853x_make_dll_payload(AMPLIFIER_MODULEID, Reboot_cmd, buf, 0, target_buf);
	/* no parameters */
	tmp_length += 2 + 0;
	/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
	tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
	tmp_length += 3;

	*length = tmp_length;

	return 0;
}


/*
 * section: read commands
 */

/* GetDeviceState */
static int tdf853x_GetDeviceState_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetDeviceState_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetDeviceState_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	*((int *)result) = buf[GetDeviceState_repl_DeviceState + MODULEID];
	return 0;
}

/* GetFaultStatus_req */
static int tdf853x_GetFaultStatus_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetFaultStatus_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetFaultStatus_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	uint8_t *data = (uint8_t *)result;

	*data = buf[GetFaultStatus_repl_byte1 + MODULEID];
	data++;
	*data = buf[GetFaultStatus_repl_byte2 + MODULEID];

	return 0;
}

/* GetSupplyVoltage_req */
static int tdf853x_GetSupplyVoltage_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetSupplyVoltage_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetSupplyVoltage_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	*((int *)result) = (buf[GetTemperature_repl_Temperature1 + MODULEID] << 8) + buf[GetTemperature_repl_Temperature2 + MODULEID];
	return 0;
}

/* GetTemperature_req */
static int tdf853x_GetTemperature_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetTemperature_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetTemperature_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	*((double *)result) = ((buf[GetTemperature_repl_Temperature1 + MODULEID] << 8) + buf[GetTemperature_repl_Temperature2 + MODULEID]) / 100.0;
	return 0;
}



/* GetStartupDiagStatus */
static int tdf853x_GetStartupDiagStatus_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetStartupDiagStatus_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetStartupDiagStatus_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	(*((amp_StartupDiagStatus *)result)).DCLoadDetectionStatus = buf[GetStartupDiagStatus_repl_DCLoadDetectionStatus + MODULEID];
	(*((amp_StartupDiagStatus *)result)).ACLoadDetectionStatus = buf[GetStartupDiagStatus_repl_ACLoadDetectionStatus + MODULEID];
	(*((amp_StartupDiagStatus *)result)).StartupDiagnosticStatus = buf[GetStartupDiagStatus_repl_StartupDiagnosticStatus + MODULEID];
	return 0;
}

/* GetDCLoadDetectionResult */
static int tdf853x_GetDCLoadDetectionResult_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetDCLoadDetectionResult_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetDCLoadDetectionResult_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	int i = 0;

	amp_LoadDetectionResult *dcload_result = (amp_LoadDetectionResult *)result;

	dcload_result->NrOfChannels = buf[GetDCLoadDetectionResult_repl_Channels + MODULEID];

	uint8_t *pt = &buf[GetDCLoadDetectionResult_repl_Channels + MODULEID + 1];

	for(i = 0; i < dcload_result->NrOfChannels; i++){
		dcload_result->result[i].ChannelNr = *(pt + 0);
		dcload_result->result[i].LoadCondition = *(pt + 1);
		pt += 7;
	}

	return 0;
}

/* GetACLoadDetectionResult */
static int tdf853x_GetACLoadDetectionResult_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetACLoadDetectionResult_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetACLoadDetectionResult_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	int i = 0;

	amp_LoadDetectionResult *acload_result = (amp_LoadDetectionResult *)result;

	acload_result->NrOfChannels = buf[GetACLoadDetectionResult_repl_Channels + MODULEID];

	uint8_t *pt = &buf[GetACLoadDetectionResult_repl_Channels + MODULEID + 1];

	for(i = 0; i < acload_result->NrOfChannels; i++){
		acload_result->result[i].ChannelNr = *(pt + 0);
		acload_result->result[i].LoadCondition = *(pt + 1);
		pt += 8;
	}

	return 0;
}

/* GetChannelStatus */
static int tdf853x_GetChannelStatus_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetChannelStatus_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetChannelStatus_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	int i = 0;

	amp_ChannelStatus *channel_status_result = (amp_ChannelStatus *)result;

	channel_status_result->NrOfChannels = buf[GetChannelStatus_repl_Channels + MODULEID];

	uint8_t *pt = &buf[GetChannelStatus_repl_Channels + MODULEID + 1];

	for(i = 0; i < channel_status_result->NrOfChannels; i++){
		channel_status_result->result[i].ChannelNr = *(pt + 0);
		channel_status_result->result[i].error_byte = *(pt + 4);
		pt += 6;
	}

	return 0;
}

/* GetFilterDiagnosticResult */
static int tdf853x_GetFilterDiagnosticResult_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetFilterDiagnosticResult_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetFilterDiagnosticResult_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	int i = 0;

	amp_FilterDiagnosticResult *filter_diagnostic_result = (amp_FilterDiagnosticResult *)result;

	filter_diagnostic_result->NrOfChannels = buf[GetFilterDiagnosticResult_repl_Channels + MODULEID];

	uint8_t *pt = &buf[GetFilterDiagnosticResult_repl_Channels + MODULEID + 1];

	for(i = 0; i < filter_diagnostic_result->NrOfChannels; i++){
		filter_diagnostic_result->result[i].ChannelNr = *(pt + 0);
		filter_diagnostic_result->result[i].kvalue = *(pt + 1);
		filter_diagnostic_result->result[i].kmin = *(pt + 2);
		filter_diagnostic_result->result[i].kmax = *(pt + 3);
		filter_diagnostic_result->result[i].status = *(pt + 4);
		pt += 5;
	}

	return 0;
}

/* GetIdentification */
static int tdf853x_GetIdentification_req(uint8_t* target_buf, int* length, void *context_var)
{
    int tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetIdentification_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetIdentification_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	amp_id_t *id = (amp_id_t *)result;
	id->amp_typename = buf[GetIdentification_repl_TypeName + MODULEID];
	id->amp_hwVersionMajor = buf[GetIdentification_repl_HWVersionMajor + MODULEID];
	id->amp_hwVersionMinor = buf[GetIdentification_repl_HWVersionMinor + MODULEID];
	id->amp_swVersionMajor = buf[GetIdentification_repl_SWVersionMajor + MODULEID];
	id->amp_swVersionMinor = buf[GetIdentification_repl_SWVersionMinor + MODULEID];
	id->amp_swVersionSub = buf[GetIdentification_repl_SWVersionSub + MODULEID];
	return 0;
}

/* GetMsgErrorStatus */
static int tdf853x_GetMsgErrorStatus_req(uint8_t* target_buf, int* length, void *context_var)
{
    uint8_t tmp_length;
    uint8_t buf[1];

    tmp_length = 0;

    *length = 0;

    /* <moduleId><CmdId><Parameters> */
    tdf853x_make_dll_payload(AMPLIFIER_MODULEID, GetMsgErrorStatus_req, buf, 0, target_buf);
    /* no parameters */
    tmp_length += 2 + 0;
    /* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */
    tdf853x_make_i2c_payload(STX, 0, tmp_length, target_buf);
    tmp_length += 3;

    *length = tmp_length;

    return 0;
}

static int tdf853x_GetMsgErrorStatus_repl(uint8_t* buf, int* length, void *context_var, void *result)
{
	uint8_t *result_data = result;
	*result_data = buf[GetMsgErrorStatus_repl_LatestCommandId + MODULEID];
	result_data++;
	*result_data = buf[GetMsgErrorStatus_repl_Result + MODULEID];
	return 0;
}



/*
 * section: command table
 */
tdf853x_read_command_t tdf853x_read_commands[] = {
		{GetDeviceState_req, tdf853x_GetDeviceState_req, tdf853x_GetDeviceState_repl},
		{GetFaultStatus_req, tdf853x_GetFaultStatus_req, tdf853x_GetFaultStatus_repl},
		{GetTemperature_req, tdf853x_GetTemperature_req, tdf853x_GetTemperature_repl},
		{GetSupplyVoltage_req, tdf853x_GetSupplyVoltage_req, tdf853x_GetSupplyVoltage_repl},
		{GetStartupDiagStatus_req, tdf853x_GetStartupDiagStatus_req, tdf853x_GetStartupDiagStatus_repl},
		{GetDCLoadDetectionResult_req, tdf853x_GetDCLoadDetectionResult_req, tdf853x_GetDCLoadDetectionResult_repl},
		{GetACLoadDetectionResult_req, tdf853x_GetACLoadDetectionResult_req, tdf853x_GetACLoadDetectionResult_repl},
		{GetChannelStatus_req, tdf853x_GetChannelStatus_req, tdf853x_GetChannelStatus_repl},
		{GetFilterDiagnosticResult_req, tdf853x_GetFilterDiagnosticResult_req, tdf853x_GetFilterDiagnosticResult_repl},
		{GetIdentification_req, tdf853x_GetIdentification_req, tdf853x_GetIdentification_repl},
		{GetMsgErrorStatus_req, tdf853x_GetMsgErrorStatus_req, tdf853x_GetMsgErrorStatus_repl},
};

tdf853x_write_command_t tdf853x_write_commands[] = {
		{SetClockState_cmd, tdf853x_SetClockState_cmd},
		{SetAudioConfig_cmd, tdf853x_SetAudioConfig_cmd},
		{SetChannelDisable_cmd, tdf853x_SetChannelDisable_cmd},
		{SetChannelEnable_cmd, tdf853x_SetChannelEnable_cmd},
		{SetChannelMute_cmd, tdf853x_SetChannelMute_cmd},
		{SetChannelUnMute_cmd, tdf853x_SetChannelUnMute_cmd},
		{Reboot_cmd, tdf853x_Reboot_cmd},
};

/*
 * section: read write function
 */
int tdf853x_read(int cmd_id, tdf853x_instance* instance, void *context_var, void *result)
{
    int ret;
    int length = 0;
    int i = 0;

    prepare_buffer prepare_buffer_func = NULL;
    process_result process_result_func = NULL;
    int command_found = 0;

    for(i = 0; i < (sizeof(tdf853x_read_commands) / sizeof(tdf853x_read_command_t)); i++){
    	if(cmd_id == tdf853x_read_commands[i].cmd_id){
    		prepare_buffer_func = tdf853x_read_commands[i].prepare_buffer_func;
    		process_result_func = tdf853x_read_commands[i].process_result_func;
    		command_found = 1;
    		break;
    	}
    }

    if(command_found == 0){
    	return -1;
    }

    /* max length for one i2c packet in tdf853x is 64bytes */
    uint8_t buffer[TDF853X_I2C_MAX_PAYLOAD_LENGTH];

    /* get command data */
    length = 0;
    if(prepare_buffer_func != NULL){
    	prepare_buffer_func(buffer, &length, context_var);
    }
    if(length <= 0){
        return -1;
    }

    ret = tdf853x_read_command(instance, buffer, &length);
    if(ret != 0){
        return -1;
    }

    if(process_result_func != NULL){
    	process_result_func(buffer, &length, context_var, result);
    }

    return 0;
}

int tdf853x_write(int cmd_id, tdf853x_instance* instance, void *context_var)
{
    int ret;
    int length = 0;
    int i = 0;

    prepare_buffer prepare_buffer_func = NULL;
    int command_found = 0;

    for(i = 0; i < (sizeof(tdf853x_write_commands) / sizeof(tdf853x_write_command_t)); i++){
    	if(cmd_id == tdf853x_write_commands[i].cmd_id){
    		prepare_buffer_func = tdf853x_write_commands[i].prepare_buffer_func;
    		command_found = 1;
    		break;
    	}
    }

    if(command_found == 0){
    	return -1;
    }

    /* max length for one i2c packet in tdf853x is 64bytes */
    uint8_t buffer[TDF853X_I2C_MAX_PAYLOAD_LENGTH];

    /* get command data */
    length = 0;
    if(prepare_buffer_func != NULL){
    	prepare_buffer_func(buffer, &length, context_var);
    }
    if(length <= 0){
        return -1;
    }

    ret = tdf853x_write_command(instance, buffer, length);
    if(ret != 0){
        return -1;
    }

    return ret;
}

int tdf853x_write_array(tdf853x_instance* instance, void *buf, int length)
{
    int ret;

    ret = tdf853x_write_command(instance, buf, length);
    if(ret != 0){
        return -1;
    }

    return ret;
}
