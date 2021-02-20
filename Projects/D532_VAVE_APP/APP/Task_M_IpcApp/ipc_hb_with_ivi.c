#include "ipc_hb_with_ivi_if.h"
#include "Task_M_IpcApp_Cfg.h" 
#include "trace.h"

#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)

IocMonitorSoc_InfoType gIocMonitorIpcWithIVI=
{
	.socPowerOn				= FALSE,
	.newCmd    				= FALSE,
	.rxHMICmd    			= {"MTR"},
	.txHMICmd 				= {"MTR"},
	.taskCycle   			= 10,
	.monitorCycle   		= 500,
	.monitorCycleTimeOut	= 2000,
};

boolean get_ipc_hb_with_ivi_status()
{
	return gIocMonitorIpcWithIVI.socPowerOn;
}

void ipc_hb_with_ivi_update(uint8 *data,uint8 len)
{
	gIocMonitorIpcWithIVI.newCmd = TRUE;
	memcpy(&gIocMonitorIpcWithIVI.rxHMICmd[0],data,len);
}

void ipc_hb_with_ivi(void)
 {
	static boolean sRxHbFlag = FALSE;
	static uint16 monitorCounter = 50,monitorTimeOutCounter=200,iocWaitSocPowerOnCounter=12000;

	if( gIocMonitorIpcWithIVI.newCmd == TRUE )
	{
		gIocMonitorIpcWithIVI.newCmd = FALSE;
		if( ( gIocMonitorIpcWithIVI.rxHMICmd[0] == 'M' ) && ( gIocMonitorIpcWithIVI.rxHMICmd[1] == 'T' ) && ( gIocMonitorIpcWithIVI.rxHMICmd[2] == 'R' ) )
		{
			//CLR
			gIocMonitorIpcWithIVI.rxHMICmd[0] = 0;
			gIocMonitorIpcWithIVI.rxHMICmd[1] = 0;
			gIocMonitorIpcWithIVI.rxHMICmd[2] = 0;
			//
			//monitorCounter = gIocMonitorIpcWithIVI.monitorCycle / gIocMonitorIpcWithIVI.taskCycle ;
			monitorTimeOutCounter = gIocMonitorIpcWithIVI.monitorCycleTimeOut/gIocMonitorIpcWithIVI.taskCycle;
			iocWaitSocPowerOnCounter = 120000 / gIocMonitorIpcWithIVI.taskCycle;
			//
			gIocMonitorIpcWithIVI.socPowerOn = TRUE;
			if(sRxHbFlag == FALSE)
			{
				sRxHbFlag = TRUE;
				TracePrintf(M_IPC_TRACE, TRACE_WARNING, "IPC HB with IVI OK :)!!!\r\n");
			}
		}
	}

	if( gIocMonitorIpcWithIVI.socPowerOn == TRUE )
	{
		//cycle send
		if( monitorCounter > 0 )
			monitorCounter--;
		else
		{
			monitorCounter = gIocMonitorIpcWithIVI.monitorCycle/gIocMonitorIpcWithIVI.taskCycle;
			ipc_send_notification(IOC_MONITOR_SOC_IPC_HW_CHANNEL,IOC_MONITOR_SOC_IPC_SW_CHANNEL,IOC_MONITOR_SOC_IPC_SW_FUNCTION_ID,&gIocMonitorIpcWithIVI.txHMICmd[0],IOC_MONITOR_SOC_DATA_LEN);
			#if( STD_ON == ACCTL_DEBUG_TRACE )
 				//TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[iocMTRsoc]: send heart package !!!\r\n");
 			#endif
		}

		//soc heart package monitor
		if( monitorTimeOutCounter > 0 )
			monitorTimeOutCounter--;
		else
		{
			sRxHbFlag = FALSE;
			gIocMonitorIpcWithIVI.socPowerOn = FALSE;
			TracePrintf(M_IPC_TRACE, TRACE_WARNING, "IPC HB with IVI lost :(!!!\r\n");
		}
	}
	else
	{
		if( iocWaitSocPowerOnCounter > 0 )
			iocWaitSocPowerOnCounter--;
		else
		{
			iocWaitSocPowerOnCounter = (120000) / gIocMonitorIpcWithIVI.taskCycle ;//120s
			TracePrintf(M_IPC_TRACE, TRACE_WARNING, "IPC HB with IVI lost[120s] :(!!!\r\n");
		}
	}
 }

 #endif
