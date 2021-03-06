/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: System_Signal_Cfg.h
Author: Stephen Du
Version: V1.0
Timestamp: 2019-12-16 11:07:08
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _SYSTEM_SIGNAL_CFG_H_
#define _SYSTEM_SIGNAL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
/*===============================================================================*/
/*====================== START: USER SIGNAL INCLUDED HERE =======================*/
/*===============================================================================*/
#include "VirtualSig.h"
#include "Alarm.h"

/*===============================================================================*/
/*========================== END: USER SIGNAL INCLUDE ===========================*/
/*===============================================================================*/

#define SYSTEM_SIGNAL_DEBUG_TRACE STD_OFF

#define SYSTEM_SIG_ID_OFFSET_BIT 6

#define SYSTEM_SIG_MODULEID_ALARM (0<<SYSTEM_SIG_ID_OFFSET_BIT)
#define SYSTEM_SIG_MODULEID_VIRTUALSIG (1<<SYSTEM_SIG_ID_OFFSET_BIT)
#define SYSTEM_SIG_MODULEID_MAX 2

/*=====START:AlarmSigFlag Line,Don't modify this line=====*/
#define SYSTEM_SIG_ID_1MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_1MS)
#define SYSTEM_SIG_ID_2MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_2MS)
#define SYSTEM_SIG_ID_5MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_5MS)
#define SYSTEM_SIG_ID_6MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_6MS)
#define SYSTEM_SIG_ID_10MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_10MS)
#define SYSTEM_SIG_ID_13MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_13MS)
#define SYSTEM_SIG_ID_100MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_100MS)
#define SYSTEM_SIG_ID_60S (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_60S)
#define SYSTEM_SIG_ID_500MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_500MS)
#define SYSTEM_SIG_ID_1S (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_1S)
#define SYSTEM_SIG_ID_5S (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_5S)
#define SYSTEM_SIG_ID_30S (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_30S)
#define SYSTEM_SIG_ID_7MS (SYSTEM_SIG_MODULEID_ALARM|ALARM_ID_7MS)
/*======END:AlarmSigFlag Line,Don't modify this line======*/

/*=====START:VirtualSigFlag Line,Don't modify this line=====*/
#define SYSTEM_SIG_ID_DIAGCMD (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_DIAGCMD)
#define SYSTEM_SIG_ID_NM_WAKEUP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_NM_WAKEUP)
#define SYSTEM_SIG_ID_BUS_SLEEP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_BUS_SLEEP)
#define SYSTEM_SIG_ID_VOLTSTATE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_VOLTSTATE)
#define SYSTEM_SIG_ID_RESTART (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_RESTART)
#define SYSTEM_SIG_ID_UDS_REQ (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_UDS_REQ)
#define SYSTEM_SIG_ID_TEMPAMP_STATUS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_TEMPAMP_STATUS)
#define SYSTEM_SIG_ID_TEMPBOARD_STATUS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_TEMPBOARD_STATUS)
#define SYSTEM_SIG_ID_BATTVOLT_STATUS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_BATTVOLT_STATUS)
#define SYSTEM_SIG_ID_IGN (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_IGN)
#define SYSTEM_SIG_ID_ACC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_ACC)
#define SYSTEM_SIG_ID_TCU_ACC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_TCU_ACC)
#define SYSTEM_SIG_ID_REVERSE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_REVERSE)
#define SYSTEM_SIG_ID_REMOTE_START (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_REMOTE_START)
#define SYSTEM_SIG_ID_CUSTOMER_HANDOVER (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_CUSTOMER_HANDOVER)
#define SYSTEM_SIG_ID_CAN_AWAKE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_CAN_AWAKE)
#define SYSTEM_SIG_ID_CAN_SLEEP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_CAN_SLEEP)
#define SYSTEM_SIG_ID_LCD_USR_CTR (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_LCD_USR_CTR)
#define SYSTEM_SIG_ID_SWC_PRESS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SWC_PRESS)
#define SYSTEM_SIG_ID_SWC_REL (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SWC_REL)
#define SYSTEM_SIG_ID_SWC_HOLD (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SWC_HOLD)
#define SYSTEM_SIG_ID_SWC_HOLD_REL (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SWC_HOLD_REL)
#define SYSTEM_SIG_ID_LCD (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_LCD)
#define SYSTEM_SIG_ID_LVI_SLEEP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_LVI_SLEEP)
#define SYSTEM_SIG_ID_CAN_SEND_ENABLE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_CAN_SEND_ENABLE)
#define SYSTEM_SIG_ID_UPDATE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_UPDATE)
#define SYSTEM_SIG_ID_AUDIO_BEEP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_BEEP)
#define SYSTEM_SIG_ID_AUDIO_BEEP_PDC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_BEEP_PDC)
#define SYSTEM_SIG_ID_AUDIO_CLICKCLACK (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_CLICKCLACK)
#define SYSTEM_SIG_ID_AUDIO_MUTE_BEEP (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_MUTE_BEEP)
#define SYSTEM_SIG_ID_AUDIO_SETTING (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_SETTING)
#define SYSTEM_SIG_ID_AUDIO_EXECUTE_LC_FILTER_DIAGNOSTICS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_EXECUTE_LC_FILTER_DIAGNOSTICS)
#define SYSTEM_SIG_ID_AUDIO_DO_ECALL (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AUDIO_DO_ECALL)
#define SYSTEM_SIG_ID_AMP_ON (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AMP_ON)
#define SYSTEM_SIG_ID_AMP_OFF (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AMP_OFF)
#define SYSTEM_SIG_ID_AMP_EXECUTE_LC_FILTER_DIAGNOSTICS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AMP_EXECUTE_LC_FILTER_DIAGNOSTICS)
#define SYSTEM_SIG_ID_AMP_ENGINEER_SET_PWM (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AMP_ENGINEER_SET_PWM)
#define SYSTEM_SIG_ID_AMP_ENGINEER_SET_CHANNEL_PHASE (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_AMP_ENGINEER_SET_CHANNEL_PHASE)
#define SYSTEM_SIG_ID_READYUPDATA (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_READYUPDATA)
#define SYSTEM_SIG_ID_SOCUPDATARTC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SOCUPDATARTC)
#define SYSTEM_SIG_ID_DIAGREQSOC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_DIAGREQSOC)
#define SYSTEM_SIG_ID_BC_READRTC (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_BC_READRTC)
#define SYSTEM_SIG_ID_DIAG_TRIGGER_SOC_MFG (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_DIAG_TRIGGER_SOC_MFG)
#define SYSTEM_SIG_ID_ECALL_SPK_STATUS (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_ECALL_SPK_STATUS)
#define SYSTEM_SIG_ID_SOC_READY_TO_POWER_OFF (SYSTEM_SIG_MODULEID_VIRTUALSIG|VIRTUALSIG_ID_SOC_READY_TO_POWER_OFF)
/*======END:VirtualSigFlag Line,Don't modify this line======*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: System_Signal_Cfg.h ===========================*/

