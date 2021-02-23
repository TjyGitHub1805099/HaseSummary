#include"sid0x28_if.h"

void sid0x28Init()
{

}

tUdsNRCType sid0x28CheckPreCondition(const uint8*data,uint16 len)
{
	if(diagCheckIfVehicleIsStatic() == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}

