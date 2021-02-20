#ifndef _IVI_IPC_HEART_BEAT_IF_H
#define _IVI_IPC_HEART_BEAT_IF_H
#include "Std_Types.h"
#include "Task_M_IpcApp_Cfg.h" 

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

extern IocMonitorSoc_InfoType gIocMonitorIpcWithIVI;
extern void ipc_hb_with_ivi(void);
extern boolean get_ipc_hb_with_ivi_status();
extern void ipc_hb_with_ivi_update(uint8 *data,uint8 len);

#endif
/*=================================[IOC MONITOR WITH SOC]=======================================*/

#endif