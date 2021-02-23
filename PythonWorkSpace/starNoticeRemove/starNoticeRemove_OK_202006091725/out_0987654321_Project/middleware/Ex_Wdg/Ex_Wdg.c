

#include "BoardDefines.h"




void Ex_Wdg_Enable(void)
{
	Port_SetOutLevel(WDI_EN,PORT_LOW);
}

void Ex_Wdg_Disable(void)
{
	Port_SetOutLevel(WDI_EN,PORT_HIGH);
}

void Ex_Wdg_Refresh(void)
{
#if 0
#if 0
	Port_SetOutLevel(WDI,PORT_HIGH);
	DelayMs(1);
	Port_SetOutLevel(WDI,PORT_LOW);
#else
	Port_SetOutLevel(WDI,PORT_LOW);
	
	vTaskDelay(1);
	
	
	
	Port_SetOutLevel(WDI,PORT_HIGH);
	
	
	
	
	

#endif
#else
	PINS_DRV_TogglePins(PORTGP_WDI,(1<<PINNUM_WDI));
#endif
}



