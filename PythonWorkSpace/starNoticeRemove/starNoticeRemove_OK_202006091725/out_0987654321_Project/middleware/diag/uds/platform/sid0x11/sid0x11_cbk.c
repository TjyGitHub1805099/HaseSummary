#include"sid0x11_if.h"
#include"uds_if.h"
#include"trace.h"
#include "uds_cfg.h"

void sid0x11Init()
{

}

tUdsNRCType sid0x11CheckPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}


tUdsNRCType sid0x11CheckResetType(uint32 resetType)
{
	tUdsNRCType ret = NRC_NONE;
	
	switch(resetType)
	{
	case UDS_HARD_RESET:
	case UDS_HARD_RESET_SUPPRESS_POS_RESP:
		break;
	default:
		
		
		

		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}

void sid0x11ResetSys(uint32 resetType)
{	
	

	if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
	{
		switch(resetType)
		{
		case UDS_HARD_RESET:
			
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:sysSoftReset()\r\n");
			break;
		#if 0 
		case UDS_KEY_OFF_ON_RESET:
			
			break;
		case UDS_SOFT_RESET:
			
			break;
		#endif
		default:
			break;
		}
	}

	diagReqSysReset();
}

