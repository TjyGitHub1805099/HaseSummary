/**
* @file:    ame_Cfg.h
* @brief:   Add your description here for this file.
* @author:  qiweifeng
* @date:    2018-09-29 17:40:29
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  qiweifeng  2018-09-29 17:40:29  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _AME_CFG_H_
#define _AME_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "trace.h"
#include "diagCommon.h"


/*------------------------------ END: USER INCLUDE ------------------------------*/
#ifdef DIAG_OS_UCOS
#define AME_TRACE_TXT(MODULE,TRACE_LEVEL,STRING) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING);
#define AME_TRACE_VALUE1(MODULE,TRACE_LEVEL,STRING,VALUE) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE);
#define AME_TRACE_VALUE2(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2);
#define AME_TRACE_VALUE3(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3);
#define AME_TRACE_VALUE4(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3,VALUE4);
#define AME_TRACE_VALUE5(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5);
#else
#define AME_TRACE_TXT(MODULE,TRACE_LEVEL,STRING) TracePrintf(MODULE,TRACE_LEVEL,STRING);
#define AME_TRACE_VALUE1(MODULE,TRACE_LEVEL,STRING,VALUE) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE);
#define AME_TRACE_VALUE2(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2);
#define AME_TRACE_VALUE3(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3);
#define AME_TRACE_VALUE4(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4);
#define AME_TRACE_VALUE5(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5);

#endif


#define UNLOCK_AME_DATA_LEN 10
#define AME_SEED_LEN  4
#define AME_KEY_LEN  4


#define DID_COUNT_MAX     50
#define AME_RX_BUFF_LEN   64
#define AME_TX_BUFF_LEN   128




#define AME_DID_TUNER_LEN   3
#define AME_DID_BT_LEN      6      // lenght is macaddr
#define AME_DID_WIFI_LEN    18     // lenght is macaddr and password that up to 12 digits
#define AME_DID_MEDIA_LEN   0
#define AME_DID_AUDIO_LEN   1
#define AME_DID_NAVI_LEN    0


#define AME_CMD_RESET_MODULE 0x11	//this cmd is only used to recovery ame session if there is no resp after long time


typedef tUdsNRCType (*tAmeIOCtrlFnCbk)(uint16 didNum, uint8* data, uint16 len);

typedef uint8 (*subServiceFnCbk)(uint32 ch, uint8* data, uint16 len);



typedef enum{
	AME_DID_TUNER = 0x5401, 
	AME_DID_BT    = 0x5402, 
	AME_DID_WIFI  = 0x5403, 
	AME_DID_MEDIA = 0x5404, 
	AME_DID_AUDIO = 0x5405, 
	AME_DID_NAVI  = 0x5406, 
}tAmeIOCtrlDID;


typedef struct
{
	uint16  id;
	boolean fromSocFlag;           // TRUE means this DID need be processed by SOC
	boolean isVariousLenFlag;
	uint16  dataLen;
	tAmeIOCtrlFnCbk IOCtrlFnCbk;
}tAmeIOCtrlDidInfo;


typedef struct{
	uint8 sid;
	subServiceFnCbk FnCbk;
}tAmeSubServiceFnCbk;


typedef enum{
	AME_TRACE_LEVEL_0,
	AME_TRACE_LEVEL_1,
	AME_TRACE_LEVEL_2,
	AME_TRACE_LEVEL_3,
}tAmeTraceLevel;


extern uint8 gUnlockAmeKey[];
extern const tAmeSubServiceFnCbk gAmeSubServiceFnCbk[];
extern uint16 gAmeSubServiceFnCbkTotalNum;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_AME_CFG_H_
/*============================= END OF FILE: ame_Cfg.h ============================*/
