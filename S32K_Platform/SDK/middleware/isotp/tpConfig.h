#ifndef _TP_CONFIG_H
#define _TP_CONFIG_H

#define APPLICATION_TP
#undef BOOTLOADER_TP

//#define TP_OS_UCOS

#ifdef APPLICATION_TP
#include"trace.h"

#define TP_USE_AME
#define TP_USE_UDS

#ifdef TP_OS_UCOS
#define TP_TRACE_TXT(MODULE,TRACE_LEVEL,STRING) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING);
#define TP_TRACE_VALUE1(MODULE,TRACE_LEVEL,STRING,VALUE) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE);
#define TP_TRACE_VALUE2(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2);
#define TP_TRACE_VALUE3(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3);
#define TP_TRACE_VALUE4(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3,VALUE4);
#define TP_TRACE_VALUE5(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5) TracePrintf(MODULE,TRACE_LEVEL,FILE_ID,LINE_NUM,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5);
#else
#define TP_TRACE_TXT(MODULE,TRACE_LEVEL,STRING) TracePrintf(MODULE,TRACE_LEVEL,STRING);
#define TP_TRACE_VALUE1(MODULE,TRACE_LEVEL,STRING,VALUE) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE);
#define TP_TRACE_VALUE2(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2);
#define TP_TRACE_VALUE3(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3);
#define TP_TRACE_VALUE4(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4);
#define TP_TRACE_VALUE5(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5) TracePrintf(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5);

#endif
#else
#define TP_TRACE_TXT(MODULE,TRACE_LEVEL,STRING)
#define TP_TRACE_VALUE1(MODULE,TRACE_LEVEL,STRING,VALUE)
#define TP_TRACE_VALUE2(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2)
#define TP_TRACE_VALUE3(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3)
#define TP_TRACE_VALUE4(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4)
#define TP_TRACE_VALUE5(MODULE,TRACE_LEVEL,STRING,VALUE1,VALUE2,VALUE3,VALUE4,VALUE5)

#endif


typedef enum{
	TP_TRACE_LEVEL_0,
	TP_TRACE_LEVEL_1,
	TP_TRACE_LEVEL_2,
	TP_TRACE_LEVEL_3,
	TP_TRACE_LEVEL_4
}tpTraceLevel;

//#define TP_TRACE_LEVEL TP_TRACE_LEVEL_1
#define TP_BUFF_PADDING_DATA 0x55
#define TP_MAX_CF_SEQ 0xF

#ifdef APPLICATION_TP
#define TP_RX_BUFFER_NUM 8 //if client tx data too fast,ECU can't process as fast as possible,we can extend this num to bigger value if memory is enough
#define TP_BS 8 // block size, max value is 0xF
#define TP_ST_MIN 20 // 20 milliseconds
#define TP_TASK_INTERVAL 6// task will operate every 6 milliseconds
#else
#define TP_RX_BUFFER_NUM 20
#define TP_BS 0 // in bootloader,value is 0
#define TP_ST_MIN 0 //in bootloader,value is 0
#endif


#ifdef APPLICATION_TP

#define PDU_ID_UDS_PHY_RX 10//get this value from canIf module
#define PDU_ID_UDS_FUNC_RX 11//get this value from canIf module
#define PDU_ID_AME_RX 0//get this value from canIf module


#define PDU_ID_UDS_PHY_TX 6 //get this value from canIf module
#define PDU_ID_UDS_FUNC_TX  7//get this value from canIf module
#define PDU_ID_AME_TX 0 //get this value from canIf module

#else

#define PDU_ID_UDS_PHY_RX 1//get this value from canIf module
#define PDU_ID_UDS_FUNC_RX 2//get this value from canIf module

#define PDU_ID_AME_TX 1 //get this value from canIf module
#define PDU_ID_UDS_PHY_TX 0 //get this value from canIf module
#define PDU_ID_UDS_FUNC_TX 1 //get this value from canIf module
#endif


#define	TP_TIMER_VALUE_N_AR 30 //[Time for the transmission of a PDU on the receiver side],
#define TP_TIMER_VALUE_N_BR 30 //[Time until the transmission of the next FC PDU]
#define TP_TIMER_VALUE_N_CR 150 //[Time until the next CF PDU is received]
	
#define	TP_TIMER_VALUE_N_AS 30 // [Time for the transmission of a PDU on the sender side],
#define TP_TIMER_VALUE_N_BS 150 //[Time until the next FC PDU is received]
#define TP_TIMER_VALUE_N_CS 30 //[Time until the transmission of the next CF PDU]

#undef LOCAL_AND_REMOTE_CPUS_TYPE_DIFFER	//if local cpu and remote cpu big/little endian is different, we should swap byte order
#define LOCAL_AND_REMOTE_CPUS_TYPE_SAME

#define CAN_TX_DATA(PDU_ID,DATA) CanIf_Transmit((PDU_ID),(DATA))

#endif
