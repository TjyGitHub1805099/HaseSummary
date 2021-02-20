/*================================================================================
attention:
1. when External Watch Dog was enable,in 100ms must refresh it at less one time.
2. in task powermoding,every 10 ms refresh it .

be carefor:
1.not only in APP but also in BOOTLOADER must disable ex_watch_dog at the start of main.c
2.cause vTaskSuspendAll ,so need in Delayms() refresh ex_watch_dog.

================================================================================*/

#include "BoardDefines.h"
//#include "Std_Types.h"
//#include "Delay.h"
//#include "System_Task.h"

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
	//DelayUs(2);
	vTaskDelay(1);
	//System_Task_Delay(1);
	//__asm("nop");
	//__asm("nop");
	Port_SetOutLevel(WDI,PORT_HIGH);
	//DelayUs(1);
	//DelayMs(1);
	//__asm("nop");
	//__asm("nop");
	//Port_SetOutLevel(WDI,PORT_LOW);

#endif
#else
	PINS_DRV_TogglePins(PORTGP_WDI,(1<<PINNUM_WDI));//modified by tjy ,@2019-12-12
#endif
}



