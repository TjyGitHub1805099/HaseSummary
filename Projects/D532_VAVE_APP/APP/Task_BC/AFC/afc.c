//******************************************************************************
//file		  		: afc.c 
//Description	  	: calculate output afc information
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			       Author		   Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup afc
//! @{
//
//*****************************************************************************

#include "afc_cfg.h"                
#include "afc.h"    
#include "odo.h"
#include "MemMap.h"

/** AFC increased distance information */
typedef struct  
{
	uint16_t u16IncDistance;	/**< distance value */
	uint8_t u8ValidIncDis;	/**< distance validity */	
}AFC_tstIncrDis;


/** define next mileage information */
BACKRAM_SEC_VAR_UNSPECIFIED AFC_tstMileage AFC_stNextMileage = {0};

/** define first mileage information */
BACKRAM_SEC_VAR_UNSPECIFIED AFC_tstMileage AFC_stFirstMileage = {0};

/** sum distance a, unit cm */
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t u32SumDisA = 0;

/** sum fule consumption a, unit ul */
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t u32SumFuelA = 0;

/** sum distance b, unit cm */
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t u32SumDisB = 0;

/** sum fule consumption b, unit ul */
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t u32SumFuelB = 0;

/** define avs publish varible */
BACKRAM_SEC_VAR_UNSPECIFIED AFC_Data_STRU AFC_stOutputData = {0};

BACKRAM_SEC_VAR_UNSPECIFIED uint16_t AFC_TimecntA = 0;//yangxl
BACKRAM_SEC_VAR_UNSPECIFIED uint16_t AFC_TimecntB = 0;//yangxl

BACKRAM_SEC_VAR_UNSPECIFIED AFC_STR g_Afc = {0};




/**
  * \brief get Distance every cycle
  *
  *\param None
  *   
  * \return increased distance and validity
  */
static AFC_tstIncrDis AFC_stGetDistance(void)
{
	AFC_tstIncrDis stIncDisInfor;
	static uint32_t s_OneMeterCntBak = 0U; 
	uint32_t l_OneMeterCnt = 0U;

	l_OneMeterCnt = ODO_GetOdoOneMeterCnt();
	if(s_OneMeterCntBak == 0)
	{
		s_OneMeterCntBak = l_OneMeterCnt;
		stIncDisInfor.u16IncDistance = 0;
		stIncDisInfor.u8ValidIncDis = AFC_False;
	}
	else
	{
		if(l_OneMeterCnt > s_OneMeterCntBak)
		{
			stIncDisInfor.u16IncDistance = (l_OneMeterCnt - s_OneMeterCntBak);
			s_OneMeterCntBak = l_OneMeterCnt;
		}
		else
		{
			stIncDisInfor.u16IncDistance = 0;
		}
		stIncDisInfor.u8ValidIncDis = AFC_True;
	}

	return stIncDisInfor;
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
void Afc_vReset(void)
{
	g_Afc.u16ShowData = 0ul;
	g_Afc.u16Timecnt = 0ul;
	g_Afc.u32ConsData = 0ul;
	g_Afc.u32DisData = 0ul;
	g_Afc.u8boValid = 0u;
}


/**
  * \brief reset AFC A
  *
  *\param None
  *   
  * \return None
  */
void AFC_vResetA(void)
{
	u32SumDisA = 0;
	u32SumFuelA = 0;
	AFC_stOutputData.AFC_boValidA = 0;
	AFC_stOutputData.AFC_u16AvgConsA = 0;

	AFC_TimecntA = 0;//yangxl

	AFC_vWriteData(AFC_stOutputData);
}

/**
  * \brief reset AFC B
  *
  *\param None
  *   
  * \return None
  */
void AFC_vResetB(void)
{

   u32SumDisB = 0;
   u32SumFuelB = 0;
   AFC_stOutputData.AFC_boValidB = 0;
   AFC_stOutputData.AFC_u16AvgConsB = 0;

   AFC_TimecntB = 0;//yangxl
   
   AFC_vWriteData(AFC_stOutputData);

}

void AFC_vInit(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		u32SumDisA = 0;
		u32SumFuelA = 0;
		AFC_stOutputData.AFC_boValidA = 0;
		AFC_stOutputData.AFC_u16AvgConsA = 0;
		u32SumDisB = 0;
		u32SumFuelB = 0;
		AFC_stOutputData.AFC_boValidB = 0;
		AFC_stOutputData.AFC_u16AvgConsB = 0;

		AFC_TimecntA = 0;//yangxl
		AFC_TimecntB = 0;//yangxl

		Afc_vReset();
	}
}

/**
  * \brief calculate average fuel consumption every cycle
  *
  *\param None
  *   
  * \return None
  */ 
void AFC_vMain(void)
{
	static uint32 s_ConsDataTemp = 0ul;
	TII_Data_STRU stFuelConsum;
	AFC_tstIncrDis stIncDistance;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		//stFuelConsum = AFC_u16GetFuelInc();		//ul 获取喷油量
		//stIncDistance = AFC_stGetDistance();	//m 	获取里程数
		if((DEA_GetFilterVehicleSpeedValid()==1u)&&(DEA_GetIFC_Valid()==1u))
		{
			
			if((DEA_GetFilterEngineRevolution() > 170) && (1 == DEA_GetFilterEngineRevolutionValid()))
			{
				stFuelConsum = AFC_u16GetFuelInc();		//ul 获取喷油量
				stIncDistance = AFC_stGetDistance();	//m 	获取里程数
				//AFC A
				u32SumDisA += stIncDistance.u16IncDistance ; //实时累加里程数
				u32SumFuelA += stFuelConsum.TII_u16FuelConsum ; //实时累加喷油量

				//AFC B
				u32SumDisB += stIncDistance.u16IncDistance;
				u32SumFuelB += stFuelConsum.TII_u16FuelConsum;

				AFC_TimecntA++;//yangxl
				AFC_TimecntB++;//yangxl

				//AFC
				s_ConsDataTemp += stFuelConsum.TII_u16FuelConsum;
				if(s_ConsDataTemp >= 1000ul)
				{
					g_Afc.u32ConsData = g_Afc.u32ConsData + (s_ConsDataTemp / 1000ul);
					s_ConsDataTemp = s_ConsDataTemp % 1000ul;
				}
				g_Afc.u32DisData += stIncDistance.u16IncDistance;
				
				if(g_Afc.u32DisData >= AFC_MAX_DIS_AFC)
				{
					g_Afc.u32DisData = AFC_MAX_DIS_AFC;
				}
				if(g_Afc.u32ConsData >= AFC_MAX_DIS_AFC)
				{
					g_Afc.u32ConsData = AFC_MAX_DIS_AFC;
				}
				
				g_Afc.u16Timecnt++;
			}
		#if 0	
			if(AFC_TimecntA >= 300)	//reach 30s
			{
				AFC_TimecntA = 0;
				
    			//calculate AFC_A
    			if((u32SumDisA >= AFC_MAXDIS) ||(u32SumFuelA >= AFC_MAXFUEL)) //overflow 
    			{
    				u32SumDisA = u32SumDisA/2;
    				u32SumFuelA = u32SumFuelA/2;
    			}
    					
    			if(u32SumDisA > AFC_EnDISPLAY)	//exceed 500m /**591 project range**/
    			{
    				AFC_stOutputData.AFC_u16AvgConsA = (u32SumFuelA) / u32SumDisA; //*10(NN.N L/100km change to NNN L/100km) have expand 10 times
    				if(AFC_stOutputData.AFC_u16AvgConsA > AFC_MAXAFC)
    				{
    					AFC_stOutputData.AFC_u16AvgConsA = AFC_MAXAFC;
    				}
    				AFC_stOutputData.AFC_boValidA = AFC_True;
    			}
    			else
    			{
    				AFC_stOutputData.AFC_boValidA = AFC_False;
    			}
    			
			}



			
			if(AFC_TimecntB >= 300)	//reach 30s
			{

				AFC_TimecntB = 0;
				
    			//calculate AFC_B
    			if((u32SumDisB >= AFC_MAXDIS) || (u32SumFuelB >= AFC_MAXFUEL)) //overflow
    			{
    				u32SumDisB = u32SumDisB/2;
    				u32SumFuelB = u32SumFuelB/2;
    			}
    
    			if(u32SumDisB > AFC_EnDISPLAY)	//exceed 500m /**591 project range**/
    			{
    				AFC_stOutputData.AFC_u16AvgConsB = (u32SumFuelB) / u32SumDisB; //*10(NN.N L/100km change to NNN L/100km) have expand 10 times
    				if(AFC_stOutputData.AFC_u16AvgConsB > AFC_MAXAFC)
    				{
    					AFC_stOutputData.AFC_u16AvgConsB = AFC_MAXAFC;
    				}
    
    				AFC_stOutputData.AFC_boValidB = AFC_True;
    			}
    			else
    			{
    				AFC_stOutputData.AFC_boValidB = AFC_False;
    			}
			}
		#else
			if((AFC_TimecntA >= 300) && (u32SumDisA > AFC_EnDISPLAY)) //reach 30s exceed 500m //calculate AFC_A
			{
				AFC_TimecntA = 0;
    			if((u32SumDisA >= AFC_MAXDIS) ||(u32SumFuelA >= AFC_MAXFUEL)) //overflow 
    			{
    				u32SumDisA = u32SumDisA/2;
    				u32SumFuelA = u32SumFuelA/2;
    			}
				
				AFC_stOutputData.AFC_u16AvgConsA = (u32SumFuelA) / u32SumDisA; //*10(NN.N L/100km change to NNN L/100km) have expand 10 times
				if(AFC_stOutputData.AFC_u16AvgConsA > AFC_MAXAFC)
				{
					AFC_stOutputData.AFC_u16AvgConsA = AFC_MAXAFC;
				}
				AFC_stOutputData.AFC_boValidA = AFC_True;
			}else
			{
				;//AFC_stOutputData.AFC_boValidA = AFC_False;
			}

			if((AFC_TimecntB >= 300) && (u32SumDisB > AFC_EnDISPLAY)) //reach 30s exceed 500m //calculate AFC_B //calculate AFC_B
			{
				AFC_TimecntB = 0;
    			if((u32SumDisB >= AFC_MAXDIS) || (u32SumFuelB >= AFC_MAXFUEL)) //overflow
    			{
    				u32SumDisB = u32SumDisB/2;
    				u32SumFuelB = u32SumFuelB/2;
    			}
				AFC_stOutputData.AFC_u16AvgConsB = (u32SumFuelB) / u32SumDisB; //*10(NN.N L/100km change to NNN L/100km) have expand 10 times
				if(AFC_stOutputData.AFC_u16AvgConsB > AFC_MAXAFC)
				{
					AFC_stOutputData.AFC_u16AvgConsB = AFC_MAXAFC;
				}

				AFC_stOutputData.AFC_boValidB = AFC_True;
				
			}else
			{
				;//AFC_stOutputData.AFC_boValidB = AFC_False;
			}
		#endif

			//AFC
			if((g_Afc.u16Timecnt >= 300ul) && (g_Afc.u32DisData >= 500ul))
			{
				g_Afc.u16Timecnt = 0UL;
				if(g_Afc.u32DisData >= 100000ul)
				{
					g_Afc.u16ShowData = g_Afc.u32ConsData / (g_Afc.u32DisData / 1000ul);
				}else
				{
					g_Afc.u16ShowData = (g_Afc.u32ConsData * 1000ul) / g_Afc.u32DisData;
				}
				
				if(g_Afc.u16ShowData >= AFC_MAXAFC)
				{
					g_Afc.u16ShowData = AFC_MAXAFC;
				}
				g_Afc.u8boValid = AFC_True;
				
			}else
			{
				;//g_Afc.u8boValid = AFC_False;
			}
		}
		AFC_vWriteData(AFC_stOutputData);
	}
	else
	{
		AFC_TimecntA = 0;//yangxl
		AFC_TimecntB = 0;//yangxl
		g_Afc.u16Timecnt = 0UL;
		//memset(&AFC_stOutputData,0,sizeof(AFC_stOutputData));
		//AFC_vWriteData(AFC_stOutputData);
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
uint16 GetAfcShow(void)
{
	return g_Afc.u16ShowData;
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
uint8 GetAfcValid(void)
{
	return g_Afc.u8boValid;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************






