//******************************************************************************
//file		  		: cti.c 
//Description	  	: calculate output speed information
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/5/8         eason             First draft version
//2017/6/1         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup cti
//! @{
//
//*****************************************************************************

#include "cti.h"
#include "cti_cfg.h"
#include "string.h"  
#include "Task_IpcApp_Cfg.h" /**warning message**/

#include <stdio.h>
#include <trace.h>



/** cti processing  state structure*/
typedef enum 
{
   CTI_Normal,		/**< Normal State */
   CTI_Exception,	/**< Exception State */		
} CTI_Mode;

typedef enum
{
	CTI_RSP_FAST,
	CTI_RSP_MID,
	CTI_RSP_SLOW,
}CTI_RSP_ENUM;

/** define  cti processing  state varible */
static CTI_Mode CTI_enMode;     

/** define  DPOOL type for cti information*/
static CTI_Data_STRU stCti;

static CTI_RSP_ENUM CTI_RspMode;



/**
  * \brief judge if exceed range
  *
  *\param CTI_u16Cti current coolant temperature value  
  *   
  * \return exceed cti flag
  */
static unsigned char CTI_u8ExceedCtiJudge(unsigned char CTI_u8Cti)
{
	unsigned char CTI_u8ExceedCtiFlag;
	
	if(CTI_u8Cti > CTI_u8MaxCti)
	{
		CTI_u8ExceedCtiFlag = CTI_True;
	}
	else
	{
		CTI_u8ExceedCtiFlag = CTI_False;
	}
	
	return CTI_u8ExceedCtiFlag;
}

/**
  * \brief exception process except exceed value
  *
  *\param None
  *   
  * \return None
  */
static void CTI_vExceptionProcess(void)
{
	memset( &stCti, 0, sizeof( stCti));
	//stCti.CTI_SegValue = 8;	/* Signal invalid, reserved or timeout, highlight all LED's */
}

/**
  * \brief Initial cti processing  state , Initial dpool data
  *
  *\param None  
  *   
  * \return None
  */
void CTI_vInit(void)
{
	CTI_enMode = CTI_Normal;
	memset( &stCti, 0, sizeof( stCti));
	CTI_vWrite(stCti);
}


void CTI_SegDisplayProc(uint8_t coolant) /**Ë®ÎÂ¶ÎÂëÏÔÊ¾**/
{
	static uint16_t s_cnt = 0;
	uint8_t l_SegValue = 0;

	switch(CTI_RspMode)
	{
		case CTI_RSP_FAST:
		{
			stCti.CTI_SegValue = CTI_CalSeg(coolant);
			CTI_RspMode = CTI_RSP_MID;
			s_cnt = 0;
			break;
		}
		case CTI_RSP_MID:
		{
			l_SegValue = CTI_CalSeg(coolant);
			if(l_SegValue == stCti.CTI_SegValue)
			{
				s_cnt = 0;
				CTI_RspMode = CTI_RSP_SLOW;
			}
			else
			{
				s_cnt++;
				if(s_cnt >= CTI_RspTimeReachMid)	/* 0.1S */
				{
					s_cnt = 0;

					if(stCti.CTI_SegValue < l_SegValue)
					{
						stCti.CTI_SegValue++;
					}
					else
					{
						stCti.CTI_SegValue--;
					}
				}
			}
			break;
		}
		case CTI_RSP_SLOW:
		{
			l_SegValue = CTI_CalSeg(coolant);
			if(l_SegValue == stCti.CTI_SegValue)
			{
				s_cnt = 0;
			}
			else
			{
				s_cnt++;
				if(s_cnt >= CTI_RspTimeReachSlow)	/* 3S */
				{
					s_cnt = 0;

					if(stCti.CTI_SegValue < l_SegValue)
					{
						stCti.CTI_SegValue++;
					}
					else
					{
						stCti.CTI_SegValue--;
					}
				}
			}
			break;
		}
		default:
			break;
	}	
}

/**
  * \brief From cti input information , calurate dpool data
  *
  *\param None  
  *   
  * \return None
  */
void CTI_vMain(void)
{	
	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;
	static CTI_Mode CTI_enModeBak = CTI_Normal;
	CTI_InputInfor CTI_stInputInfor;

	static uint16_t CtiCnt = 0;
	static uint16_t CtiExceptonCnt = 0;
	
	unsigned char CTI_ArmFlag = 0xFF;	//Cti ArmeFLag
	static TELLTALE_ENUM s_CtiArlmeInd = TELLTALE_OFF;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		CTI_stInputInfor = CTI_stUpdateCtiInfor();

		switch(CTI_enMode)
		{
			case CTI_Normal:
				
				if((CTI_stInputInfor.u8CtiValid == CTI_True) && (CTI_stInputInfor.u8CtiTimeoutFlag == CTI_False))
				{
					CtiExceptonCnt = 0;
					stCti.CTI_u8CoolTempValue = CTI_stInputInfor.u8CtiValue;
		
					if(CTI_u8ExceedCtiJudge(stCti.CTI_u8CoolTempValue))	//exception valid value Exceed max
					{
						stCti.CTI_u8CoolTempValue = CTI_u8MaxCti;
					}
					
					stCti.CTI_boTempValid = CTI_True;

					
					if(stCti.CTI_u8CoolTempValue > 1090) //yangxl 109
					{
						
						CTI_ArmFlag = CTI_True;
						
					}
					else if(stCti.CTI_u8CoolTempValue < 1060)//yangxl 106
					{
						
						CTI_ArmFlag = CTI_False;
						
					}

					/**591 project cti angle indication calculate**/
					//the angle has expanded 10times 
					
					if(stCti.CTI_u8CoolTempValue <= 500)	//cti less than 50
					{
						stCti.CTI_AngleValue = 0;	
					}
					else if(stCti.CTI_u8CoolTempValue < 700)	//50 < x <70
					{
						stCti.CTI_AngleValue = 0 + 343*(stCti.CTI_u8CoolTempValue - 500)/200;	//(343/20)*((stCti.CTI_u8CoolTempValue - 500)/10)
					}
					else if(stCti.CTI_u8CoolTempValue <= 1050)	//70 <= x <= 105
					{
						stCti.CTI_AngleValue = 343;
					}
					else if(stCti.CTI_u8CoolTempValue < 1100)	//105 < x < 110
					{
						stCti.CTI_AngleValue = 343 + 391*(stCti.CTI_u8CoolTempValue - 1050)/50;	//(391/14)*((stCti.CTI_u8CoolTempValue - 1050)/10)
					}
					else if(stCti.CTI_u8CoolTempValue == 1100)	//x = 110
					{
						stCti.CTI_AngleValue = 734;
					}
					else if(stCti.CTI_u8CoolTempValue < 1300)	//119 < x < 130
					{
						stCti.CTI_AngleValue = 734 + 166*(stCti.CTI_u8CoolTempValue - 1090)/210;	//(166/11)*((stCti.CTI_u8CoolTempValue - 1190)/10)
					}
					else
					{
						stCti.CTI_AngleValue = 900;		// x >= 130
					}
					
					
					/**the cti segement display**/
					#if 0
					if((s_IgnBak == IGN_OFF) || (CTI_enModeBak == CTI_Exception)) 
					{
						CTI_RspMode = CTI_RSP_MID;
						CTI_SegDisplayProc(stCti.CTI_u8CoolTempValue);
					}
					else
					{					
						CTI_SegDisplayProc(stCti.CTI_u8CoolTempValue);
					}
					#endif
					
				}
				else
				{
					if(CtiExceptonCnt < 60*10) //60S
					{
						CtiExceptonCnt++;
					}
					else
					{
						CtiCnt = 0;
						CTI_vExceptionProcess();
						CTI_enMode = CTI_Exception;
						CTI_enModeBak = CTI_Exception;
						stCti.CTI_boTempValid = CTI_False;

						CTI_ArmFlag = CTI_False;	
					}
				}
				
				break;
				
			case CTI_Exception:
				
				if((CTI_stInputInfor.u8CtiValid == CTI_True) && (CTI_stInputInfor.u8CtiTimeoutFlag == CTI_False)) 
				{
					CTI_enMode = CTI_Normal;
					CTI_ArmFlag = CTI_False;
				}
				
				break;		

		}
		
		CTI_vWrite(stCti);
		s_IgnBak = IGN_ON;
		
	
		if(CTI_ArmFlag == CTI_True)
		{
			if(s_CtiArlmeInd != TELLTALE_ON)
			{
				s_CtiArlmeInd = TELLTALE_ON;
			 	IPC_PutAlarmMsg(NORMAL_MSG_ID_Engineoverheated, TELLTALE_ON, TELLTALE_ON, ACOUSTIC_OFF);//led = ON;icon = OFF;voice = OFF
			}
		}
		else if(CTI_ArmFlag == CTI_False)
		{
			if(s_CtiArlmeInd != TELLTALE_OFF)
			{
				s_CtiArlmeInd = TELLTALE_OFF;
				IPC_PutAlarmMsg(NORMAL_MSG_ID_Engineoverheated, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//led = OFF;icon = OFF;voice = OFF
			}		
		}

	}
	else
	{
		if(s_IgnBak == IGN_ON)
		{
			s_IgnBak = IGN_OFF;
		}

		if(s_CtiArlmeInd != TELLTALE_OFF)
		{
			s_CtiArlmeInd = TELLTALE_OFF;
			IPC_PutAlarmMsg(NORMAL_MSG_ID_Engineoverheated, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//led = OFF;icon = OFF;voice = OFF
		}

		memset(&stCti,0,sizeof(stCti));	//clear the data
		CTI_vWrite(stCti);
	}
}


void CtiTest(void)
{
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[BC]:CTI_CoolTempValue = %d \r\n", (stCti.CTI_u8CoolTempValue/10));
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[BC]:CTI_AngleValue = %d \r\n", (stCti.CTI_AngleValue/9));
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




