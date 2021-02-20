#include "dea.h"
#include "HistoryAfc.h"
#include "MemMap.h"
#include <stdio.h>
#include <trace.h>


/*
BACKRAM_SEC_VAR_UNSPECIFIED HIS_AFC	HIS_afc;	//prepare the passing data

BACKRAM_SEC_VAR_UNSPECIFIED uint8_t HAVS[6] = {0};
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t show_valid = 0;

BACKRAM_SEC_VAR_UNSPECIFIED uint16_t best_afc = 300;		//the inicial afc = 30.0l/100km
BACKRAM_SEC_VAR_UNSPECIFIED uint16_t MIN = 300;
*/

BACKRAM_SEC_VAR_UNSPECIFIED HIS_AFC_STR g_His_afc;

//get the distence of 100ms (uint:m)
uint16_t HAFC_Distance(void)
{
	static uint16_t distence = 0U;
	static uint32_t s_OneMeterCntBak = 0U; 
	uint32_t l_OneMeterCnt = 0U;
 
	l_OneMeterCnt = ODO_GetOdoOneMeterCnt();	//100ms
	if(s_OneMeterCntBak == 0)
	{
		s_OneMeterCntBak = l_OneMeterCnt;	//per 100m the  cnt_value +1
		distence = 0;
	}
	else
	{
		if(l_OneMeterCnt > s_OneMeterCntBak)
		{
			distence = (l_OneMeterCnt - s_OneMeterCntBak);
			s_OneMeterCntBak = l_OneMeterCnt;
		}
		else
		{
			distence = 0;
		}
	}

	return distence;
}


/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 2019-12-20 19:27:29
*****************************************************************************/
uint16 GetCurrentAFData(void)
{
	return g_His_afc.CurrentAF;
}

/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 2019-12-20 19:29:05
*****************************************************************************/
void SetCurrentAFData(uint16 CurrentAFData)
{
	g_His_afc.CurrentAF = CurrentAFData;
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
void HisAfcDataInit(void)
{
	uint8_t l_clc = 0u;

	for (l_clc = 0; l_clc < HIS_AFC_MAX_LENTH; l_clc++)
	{
		g_His_afc.fuelcons_value[l_clc] = 0UL;
		g_His_afc.mileage_value[l_clc] = 0UL;
		g_His_afc.ipc.LastAF[l_clc] = HIS_AFC_INIT_DATA;
	}
	g_His_afc.value_cnt = 0U;
	g_His_afc.value_30s_flg = 0u;
	g_His_afc.updateflg = 0u;
	g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 0u;
	g_His_afc.ipc.BestAvgFuelFlag = 0u;
	g_His_afc.ipc.EcoRank = 0u;
	g_His_afc.ipc.EcoRankFlag = 0u;
	//g_His_afc.ipc.total_afc_value = HIS_AFC_INIT_DATA;
	g_His_afc.ipc.total_afc_value = 0xffff;
	g_His_afc.ipc.best_afc_value = HIS_AFC_INIT_DATA;
	g_His_afc.CurrentAF = 0; //2019-12-20 19:26:13
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
void HisAfcInit(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		HisAfcDataInit();
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

void ReSetHisAfc(void)
{
	HisAfcDataInit();
	g_His_afc.updateflg = 1u;
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

void HisAfcTotalSingleCalc(void)
{
	uint32_t l_mileage_value =0ul;
	uint32_t l_fuelcons_value =0ul;
	uint8_t l_clc =0u;
	uint8_t l_cnt=0u;

	for (l_clc = 0; l_clc < HIS_AFC_MAX_LENTH; l_clc++)
	{
		l_fuelcons_value += g_His_afc.fuelcons_value[l_clc];
		l_mileage_value += g_His_afc.mileage_value[l_clc];

		if(l_clc <= (HIS_AFC_MAX_LENTH - 2))
		{
			//g_His_afc.ipc.LastAF[HIS_AFC_MAX_LENTH-l_clc-2] = g_His_afc.ipc.LastAF[HIS_AFC_MAX_LENTH-l_clc-1];
			g_His_afc.ipc.LastAF[HIS_AFC_MAX_LENTH-l_clc-1] = g_His_afc.ipc.LastAF[HIS_AFC_MAX_LENTH-l_clc-2];
		}
	}
	
	if(l_mileage_value > 0u)
	{
		g_His_afc.ipc.total_afc_value = l_fuelcons_value / l_mileage_value;
		
		if(g_His_afc.ipc.total_afc_value > HIS_AFC_MAX_DATA)
		{
			g_His_afc.ipc.total_afc_value = HIS_AFC_MAX_DATA;
		}
	}else
	{
		g_His_afc.ipc.total_afc_value = HIS_AFC_MAX_DATA;
	}
	
	l_cnt = g_His_afc.value_cnt;
	
	if(g_His_afc.mileage_value[l_cnt]>0u)
	{
		g_His_afc.ipc.LastAF[0] = g_His_afc.fuelcons_value[l_cnt] / g_His_afc.mileage_value[l_cnt];

		if(g_His_afc.ipc.LastAF[0] > HIS_AFC_MAX_DATA)
		{
			g_His_afc.ipc.LastAF[0] = HIS_AFC_MAX_DATA;
		}
	}else
	{
		g_His_afc.ipc.LastAF[0] = HIS_AFC_MAX_DATA;
	}

	g_His_afc.value_cnt++;
	
	if(g_His_afc.value_cnt>= HIS_AFC_MAX_LENTH)
	{
		g_His_afc.value_cnt = 0u;
	}
	
	//if(g_His_afc.ipc.LastAF[0]<g_His_afc.ipc.best_afc_value)
	if((g_His_afc.ipc.LastAF[0]<g_His_afc.ipc.best_afc_value) || \
		((g_His_afc.ipc.best_afc_value == HIS_AFC_INIT_DATA) && (g_His_afc.ipc.LastAF[0] != HIS_AFC_INIT_DATA)))
	{
		g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 2u;
	}else
	{
		g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 1u;
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
void HisAfcMain(void)
{
	static uint16_t s_timecnt = 0;
	static uint8_t s_ign_status = IGN_OFF;
	static uint32_t s_mileage_value =0;
	static uint32_t s_fuelcons_value =0;
	uint8_t l_cnt =0u;

	l_cnt = g_His_afc.value_cnt;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_ign_status == IGN_OFF)
		{
			s_ign_status = IGN_ON;
			g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 0u;
			SetHisAfcUpdateFlg(1u);
			SetCurrentAFData(0u);//2019-12-20 19:32:15
		}
		if((DEA_GetFilterVehicleSpeedValid()==1u)&&(DEA_GetIFC_Valid()==1u))
		{
			if((DEA_GetFilterEngineRevolution() > 170) && (1 == DEA_GetFilterEngineRevolutionValid()))
			{
				if(s_timecnt<300ul)
				{
					s_timecnt++;
					g_His_afc.value_30s_flg = 0u;
				}else
				{
					if(s_mileage_value > 500ul)
					{
						g_His_afc.value_30s_flg = 1u;
					}
				}
				
				if(s_mileage_value < MAX_DIS)
				{
					s_mileage_value += HAFC_Distance();			//calculate the drive distence during IGN_ON	(uint: m)
					s_fuelcons_value += DEA_GetTII_Value();	//calculate the fuel consume during IGN_ON	(uint: ul)
				}
				
		#if 0
				if(g_His_afc.mileage_value[l_cnt] < MAX_DIS)
				{
					g_His_afc.mileage_value[l_cnt] += HAFC_Distance();			//calculate the drive distence during IGN_ON	(uint: m)
					g_His_afc.fuelcons_value[l_cnt] += DEA_GetTII_Value();	//calculate the fuel consume during IGN_ON	(uint: ul)
				}
		#endif
				
			}
		}
			
	}else
	{
		if(s_ign_status == IGN_ON)
		{
			s_ign_status = IGN_OFF;
			SetHisAfcUpdateFlg(1u);
			//g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 0u;
		}
		s_timecnt = 0ul;
		g_His_afc.mileage_value[l_cnt] = s_mileage_value;
		g_His_afc.fuelcons_value[l_cnt] = s_fuelcons_value;	
		if(g_His_afc.value_30s_flg == 1u)
		{
			g_His_afc.value_30s_flg = 0u;
			HisAfcTotalSingleCalc();
			SetCurrentAFData(GetHisLastAfcData());//2019-12-20 19:31:30
			//g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 1u;
			g_His_afc.updateflg = 1u;
		}
		//g_His_afc.mileage_value[l_cnt] = 0ul;
		//g_His_afc.fuelcons_value[l_cnt] = 0ul;
		s_mileage_value = 0;
		s_fuelcons_value =0;
		
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
void UpdateBestAfc(void)
{
	SetHisAfcUpdateFlg(1u);
	//if(g_His_afc.ipc.LastAF[0]<g_His_afc.ipc.best_afc_value)
	if((g_His_afc.ipc.LastAF[0]<g_His_afc.ipc.best_afc_value) || \
		((g_His_afc.ipc.best_afc_value == HIS_AFC_INIT_DATA) && (g_His_afc.ipc.LastAF[0] != HIS_AFC_INIT_DATA)))
	{
		g_His_afc.ipc.best_afc_value = g_His_afc.ipc.LastAF[0];
		g_His_afc.ipc.BestAvgFuelFlag = 1u;
		g_His_afc.ipc.HistoryAvgFuelEconomyFlag = 0u;
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
uint8_t GetHisAfcUpdateFlg(void)
{
	return g_His_afc.updateflg;
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
void SetHisAfcUpdateFlg(uint8_t flg)
{
	g_His_afc.updateflg = flg;
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

IPC_APP_HISTORY_AFC_STRU GetHisAfcData(void)
{
	return g_His_afc.ipc;
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
uint16 GetHisBestAfcData(void)
{
	return g_His_afc.ipc.best_afc_value;
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
uint16 GetHisLastAfcData(void)
{
	return g_His_afc.ipc.LastAF[0];
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
void HisAfcDataTest(void)
{
	uint8_t cycle =0u;
	
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_Bc_HisAfc]:data \r\n");
	TracePrintf(COM_TRACE, TRACE_NONMASK, "HistoryAvgFuelEconomyFlag = %d \r\n", g_His_afc.ipc.HistoryAvgFuelEconomyFlag);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "EcoRankFlag = %d \r\n", g_His_afc.ipc.EcoRankFlag);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "EcoRank = %d \r\n", g_His_afc.ipc.EcoRank);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "bast_afc_value = %d \r\n", g_His_afc.ipc.best_afc_value);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "total_afc_value = %d \r\n", g_His_afc.ipc.total_afc_value);
	
	for (cycle = 0; cycle < HIS_AFC_MAX_LENTH; cycle++)
	{
		TracePrintf(COM_TRACE, TRACE_NONMASK, "LastAF%d = %d \r\n",cycle ,g_His_afc.ipc.LastAF[cycle]);
	}
}

#if 0
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void HisAfcDataTest1(IPC_APP_HISTORY_AFC_STRU HisAvgFuelData)
{
	uint8_t cycle =0u;
	
	TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_Bc_HisAfc]:data \r\n");
	TracePrintf(COM_TRACE, TRACE_NONMASK, "HistoryAvgFuelEconomyFlag = %d \r\n", HisAvgFuelData->HistoryAvgFuelEconomyFlag);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "EcoRankFlag = %d \r\n",  HisAvgFuelData->EcoRankFlag);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "EcoRank = %d \r\n",  HisAvgFuelData->EcoRank);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "bast_afc_value = %d \r\n",  HisAvgFuelData->bast_afc_value);
	TracePrintf(COM_TRACE, TRACE_NONMASK, "total_afc_value = %d \r\n",  HisAvgFuelData->total_afc_value);
	
	for (cycle = 0; cycle < HIS_AFC_MAX_LENTH; cycle++)
	{
		TracePrintf(COM_TRACE, TRACE_NONMASK, "LastAF%d = %d \r\n",cycle , HisAvgFuelData->LastAF[cycle]);
	}
}
#endif

#if 0
void HAFC_Init(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		best_afc = 300;
		show_valid = 0;
		MIN = 300;
		HIS_afc.value_one = 300;
		HIS_afc.value_two = 300;
		HIS_afc.value_three = 300;
		HIS_afc.value_four = 300;
		HIS_afc.value_five = 300;
		HIS_afc.value_six = 300;
	}
	DEA_SetFilterHistoryAfcValue(HIS_afc);
	DEA_SetFilterBestHistoryAfcValue(best_afc);
	DEA_SetFilterHistoryAfcShowValid(show_valid);
	IPC_SetHistoryAvgFuel(HIS_afc.value_one, HIS_afc.value_two, HIS_afc.value_three, 
						HIS_afc.value_four, HIS_afc.value_five, HIS_afc.value_six, best_afc);
}


//update the best afc
void Update_BestAfc(void)
{
	best_afc = MIN;
	DEA_SetFilterBestHistoryAfcValue(best_afc);
}
//make the reset funcation for button pressed clear
void ReSet_HAFC(void)
{
	HIS_afc.value_one = 300;
	HIS_afc.value_two = 300;
	HIS_afc.value_three = 300;
	HIS_afc.value_four = 300;
	HIS_afc.value_five = 300;
	HIS_afc.value_six = 300;
	MIN = 300;
	best_afc = 300;
	DEA_SetFilterHistoryAfcValue(HIS_afc);
	DEA_SetFilterBestHistoryAfcValue(best_afc);
}


//get the distence of 100ms (uint:m)
uint16_t HAFC_Distance(void)
{
	static uint16_t distence = 0U;
	static uint32_t s_OneMeterCntBak = 0U; 
	uint32_t l_OneMeterCnt = 0U;
 
	l_OneMeterCnt = ODO_GetOdoOneMeterCnt();	//100ms
	if(s_OneMeterCntBak == 0)
	{
		s_OneMeterCntBak = l_OneMeterCnt;	//per 100m the  cnt_value +1
		distence = 0;
	}
	else
	{
		if(l_OneMeterCnt > s_OneMeterCntBak)
		{
			distence = (l_OneMeterCnt - s_OneMeterCntBak);
			s_OneMeterCntBak = l_OneMeterCnt;
		}
		else
		{
			distence = 0;
		}
	}

	return distence;
}




void HisAfc_VMAIN()	//100ms cycle
{
	
	static uint32_t dis_value = 0;
	static uint32_t tii_value = 0;

	static IGN_STAT_ENUM ign_state = IGN_OFF;

	//static uint16_t afc[6] = {0};	//default value 0l/100km
	

	static uint16_t timecnt = 0;
	static uint8_t ign_on_30s = 0;
	static uint16_t afc_value = 0;

	uint8_t i = 0,j = 0;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		dis_value += HAFC_Distance();			//calculate the drive distence during IGN_ON	(uint: m)
		tii_value += DEA_GetTII_Value();	//calculate the fuel consume during IGN_ON	(uint: ul)

		if(timecnt < 300)	//< 30s
		{
			timecnt++;

			if(ign_state == IGN_OFF)
			{
				ign_state = IGN_ON;
			}
		}
		else				//30s cycle come
		{
			timecnt = 0;
			ign_on_30s = 1;

			if((tii_value/1000) >= MAX_VOL)
			{
				tii_value = MAX_VOL;
			}

			if(dis_value >= MAX_DIS)
			{
				dis_value = MAX_DIS;
			}
		}

		show_valid = 0;
		DEA_SetFilterHistoryAfcShowValid(show_valid);
	}
	else
	{
		
		if(ign_state == IGN_ON)
		{
			if((dis_value >= 500) && (ign_state == IGN_ON) && (1 == ign_on_30s))
			{	
				//afc_value = (tii_value/1000000)/(dis_value/1000)	(l/km) -> afc_value =  x/(y*1000) = ((x*100)/(y*1000))/100km = x/(y*10)/100km = (x/y)/100km(expand 10 times)
				afc_value = tii_value/dis_value;	//l/100km		expand 10times
				if(afc_value >= 300)
				{
					afc_value = 300;
				}

				//remove the oldest value
				#if 0
				for(i = 5;i >= 1; i--)
				{
					afc[i] = afc[i - 1];
				}
				afc[0] = afc_value;
				#endif 
				HIS_afc.value_six   = HIS_afc.value_five;
				HIS_afc.value_five  = HIS_afc.value_four;
				HIS_afc.value_four	= HIS_afc.value_three;		
				HIS_afc.value_three = HIS_afc.value_two ;
				HIS_afc.value_two 	= HIS_afc.value_one;
				HIS_afc.value_one 	= afc_value;
				

				//find the min value
				#if 0
				for(i = 0;i < 6; i++)	
				{
					if(afc[i] < MIN)
					{
						MIN = afc[i];
					}
				}
				#endif


				MIN = afc_value;


				//judge if exist more smaller best AFC
				if(MIN < best_afc)
				{
					show_valid = 2;		
				}
				else if(MIN >= best_afc)
				{
					show_valid = 1; 	
				}
				
			}

			ign_state = IGN_OFF;
			
			DEA_SetFilterHistoryAfcValue(HIS_afc);
			DEA_SetFilterBestHistoryAfcValue(best_afc);
			DEA_SetFilterHistoryAfcShowValid(show_valid);

		}
		
		dis_value = 0;
		tii_value = 0;
		
		timecnt = 0;
		ign_on_30s = 0;	//clear the flag
		
	}
}

#endif

