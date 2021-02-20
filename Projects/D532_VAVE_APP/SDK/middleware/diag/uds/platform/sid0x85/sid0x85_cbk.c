#include"sid0x85_if.h"

void sid0x85Init()
{

}

tUdsNRCType sid0x85CheckPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}


void sid0x85StatusSwitchCbk(uint32 ch,void *data,uint16 len)
{
	tDtcSettingStatus dtcSettingStatus;
	
	if(data && len == sizeof(dtcSettingStatus))
	{
		dtcSettingStatus = *((tDtcSettingStatus*)data);
		if(dtcSettingStatus == DTC_SETTING_STATUS_OFF)
		{
			
		}
		else // DTC_SETTING_STATUS_ON
		{
			
		}
	}
	
}
