
#ifndef _SPI_IPC_CFG_H_
#define _SPI_IPC_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "Std_Types.h"
#include "SpiIpc.h"

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

//for DA move from #include "Task_M_IpcApp_Cfg.h"  by hyl
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

#ifdef __cplusplus
}
#endif 

#endif


