#include "Std_Types.h"
#include "BoardDefines.h"
#include "System_Module_Cfg.h"
#if (STD_ON == SYS_MODULE_ADD_WDG)
#include "wdg_pal.h"
#include "wdog_pal_cfg.h"
#endif

void DelayUs(uint16 TimeUs)
{
	uint8 Count = 0u;

	while (TimeUs--)
	{
		for (Count = 0; Count <2; Count++)
		{
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
		}
	}
}

void DelayMs(uint16 TimeMs)
{
	uint16 DelayLoop = 0u;
	uint8  Wdg_Refresh_Cnt = 0 ;

	//This is a accurate delay.
	//the logic of refresh wdg , add by tjy , @2019-12-12
	//cause when NVM write system vTaskSuspendAll ,so need in delay refresh watch dog
	Ex_Wdg_Refresh();//1.ensure each delay can be refresh wdg at less once
	while (TimeMs--)
	{
		if( ((++Wdg_Refresh_Cnt) % 10) == 0 )
		{
			Ex_Wdg_Refresh();//2.refresh wdg,every 10 ms.
		}
		for (DelayLoop = 0; DelayLoop <2000; DelayLoop++)
		{
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
			__asm("nop");
		}
	}
}

