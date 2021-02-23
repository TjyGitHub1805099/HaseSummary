#include"sid0x10_if.h"
#include"uds_if.h"
#include"trace.h"

void sid0x10Init()
{

}

tUdsNRCType sid0x10CheckPreCondition()
{
#if 0
	if(diagCheckIfVehicleIsStatic() == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	else
#endif
		return NRC_NONE;
}

tUdsNRCType sid0x10CheckSesstionType(uint8 sessionType,uint32 *innerSessionType)
{
	tUdsNRCType ret = NRC_NONE;
	
	if(innerSessionType)
	{
		switch(sessionType)
		{
		case UDS_EXTERNAL_DEFAULT_SESSION:
		case UDS_EXTERNAL_DEFAULT_SESSION_SUPPRESS_POS_RESP:
			*innerSessionType = UDS_DEFAULT_SESSION;
			break;
		case UDS_EXTERNAL_EXTENDED_DIAG_SESSION:
		case UDS_EXTERNAL_EXTENDED_DIAG_SESSION_SUPPRESS_POS_RESP:
			*innerSessionType = UDS_EXTENDED_DIAG_SESSION;
			break;
			
		#ifdef UDS_ENABLE_PROGRAMMING_SESSION
		case UDS_EXTERNAL_PROGRAMMING_SESSION:
		case UDS_EXTERNAL_PROGRAMMING_SESSION_SUPPRESS_POS_RESP:
			*innerSessionType = UDS_PROGRAMMING_SESSION;
			break;
		#endif
		
		#ifdef UDS_ENABLE_CODING_SESSION
		case UDS_EXTERNAL_CODING_SESSION:
		case UDS_EXTERNAL_CODING_SESSION_SUPPRESS_POS_RESP:
			*innerSessionType = UDS_CODING_SESSION;
			break;
		#endif
		
		default:
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
	}
	else
		ret = NRC_GENERAL_REJECT;

	return ret;
}


//this function logic is project dependently
tUdsNRCType sid0x10ChangeSessionStrategy(uint32 ch,tUdsSessionInnerType newSession)
{
	tUdsSessionInnerType curSession = sid0x10GetCurSession(ch);
	tUdsNRCType ret = NRC_NONE;

	switch(curSession)
	{
	case UDS_DEFAULT_SESSION:
	#ifdef SW_PHASE_APPLICATION // in app,PROG and CODING session can only be transimitted from EXTEND sesssion directly
		if(newSession == UDS_PROGRAMMING_SESSION || newSession == UDS_CODING_SESSION)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
	#else // BOOT_LOADER, default session can't transimitted to CODING_SESSION
		if(newSession == UDS_CODING_SESSION)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
	#endif
		break;
	case UDS_EXTENDED_DIAG_SESSION:
		if(newSession == UDS_DEFAULT_SESSION)
		{
		#ifndef SW_PHASE_APPLICATION // in bootloader
			udsSetResetSysType(UDS_HARD_RESET);//uds core will call system Reboot api later
		#endif
		
			return NRC_NONE;
		}
		else if(newSession == UDS_PROGRAMMING_SESSION)
		{
		#ifdef SW_PHASE_APPLICATION
			udsSetResetSysType(UDS_HARD_RESET);
			//set update_system_flag in eeprom,
			//after system reset, IOC will switch to bootloader, and uds will switch to UDS_PROGRAMMING_SESSION directly.
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:set update flag\r\n");
			#ifdef DIAG_OS_UCOS
			Swdl_SetUpdateFlag(SWDL_TYPE_CAN);
			#else
			// TODO:
			#endif
			return NRC_RESPONSE_PENDING;
		#else
			return NRC_NONE;
		#endif
		}
		else if(newSession == UDS_CODING_SESSION)
		{
		#ifndef SW_PHASE_APPLICATION // CODING session is not supported in bootloader uds
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		#endif
		}
		break;

	case UDS_PROGRAMMING_SESSION:
		if(newSession == UDS_DEFAULT_SESSION)
		{
			udsSetResetSysType(UDS_HARD_RESET);
			return NRC_NONE;
		}
		else
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;

	case UDS_CODING_SESSION:
		if(newSession == UDS_PROGRAMMING_SESSION)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
		break;
	default:
		return NRC_GENERAL_REJECT;
	}

	return ret;
}


