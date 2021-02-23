#include"sid0x14_if.h"

void sid0x14Init()
{

}

tUdsNRCType sid0x14CheckPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}

