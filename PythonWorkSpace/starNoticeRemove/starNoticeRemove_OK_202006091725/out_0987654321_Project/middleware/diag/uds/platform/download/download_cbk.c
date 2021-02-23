
#include"download_if.h"
#include"sid0x27_if.h"
#include "Eeprom.h"
#include "PowerSupply.h"
#include "dea.h"
#include "task.h"

const uint32 gUsbUpdateFlag = USB_UPDATE_ENABLE_FLAG;
const uint32 gClearUpdataFlag = 0;


tUdsNRCType sid0x34CheckMainPreCondition()
{
#if 0
	#ifdef UDS_WITH_SID0X27
	if(sid0x27GetInnerSecurityLevel(ch)!=UDS_INNER_SECURITY_LEVEL_4)
		return NRC_SECURITY_ACCESS_REQUEST;
	#endif
#endif
	
	return NRC_NONE;
}

tUdsNRCType sid0x36CheckMainPrecondition()
{
	tDiagSysVoltageStatus sysVolStatus;

	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		sysVolStatus=diagCheckSysVoltageStatus();
		if(sysVolStatus== DIAG_SYS_VOLTAGE_STATUS_LOW)
			return NRC_VOLTAGE_TOO_LOW;
		else if(sysVolStatus== DIAG_SYS_VOLTAGE_STATUS_HIGH)
			return NRC_VOLTAGE_TOO_HIGH;
		else
			return NRC_CONDITION_NOT_CORRECT;
	}
	
	return NRC_NONE;
}

tUdsNRCType sid0x31IpcUpdater()
{
	
	if(NRC_NONE != sid0x34CheckMainPreCondition())
	{

	}
	DEA_SetResetMagicWord(0);
	SysResetWrapper(IPC_UPDATER);
	return NRC_NONE;
}



