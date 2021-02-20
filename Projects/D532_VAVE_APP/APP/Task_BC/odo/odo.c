/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : odo.c
**
**          Date  : 2017-05-26
**          
**          
******************************************************************************/


/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "odo.h"
//#include "CanApp_AliveProc.h"
#include "NvM.h"
#include "string.h"
#include "MemMap.h"
#include "stdlib.h"
//#include "NvM_if.h"

//#include "portmacro.h"
//#include "Interrupt.h"

#include "Vel_cfg.h"

/******************************************************************************
**    MACROS
******************************************************************************/


/******************************************************************************
**    VARIABLE DEFINITIONS
******************************************************************************/

int odo_init_flag = 0;
uint8_t WriteNvmFlag = WRITE_SUCESS;

//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED uint32_t g_odo_back = 0; 
BACKRAM_SEC_VAR_UNSPECIFIED ODO_STRU g_odo = {0};
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t NvmOdoDisplayCrc;
NVM_SEC_VAR_UNSPECIFIED ODO_PARA_STRU g_odo_para = {0};
NVM_SEC_VAR_UNSPECIFIED uint32_t g_odo_Odo = 0;
NVM_SEC_VAR_UNSPECIFIED uint32_t g_odo_back1 = 0;
static uint32_t s_odo_crc_power_up = 0;
static uint8_t NvmReadFlag = 0;



//#pragma segment BACK_RAM_END

uint8_t ODO_IsMillageSingalValid(void)
{
	/*if((DEA_GetCanMillage() != 0xFFFF) && (CanApp_GetAliveErrorStat(ALIVE_ID_EPBi_MILEAGE) == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	*/
	return 1;
}

static uint16_t s_millage = 0;
uint16_t GetMillageSingal(void)
{
	return DEA_GetCanMillage();
}

uint8_t IsBcmVinValid(void)
{
	/*if(DEA_GetCanTimeOutStat(IPDU_HND_BCM_VEHICLE_IDENT_NUMBER) != 0)
	{
		return 0;
	}*/

	if(!DEA_GetBCM_VinValid())
	{
		return 0;
	}
	
	return 1;
}

uint8_t IsIpcBcmVinSame(void)
{
	/*uint8_t l_ipc_vin[7] = {0}, l_bcm_vin[7] = {0};
	DEA_GetBCM_Vin(l_bcm_vin);
	DEA_GetCodingVIN(l_ipc_vin);

	if(memcmp(l_bcm_vin, l_ipc_vin, 7) != 0)
	{
		return 0;
	}*/
	return 1;
}

uint8_t IsNVRAMValid(void)
{
	return 1;
}

uint8_t IsMillageCrcMatch(void)
{
	return DEA_GetBCM_ODO_CrcMatch();
}

uint8_t ReadOdoClearTimesFromNVRAM(uint8_t *OdoClearTimes)
{
	if(NvM_ReadBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_READ_NVRAM_ERR;
		return 0;
	}
	else
	{
		*OdoClearTimes = g_odo_para.OdoClearTimes;
		 return 0;
	}
}

uint8_t WriteOdoClearTimesToNVRAM(uint8_t OdoClearTimes)
{
	g_odo_para.OdoClearTimes = OdoClearTimes;
	if(NvM_WriteBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_WRITE_NVRAM_ERR;
		return 0;
	}
	else
	{
		return 0;
	}
}

uint8_t ReadOdoWriteTimesFromNVRAM(uint8_t *OdoWriteTimes)
{
	if(NvM_ReadBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_READ_NVRAM_ERR;
		return 0;
	}
	else
	{
		*OdoWriteTimes = g_odo_para.OdoWriteTimes;
		return 0;
	}
}

uint8_t WriteOdoWriteTimesToNVRAM(uint8_t OdoWriteTimes)
{
	g_odo_para.OdoWriteTimes = OdoWriteTimes;
	if(NvM_WriteBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_WRITE_NVRAM_ERR;
		return 0;
	}
	else
	{
		return 0;
	}
}


uint8_t ReadOdoOffsetFromNVRAM(uint16_t *OdoOffsetValue)
{
	if(NvM_ReadBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_READ_NVRAM_ERR;
		return 0;
	}
	else
	{
		*OdoOffsetValue = g_odo_para.OdoOffsetValue;
		return 0;
	}
}

uint8_t WriteOdoOffsetToNVRAM(uint16_t OdoOffsetValue)
{
	g_odo_para.OdoOffsetValue = OdoOffsetValue;
	if(NvM_WriteBlock(ODO_PARA_NVRAM_INDEX, (uint8_t *)&g_odo_para) != E_OK)
	{
		//return ODO_WRITE_NVRAM_ERR;
		return 0;
	}
	else
	{
		return 0;
	}
}


uint8_t ReadOdoFromNVRAM(uint32_t *OdoValue)
{
	if(NvM_ReadBlock(ODO_NVRAM_INDEX, (uint8_t *)OdoValue) != E_OK)
	{
		//return ODO_READ_NVRAM_ERR;
		return 0;
	}
	else
	{
		return 0;
	}
}

uint8_t WriteOdoToNVRAM(uint32_t *pOdoValue)
{
	uint8_t l_ret = 0;

	g_odo_back = *pOdoValue;
	g_odo_back1 = *pOdoValue;
	l_ret |= NvM_WriteBlock(ODO_BACK_NVRAM_INDEX, (uint8_t *)(&g_odo_back));
	l_ret |= NvM_WriteBlock(ODO_BACK1_NVRAM_INDEX, (uint8_t *)(&g_odo_back1));
	l_ret |= NvM_WriteBlock(ODO_NVRAM_INDEX, (uint8_t *)pOdoValue);
	if(l_ret != E_OK)
	{
		return ODO_WRITE_NVRAM_ERR;
	}
	else
	{
		return 0;
	}

}

uint8_t OdoWriteCallback(uint8 ServiceId, uint8_t JobResult)
{
	static uint8_t res = 0;
	res = JobResult;
	return 0;
}


uint8_t ODO_WriteOdo(uint32_t NewOdoValue)
{
	static uint32_t s_NewOdoValue = 0;
	uint8_t OdoWriteTimes = 0;
	ReadOdoWriteTimesFromNVRAM(&OdoWriteTimes);
	if((OdoWriteTimes < ODO_MAX_WRITE_TIMES) && (NewOdoValue <= ODO_MAX_VALUE)) 
	{
		if(NewOdoValue <= g_odo.OdoDisplay)
		{
			return ODO_WRITE_LE_ERR;
		}
		else
		{
			s_NewOdoValue = NewOdoValue + g_odo.OdoOffset;
			if(WriteOdoToNVRAM(&s_NewOdoValue) != 0)
			{
				return ODO_WRITE_NVRAM_ERR;
			}
			else
			{
				g_odo_Odo = NewOdoValue + g_odo.OdoOffset;
				g_odo.OdoDisplay = NewOdoValue;
				OdoWriteTimes++;
				WriteOdoWriteTimesToNVRAM(OdoWriteTimes);
			}
		}
	}
	else
	{
		return ODO_WRITE_TIMES_OVERFLOW_ERR;
	}
	return ODO_OK;
}

uint8_t ODO_ResetOdo(void)
{
	uint8_t OdoClearTimes = 0;
	uint8_t l_tryTimes = 0;
	
	ReadOdoClearTimesFromNVRAM(&OdoClearTimes);

	/*
	* If ODO = 0km reset will be denied
	* IPC shall only allow to reset <255km. If ODO = > 255km a reset will be denied. 
	* Allow 100 time reset 
	* Max reset is 510km		   
	*/
	if((g_odo.OdoDisplay > 0) && (g_odo.OdoDisplay < ODO_MAX_RESET_VALUE) && (OdoClearTimes < ODO_MAX_RESET_TIMES) && (g_odo_Odo <= ODO_OFFSET_MAX_VALUE))
	{
		while(l_tryTimes++ < 3)
		{
			if(WriteOdoOffsetToNVRAM( g_odo_Odo) != 0)	//make OdoOffset equal to Odo
			{
				continue;
			}
			else
			{
				ReadOdoOffsetFromNVRAM(&g_odo.OdoOffset);
				if(g_odo.OdoOffset != g_odo_Odo)
				{
					continue;
				}
				
				OdoClearTimes++;
				if(WriteOdoClearTimesToNVRAM( OdoClearTimes ) != 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
		
		if(l_tryTimes >= 3)
		{
			return ODO_WRITE_NVRAM_ERR;
		}
		else
		{
			return ODO_OK;
		}
	}
	else
	{
		if(g_odo.OdoDisplay >= ODO_MAX_RESET_VALUE)
		{
			return ODO_DISPLAY_OVER_ERR;
		}
		else if(OdoClearTimes >= ODO_MAX_RESET_TIMES)
		{
			return ODO_RESET_TIMES_OVER_ERR;
		}
		else if(g_odo.OdoDisplay == 0)
		{
			return ODO_ALREADY_RESET_ERR;
		}
		else
		{
			return ODO_RESET_ERR;
		}
	}
}

void ODO_FactoryReset(void)
{
	uint32_t l_u32Value = 0;
	WriteOdoToNVRAM(&l_u32Value);
	WriteOdoOffsetToNVRAM(0);
	WriteOdoClearTimesToNVRAM(0);
	WriteOdoWriteTimesToNVRAM(0);

	g_odo.Millage = 0;
	g_odo_Odo = 0;
	g_odo.OdoCnt = 0;
	g_odo.OdoDisplay = 0;
	g_odo.OdoDisplayMode = 0;
	g_odo.OdoOffset = 0;
	g_odo.OdoOneMeterCnt = 0;
}

uint8_t ODO_ResetTripA(void) //for current tripB clear
{
	//Std_GlobalInterruptDisable();
	g_odo.TripA = 0;
	//Std_GlobalInterruptRestore();
	return ODO_OK;
}

uint8_t ODO_ResetTripB(void) //for current tripB clear
{
	//Std_GlobalInterruptDisable();
	g_odo.TripB = 0;
	//Std_GlobalInterruptRestore();
	return ODO_OK;
}

uint32_t ODO_GetOdo(void)
{
	return g_odo.OdoDisplay;
}

uint8_t ODO_GetTripA_Status(void)
{
	return g_odo.TripA_DisplayValid;
}

uint8_t ODO_GetTripB_Status(void)
{
	return g_odo.TripB_DisplayValid;
}

uint32_t ODO_GetTripA(void) //get tripA
{
	return g_odo.TripA;
}

uint32_t ODO_GetTripB(void) //get tripB
{
	return g_odo.TripB;
}

uint32_t ODO_GetOdoOneMeterCnt(void)
{
	return g_odo.OdoOneMeterCnt;
}

uint8_t ODO_GetOdoValid(void)
{
	return !g_odo.OdoDisplayMode;
}

uint32_t ODO_GetOdo10Tick(void)
{
	return 0;
}

uint32_t ODO_Check(uint8_t state0, uint8_t state1, uint8_t state2)
{
	uint32_t l_best_odo = 0;
	if(((state0 == NVM_REQ_OK) && (g_odo_Odo <= ODO_MAX_VALUE)) 
		&& ((state1 == NVM_REQ_OK) && (g_odo_back <= ODO_MAX_VALUE)) && ((state2 == NVM_REQ_OK) && (g_odo_back1 <= ODO_MAX_VALUE)))
	{
		if((g_odo_Odo == g_odo_back) && (g_odo_Odo == g_odo_back1))	//three block are same
		{
			l_best_odo = g_odo_Odo;
		}
		else if(g_odo_Odo == g_odo_back)	//block 0 same to block 1
		{
			l_best_odo = g_odo_Odo;
		}
		else if(g_odo_Odo == g_odo_back1)	//block 0 same to block 2
		{
			l_best_odo = g_odo_Odo;
		}
		else if(g_odo_back == g_odo_back1)	//block 1 same to block 2
		{
			l_best_odo = g_odo_back;
		}
		else //find middle block
		{
			if((g_odo_Odo < g_odo_back) && (g_odo_back < g_odo_back1))
			{
				l_best_odo = g_odo_back;
			}
			else if((g_odo_back < g_odo_Odo) && (g_odo_Odo < g_odo_back1))
			{
				l_best_odo = g_odo_Odo;
			}
			else
			{
				l_best_odo = g_odo_back1;
			}
		}
	}
	else if(((state0 == NVM_REQ_OK) && (g_odo_Odo <= ODO_MAX_VALUE)) && ((state1 == NVM_REQ_OK) && (g_odo_back <= ODO_MAX_VALUE)))
	{
		if(g_odo_Odo == g_odo_back)
		{
			l_best_odo = g_odo_Odo;
		}
		else if(g_odo_Odo < g_odo_back)	//find small value
		{
			l_best_odo = g_odo_Odo;
		}
		else
		{
			l_best_odo = g_odo_back;
		}
	}
	else if(((state0 == NVM_REQ_OK) && (g_odo_Odo <= ODO_MAX_VALUE)) && ((state2 == NVM_REQ_OK) && (g_odo_back1 <= ODO_MAX_VALUE)))
	{
		if(g_odo_Odo == g_odo_back1)
		{
			l_best_odo = g_odo_Odo;
		}
		else if(g_odo_Odo < g_odo_back1)
		{
			l_best_odo = g_odo_Odo;
		}
		else
		{
			l_best_odo = g_odo_back1;
		}
	}
	else if(((state1 == NVM_REQ_OK) && (g_odo_back <= ODO_MAX_VALUE)) && ((state2 == NVM_REQ_OK) && (g_odo_back1 <= ODO_MAX_VALUE)))
	{
		if(g_odo_back == g_odo_back1)
		{
			l_best_odo = g_odo_back;
		}
		else if(g_odo_back < g_odo_back1)
		{
			l_best_odo = g_odo_back;
		}
		else
		{
			l_best_odo = g_odo_back1;
		}
	}
	else if((state0 == NVM_REQ_OK) && (g_odo_Odo <= ODO_MAX_VALUE))
	{
		l_best_odo = g_odo_Odo;
	}
	else if((state1 == NVM_REQ_OK) && (g_odo_back <= ODO_MAX_VALUE))
	{
		l_best_odo = g_odo_back;
	}
	else if((state2 == NVM_REQ_OK) && (g_odo_back1 <= ODO_MAX_VALUE))
	{
		l_best_odo = g_odo_back1;
	}
	else
	{
		l_best_odo = 0;
	}
	
	return l_best_odo;
}


void ODO_Init(void)
{
	uint8_t NvmGetState = 0;
	uint8_t NvmGetState_bak = 0;
	uint8_t NvmGetState_bak1 = 0;
	uint8_t l_updateOdoFlag = 0;
	uint32_t l_odo_best = 0;

	if(odo_init_flag == 0)
	{
		NvM_GetErrorStatus(ODO_NVRAM_INDEX,&NvmGetState);
		NvM_GetErrorStatus(ODO_BACK_NVRAM_INDEX,&NvmGetState_bak);
		NvM_GetErrorStatus(ODO_BACK1_NVRAM_INDEX,&NvmGetState_bak1);
		
		if((NvmGetState != NVM_REQ_OK) || (NvmGetState_bak != NVM_REQ_OK) || (NvmGetState_bak1 != NVM_REQ_OK))
		{
			NvmReadFlag = 1;
		}

		s_odo_crc_power_up = crc32(&(g_odo.OdoDisplay), 4);	//crc cheak
		
		l_odo_best = ODO_Check(NvmGetState, NvmGetState_bak, NvmGetState_bak1);

		if((DEA_GetSysRstType() == RST_IRRST) || (s_odo_crc_power_up != NvmOdoDisplayCrc))	//BAT ON or backram crc error
		{
			if(NvmReadFlag) //wait to judge recover fail
			{
				g_odo.OdoOffset = 0;
				g_odo.OdoOneMeterCnt = 0;
				if(l_odo_best == 0)
				{
					//g_odo.OdoDisplay = 44;	//44 to show BAT_ON && Read NVM Error
					g_odo.OdoDisplay = 0;	//2020-3-11 17:17:16
				}
				else
				{
					g_odo.OdoDisplay = l_odo_best;
				}
				g_odo.OdoDisplayMode = ODO_DISP_MODE_INVALID;
			}
			else
			{
				g_odo.OdoOffset = g_odo_para.OdoOffsetValue;
				g_odo.OdoOneMeterCnt = l_odo_best*1000;
				g_odo.OdoDisplay = l_odo_best;	//- g_odo.OdoOffset;	//to send the NVM odo to HMI show
				if(g_odo.OdoDisplay > ODO_MAX_VALUE)
				{
					g_odo.OdoDisplay = ODO_MAX_VALUE;
				}
				NvmOdoDisplayCrc = crc32(&(g_odo.OdoDisplay), 4);	//update the odo_dispaly crc code
				g_odo.OdoDisplayMode = ODO_DISP_MODE_NORMAL;
			}

			l_updateOdoFlag = 1;
		}
		else
		{
			if((l_odo_best != 0) && (abs(g_odo.OdoDisplay - l_odo_best) > 1) && (l_odo_best <= ODO_MAX_VALUE))
			{
				l_updateOdoFlag = 1;
				g_odo.OdoOneMeterCnt = l_odo_best*1000;
				g_odo.OdoDisplay = l_odo_best;
			}
			else
			{
				if(g_odo.OdoDisplay > ODO_MAX_VALUE)
				{
					g_odo.OdoOneMeterCnt = 0;
					g_odo.OdoDisplay = 55;	//55 to show not BAT_ON && Read NVM Error
					l_updateOdoFlag = 1;
				}
			}			
		}

		if(l_updateOdoFlag)
		{	
			g_odo.Millage = 0;
			g_odo.TripOneMeterCnt = 0;
			g_odo.OdoCnt = 0;
			g_odo.TripA = 0;
			g_odo.TripB = 0;

			g_odo.TripA_DisplayValid = 1U;
			g_odo.TripB_DisplayValid = 1U;
			g_odo.OdoDisplayMode = ODO_DISP_MODE_NORMAL;
		}
		odo_init_flag = 1;
	}
}

void ODO_MillageProc(void) //10ms cycle
{
	static uint32_t s_red = 0;
	uint32_t l_millage = 0;
	uint16_t l_u16Value = 0;
	uint32_t VechileSpeed_vo2 = 0;

	uint32_t l_temp = 0;
	static uint32_t s_temp = 0;

	if(DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 0)   //signal not timeout
	{
		Com_ReceiveSignal(SIG_HND_VSO, &l_u16Value); //get the inicial vehicle speed 
		//VechileSpeed_vo2 = (l_u16Value*1011U);	//expand 100*1000 times
		//VechileSpeed_vo2 = (l_u16Value*1011U);	//expand 100*1000 times
		VechileSpeed_vo2 = (l_u16Value*VEL_B);    //expand 100*1000 times
		
		if((l_u16Value < 65535) && (VechileSpeed_vo2 >= (2*100*1000U)) && (VechileSpeed_vo2 <= (350*100*1000)))  //signal valid
		{
			/*if(VechileSpeed_vo2 > (350*100*1000))
			{
				VechileSpeed_vo2 = (350*100*1000);
			}*/
			
			l_millage = s_millage;  

			//get the vehcile speed(km/h) and change km/h to cm/s:  vehcileSpeed/3.6
			//l_millage += (((uint32_t)l_u16Value*15625*10)/((uint32_t)360*1000)+5)/10;
				
			
			l_temp = (VechileSpeed_vo2 + s_temp)/(1000);	//the parameter is 1.011
			s_temp = (VechileSpeed_vo2 + s_temp)%(1000);
			
			l_millage += (l_temp + s_red)/360;  //591 project uint:0.01
			s_red  = (l_temp + s_red)%360;  //get the rest data
			
			if(l_millage < s_millage)
			{
				s_millage = l_millage + 1;
			}
			else
			{
				s_millage = l_millage;
			}
			DEA_SetCanMileage(s_millage);//uint: Cm
		}
		else
		{
			s_red = 0;//the rest of calculation
		}
	}
	else
	{	
		s_red = 0;
	}
}

static uint32_t g_odo_bak = 0;
void ODO_Main(void)
{
	static IGN_STAT_ENUM IgnStatBak = IGN_OFF;
	static uint8_t ErrorMode = 0U;
	static uint16_t s_OdoComparisonTimerCnt = 0;
	uint16_t millage_offset = 0U, millage = 0U;
	uint16_t l_MeterCnt = 0U;
	//uint8_t l_tryTimes = 0;

	uint8_t l_NvmGetState = 0;
	uint8_t l_NvmGetState_bak = 0;
	uint8_t l_NvmGetState_bak1 = 0;
	uint32_t l_odo_best = 0;

	static uint16_t readNvmCnt = 0;
	static uint16_t retryNvmCnt = 0;
	
	//make sure the NVM state is OK ,if not OK  restart the system
	if(1 == NvmReadFlag)
	{
		if(0 == readNvmCnt)
		{
			NvM_GetErrorStatus(ODO_NVRAM_INDEX, &l_NvmGetState);
			NvM_GetErrorStatus(ODO_BACK_NVRAM_INDEX, &l_NvmGetState_bak);
			NvM_GetErrorStatus(ODO_BACK1_NVRAM_INDEX, &l_NvmGetState_bak1);
			if(l_NvmGetState != NVM_REQ_OK)
				NvM_ReadBlock(ODO_NVRAM_INDEX, &g_odo_Odo);
			if(l_NvmGetState_bak != NVM_REQ_OK)
				NvM_ReadBlock(ODO_BACK_NVRAM_INDEX, &g_odo_back);
			if(l_NvmGetState_bak1 != NVM_REQ_OK)
				NvM_ReadBlock(ODO_BACK1_NVRAM_INDEX, &g_odo_back1);
		}
		
		if(readNvmCnt < 2)
		{
			readNvmCnt++;
		}
		else
		{
			NvM_GetErrorStatus(ODO_NVRAM_INDEX, &l_NvmGetState);
			NvM_GetErrorStatus(ODO_BACK_NVRAM_INDEX, &l_NvmGetState_bak);
			NvM_GetErrorStatus(ODO_BACK1_NVRAM_INDEX, &l_NvmGetState_bak1);
			
			/*TracePrintf(COM_TRACE, TRACE_WARNING, "state1:%d\r\n state2:%d\r\n state3:%d\r\n odo1:%d\r\n odo2:%d\r\n odo3:%d\r\n",
			l_NvmGetState,l_NvmGetState_bak,l_NvmGetState_bak1,g_odo_Odo,g_odo_back,g_odo_back1); */
			
			if((l_NvmGetState != NVM_REQ_OK) || (l_NvmGetState_bak != NVM_REQ_OK) || (l_NvmGetState_bak1 != NVM_REQ_OK))
			{
				if(retryNvmCnt < 10)
				{
					retryNvmCnt++;
					readNvmCnt = 0;
				}
				else
				{
					NvmReadFlag = 0;
					l_odo_best = ODO_Check(l_NvmGetState, l_NvmGetState_bak, l_NvmGetState_bak1);
					if(s_odo_crc_power_up == NvmOdoDisplayCrc)	//backram value is valid
					{
						if(l_odo_best == 0)	//three block are invalid,use backram data recovery
						{
							g_odo_Odo = g_odo.OdoDisplay;
							g_odo_back = g_odo.OdoDisplay;
							g_odo_back1 = g_odo.OdoDisplay;
						}
						else if(abs(g_odo.OdoDisplay-l_odo_best) >= 1)	//use backram data recovery main block
						{
							g_odo_Odo = g_odo.OdoDisplay;
						}
					}
					else
					{
						if(l_odo_best != 0)	//use best block data recovery backram data
						{
							g_odo_Odo = l_odo_best;
							g_odo.OdoDisplay = l_odo_best;
						}
					}
				}
			}
			else
			{
				NvmReadFlag = 0;
				l_odo_best = ODO_Check(l_NvmGetState, l_NvmGetState_bak, l_NvmGetState_bak1);
				g_odo_Odo = l_odo_best;
				g_odo.OdoDisplay = l_odo_best;
			}
		}
	}
	else
	{
		if(DEA_GetIgnStat() == IGN_ON)
		{
			if(IgnStatBak == IGN_OFF)
			{
				IgnStatBak = IGN_ON; //set the flag
				s_OdoComparisonTimerCnt = 0;//start to count
				g_odo.Millage = GetMillageSingal();	//unit:cm
			}

			if(/*DEA_GetRawVehicleSpeed() < 3500*/1)
			{
				if(g_odo_bak != g_odo.OdoDisplay)
				{
					g_odo_bak = g_odo.OdoDisplay;
					TracePrintf(0,0,0,0,"Odo:%d\r\n", g_odo.OdoDisplay);
				}
			
				if(s_OdoComparisonTimerCnt <= ODO_COMP_TIME)//not come to 8s
				{
					if(s_OdoComparisonTimerCnt == ODO_COMP_TIME)
					{
						if(g_odo.OdoDisplay >= 255 && IsMillageCrcMatch())	//odo >= 255km  and millage CRC Match
						{
							if((DEA_GetBcmOdo() < g_odo.OdoDisplay) || !IsIpcBcmVinSame() || !IsBcmVinValid())
							{
								//ErrorMode = 1U;
							}
						}
					}
					s_OdoComparisonTimerCnt++;
				}
			
				if(ODO_IsMillageSingalValid())
				{

					if(WRITE_FAILED == WriteNvmFlag)	//cheak if the last write action is successed
					{
						if(WriteOdoToNVRAM( &g_odo_Odo ) == 0)
						{
							WriteNvmFlag = WRITE_SUCESS;
						}
					}
					else
					{
						WriteNvmFlag = WRITE_SUCESS;
					}
				
					if(ErrorMode == 0)
					{
						if(g_odo.OdoDisplayMode == ODO_DISP_MODE_ERR)
						{
							g_odo.OdoDisplayMode = ODO_DISP_MODE_NORMAL;
						}
					}
					else
					{
						g_odo.OdoDisplayMode  = ODO_DISP_MODE_ERR;
					}
				
				
					millage = GetMillageSingal();	//unit:cm
					if(millage >= g_odo.Millage)	//get the distence betwen of the two cycle
					{
						millage_offset = millage - g_odo.Millage;	/**the trip have passed**/ 
					}
					else
					{
						millage_offset = millage - g_odo.Millage - 1U;
					}

					//if((millage_offset >= 100U) && (DEA_GetFilterVehicleSpeed() > 20))	// 1m = 100cm,if the error of two times sample over 1m and the vechile speed over 2km/h(591 project requirement)
					if(millage_offset >= 100U)//2020-3-19 16:30:14
					{
						g_odo.Millage = millage - millage_offset%100;
					
						l_MeterCnt = millage_offset/100U;	/**make sure how many 100cm have passed**/
						g_odo.TripOneMeterCnt += l_MeterCnt;
						g_odo.OdoOneMeterCnt += l_MeterCnt;

						/**tripA/tripB**///per 100m calculate 
						if(g_odo.TripOneMeterCnt >= 100U)	// 0.1km
						{
							g_odo.TripOneMeterCnt = g_odo.TripOneMeterCnt%100U;	/**together the left of not exceed 100m**/

							/**tripA**/
							g_odo.TripA++;	
							if(g_odo.TripA > TRIP_MAX_VALUE)	//999.9km
							{
								g_odo.TripA = 0;
							}

							/**tripB**/
							g_odo.TripB++;	
							if(g_odo.TripB > TRIP_MAX_VALUE)	//999.9km
							{
								g_odo.TripB = 0;
							}

							//per 10 numbers of 0.1km calculate
							if(ErrorMode == 0)//get the odo
							{
								g_odo.OdoCnt++;
								if(g_odo.OdoCnt >= 10U)	//1km
								{
									g_odo.OdoCnt = 0;

									g_odo.OdoDisplay = g_odo_Odo;// - g_odo.OdoOffset;
									if(g_odo.OdoDisplay < ODO_MAX_VALUE)	//999999km(里程总计还未达到最大阈值)
									{
										g_odo_Odo++;
										g_odo.OdoDisplay++;
										NvmOdoDisplayCrc = crc32(&(g_odo.OdoDisplay), 4);	//update the odo_dispaly crc code
	
										if(WriteOdoToNVRAM( &g_odo_Odo ) != 0)
										{
											WriteNvmFlag = WRITE_FAILED;
										}
										else
										{
											WriteNvmFlag = WRITE_SUCESS;
										}
									}
								}
							}
						}	
					}
					g_odo.TripA_DisplayValid = 1U;
					g_odo.TripB_DisplayValid = 1U;
				}
				else
				{
					g_odo.TripA_DisplayValid = 0;
					g_odo.TripB_DisplayValid = 0;
				}	
			}
		}
		else
		{
			IgnStatBak = IGN_OFF;
			ErrorMode = 0;
		}
	}
}
