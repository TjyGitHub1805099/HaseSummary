#include"sid0x11_if.h"
#include"uds_if.h"
#include"trace.h"

extern const tUdsCfgInfor gUdsCfgInfor;

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
		//case UDS_KEY_OFF_ON_RESET://Smulate KL15 OFF--> KL15 ON process
		//case UDS_KEY_OFF_ON_RESET_SUPPRESS_POS_RESP:
		//case UDS_SOFT_RESET:

		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}

void sid0x11ResetSys(uint32 resetType)
{	
	//const tUdsCfgInfor *udsCfgInfor = udsGetGlobalCfgInfor();

	if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
	{
		switch(resetType)
		{
		case UDS_HARD_RESET:
			//2 TODO: send signal to PM to reset application OS??
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:sysSoftReset()\r\n");
			break;
		#if 0 // not supported
		case UDS_KEY_OFF_ON_RESET:
			//TODO: call CanCom API to simulate KL15 OFF-->KL15 ON process
			break;
		case UDS_SOFT_RESET:
			//;
			break;
		#endif
		default:
			break;
		}
	}

	diagReqSysReset();
}

