#ifndef _TRACE_H
#define _TRACE_H

/***********************************************************************************
 Constant Macros
***********************************************************************************/

#include <stdarg.h>
#include "Std_Types.h" 
#include "string.h"

#define	TRACE_INFO           0
#define	TRACE_WARNING        1
#define	TRACE_ERRO           2
#define	TRACE_NONMASK        3

#define TRACE_TX_BUFFER_MAX 80

#define TRACE_ANSI_COLOR STD_OFF

#if (STD_ON == TRACE_ANSI_COLOR)
#define TRACE_COLOR_NONE "\033[m"
#define TRACE_COLOR_BLACK "\033[30m"
#define TRACE_COLOR_RED "\033[31m"
#define TRACE_COLOR_GREEN "\033[32m"
#define TRACE_COLOR_YELLO "\033[33m"
#define TRACE_COLOR_BLUE "\033[34m"
#define TRACE_COLOR_PURPLE "\033[35m"
#define TRACE_COLOR_CYAN "\033[36m"
#define TRACE_COLOR_WHITE "\033[37m"
#endif

#define TRACE_CMD_NUM 10
#define TRACE_CMD_ELEMENT_LEN 10

typedef enum{
	SYSTEM_TRACE = 0,
    IPC_TRACE,
	COM_TRACE,
	CANAPP_TRACE,
	NM_TRACE,
	PM_TRACE,
	INPUT_TRACE,
	TP_TRACE,
	J1939TP_TRACE,
	UDS_TRACE,
	DIAGAPP_TRACE,
	DTCCORE_TRACE,
	AME_TRACE,
	ALARM_TRACE,
	VIRTUAL_TRACE,
	GENERAL_TRACE,
	TEST_TRACE,
	TRACE_TRACE,
	MODULE_TRACE_NAME_MAX
}Module_Trace_Name;

typedef struct{
	uint8 moduleName[TRACE_CMD_ELEMENT_LEN];
	uint8 traceIndex;
	uint8 switchOnOff;
	uint8 traceLevel;
}tTraceModuleInforList;

void TraceInit(void);
uint16 TraceGetFailPushBytes();
uint16 TraceGetFailTxBytes();

//void TracePrintf (uint16 moduleId, uint8 level, uint16 fileId, uint16 lineNum, const char *fmt, ...);
void TracePrintf (uint16 moduleId, uint8 level,const char *fmt, ...);

#ifdef TRACE_DEBUG_NEW
uint16 BKDRHash(char *str);
void TraceTest(uint16 moduleId, uint8 level, uint16 fileId, uint16 lineNum, const char *fmt, ...);
//extern unsigned int test123,test234;
//TraceTest(FILE_ID,LINE_NUM)
#include <stdio.h>
#endif
//#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)
//#define __FILENAME__ (p_fileId,p_fileId=strrchr(__FILE__, '\\') ? (p_fileId+1):__FILE__)
//#define __FILENAME__ strstr(__FILE__,"\\sh2")

#define FILE_ID 0//BKDRHash(__FILE__)
#define LINE_NUM 0//__LINE__
#define TRACE_UART_CHANNEL 0

//#define TraceTest(fileId,lineNum) {test123=FILE_ID;test234=LINE_NUM;}

#endif

