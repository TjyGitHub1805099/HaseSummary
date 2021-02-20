#if 0

#ifndef AC_CONTROL_H
#define AC_CONTROL_H

/*=============[I N T E R F A C E  W I T H  A P P L I C A T I O N  S O F T W A R E]=============*/
/************************************************************************************************/
#include "Task_M_IpcApp_Cfg.h" 
/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#define ACCTL_DEBUG_TRACE	STD_ON
/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/
//air-condition control reference file : DLF 591MC V1_3 20180927
#define ACCTL_H_AR_MAJOR_VERSION   1U
#define ACCTL_H_AR_MINOR_VERSION   3U
#define ACCTL_H_AR_PATCH_VERSION   0U

#define ACCTL_H_SW_MAJOR_VERSION   1U
#define ACCTL_H_SW_MINOR_VERSION   0U
#define ACCTL_H_SW_PATCH_VERSION   0U

/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
#define AC2DA_FRAME_HEAD 0X3C
#define AC2DA_FRAME_CMD  0X02
#define AC2DA_FRAME_LEN  16

#define DA2AC_FRAME_HEAD 0X3E
#define DA2AC_FRAME_CMD  0X02
#define DA2AC_FRAME_LEN  12

#define DA_FRAME_HMICMD_LEN		0X03//key_type key_key key_vlu
#define DA_FRAME_CMDHMI_REQUEST_ALL	0X5A


#define ACCT_IPC_HW_CHANNEL		1//SPI1
#define ACCT_IPC_SW_CHANNEL		IPC_GW_CHANNEL_KEYSERVICE//   IPC_GW_CHANNEL_KEYSERVICE = 4,    /* hardkey service */
#define ACCT_IPC_SW_FUNCTION_ID	IPC_SW_KEYSERVICE_FUCTION_ACCT

#define ACCT_IPC_SW_FUNCTION_DATA_LEN	DA_FRAME_HMICMD_LEN
#define ACCT_IPC_SW_CMDHMI_REQUEST_ALL	DA_FRAME_CMDHMI_REQUEST_ALL


#if  0
/*============================[ACP IOC cmd DEFINE]===========================================*/
/* acp ioc cmd define : @[ref]DLF_MC591_20181102.world*/
#define ACP_IOC_AUTO  		0
#define ACP_IOC_AC 			1
#define ACP_IOC_RECIRC 		2
#define ACP_IOC_VOICE 		3
#define ACP_IOC_DEFROST		4
#define ACP_IOC_MODE 		5
#define ACP_IOC_REDEFROST 	6
#define ACP_IOC_ONOFF 		7

#define ACP_IOC_TBV			8
#define ACP_IOC_TGTLTMP 	9
#define ACP_IOC_TGTRTMP 	10

#define ACP_IOC_SYNC 		11
#define ACP_IOC_WPC 		12
#define ACP_IOC_REMOTE		13
/*============================[DATA0: BYTE 4]============================================*/
/* bit	:	7		6			5		4		3			2		1	0	*/
/* fun	:	On/Off	Redefrost	Mode	Defrost	VoiceCmd	Recirc	AC	Auto*/	
#define DA2AC_Auto_BTN_MASK			0x01
#define DA2AC_Auto_BTN_SHIFT		0x00
#define DA2AC_Auto_BTN_IPC_CMD		ACP_IOC_AUTO

#define DA2AC_AC_BTN_MASK			0x02
#define DA2AC_AC_BTN_SHIFT			0x01
#define DA2AC_AC_BTN_IPC_CMD		ACP_IOC_AC

#define DA2AC_Recirc_BTN_MASK		0x04
#define DA2AC_Recirc_BTN_SHIFT		0x02
#define DA2AC_Recirc_BTN_IPC_CMD	ACP_IOC_RECIRC

#define DA2AC_VoiceCmd_BTN_MASK		0x08
#define DA2AC_VoiceCmd_BTN_SHIFT	0x03
#define DA2AC_VoiceCmd_BTN_IPC_CMD	ACP_IOC_VOICE

#define DA2AC_Defrost_BTN_MASK		0x10
#define DA2AC_Defrost_BTN_SHIFT		0x04
#define DA2AC_Defrost_BTN_IPC_CMD	ACP_IOC_DEFROST
				
#define DA2AC_Mode_BTN_MASK			0x20
#define DA2AC_Mode_BTN_SHIFT		0x05
#define DA2AC_Mode_BTN_IPC_CMD		ACP_IOC_MODE

#define DA2AC_Redefrost_BTN_MASK	0x40
#define DA2AC_Redefrost_BTN_SHIFT	0x06
#define DA2AC_Redefrost_BTN_IPC_CMD	ACP_IOC_REDEFROST

#define DA2AC_On_Off_BTN_MASK		0x80
#define DA2AC_On_Off_BTN_SHIFT		0x07
#define DA2AC_On_Off_BTN_IPC_CMD	ACP_IOC_ONOFF

/*============================[DATA1: BYTE 5]============================================*/
/* bit	:	7 6 5						4 			3			2			1			0	*/
/* fun	:	Target Mode Value			reserved	reserved	reserved	reserved	reserved*/	
#define DA2AC_Mode_Vlu_MASK			0xE0
#define DA2AC_Mode_Vlu_SHIFT		0x05
#define DA2AC_Mode_Vlu_IPC_CMD		ACP_IOC_MODE

/*============================[DATA2: BYTE 6]============================================*/
/* bit	:	7 6 5 4 								3				2				1			0	*/
/* fun	:	Target blower value						BlowerTSplus	BlowerTSminus	reserved	reserved*/	
#define DA2AC_Blower_Vlu_MASK		0xF0
#define DA2AC_Blower_Vlu_SHIFT		0x04
#define DA2AC_Blower_Vlu_IPC_CMD	ACP_IOC_TBV

/*============================[DATA3: BYTE 7]============================================*/
/* bit	:	7 6 5 4 3 2 1 0	*/
/* fun	:	TargetLTemp_TS 	*/	
#define DA2AC_LTemp_Vlu_MASK		0xFF
#define DA2AC_LTemp_Vlu_SHIFT		0x00
#define DA2AC_LTemp_Vlu_IPC_CMD		ACP_IOC_TGTLTMP

/*============================[DATA4: BYTE 8]============================================*/
/* bit	:	7 6 5 4 3 2 1 0	*/
/* fun	:	TargetRTemp_TS 	*/	
#define DA2AC_RTemp_Vlu_MASK		0xFF
#define DA2AC_RTemp_Vlu_SHIFT		0x00
#define DA2AC_RTemp_Vlu_IPC_CMD		ACP_IOC_TGTRTMP

/*============================[DATA5: BYTE 9]============================================*/
/* bit	:	7				6				5				4				3		2		1					0	*/
/* fun	:	Rtemp_TS plus	Rtemp_TS minus	Ltemp_TS plus	Ltemp_TS minus	Reserve	Remote	Wirelss Setting CMD	SYNC*/	
#define DA2AC_SYNC_BTN_MASK			0x01
#define DA2AC_SYNC_BTN_SHIFT		0x00
#define DA2AC_SYNC_BTN_IPC_CMD		ACP_IOC_SYNC

#define DA2AC_WPC_BTN_MASK			0x02
#define DA2AC_WPC_BTN_SHIFT			0x01
#define DA2AC_WPC_BTN_IPC_CMD		ACP_IOC_WPC

#define DA2AC_REMOTE_CMD_MASK		0x02
#define DA2AC_REMOTE_CMD_SHIFT		0x01
#define DA2AC_REMOTE_CMD_IPC_CMD	ACP_IOC_REMOTE

#endif
/*============================[T Y P E  D E F I N I T I O N]====================================*/
typedef struct{
	uint8 key_key;
	uint8 key_pos;
	uint8 key_shift;
	uint8 key_mask;
}AcPanelMapStruct;

typedef struct {
    uint8 key_type;
    uint8 key_key;
    uint8 key_vlu;
} AcDaIpcStruct;

typedef struct{
	uint8 key_key;
	uint8 key_pos;
	uint8 key_shift;
	uint8 key_mask;
	uint8 key_pos2;
	uint8 key_shift2;
	uint8 key_mask2;
}DaKeyMapStruct;

/* AC frame info : key */
typedef enum
{
	AC_KEY_NONE=0,
	AC_KEY_LTMP_ADD=1,
	AC_KEY_LTMP_DEC=2,
	AC_KEY_BLOWER_ADD=3,
	AC_KEY_BLOWER_DEC=4,
	AC_KEY_MODE=5,
	AC_KEY_ON_OFF=6,
	AC_KEY_CYCLE=7,
	AC_KEY_AUTO=8,
	AC_KEY_DEFROST=9,
	AC_KEY_REDEFROST=10,
	AC_KEY_AC=11,
	AC_KEY_RTMP_ADD=12,
	AC_KEY_RTMP_DEC=13,
	AC_KEY_SYNC=14,
	AC_KEY_VOICE_FB=15,
	AC_KEY_RECIRC=16,
	AC_KEY_FRESH=17,
	AC_KEY_DISPLAY=18,
	AC_KEY_CHAGE=19,
	AC_KEY_CHAGE_STATUS=20,
	AC_KEY_END
} AC_KeyIndexType;

/* AC frame info : enum */
typedef enum
{
	AC_HEADER=0,/*AC_DATA_0 = 0x00u, FRAME HEADER*/
	AC_ID,/*AC_DATA_1, FRAME ID*/
	AC_DATA_LEN,/*AC_DATA_2, DATA LEN */
	AC_DATA_3,
	AC_DATA_4,
	AC_DATA_5,
	AC_DATA_6,
	AC_DATA_7,
	AC_DATA_8,
	AC_DATA_9,
	AC_DATA_10,
	AC_DATA_11,
	AC_DATA_12,
	AC_DATA_13,
	AC_DATA_14,
	AC_ADDC,/*AC_DATA_15, DATA verify */
	AC_FRAME_END,
} AC_FrameIndexType;
/************************************************************************************************/
/* DA frame info : enum */
typedef enum
{
	DA_HEADER = 0x00u,
	DA_ID,
	DA_DATA_LEN,
	DA_DATA_3,
	DA_DATA_4,
	DA_DATA_5,
	DA_DATA_6,
	DA_DATA_7,
	DA_DATA_8,
	DA_DATA_9,
	DA_DATA_10,
	DA_ADDC,/* DATA verify */
	DA_FRAME_END,
} DA_FrameIndexType;
/* DA frame info : struct */
typedef enum
{
	CMD_KEY_TYPE = 0x00u,
	CMD_KEY_KEY,
	CMD_KEY_VLU
} DA_HmiCmdType;


/* AC frame info : struct */
typedef struct
{
	uint8 dealDone;
	uint8 newData;
	uint8 preData[AC2DA_FRAME_LEN];/* frame header(1) + info identify(1) + Effective data length(1) + Effective data(7) + adc add with carry(1)*/
	
	uint8 curData[AC2DA_FRAME_LEN];/* frame header(1) + info identify(1) + Effective data length(1) + Effective data(7) + adc add with carry(1)*/
	uint8 rxData[AC2DA_FRAME_LEN];/* frame header(1) + info identify(1) + Effective data length(1) + Effective data(7) + adc add with carry(1)*/

	uint8 txData[DA2AC_FRAME_LEN];
} AC_FrameInfoType;

typedef struct
{
	uint8 newCmd;
	uint8 rxHMICmd[DA_FRAME_HMICMD_LEN];//key_type key_key key_vlu
	uint8 txHMICmd[DA_FRAME_HMICMD_LEN];//key_type key_key key_vlu

	uint8 cmdRequestall;//da request all ac panel status
	uint8 cmdRequestall_i;//index
} DA_FrameInfoType;


extern AC_FrameInfoType acFrame;
extern DA_FrameInfoType daFrame;
/************************************************************************************************/
/*
 * Function Name       NdsCanSm_MainFunction
 * Brief               Returns the state and the mode of the network management.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
 /************************************************************************************************/
void AcCtl_MainFunction();

/*=================================[IOC MONITOR WITH SOC]=======================================*/
/************************************************************************************************/
#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)

#define IOC_MONITOR_SOC_IPC_HW_CHANNEL		1//SPI1
#define IOC_MONITOR_SOC_IPC_SW_CHANNEL		IPC_GW_CHANNEL_KEYSERVICE//   IPC_GW_CHANNEL_KEYSERVICE = 4,    /* hardkey service */
#define IOC_MONITOR_SOC_IPC_SW_FUNCTION_ID	IPC_SW_KEYSERVICE_FUCTION_MONITOR//IPC_SW_KEYSERVICE_FUCTION_MONITOR = 65 ,ioc monitor with soc function ID
#define IOC_MONITOR_SOC_DATA_LEN			3

typedef struct
{
	uint8 socPowerOn;

	uint8 newCmd;
	uint8 rxHMICmd[IOC_MONITOR_SOC_DATA_LEN];
	uint8 txHMICmd[IOC_MONITOR_SOC_DATA_LEN];

	uint16 taskCycle,monitorCycle,monitorCycleTimeOut;
} IocMonitorSoc_InfoType;

extern IocMonitorSoc_InfoType IocMonitorSocFrame;
extern void IocMonitorWithSoc(void);

#endif
/*=================================[IOC MONITOR WITH SOC]=======================================*/
/************************************************************************************************/

#endif /* AC_CONTROL_H */

#endif
