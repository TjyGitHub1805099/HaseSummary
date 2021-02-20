

#ifndef _CANAPP_PDU_MSG_PROC_H_
#define _CANAPP_PDU_MSG_PROC_H_


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**金康项目按钮功能**/
#define		Release		0
#define		ShortPress	1
#define		LongPress	2

#define		TimeLimit	100

typedef enum
{
	SELF_DIAG_MODE_OFF,
	SELF_DIAG_MODE_START,
	SELF_DIAG_MODE_ON_OFF_3_CHECK,
	SELF_DIAG_MODE_ENTER,
}SELF_DIAG_MODE_ENUM;

typedef enum
{
	CAN_APP_RadarFCWOff,
	CAN_APP_RadarFCWGreen,
	CAN_APP_RadarFCWWaitClear,
	CAN_APP_RadarFCWYellow,
	CAN_APP_RadarFCWRed,
	CAN_APP_RadarFCWPayAttention,
	CAN_APP_RadarFCWSystemFalut,
}CAN_APP_FCW_ENUM;

typedef enum
{
	CAN_APP_RadarEBAOff,
	CAN_APP_RadarEBAGreen,
	CAN_APP_RadarEBAWaitClear,
	CAN_APP_RadarEBAYellow,
	CAN_APP_RadarEBARedBlink,
	CAN_APP_RadarEBASysFault,
	CAN_APP_RadarEBARed,
}CAN_APP_EBA_ENUM;

typedef enum
{
	CAN_APP_LdwsOff,
	CAN_APP_LdwsGreen,
	CAN_APP_LdwsYellow,
	CAN_APP_LdwsSysStatus,
	CAN_APP_LdwsLeftDepar,
	CAN_APP_LdwsRightDepar,
}CAN_APP_LDWS_ENUM;

typedef enum
{
	CAN_APP_WlcOff,
	CAN_APP_WlcChargingIsStarted,
	CAN_APP_WlcChargingIsStoped,
	CAN_APP_WlcPowerIsFull,
}CAN_APP_WLC_ENUM;

void AllPduMsgProc(void);

void Botton_Proc(uint8 keyEnum, uint8 ketType);

void ShortPressRelease(void);

void IC_MessageTX(void);
void KeyStrgTest(void);


#if 0
void RightBotton_Cheak();
void LeftBotton_Cheak();
void UpBotton_Cheak();
void DownBotton_Cheak();
void BackBotton_Cheak();
void ConfirmBotton_Cheak();
#endif







#ifdef __cplusplus
}
#endif // __cplusplus

#endif

/*============================= END OF FILE: ==========================*/



