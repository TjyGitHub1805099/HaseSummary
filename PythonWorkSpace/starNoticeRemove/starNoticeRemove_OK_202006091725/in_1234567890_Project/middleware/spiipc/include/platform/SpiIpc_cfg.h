
#ifndef _SPI_IPC_CFG_H_
#define _SPI_IPC_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "Std_Types.h"
#include "SpiIpc.h"
#include "osif.h"
#include "SPI_CircularQueue.h"
//#define  SPI_IPC_DEBUG

#define SPI_IPC_MERGE_SUPPORT			(1) 	//0-nonsupport 1-support
#define SPI_IPC_QUEUE_DYNAMIC			(0)		//0-static 1-dynamic
#define SPI_IPC_CRC_SUPPORT				(1)

#if (SPI_IPC_QUEUE_DYNAMIC != 0)
#define SPI_IPC_QUEUE_MIN_SIZE		(128)	
#endif
 
#define SPI_IPC_DEBUG_TRACE 			STD_OFF
enum{
SPI_HW_IPC_NUM0 = 0,
SPI_HW_IPC_NUM1 = 1,
HW_SPI_IPC_MAX_NUM
};

enum{
SPI_STATUS_SUCESS=0,
SPI_STATUS_BUSY,
SPI_STATUS_ERROR
};

//for DA move from #include "Task_M_IpcApp_Cfg.h" by hyl
typedef enum {
   IPC_GW_CHANNEL_SWDL = 2,          /* software downloader */
   IPC_GW_CHANNEL_POWERMODING = 3,   /* powermoding */   
   IPC_GW_CHANNEL_KEYSERVICE = 4,    /* hardkey service */
   IPC_GW_CHANNEL_AUDIO = 5,         /* audio serice */
   IPC_GW_CHANNEL_CANAPP = 6,        /* include can app */
   IPC_GW_CHANNEL_DIAG_IVI = 7,      /* IVI diagnose */
   IPC_GW_CHANNEL_SYSTEM = 8,        /* include time info, sys info, version info, back light*/
   IPC_GW_CHANNEL_AVM = 9,           /* AVM KEY,DIAG...*/
   IPC_GW_CHANNEL_SWDL_APP = 10,     /* software downloader in app */
   IPC_GW_CHANNEL_MAX
}ipc_gw_chn_id;

typedef uint16 (*SPI_QueueSizeGroupType)[2];

extern const SpiIpcChCfg SpiIpcUserCfg[HW_SPI_IPC_MAX_NUM];

extern SpiCqMutex gSpiIpcCQueueMutexIndex[HW_SPI_IPC_MAX_NUM];   //define in SpiIpc_cfg.c line 32
extern const SpiIpcGlobalStruct gSpiIpcGlobalCfg ;               //define in SpiIpc_cfg.c line 521
extern const spi_initfun spi_initGroup[];
extern const spi_ReqAckfun spiReq[];
extern const spi_ReqAckfun spiAck[];
extern const spi_TxRxfun SpiReadDataGroup[];
extern const spi_Transfun SpiTransDataGroup[];
extern const spi_TxRxfun SpiSendDataGroup[];
extern const spi_GetStatus SpiGetStatusGroup[];
extern const spi_AbortTransfer SpiAbortTransferGroup[];
extern const spi_GetLastTransErrorState spi_GetLastTransErrorStateGroup[];
extern const spi_SetLastTransErrorState spi_SetLastTransErrorStateGroup[];
extern const uint8 spi_HwChannelToInstance[];
extern const spi_GetChMergeType SpiGetChMergeTypeGroup[];
extern semaphore_t gSpiSemPool[];
//extern mutex_t ipc_mutex_tx[];
extern mutex_t gSpiIpcMutexHandlePool[];
extern Spi_notification_callback  *GroupSpi_Ntf_cbs[];

extern SPI_CQueue_ManagerType *SPI_TxQueueM_Group[];
extern SPI_CQueue_ManagerType *SPI_RxQueueM_Group[];
#if (SPI_IPC_QUEUE_DYNAMIC == 0)
extern uint8 *SPI_TxQueueGroup[];
extern uint8 *SPI_RxQueueGroup[];

#else
extern SPI_QueueSizeGroupType SPI_QueueSizeGroup[HW_SPI_IPC_MAX_NUM];
#endif

#ifdef __cplusplus
}
#endif 

#endif


