#include "usb_swdl.h"
#include "Eeprom.h"
#include "BoardDefines.h"


void SetUsbUpgradeFlag(boolean Flag)
{
	static uint32 sUpgradeFlag = 0xff;
	static uint32 sReadBackValue = 0xff;
	uint8 i = 0;
	
	if(Flag == STD_ON)
	{
		sUpgradeFlag = USB_UPDATE_ENABLE_FLAG;
	}
	else
	{
		sUpgradeFlag = 0;
	}

	for(i=0;i<5;i++)
	{
		Eep_Write(0,(uint8 *)&sUpgradeFlag,sizeof(sUpgradeFlag));
		Eep_Read(0,(uint8 *)&sReadBackValue,sizeof(sReadBackValue));
		if(sReadBackValue == sUpgradeFlag)
			break;
	}
	
}

