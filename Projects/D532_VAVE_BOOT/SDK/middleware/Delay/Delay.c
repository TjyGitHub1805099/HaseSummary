#include "Std_Types.h"
#include "BoardDefines.h"
#include "System_Module_Cfg.h"
#if (STD_ON == SYS_MODULE_ADD_WDG)
//#include "wdg_pal.h"
//#include "wdog_pal_cfg.h"
#endif

void DelayUs(uint16 TimeUs)
{
	uint8 Count = 0u;

	while (TimeUs--)
	{
		#if (STD_ON == SYS_MODULE_ADD_WDG)
		//WDG_Refresh(INST_WDOG_PAL_CFG);
		#endif
		for (Count = 0; Count < 4; Count++)
		{
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");

		}
	}
}

void DelayMs(uint16 TimeMs)
{
	uint16 DelayLoop = 0u;

	//This is a accurate delay.
	while (TimeMs--)
	{
		#if (STD_ON == SYS_MODULE_ADD_WDG)
		//WDG_Refresh(INST_WDOG_PAL_CFG);
		#endif
		for (DelayLoop = 0; DelayLoop <3500; DelayLoop++)
		{
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");
			__asm__("nop");

		}
	}
}

