
#ifndef _PM_CFG_H
#define _PM_CFG_H
#include "Std_Types.h"
#define   TASK_POWMODING_DEBUG_TRACE     STD_ON

typedef enum
{
	PM_SIG_ACC_ON = 0,        // 0
	PM_SIG_ACC_OFF,           // 1 
	PM_SIG_IGN_ON,            // 2
	PM_SIG_IGN_OFF,           // 3 
	PM_SIG_TCU_ACC_ON,        // 4
	PM_SIG_TCU_ACC_OFF,       // 5
	PM_SIG_REMOTE_START,      // 6
	PM_SIG_REMOTE_STOP,       // 7
	PM_SIG_CAN_AWAKE,         // 8
	PM_SIG_CAN_SLEEP,         // 9
	PM_SIG_CUSTOMER_HANDOVER, // 10
	PM_SIG_CYCLE,             // 11
	PM_SIG_FIVE_SECONDS,       // 12
	PM_SIG_VOLT_NORMAL,       // 13
	PM_SIG_VOLT_WARNING,      // 14
	PM_SIG_VOLT_ERROR,        // 15
	PM_SIG_SYS_UPDATE,        // 16
	PM_SIG_ONE_SECOND,        // 17
	PM_SIG_500MS, 			  // 18
	PM_SIG_7MS, 			  // 19
	PM_SIG_60S, 			  // 20
	#if 0
	PM_SIG_NM_WAKEUP,// 2
	PM_SIG_BUS_SLEEP,// 3
	PM_SIG_LCD_ON,// 4
	PM_SIG_LCD_OFF,// 5
	PM_SIG_RESTART,// 6
	PM_SIG_CYCLE,// 7
	PM_SIG_TIMER_2S,//8
	PM_SIG_LOCAL_SLEEP,//9
	PM_SIG_LOCAL_AWAKE,//10
	PM_SIG_PWR_NORMAL,//11
	PM_SIG_PWR_PROTECT,//12
	PM_SIG_PWR_CLOSE_LCD,//13
	PM_SIG_TIMER_200MS,//14
	#endif
	PM_SIG_MAX
} PM_SIG_Type;

#endif



