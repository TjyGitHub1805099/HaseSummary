#include "uds_if.h"

#ifdef SW_PHASE_APPLICATION
#if 0
#include"System_Task_Cfg.h"
#include"System_Task.h"
#include"System_Signal_Cfg.h"
#endif
#endif

extern const tUdsCfgInfor gUdsCfgInfor;

//this function is project dependently. maybe different customer,different update sys stratagy
boolean udsCheckIfNeedToUpdateSystem()
{
	boolean ret = FALSE;
	
	if(gUdsCfgInfor.udsWorkMode ==UDS_WORK_MODE_IN_BOOTLOADER) // in bootloader now
	{
		#if 0 // here is example logic
		if(E_OK == Swdl_GetUpdateFlag(&updateFalg))
		{
			if ((SWDL_TYPE_CAN&l_updateFalg) != 0) // this is mean updating sys by uds is request
			{
				Swdl_ClearUpdateFlag(); // clear this flag
				//swith to Programming Session directly
				sid0x10SetSession(ch,UDS_PROGRAMMING_SESSION);
				udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
				ret = TRUE;
			}
		}
		#endif
	}
	return ret;
}


