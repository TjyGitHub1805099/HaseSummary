/*
 * SecondPanel.h
 *
 *  Created on: 20190720
 *      Author: huangyonglin
 */
#ifndef MIDDLEWARE_SECONDPANEL_SECONDPANEL_H_
#define MIDDLEWARE_SECONDPANEL_SECONDPANEL_H_

#include "uart_acpanel_cfg.h"
#include "CircularQueue.h"
#include "System_Mutex.h"
#include "Com_Cfg.h"
#include "trace.h"
#include "ExternRTC.h"

#define PANEL_UART_CH	INST_UART_ACPANEL_CFG
#define UART_PANEL_CQUEUE_BUFF_LEN	128
#define PANEL_RX_RAW_DATA_BUFF_LEN	64
#define SEND_DATA_CODING_LEN  5
#define CYCLE_UPADATE_DATA_NUM  7
#define RECV_DATA_NUM  10

#define SECONDPANEL_IPC_SW_OUTSIDEPM25_FUNCTION_ID	   IPC_SW_SYSTEM_FUCTION_OUTSIDEPM25 
#define SECONDPANEL_IPC_SW_OUTSIDETEMP_FUNCTION_ID	   IPC_SW_SYSTEM_FUCTION_OUTSIDETEMP 
#define SECONDPANEL_IPC_SW_LIGHTCTL_FUNCTION_ID    IPC_SW_SYSTEM_FUCTION_LIGHTCTL  

typedef enum
{
	SecondPanel_Outside = 0,
	SecondPanel_Inside,
	SecondPanel_LocaTypeMax
}LocaType;

typedef enum
{
	SecondPanel_TurnOn = 0,
	SecondPanel_TurnOff,
	SecondPanel_SwitchTypeMax
}SwitchType;

typedef enum
{
	SecondPanel_Pix100 = 0,
	SecondPanel_Pix70,
	SecondPanel_LightnessMax
}Lightness;

typedef enum
{
	SecondPanel_ValidOutsidePM25 = 0,
	SecondPanel_ValidInsidePM25,
	SecondPanel_ValidOutsideTemp,
	SecondPanel_ValidInsideTemp,
	SecondPanel_ValidSoundWare,
	SecondPanel_ValidLightness,
	SecondPanel_ValidRtcTime,
	SecondPanel_ValidSoundWareEnable,
	SecondPanel_ValidEnable,
	SecondPanel_ValidFlagMax
}SecondPanel_ValidFlag;

typedef struct
{
	uint8 hour;		
	uint8 minutes;	
}TimeList;

typedef struct
{
	int OutPM25Value;
	int InPM25Value;
	int OutTemp;
	int InTemp;
	uint8 SoundWare;
	Lightness LightnessValue;
	TimeList stTimeListValue;
}SecondPanelPara;

typedef enum
{
	SoundWare_TurnOn = 7,
	SoundWare_TurnOff,
	SoundWare_ValidValueMax
}SoundWare_ValidSwitchType;

extern uint16 g_sSecondPanelSendDataFlag;

uint8 UART2Init(void);
void UartPanelRxCbk(void *driverState, uart_event_t event, void *userData);
void ErrorHandler(void);
void SecondPanelParaInit(void);
uint8 SendDataToSecondPanel(void);

void TemperatureCoding(uint8 cType, int nSrcData, uint8 *cDstData);
void PM25Coding(uint8 cType, int nSrcData, uint8 *cDstData);
void SoundWareCoding(uint8 SrcData, uint8 *cDetData);
void LightnessCoding(uint8 SrcData, uint8 *cDetData);
void SecondPanelSwitchCoding(uint8 cType, uint8 *cDstData);
void RtcTimeCoding(void *SrcData, uint8 *cDstData);

uint8 SetSecondPanelSoundWare(uint8 cData);

uint8 GetSecondPanelPM25Value(uint8 cType, uint8 *cData);
uint8 GetSecondPanelTemp(uint8 cType, uint8 *cData);
uint8 GetSecondPanelSoundWare(uint8 cData);
uint8 GetSecondPanelLightnessValue(uint8 cData);
uint8 GetSecondPanelRtcTimeValue(void);

uint8 UART2SendDataFromTemp(uint8 cType);
uint8 UART2SendDataFromPM25(uint8 cType);
uint8 UART2SendDataFromSoundWare(void);
uint8 UART2SendDataFromLightness(void);
uint8 UART2SendDataFromSwitch(uint8 cType);
uint8 UART2SendDataFromRtcTime(void);

uint8 SetFlagBit(uint16 *sSrc, uint8 cBit);
uint8 GetFlagBit(uint16 *sSrc, uint8 cBit);
uint8 ClearFlagBit(uint16 *sSrc, uint8 cBit);

#endif /* MIDDLEWARE_ACPANEL_ACPANEL_H_ */
