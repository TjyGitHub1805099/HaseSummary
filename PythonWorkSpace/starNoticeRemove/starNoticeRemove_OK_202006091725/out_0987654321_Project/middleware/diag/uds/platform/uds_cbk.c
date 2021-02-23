#include "uds_if.h"
#include "uds_cfg.h"
#ifdef SW_PHASE_APPLICATION
#if 0
#include"System_Task_Cfg.h"
#include"System_Task.h"
#include"system_signal_cfg.h"
#endif
#endif




boolean udsCheckIfNeedToUpdateSystem()
{
	boolean ret = FALSE;
	
	if(gUdsCfgInfor.udsWorkMode ==UDS_WORK_MODE_IN_BOOTLOADER) 
	{
		#if 0 
		if(E_OK == Swdl_GetUpdateFlag(&updateFalg))
		{
			if ((SWDL_TYPE_CAN&l_updateFalg) != 0) 
			{
				Swdl_ClearUpdateFlag(); 
				
				sid0x10SetSession(ch,UDS_PROGRAMMING_SESSION);
				udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
				ret = TRUE;
			}
		}
		#endif
	}
	return ret;
}


