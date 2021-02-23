#ifndef TRACE_STORE_IN_SOC_H
#define TRACE_STORE_IN_SOC_H

#ifdef __cplusplus
extern "C" {
#endif


/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "msg_def.h"
#include "SpiIpc.h"
#include "SpiIpc_cfg.h"
#include "TraceStore_Cfg.h"
#include "CircularQueue.h"
#include "Task_M_IpcApp_Cfg.h"

#if (STD_ON == TRACE_STORE_USING )

/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/
#define TRACESTORE_IPC_HW_CHANNEL			SPI_HW_IPC_NUM1//SPI1
#define TRACESTORE_IPC_SW_CHANNEL			IPC_GW_CHANNEL_POWERMODING/* IPC_GW_CHANNEL_KEYSERVICE = 4,  hardkey service  */
#define TRACESTORE_IPC_SW_FUNCTION_ID		IPC_IOC_TRACE_STORE_SEND//55
#define TRACESTORE_IPC_SW_FUNCTION_ID_RECV	IPC_IOC_TRACE_STORE_RECV//45
#define TRACESTORE_IPC_SW_SEND_LEN			64//(SPI_MAX_DATA_SIZE-(sizeof(msg_head)))//max vaild length:6 is the length of msg_head

/*============================[T Y P E  D E F I N E]============================================*/
/************************************************************************************************/
typedef struct {
	uint16 channel;	/* circular queue chanel*/
	uint16 countChannel;/* circular queue count chanel*/

	uint16 dataLen;		/*store data len*/
	uint16 dataLenInMain;/*when sys in main trace stored pos*/
	uint8  traceStart;/*recv soc ready ,ioc start send trace to soc*/
	uint8  traceInmain;/* if not trace in main.c ,free rtos not up*/
	uint8  dataBuf[TRACESTORE_IPC_SW_SEND_LEN];/*trace buf*/
}TraceStoreType, *TraceStorePtr;


/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/
extern void TraceStore_Init(void);

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern void TraceStore_Push(uint8 *data, uint32 len, uint8 fromISR);
#else
extern void TraceStore_Push(uint8 *data, uint32 len);
#endif

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern void TraceStore_Pop(uint8 *data, uint32 len, uint8 fromISR);
#else
extern void TraceStore_Pop(uint8 *data, uint32 len);
#endif

extern void TraceStore_traceStartSet(uint8 enable);
extern void TraceStore_MainFunction(uint8 HwChannel,uint8 chanel);

#endif

#ifdef __cplusplus
}
#endif

#endif /* TRACE_STORE_IN_SOC_H */
