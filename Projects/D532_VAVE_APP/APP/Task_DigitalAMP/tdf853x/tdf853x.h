/*
 * tdf853x.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_TDF853X_TDF853X_H_
#define TASK_AUDIO_TDF853X_TDF853X_H_

#include "tdf853x_instance.h"
#include "tdf853x_config.h"

/* i2c frame length */
#define TDF853X_I2C_MAX_PAYLOAD_LENGTH 64

/* i2c message protocol */
/* <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>] */

/* MsgType */
#define STX 0x02 // single packet message
#define SOH 0x01 // start of a multi packet message
#define ETB 0x17 // intermediate packet of a multi packet message
#define ETX 0x03 // last packet of a multi packet message
#define ACK 0x06 //
#define NAK 0x15 //
#define ERR 0x18 //

/* POSITION */
#define MSGTYPE 0
#define PACKETID 1
#define LENGTH 2
#define MODULEID 3
#define CMDID 4
#define PARAMETERS 5

/* moduleId */
#define AMPLIFIER_MODULEID 0x80

/* configuration and control command */
#define SetDiagPin_cmd 0x10
#define SetDeviceConfig_cmd 0x12
#define SetDeviceIOConfig_cmd 0x14
#define SetPWMFreq_cmd 0x16
#define SetClockState_cmd 0x1A
#define SetAudioConfig_cmd 0x1C
#define SetModSchemeConfig_cmd 0x20
#define SetDCLoadDetectionConfig_cmd 0x22
#define SetChannelConfig_cmd 0x24
#define SetChannelEnable_cmd 0x26
#define SetChannelDisable_cmd 0x27
#define SetAudioRoutingConfig_cmd 0x28
#define SetLCFilterConfig_cmd 0x2A
#define SetStartupDiag_cmd 0x32
#define SetChannelMute_cmd  0x42
#define SetChannelUnMute_cmd 0x43
#define SetFilterDiagnostic_cmd 0x8B
#define Reboot_cmd 0xF8

/* read command */
#define GetDeviceState_req 0x80
#define GetDeviceState_repl 0x80
#define GetFaultStatus_req 0x81
#define GetFaultStatus_repl 0x81
#define GetSupplyVoltage_req 0x82
#define GetSupplyVoltage_repl 0x82
#define GetTemperature_req 0x84
#define GetTemperature_repl 0x84
#define GetStartupDiagStatus_req 0x85
#define GetStartupDiagStatus_repl 0x85
#define GetDCLoadDetectionResult_req 0x86
#define GetDCLoadDetectionResult_repl 0x86
#define GetACLoadDetectionResult_req 0x87
#define GetACLoadDetectionResult_repl 0x87
#define GetChannelStatus_req 0x88
#define GetChannelStatus_repl 0x88
#define GetFilterDiagnosticResult_req 0x8A
#define GetFilterDiagnosticResult_repl 0x8A
#define GetIdentification_req 0xE0
#define GetIdentification_repl 0xE0
#define GetMsgErrorStatus_req 0xE2
#define GetMsgErrorStatus_repl 0xE2

/* SetClockState_cmd */
#define SetClockState_Command 2

#define SetClockState_Disconnect 0x00
#define SetClockState_Connect 0x01

/* SetAudioConfig_cmd */
#define SetAudioConfig_SamplingFrequency_pos 3
#define SetAudioConfig_FrameSize_pos 4
#define SetAudioConfig_SlotSize_pos 5
#define SetAudioConfig_TDMFrameSync_pos 6
#define SetAudioConfig_BitClockDelay_pos 7

#define SetAudioConfig_SamplingFrequency_44 0x00
#define SetAudioConfig_SamplingFrequency_48 0x01
#define SetAudioConfig_SamplingFrequency_96 0x02
#define SetAudioConfig_SamplingFrequency_192 0x03
#define SetAudioConfig_FrameSize_2_slots 0x00
#define SetAudioConfig_FrameSize_4_slots 0x01
#define SetAudioConfig_FrameSize_8_slots 0x02
#define SetAudioConfig_FrameSize_16_slots 0x03
#define SetAudioConfig_SlotSize_16bits 0x00
#define SetAudioConfig_SlotSize_32bits 0x04
#define SetAudioConfig_TDMFrameSync_Falling 0x00
#define SetAudioConfig_BitClockDelay_NoDelay 0x00
#define SetAudioConfig_BitClockDelay_OneBitDelay 0x01

/* SetChannelEnable_cmd */
#define SetChannelEnable_NOCHANGE 0
#define SetChannelEnable_ENABLE 1
#define CHANNEL1_ENABLE_BIT_OFFSET 0
#define CHANNEL2_ENABLE_BIT_OFFSET 1
#define CHANNEL3_ENABLE_BIT_OFFSET 2
#define CHANNEL4_ENABLE_BIT_OFFSET 3

/* SetChannelDisable_cmd */
#define SetChannelDisable_NOCHANGE 0
#define SetChannelDisable_DISABLE 1
#define CHANNEL1_DISABLE_BIT_OFFSET 0
#define CHANNEL2_DISABLE_BIT_OFFSET 1
#define CHANNEL3_DISABLE_BIT_OFFSET 2
#define CHANNEL4_DISABLE_BIT_OFFSET 3

/* SetChannelMute_cmd */
#define SetChannelMute_MUTE 1

/* GetDeviceState_repl */
#define GetDeviceState_repl_DeviceState 2

#define GetDeviceState_repl_State_Boot 0x00
#define GetDeviceState_repl_State_Idle 0x01
#define GetDeviceState_repl_State_Standby 0x02
#define GetDeviceState_repl_State_Loaddiag 0x03
#define GetDeviceState_repl_State_Playing 0x04
#define GetDeviceState_repl_State_Protected 0x05
#define GetDeviceState_repl_State_Shutdown 0x06
#define GetDeviceState_repl_State_ClockFail 0x07

/* GetFaultStatus_repl */
#define GetFaultStatus_repl_byte1 2
#define GetFaultStatus_repl_byte2 3

/* GetTemperature_repl */
#define GetTemperature_repl_Temperature1 0x02
#define GetTemperature_repl_Temperature2 0x03

/* GetSupplyVoltage_repl */

/* GetStartupDiagStatus_repl */
#define GetStartupDiagStatus_repl_DCLoadDetectionStatus 2
#define GetStartupDiagStatus_repl_ACLoadDetectionStatus 3
#define GetStartupDiagStatus_repl_StartupDiagnosticStatus 4

#define GetStartupDiagStatus_repl_DC_NOT_START 0x00
#define GetStartupDiagStatus_repl_DC_IN_PROGRESS 0x01
#define GetStartupDiagStatus_repl_DC_COMPLETE 0x02

#define GetStartupDiagStatus_repl_AC_NOT_START 0x00
#define GetStartupDiagStatus_repl_AC_IN_PROGRESS 0x01
#define GetStartupDiagStatus_repl_AC_COMPLETE 0x02

#define GetStartupDiagStatus_repl_Diagnostic_NOT_START 0x00
#define GetStartupDiagStatus_repl_Diagnostic_IN_PROGRESS 0x01
#define GetStartupDiagStatus_repl_Diagnostic_COMPLETE 0x02
#define GetStartupDiagStatus_repl_Diagnostic_COMPLETE_UNSUCCESS 0x03

/* GetDCLoadDetectionResult_repl */
#define GetDCLoadDetectionResult_repl_Channels 2

#define GetDCLoadDetectionResult_repl_DCLoadCondition_Unknow 0x00
#define GetDCLoadDetectionResult_repl_DCLoadCondition_ShortLoad 0x01
#define GetDCLoadDetectionResult_repl_DCLoadCondition_ShortSupply 0x02
#define GetDCLoadDetectionResult_repl_DCLoadCondition_ShortExternalVoltage 0x03
#define GetDCLoadDetectionResult_repl_DCLoadCondition_ShortGround 0x04
#define GetDCLoadDetectionResult_repl_DCLoadCondition_NormalLoad 0x05
#define GetDCLoadDetectionResult_repl_DCLoadCondition_OpenLoad 0x06

/* GetACLoadDetectionResult_repl */
#define GetACLoadDetectionResult_repl_Channels 2

#define GetACLoadDetectionResult_repl_ACLoadCondition_Unknow 0x00
#define GetACLoadDetectionResult_repl_ACLoadCondition_NormalLoad 0x01
#define GetACLoadDetectionResult_repl_ACLoadCondition_OpenLoad 0x02

/* GetChannelStatus_repl */
#define GetChannelStatus_repl_Channels 2

/* GetFilterDiagnosticResult_repl */
#define GetFilterDiagnosticResult_repl_Channels 2

/* GetIdentification_repl */
#define GetIdentification_repl_TypeName 2
#define GetIdentification_repl_HWVersionMajor 3
#define GetIdentification_repl_HWVersionMinor 4
#define GetIdentification_repl_SWVersionMajor 5
#define GetIdentification_repl_SWVersionMinor 6
#define GetIdentification_repl_SWVersionSub 7

#define GetIdentification_repl_TypeName_TDF8534HH 0x00
#define GetIdentification_repl_TypeName_TDF8531HH 0x02
#define GetIdentification_repl_TypeName_TDF8532HH 0x04
#define GetIdentification_repl_TypeName_TDF8536HH 0x08
#define GetIdentification_repl_TypeName_TDF8534AHH 0x10
#define GetIdentification_repl_TypeName_TDF8531AHH 0x12
#define GetIdentification_repl_TypeName_TDF8532AHH 0x14
#define GetIdentification_repl_TypeName_TDF8536AHH 0x18

#define GetIdentification_repl_HWVersionMajor_N1 0x01
#define GetIdentification_repl_HWVersionMajor_N2 0x02
#define GetIdentification_repl_HWVersionMajor_N3 0x03

#define GetIdentification_repl_HWVersionMinor_00 0x00

/* GetMsgErrorStatus_repl */
#define GetMsgErrorStatus_repl_LatestCommandId 2
#define GetMsgErrorStatus_repl_Result 3

/* amp id */
typedef struct{
	uint8_t amp_typename;
	uint8_t amp_hwVersionMajor;
	uint8_t amp_hwVersionMinor;
	uint8_t amp_swVersionMajor;
	uint8_t amp_swVersionMinor;
	uint8_t amp_swVersionSub;
} amp_id_t;

/* audio config */
typedef struct{
	uint8_t samplingFrequency;
	uint8_t frameSize;
	uint8_t slotSize;
	uint8_t TDMFrameSync;
	uint8_t bitClockDelay;
} amp_audio_config_t;

typedef struct{
	uint8_t NrOfChannels;
	struct{
		uint8_t ChannelNr;
		uint8_t LoadCondition;
	} result[5];
} amp_LoadDetectionResult;

typedef struct{
	uint8_t DCLoadDetectionStatus;
	uint8_t ACLoadDetectionStatus;
	uint8_t StartupDiagnosticStatus;
} amp_StartupDiagStatus;

typedef struct{
	uint8_t NrOfChannels;
	struct{
		uint8_t ChannelNr;
		uint8_t error_byte;
	} result[5];
} amp_ChannelStatus;

typedef struct{
	uint8_t NrOfChannels;
	struct{
		uint8_t ChannelNr;
		uint8_t kvalue;
		uint8_t kmin;
		uint8_t kmax;
		uint8_t status;
	} result[5];
} amp_FilterDiagnosticResult;

/* LC filter diagnostics error */
#define LC_FILTER_DIAGNOSTICS_ERROR_OTHER 0
#define LC_FILTER_DIAGNOSTICS_ERROR_NOT_OPEN_LOAD 1
#define LC_FILTER_DIAGNOSTICS_ERROR_TEMPERATURE 2
#define LC_FILTER_DIAGNOSTICS_ERROR_VOLTAGE 3
#define LC_FILTER_DIAGNOSTICS_ERROR_WRITE_E2P 4

int tdf853x_read(int cmd_id, tdf853x_instance* instance, void *context_var, void *result);
int tdf853x_write(int cmd_id, tdf853x_instance* instance, void *context_var);
int tdf853x_write_array(tdf853x_instance* instance, void *buf, int length);
#endif /* TASK_AUDIO_TDF853X_TDF853X_H_ */
