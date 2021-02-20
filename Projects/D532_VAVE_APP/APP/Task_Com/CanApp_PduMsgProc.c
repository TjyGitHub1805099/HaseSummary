#define   IOC_PLATFORM

//#include "Task_CanApp_Cfg.h"
#if (STD_ON == TASK_CANAPP_DEBUG_TRACE)
#include "trace.h"
#endif

#include "dea.h"
#include "Task_IpcApp_Cfg.h"
//#include "CanApp_AliveProc.h"
//#include "CanApp_PduChanged.h"
#include "Vel.h"
#include "Std_Types.h"
#include "Platform_Types.h"
#include "CanApp_PduMsgProc.h"
//#include "System_Signal_If.h"
#include "System_Signal_Cfg.h"
#include "Gear.h"
#include "osif.h" 
#include "portmacro.h"
#include "string.h"

#include "Com_If.h" /**config IC_MessgeTX**/

#include "pin_wrapper_if.h"
#include "system_S32K148.h"
#include "BoardDefines.h"
#include "WaterTempWarn.h"
#include "VelSpdPulseOut.h"
#include "CanAppDiagClient.h"
#include "Key.h" //yangxl
#include <stdio.h> //yangxl
#include <trace.h> //yangxl





static uint8_t s_HmiPageReceivedFlag = 0;
static uint8_t s_DispButtonUpdatedFlag = 0;
static uint8_t s_DispButtonPressedFlag = 0;

static int s_HmiPageNum = -1;
//#include "DeaCoding.h"

#define WARN_TRANS

#if 0

/**金康项目-botton按钮程序,CAN总线信号**/
void BackBotton_Cheak()
{
	static	uint8	BackCnt = 0;
	static	uint8	BackReleaseCnt = 0;
	
	static	uint8_t	BackFlag = 0;
			uint8_t	BackStat = 0;
	static	uint8_t	BackStat_last = 0;


	if( DEA_GetBackBottonValidRaw())
	{
		BackStat = 1; /**按钮按下**/
	}
	else
	{
		BackStat = 0; /**按钮未按下**/
	}

	if((BackStat == 1) && (BackStat_last == 0)) /**上升沿**/
	{
		BackFlag = 1;
	}
	else if((BackStat == 0) && (BackStat_last == 1)) /**下降沿**/
	{
		BackFlag = 2;
	}
	else if((BackStat == 0) && (BackStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetBackBottonValue() == ShortPress) || (DEA_GetBackBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(BackReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetBackBottonValue(Release);
				BackFlag = 0;
				BackReleaseCnt = 0;
			}
		}
	}

	if(BackFlag == 1)
	{
		BackCnt++;

		if((BackStat == 1) && (BackStat_last == 1)) /**按钮未释放**/
		{
			if(BackCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetBackBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(BackFlag == 2)
	{		
		if(BackCnt < TimeLimit)
		{
			DEA_SetBackBottonValue(ShortPress); /**按钮短按**/
		}
		BackCnt = 0; /**检测到下降沿清零计数**/

		BackReleaseCnt++;
	}
	
	BackStat_last = BackStat; /**保存上一次的按钮状态值**/
	
}

void UpBotton_Cheak()
{
	static	uint8	UpCnt = 0;
	static	uint8	UpReleaseCnt = 0;
	
	static	uint8_t	UpFlag = 0;
			uint8_t	UpStat = 0;
	static	uint8_t	UpStat_last = 0;


	if( DEA_GetUpBottonValidRaw())
	{
		UpStat = 1; /**按钮按下**/
	}
	else
	{
		UpStat = 0; /**按钮未按下**/
	}

	if((UpStat == 1) && (UpStat_last == 0)) /**上升沿**/
	{
		UpFlag = 1;
	}
	else if((UpStat == 0) && (UpStat_last == 1)) /**下降沿**/
	{
		UpFlag = 2;
	}
	else if((UpStat == 0) && (UpStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetUpBottonValue() == ShortPress) || (DEA_GetUpBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(UpReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetUpBottonValue(Release);
				UpFlag = 0;
				UpReleaseCnt = 0;
			}
		}
	}

	if(UpFlag == 1)
	{
		UpCnt++;

		if((UpStat == 1) && (UpStat_last == 1)) /**按钮未释放**/
		{
			if(UpCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetUpBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(UpFlag == 2)
	{		
		if(UpCnt < TimeLimit)
		{
			DEA_SetUpBottonValue(ShortPress); /**按钮短按**/
		}
		UpCnt = 0; /**检测到下降沿清零计数**/

		UpReleaseCnt++; /**按钮释放计数**/
	}
	
	UpStat_last = UpStat; /**保存上一次的按钮状态值**/
}

void DownBotton_Cheak()
{
	static	uint8	DownCnt = 0;
	static	uint8	DownReleaseCnt = 0;
	
	static	uint8_t	DownFlag = 0;
			uint8_t	DownStat = 0;
	static	uint8_t	DownStat_last = 0;


	if( DEA_GetDownBottonValidRaw())
	{
		DownStat = 1; /**按钮按下**/
	}
	else
	{
		DownStat = 0; /**按钮未按下**/
	}

	if((DownStat == 1) && (DownStat_last == 0)) /**上升沿**/
	{
		DownFlag = 1;
	}
	else if((DownStat == 0) && (DownStat_last == 1)) /**下降沿**/
	{
		DownFlag = 2;
	}
	else if((DownStat == 0) && (DownStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetDownBottonValue() == ShortPress) || (DEA_GetDownBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(DownReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetDownBottonValue(Release);
				DownFlag = 0;
				DownReleaseCnt = 0;
			}
		}
	}

	if(DownFlag == 1)
	{
		DownCnt++;

		if((DownStat == 1) && (DownStat_last == 1)) /**按钮未释放**/
		{
			if(DownCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetDownBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(DownFlag == 2)
	{		
		if(DownCnt < TimeLimit)
		{
			DEA_SetDownBottonValue(ShortPress); /**按钮短按**/
		}
		DownCnt = 0; /**检测到下降沿清零计数**/

		DownReleaseCnt++;
	}
	
	DownStat_last = DownStat; /**保存上一次的按钮状态值**/
}

void ConfirmBotton_Cheak()
{
	static	uint8	ConfirmCnt = 0;
	static	uint8	ConfirmReleaseCnt = 0;
	
	static	uint8_t	ConfirmFlag = 0;
			uint8_t	ConfirmStat = 0;
	static	uint8_t	ConfirmStat_last = 0;


	if( DEA_GetConfirmBottonValidRaw())
	{
		ConfirmStat = 1; /**按钮按下**/
	}
	else
	{
		ConfirmStat = 0; /**按钮未按下**/
	}

	if((ConfirmStat == 1) && (ConfirmStat_last == 0)) /**上升沿**/
	{
		ConfirmFlag = 1;
	}
	else if((ConfirmStat == 0) && (ConfirmStat_last == 1)) /**下降沿**/
	{
		ConfirmFlag = 2;
	}
	else if((ConfirmStat == 0) && (ConfirmStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetConfirmBottonValue() == ShortPress) || (DEA_GetConfirmBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(ConfirmReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetConfirmBottonValue(Release);
				ConfirmFlag = 0;
				ConfirmReleaseCnt = 0;
			}
		}
	}

	if(ConfirmFlag == 1)
	{
		ConfirmCnt++;

		if((ConfirmStat == 1) && (ConfirmStat_last == 1)) /**按钮未释放**/
		{
			if(ConfirmCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetConfirmBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(ConfirmFlag == 2)
	{		
		if(ConfirmCnt < TimeLimit)
		{
			DEA_SetConfirmBottonValue(ShortPress); /**按钮短按**/
		}
		ConfirmCnt = 0; /**检测到下降沿清零计数**/

		ConfirmReleaseCnt++;
	}
	
	ConfirmStat_last = ConfirmStat; /**保存上一次的按钮状态值**/
}

void LeftBotton_Cheak()
{
	static	uint8	LeftCnt = 0;
	static	uint8	LeftReleaseCnt = 0;
	
	static	uint8_t	LeftFlag = 0;
			uint8_t	LeftStat = 0;
	static	uint8_t	LeftStat_last = 0;


	if( DEA_GetLeftBottonValidRaw())
	{
		LeftStat = 1; /**按钮按下**/
	}
	else
	{
		LeftStat = 0; /**按钮未按下**/
	}

	if((LeftStat == 1) && (LeftStat_last == 0)) /**上升沿**/
	{
		LeftFlag = 1;
	}
	else if((LeftStat == 0) && (LeftStat_last == 1)) /**下降沿**/
	{
		LeftFlag = 2;
	}
	else if((LeftStat == 0) && (LeftStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetLeftBottonValue() == ShortPress) || (DEA_GetLeftBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(LeftReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetLeftBottonValue(Release);
				LeftFlag = 0;
				LeftReleaseCnt = 0;
			}
		}
	}

	if(LeftFlag == 1)
	{
		LeftCnt++;

		if((LeftStat == 1) && (LeftStat_last == 1)) /**按钮未释放**/
		{
			if(LeftCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetLeftBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(LeftFlag == 2)
	{		
		if(LeftCnt < TimeLimit)
		{
			DEA_SetLeftBottonValue(ShortPress); /**按钮短按**/
		}
		LeftCnt = 0; /**检测到下降沿清零计数**/

		LeftReleaseCnt++;
	}
	
	LeftStat_last = LeftStat; /**保存上一次的按钮状态值**/
}

void RightBotton_Cheak()
{
	static	uint8	RightCnt = 0;
	static	uint8	RightReleaseCnt = 0;
	
	static	uint8_t	RightFlag = 0;
			uint8_t	RightStat = 0;
	static	uint8_t	RightStat_last = 0;


	if( DEA_GetRightBottonValidRaw())
	{
		RightStat = 1; /**按钮按下**/
	}
	else
	{
		RightStat = 0; /**按钮未按下**/
	}

	if((RightStat == 1) && (RightStat_last == 0)) /**上升沿**/
	{
		RightFlag = 1;
	}
	else if((RightStat == 0) && (RightStat_last == 1)) /**下降沿**/
	{
		RightFlag = 2;
	}
	else if((RightStat == 0) && (RightStat_last == 0)) /**按钮释放**/
	{
		if((DEA_GetRightBottonValue() == ShortPress) || (DEA_GetRightBottonValue() == LongPress)) /**ShortPress/LongPress before**/
		{
			if(RightReleaseCnt >= 5) /**50ms**/
			{
				DEA_SetRightBottonValue(Release);
				RightFlag = 0;
				RightReleaseCnt = 0;
			}
		}
	}

	if(RightFlag == 1)
	{
		RightCnt++;

		if((RightStat == 1) && (RightStat_last == 1)) /**按钮未释放**/
		{
			if(RightCnt >= TimeLimit) /**>1s**/
			{
				DEA_SetRightBottonValue(LongPress); /**按钮长按**/
			}		
		}
	}
	else if(RightFlag == 2)
	{		
		if(RightCnt < TimeLimit)
		{
			DEA_SetRightBottonValue(ShortPress); /**按钮短按**/
		}
		RightCnt = 0; /**检测到下降沿清零计数**/

		RightReleaseCnt++; //按钮释放计时
	}
	
	RightStat_last = RightStat; /**保存上一次的按钮状态值**/
}
#endif

#if 0

#if 0 //yangxl 2019-7-2 17:05:17
typedef enum
{
	KEY_SW_NONE =0,
	/* Steering_A Key */
	KEY_SW_SRC, 	 // 1
	KEY_SW_MENU_UP,    // 2
	KEY_SW_MENU_DOWN,  // 3
	KEY_SW_CAMERA,	 // // 4
	KEY_SW_OK,	   // 5
		
	/* Steering_B Key */
	KEY_SW_VOL_DOWN,   //6
	KEY_SW_VOL_UP,	   //7
	KEY_SW_TEL_ON,	   //8
	KEY_SW_MODE,	 //9
	KEY_SW_DISP,	 //10

};
#else
typedef enum
{
	KEY_SW_NONE =0,
	/* Steering_A Key */
	KEY_SW_MENU_UP=1,    // 2
	KEY_SW_MENU_DOWN=2,  // 3
	KEY_SW_OK=5,	   // 5
	KEY_SW_DISP=6,	 //10
	
	KEY_SW_CAMERA,	 // // 4
	KEY_SW_SRC, 	 // 1	
	/* Steering_B Key */
	KEY_SW_TEL_ON,	   //8
	KEY_SW_MODE,	 //9
	KEY_SW_VOL_DOWN=3,   //6
	KEY_SW_VOL_UP=4,	   //7



};

#endif 


void ShortPressRelease()
{
	static uint8_t timecnt = 0;
	
	if(DEA_GetSRCBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetSRCBottonValue(Release);		
		}
	}
	else if(DEA_GetMENU_UPBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetMENU_UPBottonValue(Release);		
		}
	}
	else if(DEA_GetMENU_DOWNBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetMENU_DOWNBottonValue(Release);		
		}
	}
	else if(DEA_GetTEL_ONBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetTEL_ONBottonValue(Release);		
		}
	}
	else if(DEA_GetSW_OKBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetSW_OKBottonValue(Release);		
		}
	}
	else if(DEA_GetVOL_DOWNBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetVOL_DOWNBottonValue(Release);		
		}
	}
	else if(DEA_GetVOL_UPBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetVOL_UPBottonValue(Release);		
		}
	}
	else if(DEA_GetCAMERABottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetCAMERABottonValue(Release);		
		}
	}
	else if(DEA_GetMODEBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			 DEA_SetMODEBottonValue(Release);		
		}
	}
	else if(DEA_GetDISPBottonValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetDISPBottonValue(Release);		
		}
	}

}

void Botton_Proc(uint8 keyEnum, uint8 keyType)
{
	
	uint8_t keyValue = 0;

	if(keyEnum == KEY_SW_DISP)
	{
		if(SYSTEM_SIG_ID_SWC_REL == keyType || SYSTEM_SIG_ID_SWC_HOLD_REL == keyType)
		{
			SetSelfDiagDispPressedFlag(0);
		}
		else if(SYSTEM_SIG_ID_SWC_PRESS == keyType)
		{
			SetSelfDiagDispPressedFlag(1);
		}
	}


	if(SYSTEM_SIG_ID_SWC_REL == keyType) /**short pressed**/
	{
		switch(keyEnum)
		{
			
			case KEY_SW_SRC:
				
					DEA_SetSRCBottonValue(ShortPress);
		
				break;
			
			case KEY_SW_MENU_UP:
				
					DEA_SetMENU_UPBottonValue(ShortPress);
					
				break;
			
			case KEY_SW_MENU_DOWN:

					DEA_SetMENU_DOWNBottonValue(ShortPress);

				break;
			
			case KEY_SW_TEL_ON:

					DEA_SetTEL_ONBottonValue(ShortPress);

				break;
			
			case KEY_SW_OK:

					DEA_SetSW_OKBottonValue(ShortPress);

				break;

			case KEY_SW_VOL_DOWN:

					DEA_SetVOL_DOWNBottonValue(ShortPress);

				break;
			case KEY_SW_VOL_UP:

					DEA_SetVOL_UPBottonValue(ShortPress);

				break;

			case KEY_SW_CAMERA:

					DEA_SetCAMERABottonValue(ShortPress);

				break;

			case KEY_SW_MODE:

					DEA_SetMODEBottonValue(ShortPress);

				break;

			case KEY_SW_DISP:

					DEA_SetDISPBottonValue(ShortPress);

				break;
			
			default:
				break;
			
		}
	}
	else if(SYSTEM_SIG_ID_SWC_HOLD == keyType) /**long pressed**/ 
	{
		switch(keyEnum)
		{
			
			case KEY_SW_SRC:
				
					DEA_SetSRCBottonValue(LongPress);
		
				break;
			
			case KEY_SW_MENU_UP:
				
					DEA_SetMENU_UPBottonValue(LongPress);
					
				break;
			
			case KEY_SW_MENU_DOWN:

					DEA_SetMENU_DOWNBottonValue(LongPress);

				break;
			
			case KEY_SW_TEL_ON:

					DEA_SetTEL_ONBottonValue(LongPress);

				break;
			
			case KEY_SW_OK:

					DEA_SetSW_OKBottonValue(LongPress);

				break;

			case KEY_SW_VOL_DOWN:

					DEA_SetVOL_DOWNBottonValue(LongPress);

				break;
			case KEY_SW_VOL_UP:

					DEA_SetVOL_UPBottonValue(LongPress);

				break;

			case KEY_SW_CAMERA:

					DEA_SetCAMERABottonValue(LongPress);

				break;

			case KEY_SW_MODE:

					DEA_SetMODEBottonValue(LongPress);

				break;

			case KEY_SW_DISP:

					DEA_SetDISPBottonValue(LongPress);

				break;
			
			default:
				break;

		}
		
	}
	else if(SYSTEM_SIG_ID_SWC_HOLD_REL == keyType) /**long released**/
	{
		switch(keyEnum)
		{
			
			case KEY_SW_SRC:
				
					DEA_SetSRCBottonValue(Release);
		
				break;
			
			case KEY_SW_MENU_UP:
				
					DEA_SetMENU_UPBottonValue(Release);
					
				break;
			
			case KEY_SW_MENU_DOWN:

					DEA_SetMENU_DOWNBottonValue(Release);

				break;
			
			case KEY_SW_TEL_ON:

					DEA_SetTEL_ONBottonValue(Release);

				break;
			
			case KEY_SW_OK:

					DEA_SetSW_OKBottonValue(Release);

				break;

			case KEY_SW_VOL_DOWN:

					DEA_SetVOL_DOWNBottonValue(Release);

				break;
			case KEY_SW_VOL_UP:

					DEA_SetVOL_UPBottonValue(Release);

				break;

			case KEY_SW_CAMERA:

					DEA_SetCAMERABottonValue(Release);

				break;

			case KEY_SW_MODE:

					DEA_SetMODEBottonValue(Release);

				break;

			case KEY_SW_DISP:

					DEA_SetDISPBottonValue(Release);

				break;
			
			default:
				break;

		}
	}
	
}
#else
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void ShortPressRelease(void)
{
	static uint8_t timecnt = 0;
	
	if(DEA_GetKeyStrgUpValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgUpValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgDownValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgDownValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgLeftValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgLeftValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgRightValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgRightValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgOkValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgOkValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgDispValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgDispValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgTelOnValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgTelOnValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgDvrValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			DEA_SetKeyStrgDvrValue(Release);		
		}
	}
	else if(DEA_GetKeyStrgLkaValue() == ShortPress) /**judged short released**/
	{
		timecnt++;
		/**key judge again**/
		if(timecnt >= 5) /**50ms**/
		{
			timecnt = 0;	
			 DEA_SetKeyStrgLkaValue(Release);		
		}
	}

}

/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void Botton_Proc(uint8 keyEnum, uint8 keyType)
{
	
	uint8_t keyValue = 0;

	if(keyEnum == KEY_STRG_DISP)
	{
		if(SYSTEM_SIG_ID_SWC_REL == keyType || SYSTEM_SIG_ID_SWC_HOLD_REL == keyType)
		{
			SetSelfDiagDispPressedFlag(0);
		}
		else if(SYSTEM_SIG_ID_SWC_PRESS == keyType)
		{
			SetSelfDiagDispPressedFlag(1);
		}
	}


	if(SYSTEM_SIG_ID_SWC_REL == keyType) /**short pressed**/
	{
		switch(keyEnum)
		{		
			case KEY_STRG_UP:
				{
					DEA_SetKeyStrgUpValue(ShortPress);
					break;
				}
			case KEY_STRG_DOWN:
				{
					DEA_SetKeyStrgDownValue(ShortPress);
					break;
				}
			case KEY_STRG_LEFT:
				{
					DEA_SetKeyStrgLeftValue(ShortPress);
					break;
				}
			case KEY_STRG_RIGHT:
				{
					DEA_SetKeyStrgRightValue(ShortPress);
					break;
				}
			case KEY_STRG_OK:
				{
					DEA_SetKeyStrgOkValue(ShortPress);
					break;
				}
			case KEY_STRG_DISP:
				{
					DEA_SetKeyStrgDispValue(ShortPress);
					break;
				}
			case KEY_STRG_TEL_ON:
				{
					DEA_SetKeyStrgTelOnValue(ShortPress);
					break;
				}
			case KEY_STRG_DVR:
				{
					DEA_SetKeyStrgDvrValue(ShortPress);
					break;
				}
			case KEY_STRG_LKA:
				{
					DEA_SetKeyStrgLkaValue(ShortPress);
					break;
				}
				break;

			default:
				break;
			
		}
	}
	else if(SYSTEM_SIG_ID_SWC_HOLD == keyType) /**long pressed**/ 
	{
		switch(keyEnum)
		{
			case KEY_STRG_UP:
				{
					DEA_SetKeyStrgUpValue(LongPress);
					break;
				}
			case KEY_STRG_DOWN:
				{
					DEA_SetKeyStrgDownValue(LongPress);
					break;
				}
			case KEY_STRG_LEFT:
				{
					DEA_SetKeyStrgLeftValue(LongPress);
					break;
				}
			case KEY_STRG_RIGHT:
				{
					DEA_SetKeyStrgRightValue(LongPress);
					break;
				}
			case KEY_STRG_OK:
				{
					DEA_SetKeyStrgOkValue(LongPress);
					break;
				}
			case KEY_STRG_DISP:
				{
					DEA_SetKeyStrgDispValue(LongPress);
					break;
				}
			case KEY_STRG_TEL_ON:
				{
					DEA_SetKeyStrgTelOnValue(LongPress);
					break;
				}
			case KEY_STRG_DVR:
				{
					DEA_SetKeyStrgDvrValue(LongPress);
					break;
				}
			case KEY_STRG_LKA:
				{
					DEA_SetKeyStrgLkaValue(LongPress);
					break;
				}
			default:
				break;

		}
		
	}
	else if(SYSTEM_SIG_ID_SWC_HOLD_REL == keyType) /**long released**/
	{
		switch(keyEnum)
		{
			case KEY_STRG_UP:
				{
					DEA_SetKeyStrgUpValue(Release);
					break;
				}
			case KEY_STRG_DOWN:
				{
					DEA_SetKeyStrgDownValue(Release);
					break;
				}
			case KEY_STRG_LEFT:
				{
					DEA_SetKeyStrgLeftValue(Release);
					break;
				}
			case KEY_STRG_RIGHT:
				{
					DEA_SetKeyStrgRightValue(Release);
					break;
				}
			case KEY_STRG_OK:
				{
					DEA_SetKeyStrgOkValue(Release);
					break;
				}
			case KEY_STRG_DISP:
				{
					DEA_SetKeyStrgDispValue(Release);
					break;
				}
			case KEY_STRG_TEL_ON:
				{
					DEA_SetKeyStrgTelOnValue(Release);
					break;
				}
			case KEY_STRG_DVR:
				{
					DEA_SetKeyStrgDvrValue(Release);
					break;
				}
			case KEY_STRG_LKA:
				{
					DEA_SetKeyStrgLkaValue(Release);
					break;
				}
			default:
				break;

		}
	}
	
}
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void KeyStrgTest(void)
{
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgUp = %d \r\n",DEA_GetKeyStrgUpValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgDown = %d \r\n", DEA_GetKeyStrgDownValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgLeft = %d \r\n", DEA_GetKeyStrgLeftValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgRight = %d \r\n", DEA_GetKeyStrgRightValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgOk = %d \r\n", DEA_GetKeyStrgOkValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgDisp = %d \r\n", DEA_GetKeyStrgDispValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgTelOn = %d \r\n", DEA_GetKeyStrgTelOnValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgDvr = %d \r\n", DEA_GetKeyStrgDvrValue());
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[KEY]:KeyStrgLka = %d \r\n", DEA_GetKeyStrgLkaValue());
}
#endif

/**************************/
#if 0
//extern void CanApp_PduProc_SESAM_CLAMP_STAT(void);

static void Button_Proc(void)
{
	static uint8_t CanApp_ButtonStat[DEA_BUTTON_NUM] = {0};
	static uint16_t CanApp_ButtonTimerCnt[DEA_BUTTON_NUM] = {0};
	static uint8_t CanApp_ButtonShortTimerCnt[DEA_BUTTON_NUM] = {0};
	//static uint8_t CanApp_ButtonShortPressTimerCnt[DEA_BUTTON_NUM] = {0};
	static BOARD_BUTTON_ENUM s_buttonStat = BUTTON_NONE;
	DEA_BUTTON_ENUM cnt;
	
	if(DEA_GetBoardButtonStatus() != s_buttonStat)
	{
		s_buttonStat = DEA_GetBoardButtonStatus();
		if(DEA_GetBoardButtonStatus() == BUTTON_NONE)	//all button release
		{
			for(cnt = DEA_BUTTON_UP; cnt < DEA_BUTTON_NUM; cnt++)
			{
				if(CanApp_ButtonStat[cnt] == 1)	// early button pressed
				{
					CanApp_ButtonStat[cnt] = 0;
					if(DEA_GetButtonStatus(cnt) == BUTTON_LONG_PRESSED)
					{
						DEA_SetButtonStatus(cnt, BUTTON_RELEASE);
					}
					else if(CanApp_ButtonTimerCnt[cnt] >= 5) //5*10ms=50ms
					{
						DEA_SetButtonStatus(cnt, BUTTON_SHORT_PRESSED);
					}
					CanApp_ButtonTimerCnt[cnt] = 0;
				}
			}
		}
		else
		{
			for(cnt = DEA_BUTTON_UP; cnt < DEA_BUTTON_NUM; cnt++)
			{
				if(CanApp_ButtonStat[cnt] == 1)	//clear early button pressed state
				{
					CanApp_ButtonStat[cnt] = 0;
					break;
				}
			}

			if(cnt < DEA_BUTTON_NUM)	//a button pressed 
			{
				if(DEA_GetButtonStatus(cnt) == BUTTON_LONG_PRESSED)
				{
					DEA_SetButtonStatus(cnt, BUTTON_RELEASE);
				}
				else
				{
					if(CanApp_ButtonTimerCnt[cnt] >= 5) //5*10ms=50ms
					{
						DEA_SetButtonStatus(cnt, BUTTON_SHORT_PRESSED);
					}
				}
				CanApp_ButtonTimerCnt[cnt] = 0;
			}

			if(DEA_GetBoardButtonStatus() == BUTTON_UP)
			{
				CanApp_ButtonStat[DEA_BUTTON_UP] = 1;
			}
			else if(DEA_GetBoardButtonStatus() == BUTTON_DOWN)
			{
				CanApp_ButtonStat[DEA_BUTTON_DOWN] = 1;
			}
			else if(DEA_GetBoardButtonStatus() == BUTTON_OK)
			{
				CanApp_ButtonStat[DEA_BUTTON_OK] = 1;
			}
			else if(DEA_GetBoardButtonStatus() == BUTTON_BACK)
			{
				CanApp_ButtonStat[DEA_BUTTON_BACK] = 1;
			}
		}
	}
	else
	{
		for(cnt = DEA_BUTTON_UP; cnt < DEA_BUTTON_NUM; cnt++)
		{
			if(CanApp_ButtonStat[cnt] == 1)
			{
				if(CanApp_ButtonTimerCnt[cnt] < 100)	//100*10ms = 1S
				{
					CanApp_ButtonTimerCnt[cnt]++;
				}
				else
				{
					DEA_SetButtonStatus(cnt, BUTTON_LONG_PRESSED);
				}
			}

			if(DEA_GetButtonStatus(cnt) == BUTTON_SHORT_PRESSED)
			{
				if(CanApp_ButtonShortTimerCnt[cnt] < 5)
				{
					CanApp_ButtonShortTimerCnt[cnt]++;
				}
				else
				{
					CanApp_ButtonShortTimerCnt[cnt] = 0;
					DEA_SetButtonStatus(cnt, BUTTON_RELEASE);
				}
			}
			else
			{
				CanApp_ButtonShortTimerCnt[cnt] = 0;
			}
		}	
	}
}

void BcmDoorStatProc(void)
{
	static DOOR_STAT_STRU s_door_stat = {0};
	DOOR_STAT_STRU l_door_stat;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_Msg1) == 0)
	{
		DEA_GetDoorStat(&l_door_stat);
		if( l_door_stat.DoorAjarFLValid)
		{
			if(s_door_stat.DoorAjarFL != l_door_stat.DoorAjarFL)
			{
				if(l_door_stat.DoorAjarFL == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_FrontLeftDoorOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.DoorAjarFL != 0)
			{
				l_door_stat.DoorAjarFL = 0;
				s_door_stat.DoorAjarFL = 0;
				l_msg.ID = NORMAL_MSG_ID_FrontLeftDoorOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(l_door_stat.DoorAjarFRValid)
		{
			if(s_door_stat.DoorAjarFR != l_door_stat.DoorAjarFR)
			{
				if(l_door_stat.DoorAjarFR == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_FrontRightDoorOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.DoorAjarFR != 0)
			{
				l_door_stat.DoorAjarFR = 0;
				s_door_stat.DoorAjarFR= 0;
				l_msg.ID = NORMAL_MSG_ID_FrontRightDoorOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if( l_door_stat.DoorAjarRLValid)
		{
			if(s_door_stat.DoorAjarRL != l_door_stat.DoorAjarRL)
			{
				if(l_door_stat.DoorAjarRL == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_RearLeftDoorOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.DoorAjarRL != 0)
			{
				l_door_stat.DoorAjarRL = 0;
				s_door_stat.DoorAjarRL = 0;
				l_msg.ID = NORMAL_MSG_ID_RearLeftDoorOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(l_door_stat.DoorAjarRRValid)
		{
			if(s_door_stat.DoorAjarRR != l_door_stat.DoorAjarRR)
			{
				if(l_door_stat.DoorAjarRR == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_RearRightDoorOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.DoorAjarRR !=0)
			{
				l_door_stat.DoorAjarRR = 0;
				s_door_stat.DoorAjarRR = 0;
				l_msg.ID = NORMAL_MSG_ID_RearRightDoorOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(l_door_stat.DoorTrunkAjarValid)
		{
			if(s_door_stat.DoorTrunkAjar != l_door_stat.DoorTrunkAjar)
			{
				if(l_door_stat.DoorTrunkAjar == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_TrunkDoorOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.DoorTrunkAjar!= 0)
			{
				l_door_stat.DoorTrunkAjar = 0;
				s_door_stat.DoorTrunkAjar = 0;
				l_msg.ID = NORMAL_MSG_ID_TrunkDoorOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(l_door_stat.BonnetAjarValid)
		{
			if(s_door_stat.BonnetAjar != l_door_stat.BonnetAjar)
			{
				if(l_door_stat.BonnetAjar == 0)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				l_msg.ID = NORMAL_MSG_ID_EngineHoodOpen;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_door_stat.BonnetAjar != 0)
			{
				l_door_stat.BonnetAjar = 0;
				s_door_stat.BonnetAjar = 0;
				l_msg.ID = NORMAL_MSG_ID_EngineHoodOpen;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		
		s_door_stat = l_door_stat;
	}
	else
	{
		if(s_door_stat.DoorAjarFL != 0)
		{
			s_door_stat.DoorAjarFL = 0;
			l_msg.ID = NORMAL_MSG_ID_FrontLeftDoorOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_door_stat.DoorAjarFR != 0)
		{
			s_door_stat.DoorAjarFR = 0;
			l_msg.ID = NORMAL_MSG_ID_FrontRightDoorOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_door_stat.DoorAjarRL != 0)
		{
			s_door_stat.DoorAjarRL = 0;
			l_msg.ID = NORMAL_MSG_ID_RearLeftDoorOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_door_stat.DoorAjarRR != 0)
		{
			s_door_stat.DoorAjarRR = 0;
			l_msg.ID = NORMAL_MSG_ID_RearRightDoorOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_door_stat.DoorTrunkAjar != 0)
		{
			s_door_stat.DoorTrunkAjar = 0;
			l_msg.ID = NORMAL_MSG_ID_TrunkDoorOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_door_stat.BonnetAjar != 0)
		{
			s_door_stat.BonnetAjar = 0;
			l_msg.ID = NORMAL_MSG_ID_EngineHoodOpen;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
	}
}


void ABS_Proc(void)
{
	static TELLTALE_ENUM s_AbsStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_EbdStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_EscStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_EscOffStat = TELLTALE_OFF;
	static uint8_t s_timeout_flag = 0;
	static uint8_t s_timeout_for_warn_flag = 0;
	
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_ABS_Msg1))
		{
			if(s_timeout_flag == 0)
			{
#ifndef WARN_TRANS
				if(s_AbsStat != DEA_GetABS_Stat())
				{
					s_AbsStat = DEA_GetABS_Stat();
					if(s_AbsStat == TELLTALE_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}

					l_msg.ID = NORMAL_MSG_ID_ABS;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				if(s_EbdStat != DEA_GetEBD_Stat())
				{
					s_EbdStat = DEA_GetEBD_Stat();
					if(s_EbdStat == TELLTALE_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}
					l_msg.ID = NORMAL_MSG_ID_EBD;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

#else
				if(s_AbsStat != DEA_GetABS_Stat())
				{
					s_AbsStat = DEA_GetABS_Stat();
					if(s_AbsStat == TELLTALE_OFF)
					{
						l_warnMsg.BitStatus = 1;
					}
					else
					{
						l_warnMsg.BitStatus = 0;
					}

					l_warnMsg.WarnMsgId = 9;
					PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
				}

				if(s_EbdStat != DEA_GetEBD_Stat())
				{
					s_EbdStat = DEA_GetEBD_Stat();
					if(s_EbdStat == TELLTALE_OFF)
					{
						l_warnMsg.BitStatus = 1;
					}
					else
					{
						l_warnMsg.BitStatus = 0;
					}

					l_warnMsg.WarnMsgId = 12;
					PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
				}
#endif

				if(s_EscStat != DEA_GetESC_Stat())
				{
					s_EscStat = DEA_GetESC_Stat();
					if(s_EscStat == TELLTALE_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}
					l_msg.ID = NORMAL_MSG_ID_ESC;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				if(s_EscOffStat != DEA_GetESC_OFF_Stat())
				{
					s_EscOffStat = DEA_GetESC_OFF_Stat();
					if(s_EscOffStat == TELLTALE_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}
					l_msg.ID = NORMAL_MSG_ID_ESCOFF;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{		
				s_timeout_flag = 0;

				s_AbsStat = TELLTALE_OFF;
				s_EbdStat = TELLTALE_OFF;
				
				l_msg.ID = NORMAL_MSG_ID_ABS;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

				l_warnMsg.BitStatus = 1;
				l_warnMsg.WarnMsgId = 9;
				PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);

				l_msg.ID = NORMAL_MSG_ID_EBD;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
		else
		{
			if(s_timeout_flag == 0)
			{
				s_timeout_flag = 1;
				s_AbsStat = TELLTALE_ON;
				s_EbdStat = TELLTALE_ON;
				
#ifndef WARN_TRANS				
				l_msg.ID = NORMAL_MSG_ID_ABS;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

				l_msg.ID = NORMAL_MSG_ID_EBD;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
#else
				l_warnMsg.BitStatus = 0;
				l_warnMsg.WarnMsgId = 9;
				PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);

				l_warnMsg.WarnMsgId = 12;
				PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
#endif

			}
		}
	}
	else
	{
		s_AbsStat = TELLTALE_OFF;
		s_EbdStat = TELLTALE_OFF;
		s_EscStat = TELLTALE_OFF;
		s_EscOffStat = TELLTALE_OFF;
		s_timeout_flag = 0;
	}

	/***********************Warn Message**********************/
}

void EPS_Proc(void)
{
	static TELLTALE_ENUM s_EpsStat = TELLTALE_OFF;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_EpsStat != DEA_GetEPS_Stat())
		{
			s_EpsStat = DEA_GetEPS_Stat();
			if(s_EpsStat == TELLTALE_OFF)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}

			l_msg.ID = NORMAL_MSG_ID_EPS;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
	}
	else
	{
		s_EpsStat = TELLTALE_OFF;
	}
}

void SDM_Proc(void)
{
	static TELLTALE_ENUM s_AirBagStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_DrBeltStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_PsBeltStat = TELLTALE_OFF;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	WARN_MSG_STRU l_warnMsg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{

#ifndef WARN_TRANS
		if(s_AirBagStat != DEA_GetAirBag_Stat())
		{
			s_AirBagStat = DEA_GetAirBag_Stat();
			if(s_AirBagStat == TELLTALE_OFF)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}

			l_msg.ID = NORMAL_MSG_ID_AirBag;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_DrBeltStat != DEA_GetDriverSeatbelt_Stat())
		{
			s_DrBeltStat = DEA_GetDriverSeatbelt_Stat();
			if(s_DrBeltStat == TELLTALE_OFF)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}

			l_msg.ID = NORMAL_MSG_ID_DrBelt;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_PsBeltStat != DEA_GetPassSeatbelt_Stat())
		{
			s_PsBeltStat = DEA_GetPassSeatbelt_Stat();
			if(s_PsBeltStat == TELLTALE_OFF)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}

			l_msg.ID = NORMAL_MSG_ID_PsBelt;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
#else

		if(s_AirBagStat != DEA_GetAirBag_Stat())
		{
			s_AirBagStat = DEA_GetAirBag_Stat();
			if(s_AirBagStat == TELLTALE_OFF)
			{
				l_warnMsg.BitStatus = 1;
			}
			else
			{
				l_warnMsg.BitStatus = 0;
			}

			l_warnMsg.WarnMsgId = 7;
			PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
		}

		if(s_DrBeltStat != DEA_GetDriverSeatbelt_Stat())
		{
			s_DrBeltStat = DEA_GetDriverSeatbelt_Stat();
			if(s_DrBeltStat == TELLTALE_OFF)
			{
				l_warnMsg.BitStatus = 1;
			}
			else
			{
				l_warnMsg.BitStatus = 0;
			}

			l_warnMsg.WarnMsgId = 53;
			PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
		}

		if(s_PsBeltStat != DEA_GetPassSeatbelt_Stat())
		{
			s_PsBeltStat = DEA_GetPassSeatbelt_Stat();
			if(s_PsBeltStat == TELLTALE_OFF)
			{
				l_warnMsg.BitStatus = 1;
			}
			else
			{
				l_warnMsg.BitStatus = 0;
			}

			l_warnMsg.WarnMsgId = 55;
			PutToMsgRing(TYPE_MSG_WARN, (IPC_APP_MSG_STRU *)&l_warnMsg);
		}
#endif
	}
	else
	{
		s_AirBagStat = TELLTALE_OFF;
		s_DrBeltStat = TELLTALE_OFF;
		s_PsBeltStat = TELLTALE_OFF;
	}
}


void BcmLightProc(void)
{
	static TURN_IND_STAT_ENUM s_trunStat = TURN_OFF;
	static TELLTALE_ENUM s_highBeamStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_fogLightStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_fogRearStat = TELLTALE_OFF;
	static TELLTALE_ENUM s_positionLightStat = TELLTALE_OFF;
	static ICON_STAT_ENUM s_autolightStat = ICON_OFF;

	//static uint8_t s_timeout_for_warn_flag = 0;

	//WARN_MSG_STRU l_warnMsg = {0};
	
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	

	if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))
	{
		if(s_trunStat != DEA_GetTurnIndicator())
		{
			if(DEA_GetTurnIndicator() == TRUN_INVALID)
			{
				if(s_trunStat == TURN_LEFT_RIGHT)
				{
					l_msg.ID = NORMAL_MSG_ID_TurnLeft_TurnRight;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				if(s_trunStat == TURN_LEFT)
				{
					l_msg.ID = NORMAL_MSG_ID_TurnLeft;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				if(s_trunStat == TURN_RIGHT)
				{
					l_msg.ID = NORMAL_MSG_ID_TurnRight;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				s_trunStat = TRUN_INVALID;
			}
			else
			{
				if(DEA_GetTurnIndicator() == TURN_LEFT_RIGHT)
				{
					if(s_trunStat == TURN_LEFT)
					{
						l_msg.ID = NORMAL_MSG_ID_TurnLeft;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
					if(s_trunStat == TURN_RIGHT)
					{
						l_msg.ID = NORMAL_MSG_ID_TurnRight;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
					
					l_msg.ID = NORMAL_MSG_ID_TurnLeft_TurnRight;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					if(s_trunStat == TURN_LEFT_RIGHT)
					{
						l_msg.ID = NORMAL_MSG_ID_TurnLeft_TurnRight;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}


					if(!((s_trunStat == TURN_LEFT_RIGHT) && (DEA_GetTurnIndicator() == TURN_OFF)))
					{
						if(DEA_GetTurnIndicator() == TURN_LEFT)
						{
							l_msg.ID = NORMAL_MSG_ID_TurnLeft;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							l_msg.Acoustic = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						}
						else
						{
							l_msg.ID = NORMAL_MSG_ID_TurnLeft;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						}
						
						if(DEA_GetTurnIndicator() == TURN_RIGHT)
						{
							l_msg.ID = NORMAL_MSG_ID_TurnRight;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							l_msg.Acoustic = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						}
						else
						{
							l_msg.ID = NORMAL_MSG_ID_TurnRight;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						}
					}
				}
			}

			s_trunStat = DEA_GetTurnIndicator();
		}
	}
	else
	{
		//if(DEA_GetIgnStat() == IGN_ON)
		{
			if(s_trunStat != TURN_LEFT_RIGHT)
			{
				s_trunStat = TURN_LEFT_RIGHT;
				l_msg.ID = NORMAL_MSG_ID_TurnLeft_TurnRight;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))
		{
			if(s_highBeamStat != DEA_GetHighBeamStat())
			{
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				s_highBeamStat = DEA_GetHighBeamStat();
				if(s_highBeamStat == TELLTALE_INVALID)
				{
					l_msg.ID = NORMAL_MSG_ID_HighBeam;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{		
					if(s_highBeamStat == TELLTALE_OFF)
					{
						l_msg.ID = NORMAL_MSG_ID_HighBeam;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
					else
					{
						l_msg.ID = NORMAL_MSG_ID_HighBeam;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
		}
		else
		{
			if(s_highBeamStat != TELLTALE_ON)
			{
				s_highBeamStat = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_HighBeam;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	else
	{
		s_highBeamStat = TELLTALE_OFF;
	}


	if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))
	{
		l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
		if(s_fogLightStat != DEA_GetFogLampFront())
		{
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			s_fogLightStat = DEA_GetFogLampFront();
			if( DEA_GetFogLampFront() == TELLTALE_INVALID)
			{
				s_fogLightStat = TELLTALE_INVALID;
				l_msg.ID = NORMAL_MSG_ID_FogLight;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			else
			{
				if(s_fogLightStat == TELLTALE_OFF)
				{
					l_msg.ID = NORMAL_MSG_ID_FogLight;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					l_msg.ID = NORMAL_MSG_ID_FogLight;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
		}
	}
	else
	{
		//if(DEA_GetIgnStat() == IGN_ON)
		{
			if(s_fogLightStat != TELLTALE_ON)
			{
				s_fogLightStat = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_FogLight;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}

	if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))
	{
		if(s_fogRearStat != DEA_GetFogLampRear())
		{
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			s_fogRearStat = DEA_GetFogLampRear();
			if( DEA_GetFogLampRear() == TELLTALE_INVALID)
			{
				s_fogRearStat = TELLTALE_INVALID;
				l_msg.ID = NORMAL_MSG_ID_RearFog;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			else
			{
				if(s_fogRearStat == TELLTALE_OFF)
				{
					l_msg.ID = NORMAL_MSG_ID_RearFog;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					l_msg.ID = NORMAL_MSG_ID_RearFog;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
		}
	}
	else
	{
		//if(DEA_GetIgnStat() == IGN_ON)
		{
			if(s_fogRearStat != TELLTALE_ON)
			{	
				s_fogRearStat = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_RearFog;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	
	if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))	
	{
		l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
		if(s_positionLightStat != DEA_GetPositionLight())
		{
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			s_positionLightStat = DEA_GetPositionLight();
			if( s_positionLightStat == TELLTALE_INVALID )
			{
				s_positionLightStat = TELLTALE_INVALID;
				l_msg.ID = NORMAL_MSG_ID_Position;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			else
			{
				
				if(s_positionLightStat == TELLTALE_OFF)
				{
					l_msg.ID = NORMAL_MSG_ID_Position;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					l_msg.ID = NORMAL_MSG_ID_Position;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
		}
	}
	else
	{
		//if(DEA_GetIgnStat() == IGN_ON)
		{
			if(s_positionLightStat != TELLTALE_ON)
			{
				s_positionLightStat = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_Position;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	////////////////////////////////////////////////
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))	
		{
			if(s_autolightStat != DEA_GetAutolight())
			{
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				s_autolightStat = DEA_GetAutolight();
				if( DEA_GetAutolight() == ICON_INVALID)
				{
					l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_Active;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

					l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_DeaActive;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					
					if(s_autolightStat == ICON_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_DeaActive;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

						l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_Active;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_Active;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

						l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_DeaActive;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
		}
		else
		{
			if(s_autolightStat == ICON_ON)
			{
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_Active;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

				l_msg.ID = NORMAL_MSG_ID_AutomaticHeadlamps_DeaActive;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	else
	{
		s_autolightStat = ICON_OFF;
	}

	/************************** Warn Message ***********************/

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg2))
		{
			if(s_timeout_for_warn_flag == 1)
			{
				s_timeout_for_warn_flag = 0;
				l_warnMsg.WarnMsgId = 5;	//Body control module lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_timeout_for_warn_flag == 0)
			{
				s_timeout_for_warn_flag = 1;
				l_warnMsg.WarnMsgId = 5;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}

}

void BCM_MSG1_Proc(void)
{
	static uint16_t s_TerminalTimeCnt = 0;
	if(DEA_GetCanTimeOutStat(IPDU_HND_BCM_Msg1) != 0)
	{
		if(DEA_GetTerminalErrorTimerExpire() == 0)
		{
			if(s_TerminalTimeCnt < 1*100)
			{
				s_TerminalTimeCnt++;
			}
			else
			{
				DEA_SetTerminalErrorTimerExpire(1);
				CanApp_PduProc_BCM_Msg1();
			}
		}
	}
	else
	{
		s_TerminalTimeCnt = 0;
		DEA_SetTerminalErrorTimerExpire(0);
	}
}

static void TPMS_Proc(void)
{	
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	static uint8_t s_telltale_stat = NORMAL_MSG_STAT_RESET;
	static uint8_t s_tmps_msg_timeout_flag = 0;
	uint8_t l_telltale_stat = NORMAL_MSG_STAT_RESET;
	uint8_t l_acoustic_stat = NORMAL_MSG_STAT_RESET;

	TIRE_STAT_STRU *l_p_tire = NULL;
	
	static uint8_t s_timeout_flag = 0;

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingTPMS() == DEA_CODING_OFF)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 19;	//TPMS lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}

			if(s_telltale_stat == NORMAL_MSG_STAT_SET)
			{
				s_telltale_stat = NORMAL_MSG_STAT_RESET;
					
				l_msg.ID = NORMAL_MSG_ID_TPMS;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
			}
		}
		else
		{
			l_p_tire = DEA_GetTireStatPtr();
			if(0 == DEA_GetCanTimeOutStat(IPDU_HND_TPMS_Msg1))	//signal without timeout
			{
				s_tmps_msg_timeout_flag = 0;
				/*if(l_p_tire->TempWarnFL == 0x01 || l_p_tire->TempWarnFR == 0x01 || l_p_tire->TempWarnRL == 0x01 || l_p_tire->TempWarnRR == 0x01)
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;
				}

				if((l_p_tire->TireTempFL != 0xFF) && (l_p_tire->TireTempFR != 0xFF)  && (l_p_tire->TireTempRL != 0xFF)  && (l_p_tire->TireTempRR != 0xFF))
				{
					if((l_p_tire->TireTempFL > (125+40)) || (l_p_tire->TireTempFR > (125+40)) || (l_p_tire->TireTempRL > (125+40)) || (l_p_tire->TireTempRR > (125+40)))
					{
						l_telltale_stat |= NORMAL_MSG_STAT_SET;
					}
				}*/

				if((l_p_tire->PosWLampFL > 0) && (l_p_tire->PosWLampFL < 7))
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;
				}

				if((l_p_tire->PosWLampFR > 0) && (l_p_tire->PosWLampFR < 7))
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;
				}

				if((l_p_tire->PosWLampRL > 0) && (l_p_tire->PosWLampRL < 7))
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;
				}

				if((l_p_tire->PosWLampRR > 0) && (l_p_tire->PosWLampRR < 7))
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;
				}

				/*if((l_p_tire->PressureTyreFL != 0xFF) && (l_p_tire->PressureTyreFR != 0xFF)  && (l_p_tire->PressureTyreRL != 0xFF)  && (l_p_tire->PressureTyreRR != 0xFF))
				{
					if((l_p_tire->PressureTyreFL > 125) || (l_p_tire->PressureTyreFR > 125) || (l_p_tire->PressureTyreRL > 125) || (l_p_tire->PressureTyreRR > 125))
					{
						l_telltale_stat |= NORMAL_MSG_STAT_SET;
					}
				}*/

				if(DEA_GetTireFault())
				{
					l_telltale_stat |= NORMAL_MSG_STAT_SET;
					l_acoustic_stat |= NORMAL_MSG_STAT_SET;

					portENTER_CRITICAL();
					l_p_tire->TireTempFL = 0xFF;
					l_p_tire->TireTempFR = 0xFF;
					l_p_tire->TireTempRL = 0xFF;
					l_p_tire->TireTempRR = 0xFF;

					l_p_tire->PressureTyreFL = 0xFF;
					l_p_tire->PressureTyreFR = 0xFF;
					l_p_tire->PressureTyreRL = 0xFF;
					l_p_tire->PressureTyreRR = 0xFF;
					portEXIT_CRITICAL();
				}

				if(l_telltale_stat != s_telltale_stat)
				{
					s_telltale_stat = l_telltale_stat;
						
					l_msg.TelltaleStatus = s_telltale_stat;
					l_msg.ID = NORMAL_MSG_ID_TPMS;
					if(l_acoustic_stat == NORMAL_MSG_STAT_SET)
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}
					else
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
				}
			}
			else
			{
				if(s_tmps_msg_timeout_flag == 0)
				{
					s_tmps_msg_timeout_flag = 1;
					portENTER_CRITICAL();
					l_p_tire->TireTempFL = 0xFF;
					l_p_tire->TireTempFR = 0xFF;
					l_p_tire->TireTempRL = 0xFF;
					l_p_tire->TireTempRR = 0xFF;

					l_p_tire->PressureTyreFL = 0xFF;
					l_p_tire->PressureTyreFR = 0xFF;
					l_p_tire->PressureTyreRL = 0xFF;
					l_p_tire->PressureTyreRR = 0xFF;
					portEXIT_CRITICAL();
				}
				
				if(s_telltale_stat == NORMAL_MSG_STAT_RESET)
				{
					s_telltale_stat = NORMAL_MSG_STAT_SET;
						
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					l_msg.ID = NORMAL_MSG_ID_TPMS;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
				}
			}

			/**************************Warn Message****************************/
			if(DEA_GetCanTimeOutStat(IPDU_HND_TPMS_Msg1) == 0)
			{
				if(s_timeout_flag == 1)
				{
					s_timeout_flag = 0;

					l_warnMsg.WarnMsgId = 19;	//TPMS lost communication
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 1;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
			else
			{
				if(s_timeout_flag == 0)
				{
					s_timeout_flag = 1;

					l_warnMsg.WarnMsgId = 19;
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 0;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
		}
	}
	else
	{
		s_telltale_stat = NORMAL_MSG_STAT_RESET;
		s_tmps_msg_timeout_flag = 0;
		l_telltale_stat = NORMAL_MSG_STAT_RESET;
		s_timeout_flag = 0;
	}
}






void CruiseControlProc(void)
{
	static CRUISE_CONTROL_ENUM l_last_state = CC_OFF;
	static boolean s_CruiseInit = 0;
	static boolean s_CruiseGreen = 0;
	static boolean s_CruiseWhite = 0;

	static boolean l_CruiseInit = 0;
	static boolean l_CruiseGreen = 0;
	static boolean l_CruiseWhite = 0;
	
	CRUISE_CONTROL_ENUM l_state;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_DME_ST_CRUISE_CONT))	//DME_ST_CRUISE_CONT without timeout
		{
			l_state = DEA_GetCruiseControl_ST_CC();
			if(l_state != l_last_state)
			{
				if(l_state > CC_STANDBY)
				{
					if(l_state <= CC_OVERRIDE)
					{
						s_CruiseInit = 0;
						s_CruiseGreen = 1;
						s_CruiseWhite = 0;
					}
					else
					{
						s_CruiseInit = 0;
						s_CruiseGreen = 0;
						s_CruiseWhite = 0;
					}
				}
				else if(l_state == CC_STANDBY)
				{
					if(l_last_state == CC_OFF) //CC_OFF -> CC_STANDBY
					{
						s_CruiseInit = 1;
						s_CruiseWhite = 0;
						s_CruiseGreen = 0;
					}
					else if(l_last_state <= CC_OVERRIDE)	//(CC_2-CC_9) -> CC_STATDBY
					{
						s_CruiseWhite = 1;
						s_CruiseGreen = 0;
						s_CruiseInit = 0;
					}
					else
					{
						//should be Confirm by customer
						s_CruiseInit = 1;
						s_CruiseWhite = 0;
						s_CruiseGreen = 0;
					}
				}
				else
				{
					s_CruiseInit = 0;
					s_CruiseWhite = 0;
					s_CruiseGreen = 0;
					
				}

				l_last_state = l_state;
			}
		}
		else
		{
			if(l_last_state != CC_RESERVED)
			{
				l_last_state = CC_RESERVED;
				s_CruiseInit = 0;
				s_CruiseWhite = 0;
				s_CruiseGreen = 0;
			}
		}

		if(s_CruiseInit != l_CruiseInit)
		{
			l_CruiseInit = s_CruiseInit;
			if(s_CruiseInit == 1)
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_CruiseControlInit;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
		}

		if(s_CruiseWhite != l_CruiseWhite)
		{
			l_CruiseWhite = s_CruiseWhite;
			if(s_CruiseWhite == 1)
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_CruiseControlWhite;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
		}

		if(s_CruiseGreen != l_CruiseGreen)
		{
			l_CruiseGreen = s_CruiseGreen;
			if(s_CruiseGreen == 1)
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_CruiseControlGreen;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU*)&l_msg);
		}
	}
	else
	{
		l_last_state = CC_OFF;
	
		s_CruiseInit = 0;
		s_CruiseGreen = 0;
		s_CruiseWhite = 0;

		l_CruiseInit = 0;
		l_CruiseGreen = 0;
		l_CruiseWhite = 0;
	}
}

void BcmWiperProc(void)
{
	static ICON_STAT_ENUM s_Wiper = ICON_OFF;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_BCM_SWITCH_STATUS))
		{
			if( DEA_GetAutoWiping() == ICON_INVALID)
			{
				if(s_Wiper != ICON_INVALID)
				{
					s_Wiper = ICON_INVALID;
					l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_Active;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

					l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_DeaActive;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{
				if(s_Wiper != DEA_GetAutoWiping())
				{
					s_Wiper = DEA_GetAutoWiping();
					if(s_Wiper == ICON_ON)
					{
						l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_Active;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

						l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_DeaActive;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
					else
					{
						l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_DeaActive;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

						l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_Active;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
		}
		else
		{
			if(s_Wiper == ICON_ON)
			{
				s_Wiper = ICON_OFF;
				l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_Active;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

				l_msg.ID = NORMAL_MSG_ID_AutomaticWiper_DeaActive;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}
	}
	else
	{
		s_Wiper = ICON_OFF;
	}
}


void Epbi_Proc(void)
{
	static TELLTALE_ENUM s_EpbiEscStar = TELLTALE_OFF;
	static TELLTALE_ENUM s_EpbiEscOffStar = TELLTALE_OFF;
	static TELLTALE_ENUM s_EpbiHdc = TELLTALE_OFF;
	static TELLTALE_ENUM s_EpbiEsc = TELLTALE_OFF;
	static TELLTALE_ENUM s_EpbiAutoHold = TELLTALE_OFF;
	static boolean EscOffTimeOutStar = STD_OFF;
	static uint8_t s_timeout_flag = 0;
	static PDC_ENUM s_PdcStat = PDC_Disable;
	
	IPC_APP_NORMAL_MSG_STRU l_msg={0};

	WARN_MSG_STRU l_warnMsg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_FUNCTION))
		{
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			if(DEA_GetESC_Stat() == TELLTALE_INVALID)
			{
				if(s_EpbiEscStar == TELLTALE_ON)
				{
					s_EpbiEscStar = TELLTALE_OFF;
					l_msg.ID = NORMAL_MSG_ID_ESC_DSC;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{
				if(s_EpbiEscStar != DEA_GetESC_Stat())
				{
					s_EpbiEscStar = DEA_GetESC_Stat();
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					if(s_EpbiEscStar == TELLTALE_OFF)
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
					}
					l_msg.ID = NORMAL_MSG_ID_ESC_DSC;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			
		}
		else
		{
			if(s_EpbiEscStar != TELLTALE_ON)
			{
				s_EpbiEscStar = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_ESC_DSC;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_FUNCTION))
		{
			if(DEA_GetESC_OFF_Stat() == TELLTALE_INVALID)
			{
				if(s_EpbiEscOffStar == TELLTALE_ON)
				{
					s_EpbiEscOffStar = TELLTALE_OFF;
					l_msg.ID = NORMAL_MSG_ID_ESCOFF;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{
				if(s_EpbiEscOffStar != DEA_GetESC_OFF_Stat())
				{
					s_EpbiEscOffStar = DEA_GetESC_OFF_Stat();
					if(s_EpbiEscOffStar == TELLTALE_OFF)
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					}
					else
					{
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					}
					l_msg.ID = NORMAL_MSG_ID_ESCOFF;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else
				{
					if(EscOffTimeOutStar == STD_ON)
					{
						if(s_EpbiEscOffStar == TELLTALE_OFF)
						{
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						}
						else
						{
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						}
						l_msg.ID = NORMAL_MSG_ID_ESCOFF;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
			EscOffTimeOutStar = STD_OFF;
		}
		else
		{
			if(s_EpbiEscOffStar != TELLTALE_ON)
			{
				l_msg.ID = NORMAL_MSG_ID_ESCOFF;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				s_EpbiEscOffStar = TELLTALE_ON;
				EscOffTimeOutStar = STD_ON;
			}
		}


		if(DEA_GetCodingHDC() == 0)
		{
			if(s_EpbiHdc != TELLTALE_OFF)	   
			{		
				s_EpbiHdc = TELLTALE_OFF;		 
				l_msg.ID = NORMAL_MSG_ID_HDC;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;		  
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;		 
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg); 	 
			}	  
		}
		else
		{
			if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_FUNCTION))	  
			{ 	
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;	
				if(DEA_GetHDC_Stat() == TELLTALE_INVALID)	  
				{ 	   
					if(s_EpbiHdc <= 0x02)
					{		  
						s_EpbiHdc = TELLTALE_INVALID; 		 
						l_msg.ID = NORMAL_MSG_ID_HDC;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);		 
					}
				}
				else 	 
				{		  
					if(s_EpbiHdc != DEA_GetHDC_Stat())
					{		   
						s_EpbiHdc = DEA_GetHDC_Stat();
						if(s_EpbiHdc == TELLTALE_ON)
						{ 		   
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						}
						else if(s_EpbiHdc == TELLTALE_1Hz)
						{			
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
						}
						else
						{
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						}
						l_msg.ID = NORMAL_MSG_ID_HDC;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);		
					}	   
				}
			} 
			else	
			{		
				if(s_EpbiHdc != TELLTALE_ON)	   
				{		
					s_EpbiHdc = TELLTALE_ON;		 
					l_msg.ID = NORMAL_MSG_ID_HDC;		  
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;		  
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;		 
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg); 	 
				}	  
			}
		}

		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_FUNCTION))
		{
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			if(DEA_GetESC_ASR_Stat() == TELLTALE_INVALID)
			{
				if(s_EpbiEsc != TELLTALE_OFF)
				{
					s_EpbiEsc = TELLTALE_OFF;
					l_msg.ID = NORMAL_MSG_ID_ESC;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{
				if(s_EpbiEsc != DEA_GetESC_ASR_Stat())
				{
					s_EpbiEsc = DEA_GetESC_ASR_Stat();
					if(s_EpbiEsc == TELLTALE_1Hz)
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
					}
					else
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					}
					l_msg.ID = NORMAL_MSG_ID_ESC;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
		}
		else
		{
			if(s_EpbiEsc != TELLTALE_ON)
			{
				s_EpbiEsc = TELLTALE_ON;
				l_msg.ID = NORMAL_MSG_ID_ESC;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_STAT_VEHICLE_STOP))
		{
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			if(DEA_GetVehicleStopStat() == TELLTALE_INVALID)
			{
				if(s_EpbiAutoHold == TELLTALE_ON)
				{
					l_msg.ID = NORMAL_MSG_ID_Autohold;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else
			{
				if(s_EpbiAutoHold != DEA_GetVehicleStopStat())
				{
					s_EpbiAutoHold = DEA_GetVehicleStopStat();
					if(s_EpbiAutoHold == TELLTALE_ON)
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					}
					else
					{
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					}
					l_msg.ID = NORMAL_MSG_ID_Autohold;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
		}
		else
		{
			if(s_EpbiAutoHold == TELLTALE_ON)
			{
				l_msg.ID = NORMAL_MSG_ID_Autohold;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
		}

		if(((DEA_GetCodingPDC() == DEA_CodingWith4SensorNoBSD) || (DEA_GetCodingPDC() == DEA_CodingWith6SensorNoBSD))
			&& (DEA_GetCanTimeOutStat(IPDU_HND_BCM_PDC_CHIME) == 0))
		{
			if(s_PdcStat != DEA_GetBCM_PDC_Stat())
			{
				l_msg.ID = 0;
				switch(s_PdcStat)
				{
					case PDC_SelfTestTone:
						l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
						break;
					case PDC_IndividualTone_1Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
						break;
					case PDC_IndividualTone_2Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
						break;
					case PDC_IndividualTone_4Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
						break;
					case PDC_ContinousTone:
						l_msg.ID = NORMAL_MSG_ID_ContinousTone;
						break;
					default:
						break;
				}
				
				if(l_msg.ID != 0)	//reset last stat
				{
					l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				
				s_PdcStat = DEA_GetBCM_PDC_Stat();	//get new stat
				if(s_PdcStat != PDC_INVALID)
				{
					l_msg.ID = 0;
					switch(s_PdcStat)
					{
						case PDC_SelfTestTone:
							l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
							break;
						case PDC_IndividualTone_1Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
							break;
						case PDC_IndividualTone_2Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
							break;
						case PDC_IndividualTone_4Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
							break;
						case PDC_ContinousTone:
							l_msg.ID = NORMAL_MSG_ID_ContinousTone;
							break;
						default:
							break;
					}
				
					if(l_msg.ID != 0)
					{
						l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
		}
		else
		{
			if(s_PdcStat != PDC_Disable)
			{
				l_msg.ID = 0;
				switch(s_PdcStat)
				{
					case PDC_SelfTestTone:
						l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
						break;
					case PDC_IndividualTone_1Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
						break;
					case PDC_IndividualTone_2Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
						break;
					case PDC_IndividualTone_4Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
						break;
					case PDC_ContinousTone:
						l_msg.ID = NORMAL_MSG_ID_ContinousTone;
						break;
					default:
						break;
				}
				
				if(l_msg.ID != 0)
				{
					l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				s_PdcStat = PDC_Disable;
			}
		}

		/*****************************Warn Message***************************/
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_EPBi_V_VEH))
		{
			
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 10;	//Brake system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_timeout_flag == 0)
			{
				s_timeout_flag = 1;

				l_warnMsg.WarnMsgId = 10;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}
	else
	{
		s_EpbiEscStar = TELLTALE_OFF;
		s_EpbiEscOffStar = TELLTALE_OFF;
		s_EpbiHdc = TELLTALE_OFF;
		s_EpbiEsc = TELLTALE_OFF;
		s_EpbiAutoHold = TELLTALE_OFF;
		EscOffTimeOutStar = STD_OFF;

		s_timeout_flag = 0;
	}
}


void RadarProc(void)
{
	static RADAR_STRU GetRadarData = {0};
	static uint8_t s_timeout_for_warn_flag = 0;
	static uint8_t s_CodingFcwOnOff = 0;
	 
	static CAN_APP_FCW_ENUM s_RadarFCWState = CAN_APP_RadarFCWOff;
	CAN_APP_FCW_ENUM l_RadarFCWState = CAN_APP_RadarFCWOff;
 
	static CAN_APP_EBA_ENUM s_RadarEBAState = CAN_APP_RadarEBAOff;
	CAN_APP_EBA_ENUM l_RadarEBAState = CAN_APP_RadarEBAOff;


	
	IPC_APP_NORMAL_MSG_STRU l_msg  = {0};
	WARN_MSG_STRU l_warnMsg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingFCW() == DEA_CODING_OFF)
		{
			 if(DEA_CODING_OFF != s_CodingFcwOnOff)
			 {
			 	s_CodingFcwOnOff = DEA_CODING_OFF;
					
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				switch(s_RadarFCWState)
				{
					case CAN_APP_RadarFCWGreen:
						l_msg.ID = NORMAL_MSG_ID_FCWGreen;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarFCWWaitClear:
						l_msg.ID = NORMAL_MSG_ID_FCWWaitClear;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarFCWYellow:
						l_msg.ID = NORMAL_MSG_ID_FCWYellow;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarFCWRed:
						l_msg.ID = NORMAL_MSG_ID_FCWRed;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarFCWPayAttention:
						l_msg.ID = NORMAL_MSG_ID_FCWPayAttention;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarFCWSystemFalut:
						l_msg.ID = NORMAL_MSG_ID_FCW_SystemFault;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					default:
						break;
				}

				switch(s_RadarEBAState)
				{
					case CAN_APP_RadarEBAGreen:
						l_msg.ID = NORMAL_MSG_ID_EBA_Green;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarEBAWaitClear:
						l_msg.ID = NORMAL_MSG_ID_EBA_WaitClear;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarEBAYellow:
						l_msg.ID = NORMAL_MSG_ID_EBA_Yellow;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarEBARed:
					case CAN_APP_RadarEBARedBlink:
						l_msg.ID = NORMAL_MSG_ID_EBA_ReadBlink;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_RadarEBASysFault:
						l_msg.ID = NORMAL_MSG_ID_EBA_SystemFault;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					default:
						break;
				}

				s_RadarFCWState = CAN_APP_RadarFCWOff;
				s_RadarEBAState = CAN_APP_RadarEBAOff;
			 }
		}
		else
		{
			if(s_CodingFcwOnOff == DEA_CODING_OFF)
			{
				s_CodingFcwOnOff = DEA_CODING_ON;
			}
			if(0 == DEA_GetCanTimeOutStat(IPDU_HND_RADAR_STATUS))
			{
				DEA_GetRadarStat(&GetRadarData);
				if(GetRadarData.STAT_FCWValid && GetRadarData.RadarFaultInfoValid)
				{
					if(GetRadarData.STAT_FCW == 0x01 && GetRadarData.RadarFaultInfo == 0x00)	//green
					{
						l_RadarFCWState = CAN_APP_RadarFCWGreen;
					}
					else if(GetRadarData.STAT_FCW == 0x02 && GetRadarData.RadarFaultInfo == 0x01) //wait clear
					{
						l_RadarFCWState = CAN_APP_RadarFCWWaitClear;
					}
					else if(GetRadarData.STAT_FCW == 0x02 && (GetRadarData.RadarFaultInfo == 0x03||GetRadarData.RadarFaultInfo == 0x00))	//yellow
					{
						l_RadarFCWState = CAN_APP_RadarFCWWaitClear;
					}
					else if(GetRadarData.STAT_FCW == 0x03 && GetRadarData.RadarFaultInfo == 0x00)	//pay attention
					{
						l_RadarFCWState = CAN_APP_RadarFCWGreen;
					}
					else if(GetRadarData.STAT_FCW == 0x04 && GetRadarData.RadarFaultInfo == 0x02)	// system fault
					{		
						l_RadarFCWState = CAN_APP_RadarFCWGreen;
					}
					else
					{
						l_RadarFCWState = CAN_APP_RadarFCWOff;
					}
				}
				else
				{
					l_RadarFCWState = CAN_APP_RadarFCWOff;
				}

				if(l_RadarFCWState != s_RadarFCWState)
				{
					//reset last status
					if(s_RadarFCWState != CAN_APP_RadarFCWOff)
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						switch(s_RadarFCWState)
						{
							case CAN_APP_RadarFCWGreen:
								l_msg.ID = NORMAL_MSG_ID_FCWGreen;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWWaitClear:
								l_msg.ID = NORMAL_MSG_ID_FCWWaitClear;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWYellow:
								l_msg.ID = NORMAL_MSG_ID_FCWYellow;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWRed:
								l_msg.ID = NORMAL_MSG_ID_FCWRed;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWPayAttention:
								l_msg.ID = NORMAL_MSG_ID_FCWPayAttention;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWSystemFalut:
								l_msg.ID = NORMAL_MSG_ID_FCW_SystemFault;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							default:
								break;
						}
					}

		
					switch(l_RadarFCWState)
					{
						case CAN_APP_RadarFCWGreen:
							l_msg.ID = NORMAL_MSG_ID_FCWGreen;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarFCWWaitClear:
							l_msg.ID = NORMAL_MSG_ID_FCWWaitClear;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarFCWYellow:
							l_msg.ID = NORMAL_MSG_ID_FCWYellow;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarFCWRed:
							l_msg.ID = NORMAL_MSG_ID_FCWRed;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarFCWPayAttention:
							l_msg.ID = NORMAL_MSG_ID_FCWPayAttention;
							l_msg.Acoustic = NORMAL_MSG_STAT_SET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarFCWSystemFalut:
							l_msg.ID = NORMAL_MSG_ID_FCW_SystemFault;
							l_msg.Acoustic = NORMAL_MSG_STAT_SET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						default:
							break;
					}

					s_RadarFCWState = l_RadarFCWState;
				}
				
				if(GetRadarData.EBAStateValid && GetRadarData.RadarFaultInfoValid)
				{
					if((GetRadarData.EBAState == 0x01) && (GetRadarData.RadarFaultInfo == 0x00))	//green
					{
						l_RadarEBAState = CAN_APP_RadarEBAGreen;
					}
					else if((GetRadarData.EBAState == 0x02) && (GetRadarData.RadarFaultInfo == 0x01)) //wait clear
					{
						l_RadarEBAState = CAN_APP_RadarEBAWaitClear;
					}
					else if((GetRadarData.EBAState == 0x02) && (GetRadarData.RadarFaultInfo == 0x03)) //yellow
					{
						l_RadarEBAState = CAN_APP_RadarEBAYellow;
					}
					else if((GetRadarData.EBAState == 0x02) && (GetRadarData.STAT_EBAActivation == 0x00)) //yellow
					{
						l_RadarEBAState = CAN_APP_RadarEBAYellow;
					}
					else if(GetRadarData.EBAState == 0x03 && GetRadarData.RadarFaultInfo == 0x00)	//redblink
					{
						l_RadarEBAState = CAN_APP_RadarEBARedBlink;
					}
					else if(GetRadarData.EBAState == 0x04 && GetRadarData.RadarFaultInfo == 0x02)	//system fault
					{
						l_RadarEBAState = CAN_APP_RadarEBASysFault;
					}
					else
					{
						l_RadarEBAState = CAN_APP_RadarEBAOff;
					}
				}
				else
				{
					l_RadarEBAState = CAN_APP_RadarEBAOff;
				}

				if(l_RadarEBAState != s_RadarEBAState)
				{
					//reset last status
					if(s_RadarEBAState != CAN_APP_RadarEBAOff)
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						switch(s_RadarEBAState)
						{
							case CAN_APP_RadarEBAGreen:
								l_msg.ID = NORMAL_MSG_ID_EBA_Green;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBAWaitClear:
								l_msg.ID = NORMAL_MSG_ID_EBA_WaitClear;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBAYellow:
								l_msg.ID = NORMAL_MSG_ID_EBA_Yellow;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBARed:
							case CAN_APP_RadarEBARedBlink:
								l_msg.ID = NORMAL_MSG_ID_EBA_ReadBlink;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBASysFault:
								l_msg.ID = NORMAL_MSG_ID_EBA_SystemFault;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							default:
								break;
						}
					}

		
					switch(l_RadarEBAState)
					{
						case CAN_APP_RadarEBAGreen:
							l_msg.ID = NORMAL_MSG_ID_EBA_Green;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarEBAWaitClear:
							l_msg.ID = NORMAL_MSG_ID_EBA_WaitClear;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarEBAYellow:
							l_msg.ID = NORMAL_MSG_ID_EBA_Yellow;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarEBARedBlink:
							l_msg.ID = NORMAL_MSG_ID_EBA_ReadBlink;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarEBASysFault:
							l_msg.ID = NORMAL_MSG_ID_EBA_SystemFault;
							l_msg.Acoustic = NORMAL_MSG_STAT_SET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_RadarEBARed:
							l_msg.ID = NORMAL_MSG_ID_EBA_ReadBlink;
							l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
							l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
							l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						default:
							break;
					}

					s_RadarEBAState = l_RadarEBAState;
				}
			}
			else
			{
				if(s_RadarFCWState != CAN_APP_RadarFCWRed)
				{
					if(s_RadarFCWState != CAN_APP_RadarFCWOff)
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						switch(s_RadarFCWState)
						{
							case CAN_APP_RadarFCWGreen:
								l_msg.ID = NORMAL_MSG_ID_FCWGreen;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWWaitClear:
								l_msg.ID = NORMAL_MSG_ID_FCWWaitClear;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWYellow:
								l_msg.ID = NORMAL_MSG_ID_FCWYellow;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWRed:
								l_msg.ID = NORMAL_MSG_ID_FCWRed;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWPayAttention:
								l_msg.ID = NORMAL_MSG_ID_FCWPayAttention;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarFCWSystemFalut:
								l_msg.ID = NORMAL_MSG_ID_FCW_SystemFault;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							default:
								break;
						}
					}

					l_msg.ID = NORMAL_MSG_ID_FCWRed;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					
					s_RadarFCWState = CAN_APP_RadarFCWRed;
				}
				

				
				if(s_RadarEBAState != CAN_APP_RadarEBARed)
				{
					//reset last status
					if(s_RadarEBAState != CAN_APP_RadarEBAOff)
					{
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						switch(s_RadarFCWState)
						{
							case CAN_APP_RadarEBAGreen:
								l_msg.ID = NORMAL_MSG_ID_EBA_Green;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBAWaitClear:
								l_msg.ID = NORMAL_MSG_ID_EBA_WaitClear;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBAYellow:
								l_msg.ID = NORMAL_MSG_ID_EBA_Yellow;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBARed:
							case CAN_APP_RadarEBARedBlink:
								l_msg.ID = NORMAL_MSG_ID_EBA_ReadBlink;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							case CAN_APP_RadarEBASysFault:
								l_msg.ID = NORMAL_MSG_ID_EBA_SystemFault;
								PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
								break;
							default:
								break;
						}
					}

					l_msg.ID = NORMAL_MSG_ID_EBA_SystemFault;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);

					s_RadarEBAState = CAN_APP_RadarEBARed;
				}
			}

			/***********************Warn Message*****************/
			if(0 == DEA_GetCanTimeOutStat(IPDU_HND_RADAR_STATUS))
			{
				
				if(s_timeout_for_warn_flag == 1)
				{
					s_timeout_for_warn_flag = 0;

					l_warnMsg.WarnMsgId = 15;	//Radar lost communication
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 1;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
			else
			{
				if(s_timeout_for_warn_flag == 0)
				{
					s_timeout_for_warn_flag = 1;

					l_warnMsg.WarnMsgId = 15;
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 0;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
		}
	}
	else
	{
		memset(&GetRadarData, 0, sizeof(GetRadarData));
		s_RadarFCWState = CAN_APP_RadarFCWOff;
		s_RadarEBAState = CAN_APP_RadarEBAOff;
		s_timeout_for_warn_flag = 0;
	}
}

void Ldws_Proc(void)
{

	static CAN_APP_LDWS_ENUM s_LdwsState = CAN_APP_LdwsOff;
	
	CAN_APP_LDWS_ENUM l_LdwsState = CAN_APP_LdwsOff;
	LDWS_STRU l_GetLdwsData = {0};
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingLDWS() == DEA_CODING_OFF)
		{
			if(s_LdwsState != CAN_APP_LdwsOff)
			{
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				switch(s_LdwsState)
				{
					case CAN_APP_LdwsGreen:
						l_msg.ID = NORMAL_MSG_ID_LDWS_Status;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsYellow:
						l_msg.ID = NORMAL_MSG_ID_LDWS_StatusYellow;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsSysStatus:
						l_msg.ID = NORMAL_MSG_ID_LDWS_SystemStatus;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsLeftDepar:
						l_msg.ID = NORMAL_MSG_ID_LDWS_LeftLaneDeparture;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsRightDepar:
						l_msg.ID = NORMAL_MSG_ID_LDWS_RightLaneDeparture;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break; 
					default:
						break;
				}
			}
		}
		else
		{
			DEA_GetLDWS_Stat(&l_GetLdwsData);
			if((DEA_GetCanTimeOutStat(IPDU_HND_LDWS_Status) == 0) && (l_GetLdwsData.LDWStatusSignalValid && l_GetLdwsData.LDWLeftDepartureWarnValid&&l_GetLdwsData.LDWRightDepartureWarnValid&&l_GetLdwsData.LDWStatusSignalValid
				&&l_GetLdwsData.LDWSystemStatusValid))
			{
				if(l_GetLdwsData.LDWStatusSignal == 0x00)	//Off
				{
					l_LdwsState = CAN_APP_LdwsOff;

					DEA_SetLDWSOnOffStat(0);
				}
				else
				{
					DEA_SetLDWSOnOffStat(1);
					if(((l_GetLdwsData.LDWStatusSignal == 0x02)||(l_GetLdwsData.LDWStatusSignal == 0x01)) && (l_GetLdwsData.LDWSystemStatus == 0x01))
					{
						l_LdwsState = CAN_APP_LdwsSysStatus;
					}
					else if((l_GetLdwsData.LDWStatusSignal == 0x02)&&(l_GetLdwsData.LDWLeftDepartureWarn==0x01)&&(l_GetLdwsData.LDWRightDepartureWarn==0x01))
					{
						l_LdwsState = CAN_APP_LdwsSysStatus;
					}
					else if((l_GetLdwsData.LDWStatusSignal == 0x02)&&(l_GetLdwsData.LDWRightDepartureWarn==0x01)&&(l_GetLdwsData.LDWSystemStatus != 0x01)
						&&(l_GetLdwsData.LDWLeftDepartureWarn!=0x01))
					{
						l_LdwsState = CAN_APP_LdwsRightDepar;
					}
					else if((l_GetLdwsData.LDWStatusSignal == 0x02)&&(l_GetLdwsData.LDWLeftDepartureWarn==0x01)&&(l_GetLdwsData.LDWSystemStatus != 0x01)
						&&(l_GetLdwsData.LDWRightDepartureWarn !=0x01))
					{
						l_LdwsState = CAN_APP_LdwsLeftDepar;
					}
					else if(l_GetLdwsData.LDWStatusSignal == 0x01)
					{
						l_LdwsState = CAN_APP_LdwsYellow;
					}
					else if(l_GetLdwsData.LDWStatusSignal == 0x02)
					{
						l_LdwsState = CAN_APP_LdwsGreen;
					}
					else
					{
						l_LdwsState = CAN_APP_LdwsOff;
						DEA_SetLDWSOnOffStat(0);
					}
				}
			}
			else
			{
				l_LdwsState = CAN_APP_LdwsOff;
				DEA_SetLDWSOnOffStat(0);
			}

			if(s_LdwsState != l_LdwsState)
			{
				if(s_LdwsState != CAN_APP_LdwsOff)
				{
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					switch(s_LdwsState)
					{
						case CAN_APP_LdwsGreen:
							l_msg.ID = NORMAL_MSG_ID_LDWS_Status;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_LdwsYellow:
							l_msg.ID = NORMAL_MSG_ID_LDWS_StatusYellow;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_LdwsSysStatus:
							l_msg.ID = NORMAL_MSG_ID_LDWS_SystemStatus;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_LdwsLeftDepar:
							l_msg.ID = NORMAL_MSG_ID_LDWS_LeftLaneDeparture;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_LdwsRightDepar:
							l_msg.ID = NORMAL_MSG_ID_LDWS_RightLaneDeparture;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break; 
						default:
							break;
					}
				}

				switch(l_LdwsState)
				{
					case CAN_APP_LdwsGreen:
						l_msg.ID = NORMAL_MSG_ID_LDWS_Status;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsYellow:
						l_msg.ID = NORMAL_MSG_ID_LDWS_StatusYellow;
						l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsSysStatus:
						l_msg.ID = NORMAL_MSG_ID_LDWS_SystemStatus;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsLeftDepar:
						l_msg.ID = NORMAL_MSG_ID_LDWS_LeftLaneDeparture;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_LdwsRightDepar:
						l_msg.ID = NORMAL_MSG_ID_LDWS_RightLaneDeparture;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_1Hz;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break; 
					default:
						break;
				}

				s_LdwsState = l_LdwsState;
			}
		}
	}
	else
	{
		s_LdwsState = CAN_APP_LdwsOff;
	}
}

void Scu_Proc(void)
{
	SEAT_MEMORY_ENUM l_GetScuData;
	static SEAT_MEMORY_ENUM s_GetScuData = SEAT_MEMORY_OFF;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	if(DEA_GetSeatMemoryStore() == SEAT_MEMORY_INVALID)
	{
		if(s_GetScuData == SEAT_MEMORY_REQUEST)
		{
			l_msg.ID = NORMAL_MSG_ID_SeatMemoryRequest;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
		else  if(s_GetScuData == SEAT_MEMORY_DONE)
		{
			l_msg.ID = NORMAL_MSG_ID_SeatMemoryStored;
			l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
			l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
		s_GetScuData = SEAT_MEMORY_OFF;
	}
	else
	{
		if(s_GetScuData !=  DEA_GetSeatMemoryStore())
		{
			l_GetScuData = DEA_GetSeatMemoryStore();
			if(l_GetScuData == SEAT_MEMORY_OFF)
			{
				if(s_GetScuData == SEAT_MEMORY_REQUEST)
				{
					l_msg.ID = NORMAL_MSG_ID_SeatMemoryRequest;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else  if(s_GetScuData == SEAT_MEMORY_DONE)
				{
					l_msg.ID = NORMAL_MSG_ID_SeatMemoryStored;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
			}
			else if(l_GetScuData == SEAT_MEMORY_REQUEST)
			{
				l_msg.ID = NORMAL_MSG_ID_SeatMemoryRequest;
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			else if(l_GetScuData == SEAT_MEMORY_DONE)
			{
				l_msg.ID = NORMAL_MSG_ID_SeatMemoryStored;
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			s_GetScuData = l_GetScuData;
		}
	}
}

void Etg_Proc(void)
{
	ETG_STATUS_STRU l_GetEtgData;
	static uint8 s_EtgVehicle = 0;
	static uint8 s_EtgShiftState = 0;
	static uint8 s_EtgCentrolLock = 0;

	static uint8 l_EtgVehicle = 0;
	static uint8 l_EtgShiftState = 0;
	static uint8 l_EtgCentrolLock = 0;
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		DEA_GetETG_Status(&l_GetEtgData);
		
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_ETG_STATUS))
		{
			if((l_GetEtgData.ETGRemind_VehicleState ==0x01)||(l_GetEtgData.ETGRemind_ETGState == 0x01))
			{
				s_EtgVehicle = 1;
			}
			else
			{
				if(s_EtgVehicle == 1)
				{
					s_EtgVehicle = 0;
				}
			}
			if(l_GetEtgData.ETGRemind_ShiftState == 0x01)
			{
				s_EtgShiftState = 1;
			}
			else if(l_GetEtgData.ETGRemind_CentrolLockState == 0x01)
			{
				s_EtgCentrolLock = 1;
			}
			else
			{

				if(s_EtgShiftState == 1)
				{
					s_EtgShiftState = 0;
				}
				else if(s_EtgCentrolLock == 1)
				{
					s_EtgCentrolLock = 0;
				}
			}
		
		}
		else
		{
			if(s_EtgVehicle == 1)
			{
				s_EtgVehicle = 0;
			}
			else if(s_EtgShiftState == 1)
			{
				s_EtgShiftState = 0;
			}
			else if(s_EtgCentrolLock == 1)
			{
				s_EtgCentrolLock = 0;
			}
		}

		if(s_EtgVehicle != l_EtgVehicle)
		{
			l_EtgVehicle = s_EtgVehicle;

			if(s_EtgVehicle==1)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_VehicleState;
			l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_EtgShiftState != l_EtgShiftState)//
		{
			l_EtgShiftState = s_EtgShiftState;

			if(s_EtgShiftState==1)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_ShiftState;
			l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}

		if(s_EtgCentrolLock != l_EtgCentrolLock)
		{
			l_EtgCentrolLock = s_EtgCentrolLock;

			if(s_EtgCentrolLock==1)
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.Acoustic = NORMAL_MSG_STAT_SET;
			}
			else
			{
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
			}
			l_msg.ID = NORMAL_MSG_ID_CentrolLockState;
			l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
			PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
		}
	}
	else
	{
		s_EtgVehicle =0;
		s_EtgShiftState = 0;
		s_EtgCentrolLock = 0;

		l_EtgVehicle = 0;
		l_EtgShiftState = 0;
		l_EtgCentrolLock = 0;
	}
}

void Wlc_Proc(void)
{
	WLC_STRU l_GetWlcData;
	static CAN_APP_WLC_ENUM s_WlcState = CAN_APP_WlcOff;
	CAN_APP_WLC_ENUM l_WlcState = CAN_APP_WlcOff;
	
	static uint8 s_WlcForeignMatter = 0;
	uint8 l_WlcForeignMatter = 0;
	
	IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingWirelessCharging() == DEA_CODING_OFF)
		{
			if(s_WlcForeignMatter != 0)
			{
				s_WlcForeignMatter = 0;
				l_msg.ID = NORMAL_MSG_ID_WLC_ForeignMatter;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}

			if(s_WlcState != CAN_APP_WlcOff)
			{
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				switch(s_WlcState)
				{
					case CAN_APP_WlcChargingIsStarted:
						l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStarted;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_WlcPowerIsFull:
						l_msg.ID = NORMAL_MSG_ID_WLC_PowerIsFull;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_WlcChargingIsStoped:
						l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStoped;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					default:
						break;
				}
				s_WlcState = CAN_APP_WlcOff;
			}
		}
		else
		{
			DEA_GetWLC_Stat(&l_GetWlcData);
			
			if(l_GetWlcData.WLC_St_SysInfoValid)
			{
				if(l_GetWlcData.WLC_St_SysInfo == 0x00)
				{
					l_WlcState = CAN_APP_WlcOff;
				}
				else if((l_GetWlcData.WLC_St_SysInfo == 0x01)||(l_GetWlcData.WLC_St_SysInfo == 0x06))
				{
					l_WlcState = CAN_APP_WlcChargingIsStarted;
				}
				else if((l_GetWlcData.WLC_St_SysInfo == 0x02)||(l_GetWlcData.WLC_St_SysInfo == 0x07))
				{
					l_WlcState = CAN_APP_WlcPowerIsFull;	
				}
				else if((l_GetWlcData.WLC_St_SysInfo == 0x03)||(l_GetWlcData.WLC_St_SysInfo == 0x04))
				{
					l_WlcState = CAN_APP_WlcChargingIsStoped;
				}
				else
				{
					l_WlcState = CAN_APP_WlcOff;
				}
			}
			else
			{
				l_WlcState = CAN_APP_WlcOff;
			}

			if(l_GetWlcData.WLC_W_FODValid)
			{
				if((l_GetWlcData.WLC_W_FOD > 0x00) && (l_GetWlcData.WLC_W_FOD < 0x04))
				{
					l_WlcForeignMatter = 1;
				}
			}
			else
			{
				if(l_WlcForeignMatter == 1)
				{
					l_WlcForeignMatter = 0;
				}
			}

			if(s_WlcState != l_WlcState)
			{
				if(s_WlcState != CAN_APP_WlcOff)
				{
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					switch(s_WlcState)
					{
						case CAN_APP_WlcChargingIsStarted:
							l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStarted;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_WlcPowerIsFull:
							l_msg.ID = NORMAL_MSG_ID_WLC_PowerIsFull;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						case CAN_APP_WlcChargingIsStoped:
							l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStoped;
							PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
							break;
						default:
							break;
					}
				}

				switch(l_WlcState)
				{
					case CAN_APP_WlcChargingIsStarted:
						l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStarted;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_WlcPowerIsFull:
						l_msg.ID = NORMAL_MSG_ID_WLC_PowerIsFull;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					case CAN_APP_WlcChargingIsStoped:
						l_msg.ID = NORMAL_MSG_ID_WLC_ChargingIsStoped;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
						break;
					default:
						break;
				}
				s_WlcState = l_WlcState;
			}

			if(s_WlcForeignMatter != l_WlcForeignMatter)
			{
				s_WlcForeignMatter = l_WlcForeignMatter;

				if(s_WlcForeignMatter == 1)
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
				}
				else
				{
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				}
				l_msg.ID = NORMAL_MSG_ID_WLC_ForeignMatter;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}

		}
	}
	else
	{
		s_WlcForeignMatter = 0;
		s_WlcState = CAN_APP_WlcOff;
	}
}

void MHU_Proc(void)
{
	static DEA_NAVI_FOR_IPC_STRU s_Navi = {0};
	static DEA_COMPASS_FOR_IPC_STRU s_compass = {0};
	static uint16_t s_timeout_cnt = 0;
	static uint8_t s_triger_for_warn_flag = 0;
	DEA_NAVI_STRU l_navi = {0};

	uint16_t l_compass_value = 0;


	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetCanTimeOutStat(IPDU_HND_MHU_COMPASS_DATA) == 0)
	{
		s_timeout_cnt = 0;
		l_compass_value = DEA_GetCompass();
		if(l_compass_value < 360)
		{
			s_compass.CompassState = 0x00;	//0x01:Normal
			s_compass.CompassData = DEA_GetCompass();
		}
		else 
		{
			if(DEA_GetCompass() == 0x1FF)
			{
				s_compass.CompassState = 0x03;	//Show Background Picture
			}
			else
			{
				s_compass.CompassState = 0x01;	//0x01:Just Compass Wihtout Direction
			}
		}
	}
	else
	{
		if(s_timeout_cnt < 100)	//100*10 = 1S
		{
			s_timeout_cnt++;
		}
		else
		{
			s_compass.CompassState = 0x02;	//Show Compass Not Available
		}
	}

	/*if(DEA_GetNaviChangedFlag() == 0)
	{
		return;
	}
	else
	{
		DEA_SetNaviChangedFlag(0);
	}*/
	
	l_navi = DEA_GetNaviStatus();

	if(l_navi.Route == 0x00)	//not active
	{
		s_Navi.NaviStatus = 0x00;
	}
	else if(l_navi.Route == 0x01)	//active
	{
		if((l_navi.Arrow <= 0x09) && (l_navi.Distance != 0xFFFF))
		{
			s_Navi.NaviStatus = 0x01;
			s_Navi.Arrow = l_navi.Arrow;
			s_Navi.Distance = l_navi.Distance;
		}
		else
		{
			s_Navi.NaviStatus = 0x02;	//Show Without Arrow and Distance
		}
	}
	else if(l_navi.Route == 0x02)  //Power off
	{
		s_Navi.NaviStatus = 0x04; //Show Navigation is not available
	}
	else
	{
		s_Navi.NaviStatus = 0x03; //Show Background Picture
	}

	DEA_SetNaviForIPCStatus(s_Navi);
	DEA_SetCompassForIPC(s_compass);

	/*************************Warn Message*******************/
	if(0)//DEA_GetIgnStat() == IGN_ON)
	{
		if(l_navi.LvdsReady == 0)
		{
			if(s_triger_for_warn_flag == 1)
			{
				s_triger_for_warn_flag = 0;

				l_warnMsg.WarnMsgId = 26;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_triger_for_warn_flag == 0)
			{
				s_triger_for_warn_flag = 1;

				l_warnMsg.WarnMsgId = 26;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}
}

void TERMINAL_Proc(void)
{
	static uint16_t s_TerminalTimeCnt = 0;
	if(DEA_GetTerminalErrorStat() != 0)
	{
		if(DEA_GetTerminalErrorTimerExpire() == 0)
		{
			if(s_TerminalTimeCnt < 3*100)
			{
				s_TerminalTimeCnt++;
			}
			else
			{
				DEA_SetTerminalErrorTimerExpire(1);
				CanApp_PduProc_SESAM_CLAMP_STAT();
			}
		}
	}
	else
	{
		s_TerminalTimeCnt = 0;
		DEA_SetTerminalErrorTimerExpire(0);
	}
}

void ACM_Proc(void)
{
	static uint8_t s_timeout_flag = 0;
	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		/************************Warn Message********************/
		if(DEA_GetCanTimeOutStat(IPDU_HND_ACM_SEAT_OCCUPATION) == 0)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 2;	//Airbag system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_timeout_flag == 0)
			{
				s_timeout_flag = 1;

				l_warnMsg.WarnMsgId = 2;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}
	else
	{
		s_timeout_flag = 0;
	}
}

void SESAM_Proc(void)
{
	static uint8_t s_timeout_flag = 0;

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		/***************************Warn Message**************************/
		if(DEA_GetCanTimeOutStat(IPDU_HND_SESAM_CLAMP_STAT) == 0)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 16;	//Keyless entry system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_timeout_flag == 0)
			{
				s_timeout_flag = 1;

				l_warnMsg.WarnMsgId = 16;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}
}

void TBOX_Proc(void)
{
	static uint8_t s_timeout_flag = 0;

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingTelematicsSystem() == DEA_CODING_OFF)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 17;	//T-BOX system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			/********************Warn Message********************/
			if(DEA_GetCanTimeOutStat(IPDU_HND_TBOX_ALIVE) == 0)
			{
				if(s_timeout_flag == 1)
				{
					s_timeout_flag = 0;

					l_warnMsg.WarnMsgId = 17;	//T-BOX system lost communication
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 1;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
			else
			{
				if(s_timeout_flag == 0)
				{
					s_timeout_flag = 1;

					l_warnMsg.WarnMsgId = 17;
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 0;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
		}
	}
	else
	{
		s_timeout_flag = 0;
	}
}

void TCU_Proc(void)
{
	static uint8_t s_timeout_flag = 0;

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCodingTransmissionType() != DEA_CODING_DCT)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 18;	//Transmission system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			/*************************Warn Message***********************/
			if(DEA_GetCanTimeOutStat(IPDU_HND_TCU_DISP_DATA_PWTR) == 0)
			{
				if(s_timeout_flag == 1)
				{
					s_timeout_flag = 0;

					l_warnMsg.WarnMsgId = 18;	//Transmission system lost communication
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 1;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
			else
			{
				if(s_timeout_flag == 0)
				{
					s_timeout_flag = 1;

					l_warnMsg.WarnMsgId = 18;
					l_warnMsg.BitBlinking = 0;
					l_warnMsg.BitStatus = 0;
					DEA_PutICWarnMsg(l_warnMsg);
					PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
				}
			}
		}
	}
	else
	{
		s_timeout_flag = 0;
	}
}

void DME_Proc(void)
{
	static uint8_t s_timeout_flag = 0;

	WARN_MSG_STRU l_warnMsg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		/***********************Warn Message********************/
		if(DEA_GetCanTimeOutStat(IPDU_HND_DME_TORQ_CRANKSHAFT_1) == 0)
		{
			if(s_timeout_flag == 1)
			{
				s_timeout_flag = 0;

				l_warnMsg.WarnMsgId = 145;	//Engine system lost communication
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 1;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
		else
		{
			if(s_timeout_flag == 0)
			{
				s_timeout_flag = 1;

				l_warnMsg.WarnMsgId = 145;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				PutToMsgRing(TYPE_MSG_WARN, ((IPC_APP_MSG_STRU *)&l_warnMsg));
			}
		}
	}
	else
	{
		s_timeout_flag = 0;
	}
}

void BSW_Proc(void)
{
	static uint8_t s_BswStat = 0;
	static PDC_ENUM s_PdcStat = PDC_Disable;

	IPC_APP_NORMAL_MSG_STRU l_msg = {0};

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(((DEA_GetCodingPDC() == DEA_CodingWithBSDAnd4Sensor) || (DEA_GetCodingPDC() == DEA_CodingWithBSDAnd6Sensor))
				&& (DEA_GetCanTimeOutStat(IPDU_HND_PDC_CHIME) == 0))
		{
			if(s_PdcStat != DEA_GetPDC_Stat())
			{
				l_msg.ID = 0;
				switch(s_PdcStat)
				{
					case PDC_SelfTestTone:
						l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
						break;
					case PDC_IndividualTone_1Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
						break;
					case PDC_IndividualTone_2Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
						break;
					case PDC_IndividualTone_4Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
						break;
					case PDC_ContinousTone:
						l_msg.ID = NORMAL_MSG_ID_ContinousTone;
						break;
					default:
						break;
				}
				
				if(l_msg.ID != 0)	//reset last stat
				{
					l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				
				s_PdcStat = DEA_GetPDC_Stat();	//get new stat
				if(s_PdcStat != PDC_INVALID)
				{
					l_msg.ID = 0;
					switch(s_PdcStat)
					{
						case PDC_SelfTestTone:
							l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
							break;
						case PDC_IndividualTone_1Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
							break;
						case PDC_IndividualTone_2Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
							break;
						case PDC_IndividualTone_4Hz:
							l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
							break;
						case PDC_ContinousTone:
							l_msg.ID = NORMAL_MSG_ID_ContinousTone;
							break;
						default:
							break;
					}
				
					if(l_msg.ID != 0)
					{
						l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
						l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
						l_msg.Acoustic = NORMAL_MSG_STAT_SET;
						PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
					}
				}
			}
		}
		else
		{
			if(s_PdcStat != PDC_Disable)
			{
				l_msg.ID = 0;
				switch(s_PdcStat)
				{
					case PDC_SelfTestTone:
						l_msg.ID = NORMAL_MSG_ID_SelfTestTone;
						break;
					case PDC_IndividualTone_1Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone1Hz;
						break;
					case PDC_IndividualTone_2Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone2Hz;
						break;
					case PDC_IndividualTone_4Hz:
						l_msg.ID = NORMAL_MSG_ID_IndividualTone4Hz;
						break;
					case PDC_ContinousTone:
						l_msg.ID = NORMAL_MSG_ID_ContinousTone;
						break;
					default:
						break;
				}
				
				if(l_msg.ID != 0)
				{
					l_msg.LcdStatus =  NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				s_PdcStat = PDC_Disable;
			}
		}
				
		if(((DEA_GetCodingPDC() == DEA_CodingWithBSDAnd4Sensor) || (DEA_GetCodingPDC() == DEA_CodingWithBSDAnd6Sensor)) 
			||(DEA_GetCanTimeOutStat(IPDU_HND_BSW_1) == 0))
		{
			if(DEA_GetBSW_Stat() != s_BswStat)
			{
				if(s_BswStat == TRUE)
				{
					l_msg.ID = NORMAL_MSG_ID_BSW_SoundType2Hz;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}
				else if(DEA_GetBSW_Stat() == TRUE)
				{
					l_msg.ID = NORMAL_MSG_ID_BSW_SoundType2Hz;
					l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
					l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
					l_msg.Acoustic = NORMAL_MSG_STAT_SET;
					PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				}

				s_BswStat = DEA_GetBSW_Stat();
			}
		}
		else
		{
			if(s_BswStat == TRUE)
			{
				l_msg.ID = NORMAL_MSG_ID_BSW_SoundType2Hz;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				s_BswStat = 0;
			}
		}
	}
	else
	{
		s_BswStat = 0;
		s_PdcStat = PDC_Disable;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
void AllPduMsgProc(void)
{
	GEAR_GearProc();
	Button_Proc();
	TPMS_Proc();
	TERMINAL_Proc();
	CruiseControlProc();
	BcmLightProc();
	BcmDoorStatProc();
	BcmWiperProc();
	EpsProc();
	Epbi_Proc();
	RadarProc();
	Ldws_Proc();
	Scu_Proc();
	Etg_Proc();
	Wlc_Proc();
	ACM_Proc();
	SESAM_Proc();
	TCU_Proc();
	MHU_Proc();
	TBOX_Proc();
	DME_Proc();
	BSW_Proc();
}

#endif


void IC_MessageTX(void)
{
	//Com_TxSig_ic_state_rollingCounter(1, &value1);//2F2
	//Com_TxSig_ic_avgPowerConsumCrntTrip(1, &value2);//0x482
	//Com_TxSig_ic_currentTripmeter(1, &value3);//0x492


	


	uint32_t temp = 0;	
	int32_t	temp1 = 0;

	/**totalMileage unit:0.1**/
	temp = DEA_GetOdo()*10;		
//	Com_SendSignal(SIG_HND_ic_totalMileage, &temp);

	temp = DEA_GetOdoValid();
//	Com_SendSignal(SIG_HND_ic_totalMileageValid, &temp);

	/**vechile speed  unit:1**/
	temp = DEA_GetFilterVehicleSpeed(); 
//	Com_SendSignal(SIG_HND_ic_vehicleSpeedDisp, &temp);

	temp = DEA_GetRawVchSpValidValue();
//	Com_SendSignal(SIG_HND_ic_vehicleSpeedDispValid, &temp);

	/**avgFuelConsumTripA  unit:0.1**/
	temp = DEA_GetAFC_A_Value()*10;
//	Com_SendSignal(SIG_HND_ic_avgFuelConsumTrip, &temp);

	/**avgFuelConsumTripB  unit:0.1**/
	temp = DEA_GetAFC_B_Value()*10;
//	Com_SendSignal(SIG_HND_ic_avgFuelConsumCrntTrip, &temp);

	/**avgPowerConsumTripA unit:0.1**/
	temp1 = DEA_GetAvpPackAValue();
//	Com_SendSignal(SIG_HND_ic_avgPowerConsumTrip, &temp1);

	/**avgPowerConsumTripB unit:0.1**/
	temp1 = DEA_GetAvpPackBValue();
//	Com_SendSignal(SIG_HND_ic_avgPowerConsumCrntTrip, &temp1);

	/**tripA**/
	temp = DEA_GetTripA();
//	Com_SendSignal(SIG_HND_ic_tripmeter, &temp);

	/**tripB**/
	temp = DEA_GetTripB();
//	Com_SendSignal(SIG_HND_ic_avgSpdCurrentTrip, &temp);

	/**AVS_tripA unit:0.1**/
	temp = DEA_GetAVSA();
//	Com_SendSignal(SIG_HND_ic_avgSpdTrip, &temp);

	/**AVS_tripB unit:0.1**/
	temp = DEA_GetAVSB();
//	Com_SendSignal(SIG_HND_ic_avgSpdCurrentTrip, &temp);
	

}

void NightDimmer_Proc(void)
{
	static uint32_t s_NightDimmerCnt = 0;
	//if(semiActiveIndirectIlluminationGet())
	{
		if((DEA_GetIgnStat() == IGN_ON) && (DEA_GetCanTimeOutStat(IPDU_HND_BCMMSG1) == 0))
		{
			if(DEA_GetCLUSTER_ILL_REQ() == 1)
			{
				s_NightDimmerCnt = 0;
				DEA_SetNightDimmerFlag(1);
			}
			else
			{
				if(DEA_GetNightDimmerFlag() == 1)
				{
					if(s_NightDimmerCnt < 40) //400ms
					{
						if((DEA_GetEngineStatusCopy() == 0b11) && (DEA_GetStaterControlStatus() == 1))
						{
							s_NightDimmerCnt = 0;
						}
						else
						{	
							s_NightDimmerCnt++;
						}
					}
					else
					{
						DEA_SetNightDimmerFlag(0);
					}
				}
			}
		}
		else
		{
			DEA_SetNightDimmerFlag(0);
		}
	}
}

void HardWare_Proc(void)
{
	uint8_t s_EpkbStatus = 0;
	static uint8_t s_EpkbOutPutStatus = 0;
	static uint32_t s_EpkbOutputCnt = 0;
	static uint8_t s_IdleStopStatus = 0;
	static uint8_t s_EngineTempRaw = 0;
	static uint32_t s_E_PkbIgnOffCnt = 0;
	int32_t l_EngineTempNow = 0;

	/********************* F-23-3  *********************/
	if(1)
	{
		if(DEA_GetCanTimeOutStat(IPDU_HND_EPB_SystemControl) == 0)
		{
			if(DEA_GetEPB_WORKMODE() == 0b01)
			{
				s_EpkbStatus = 2; //blink
			}
			else
			{
				if(((DEA_GetDynamicBrakingStatus() == 0b01) || (DEA_GetDynamicBrakingStatus() == 0b10)) || (DEA_GetEPB_STAT() == 0b001) || (DEA_GetEPB_STAT() == 0b111))
				{
					s_EpkbStatus = 1; //on
				}
				else if((DEA_GetEPB_STAT() == 0b000) || (DEA_GetEPB_STAT() == 0b101) || (DEA_GetEPB_STAT() == 0b110))
				{
					s_EpkbStatus = 2; //blink
				}
				else
				{
					s_EpkbStatus = 0; //off
				}
			}
		}
		else
		{
			s_EpkbStatus = 2; //blink
		}
	}

	if(DEA_GetIgnStat() == IGN_ON)
	{
		s_E_PkbIgnOffCnt = 0;
	}
	else
	{
		if(s_E_PkbIgnOffCnt < 30*100) //30S
		{
			s_E_PkbIgnOffCnt++;
		}
	}

	if(s_E_PkbIgnOffCnt < 30*100) //30S
	{
		if(s_EpkbStatus == 2)
		{
			if(s_EpkbOutputCnt < 50) //500ms 
			{
				s_EpkbOutputCnt++;
			}
			else
			{
				s_EpkbOutputCnt = 0;
				s_EpkbOutPutStatus = !s_EpkbOutPutStatus;
				if(s_EpkbOutPutStatus == 1)
				{
					Port_SetOutLevel(IOC_E_PKB_STATUS_SIGNAL_OUTPUT, PORT_HIGH);
				}
				else
				{
					Port_SetOutLevel(IOC_E_PKB_STATUS_SIGNAL_OUTPUT, PORT_LOW);
				}
			}
		}
		else if(s_EpkbStatus == 1)
		{
			if(s_EpkbOutPutStatus != 1)
			{
				s_EpkbOutPutStatus = 1;
				Port_SetOutLevel(IOC_E_PKB_STATUS_SIGNAL_OUTPUT, PORT_HIGH);
			}
			s_EpkbOutputCnt = 0;
		}
		else
		{
			if(s_EpkbOutPutStatus != 0)
			{
				s_EpkbOutPutStatus = 0;
				Port_SetOutLevel(IOC_E_PKB_STATUS_SIGNAL_OUTPUT, PORT_LOW);
			}
			s_EpkbOutputCnt = 0;
		}
	}
	else
	{
		if(s_EpkbOutPutStatus != 0)
		{
			s_EpkbOutPutStatus = 0;
			Port_SetOutLevel(IOC_E_PKB_STATUS_SIGNAL_OUTPUT, PORT_LOW);
		}
		s_EpkbOutputCnt = 0;
	}

	/********************* F-33-1 Idle stop *********************/
	if(semiActiveIdleStopFunctionGet())
	{
		if(DEA_GetIgnStat() == IGN_ON)
		{
			if(DEA_GetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS) == 0)
			{
				if(DEA_GetISMSKSW() == 1)
				{
					if(s_IdleStopStatus != 1)
					{
						s_IdleStopStatus  = 1;
						Port_SetOutLevel(IOC_IDLE_STOP_SW, PORT_HIGH);
					}
				}
				else
				{
					if(s_IdleStopStatus != 0)
					{
						s_IdleStopStatus  = 0;
						Port_SetOutLevel(IOC_IDLE_STOP_SW, PORT_LOW);
					}
				}
			}
			else
			{
				if(s_IdleStopStatus != 0)
				{
					s_IdleStopStatus  = 0;
					Port_SetOutLevel(IOC_IDLE_STOP_SW, PORT_LOW);
				}
			}
		}
		else
		{
			if(s_IdleStopStatus != 0)
			{
				s_IdleStopStatus  = 0;
				Port_SetOutLevel(IOC_IDLE_STOP_SW, PORT_LOW);
			}
		}
	}

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if((DEA_GetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus) == 0) && (DEA_GetCoolantTempRawValid()))
		{		
			s_EngineTempRaw = DEA_GetCoolantTempRaw();
			if(s_EngineTempRaw != 0)
			{	
				l_EngineTempNow = s_EngineTempRaw - 40;
				if(DEA_GetTempOutputMode() == 3)	//Toc
				{
					if(l_EngineTempNow <= 100)
					{
						//output Tob 130ms
						DEA_SetTempOutputMode(2); //tob
						waterTempWarnOutput(WARNING_B);
					}
				}
				else if(DEA_GetTempOutputMode() == 1) //Toa
				{
					if(l_EngineTempNow >= 56)
					{
						//output Tob 130ms
						DEA_SetTempOutputMode(2); //tob
						waterTempWarnOutput(WARNING_B);
					}
				}
				else
				{
					if(l_EngineTempNow >= 105)
					{
						//output Toc 65ms
						DEA_SetTempOutputMode(3); //toc
						waterTempWarnOutput(WARNING_C);
					}
					else if(l_EngineTempNow <= 36)
					{
						//output Toa 195ms
						DEA_SetTempOutputMode(1); //Toa
						waterTempWarnOutput(WARNING_A);
					}
				}
			}
			else
			{
				if(DEA_GetTempOutputMode() != 1)
				{
					//output Toa 195ms
					DEA_SetTempOutputMode(1); //Toa
					waterTempWarnOutput(WARNING_A);
				}
			}
		}
		else
		{
			if(DEA_GetTempOutputMode() != 1)
			{
				//output Toa 195ms
				DEA_SetTempOutputMode(1); //Toa
				waterTempWarnOutput(WARNING_A);
			}
		}
	}
	else
	{
		if((s_EngineTempRaw != 0) || (DEA_GetTempOutputMode() != 0))
		{
			s_EngineTempRaw = 0;
			DEA_SetTempOutputMode(0);
			waterTempWarnOutput(NORMAL);
		}
	}
}


void SpeedOutProc(void)
{
	static uint8_t flag = 0xFF;
	static uint32_t s_lastVehicleSpeed = 0;
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if((DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 0) && (DEA_GetRawVehicleSpeedErrorStat() == 0))
		{
			if(flag != 0)
			{
				flag = 0;
				VehicleSpeedTimerStart();
			}
			if(s_lastVehicleSpeed != DEA_GetRawVehicleSpeed()) // > 1km/h
			{
				s_lastVehicleSpeed = DEA_GetRawVehicleSpeed();
				VehicleSpeedChangeFreq(s_lastVehicleSpeed);
				//speedPwmOutput(GetVehicleSpeedFreq()/10);
			}
		}
		else
		{
			if(flag == 0)
			{
				flag = 1;
				//noSpeedOutput();
				VehicleSpeedTimerStop();
			}
		}
	}
	else
	{
		if(flag == 0)
		{
			flag = 1;
			//noSpeedOutput();
			VehicleSpeedTimerStop();
		}
	}
}


void CrankingJudge(void)
{
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if((DEA_GetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus) == 0) && (DEA_GetCanTimeOutStat(IPDU_HND_SMART_bcmMSG_PUSH) == 0))
		{
			if((DEA_GetEngineStatusCopy() == 0b11) && (DEA_GetStaterControlStatus() == 1))
			{
				DEA_SetCrankingStatus(1);
			}
			else
			{
				DEA_SetCrankingStatus(0);
			}
		}
		else
		{
			DEA_SetCrankingStatus(0);
		}
	}
	else
	{
		DEA_SetCrankingStatus(0);
	}
}

void TPMS_Proc(void)
{
	if(0)
	{
		SysSettingFrontTireSet(1);
	}
}

void SetSelfDiagDispPressedFlag(uint8_t stat)
{
	s_DispButtonUpdatedFlag = 1;
	s_DispButtonPressedFlag = stat;
}

void SetSelfDiagHmiPage(int num)
{
	s_HmiPageNum = num;
}

void SelfDiagModeCheck(void)
{
	static IGN_STAT_ENUM s_IgnStat = IGN_OFF;
	static uint8_t s_SelfDiagStartCheckFlag = 0;
	static uint32_t s_DispButtonPressedCnt = 0;
	static uint8_t s_DispButtonPressedFlagBack = 0;
	static uint8_t s_DispButtonNum = 0;
	static uint32_t s_DispButtonIdleCnt = 0;
	static uint32_t s_DispButtonOnOffTimerCnt = 0;
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnStat == IGN_OFF)
		{
			s_IgnStat = IGN_ON;
			if(s_DispButtonPressedFlag == 1)
			{
				s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_START;
			}
			s_DispButtonPressedCnt = 0;
			s_DispButtonPressedFlagBack = 0;
			s_DispButtonNum = 0;
			s_DispButtonIdleCnt = 0;
			s_DispButtonOnOffTimerCnt = 0;
		}
		
		if(s_SelfDiagStartCheckFlag)
		{		


			if((DEA_GetRawVehicleSpeed() >= 10) && (DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 0))
			{
				s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
				//set IPC self diag flag false
				IPC_SetPredriveCheckStatus(0);
				return;
			}
			
			if((DEA_GetRawEngineRevolution() >= 170) && (DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0))
			{
				s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
				//set IPC self diag flag false
				IPC_SetPredriveCheckStatus(0);
				return;
			}
			
			/*
			if(DEA_GetRawEngineRevolution() > 170)
			{
				s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
				//set IPC self diag flag false
				IPC_SetPredriveCheckStatus(0);
				return;
			}
			*/
			
			if(s_DispButtonIdleCnt < 30*100) //30S
			{
				s_DispButtonIdleCnt++;
			}
			else
			{
				//set IPC self diag flag false
				IPC_SetPredriveCheckStatus(0);
				return;
			}
			
			switch(s_SelfDiagStartCheckFlag)
			{
				case SELF_DIAG_MODE_START:
					if(1)// s_HmiPageNum == 10) //hmi kept driving distance B & time B content
					{
						if((s_DispButtonPressedFlag == 1) && (s_DispButtonPressedCnt < 80))	//0.8S
						{
							s_DispButtonPressedCnt++;
						}
						else
						{
							if(s_DispButtonPressedCnt < 80)
							{
								s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
								return;
							}
							else
							{
								if(s_DispButtonPressedFlag == 0)
								{
									s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_ON_OFF_3_CHECK;
									s_DispButtonPressedFlagBack = 0;
									s_DispButtonUpdatedFlag = 0;
								}
							}
						}
					}
					else
					{
						s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
					}
					
					s_DispButtonOnOffTimerCnt++;
					if(s_DispButtonOnOffTimerCnt >= 7*100) //7S
					{
						s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
					}
					break;
				case SELF_DIAG_MODE_ON_OFF_3_CHECK:	//check "TRUN ON and OFF the SW for 3 cycles"
					if(s_DispButtonUpdatedFlag != 0)
					{
						s_DispButtonUpdatedFlag  = 0;
						s_DispButtonIdleCnt = 0;
						if(s_DispButtonPressedFlagBack != s_DispButtonPressedFlag)
						{
							s_DispButtonPressedFlagBack = s_DispButtonPressedFlag;
							s_DispButtonNum++;
						}
					}
					if(s_DispButtonNum >= 6)
					{
						s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_ENTER;
						//set IPC self diag flag true
						IPC_SetPredriveCheckStatus(1);
					}
					
					s_DispButtonOnOffTimerCnt++;
					if(s_DispButtonOnOffTimerCnt >= 7*100) //7S
					{
						s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
					}
					break;
				case SELF_DIAG_MODE_ENTER: //wait exit condtion
					if(s_DispButtonUpdatedFlag != 0)
					{
						s_DispButtonUpdatedFlag  = 0;
						s_DispButtonIdleCnt = 0;
					}
					break;
				default:
					s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
					break;
			}
		}
	}
	else
	{
		if(s_IgnStat != IGN_OFF)
		{
			s_IgnStat = IGN_OFF;
			s_SelfDiagStartCheckFlag = SELF_DIAG_MODE_OFF;
			//set IPC self diag flag false
			IPC_SetPredriveCheckStatus(0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
void AllPduMsgProc(void)
{
	//GEAR_GearProc();/**traditional car gear**/
	Gear_MAIN();/**591 project require gear**/
	//IC_MessageTX();/**skon_106 TX messge**/
	//Button_Proc();
#if 0
	BcmDoorStatProc();
	BcmLightProc();
	ABS_Proc();
	EPS_Proc();
	SDM_Proc();
	//BCM_MSG1_Proc();
	TPMS_Proc();
#endif
	NightDimmer_Proc();
	HardWare_Proc();
	SpeedOutProc();
	CrankingJudge();
	SelfDiagModeCheck();
	CanAppDiagClientProc();
}


