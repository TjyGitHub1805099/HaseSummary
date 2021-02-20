//******************************************************************************
//file		  		: ifc.c 
//Description	  	: calculate output ifc information
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			       Author		   Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup ifc
//! @{
//
//*****************************************************************************
#include  "ifc_cfg.h"          
#include  "ifc.h"  
#include "tii_cfg.h" 
#include "string.h"  
#include "odo.h"
#include "MemMap.h"


/** define next mileage information */
BACKRAM_SEC_VAR_UNSPECIFIED IFC_tstMileage IFC_stNextMileage;

/** define first mileage information */
BACKRAM_SEC_VAR_UNSPECIFIED IFC_tstMileage IFC_stFirstMileage;

/** define output information */
BACKRAM_SEC_VAR_UNSPECIFIED IFC_Data_STRU stIfc;

/** IFC increased distance information */
typedef struct  
{
	uint16_t u16IncDistance;	/**< distance value */
	uint16_t u8ValidIncDis;	/**< distance validity */	
}IFC_tstIncrDis;



/**
  * \brief get Distance every cycle
  *
  *\param None
  *   
  * \return increased distance and validity
  */
static uint32_t IFC_stGetDistance(void)
{
	/*
	static uint32_t s_OneMeterCntBak = 0U; 
	uint32_t l_OneMeterCnt = 0U, l_return = 0U;
	l_OneMeterCnt = ODO_GetOdoOneMeterCnt();
	if(s_OneMeterCntBak == 0)
	{
		s_OneMeterCntBak = l_OneMeterCnt;
		return 0;
	}
	else
	{
		if(l_OneMeterCnt > s_OneMeterCntBak)
		{
			l_return = (l_OneMeterCnt - s_OneMeterCntBak);
			s_OneMeterCntBak = l_OneMeterCnt;
		}
		return l_return;
	}*/

	static uint16_t s_Millage = 0;
	uint16_t millage = 0, millage_offset = 0;

	millage = DEA_GetCanMillage();	//unit:cm
	if(millage >= s_Millage)
	{
		millage_offset = millage - s_Millage;	
	}
	else
	{
		millage_offset = millage - s_Millage - 1U;
	}
	s_Millage = millage;
	
	return millage_offset;
}

/**
  * \brief initial IFC, and update dpool
  *
  *\param None
  *   
  * \return None
  */
void IFC_vInit (void)
{
	memset( &stIfc, 0, sizeof( stIfc));
	IFC_vWrite(stIfc);
}

/**
  * \brief deinit IFC, and update dpool
  *
  *\param None
  *   
  * \return None
  */
void IFC_vDeInit (void)
{
	IFC_vInit();
}

/**
  * \brief calculate ifc, and update dpool
  *
  *\param None
  *   
  * \return None
  */

void IFC_vMain (void)
{
	static uint32_t u32SumDis = 0U;
	static uint32_t u32SumFuel = 0U;	
	static uint32_t u32SumFuel_last = 0U;
	static uint16_t s_lastDisValue = 0;
	static uint8_t s_moveFlag = 0;
	static uint16_t s_LastTII_u16FuelConsum = 0;
	static uint8_t s_FilterFlag = 0;
	
	TII_Data_STRU stFuelConsum;
	uint32_t l_IncDistance;

	static uint16_t Tii[5] = {0};
	static uint8_t FirstMode_ifc = 0;
	static IGN_STAT_ENUM ign_state = IGN_OFF;
	uint32_t FuelSum = 0;
	uint32_t FuelSum_avg = 0;
	uint32_t diff = 0;
	uint8_t i = 0;

	static uint16_t s_Sistance[5] = {0};
	uint32_t DisSum = 0;

	if(DEA_GetIgnStat() == IGN_ON)
	{

		if(ign_state == IGN_OFF)
		{
			ign_state = IGN_ON;
			stIfc.IFC_u16InstCons = 200;	//20L/100KM(expand 10times)
		}
		
		l_IncDistance = IFC_stGetDistance(); 	/*! meter */
		stFuelConsum = IFC_stGetTII();			/*!	ul	*/
		
		if((stFuelConsum.TII_u8ValidFuelConsum == IFC_True) && (DEA_GetRawVehicleSpeedErrorStat() == 0)) //signal valid
		{

			if((stFuelConsum.TII_u16FuelConsum == 0) && (s_LastTII_u16FuelConsum  != 0) && (s_FilterFlag == 0))
		    {
				stFuelConsum.TII_u16FuelConsum = s_LastTII_u16FuelConsum;
				s_FilterFlag = 1;
		    }
		    else
		    {
				if(s_FilterFlag == 1)
				{
					s_FilterFlag = 0;
					if(stFuelConsum.TII_u16FuelConsum > s_LastTII_u16FuelConsum)
					{
						stFuelConsum.TII_u16FuelConsum -= s_LastTII_u16FuelConsum;
					}
					else if(stFuelConsum.TII_u16FuelConsum > 0)
					{
						stFuelConsum.TII_u16FuelConsum -= 1;
					}
				}
				
				s_LastTII_u16FuelConsum = stFuelConsum.TII_u16FuelConsum;
		    }
			u32SumFuel_last = u32SumFuel;
			
			if(1 != FirstMode_ifc)
			{
				FirstMode_ifc = 1;
				for(i = 0;i < 5; i++)
				{
					Tii[i] = stFuelConsum.TII_u16FuelConsum;
				}

				for(i = 0;i < 5; i++)
				{
					s_Sistance[i] = l_IncDistance;
				}
			}

			for(i = 4;i >= 1;i--)
			{
				Tii[i] = Tii[i - 1];
			}
			Tii[0] = stFuelConsum.TII_u16FuelConsum;

			for(i = 0;i < 5; i++)
			{
				FuelSum += Tii[i];
			}
			FuelSum_avg =  FuelSum/5;
			if(u32SumFuel_last > FuelSum_avg)
			{
				diff = (u32SumFuel_last - FuelSum_avg)/4;
				if(u32SumFuel_last >= diff)
				{
					if(FuelSum_avg == 0)
					{
						u32SumFuel = 0;
					}
					else
					{
						//u32SumFuel = u32SumFuel_last - diff;
						if(diff == 0)
						{
							u32SumFuel = u32SumFuel_last - 1;
						}else
						{
							u32SumFuel = u32SumFuel_last - diff;
						}
					}
				}
			}
			else if(u32SumFuel_last < FuelSum_avg)
			{
				diff = (FuelSum_avg - u32SumFuel_last)/4;
				if(diff > 0)
				{
					u32SumFuel = u32SumFuel_last + diff;
				}
				else
				{
					u32SumFuel = u32SumFuel_last + 1;
				}
			}
			else
			{
				//do nothing
			}

			for(i = 1;i >= 1;i--)
			{
				s_Sistance[i] = s_Sistance[i - 1];
			}
			s_Sistance[0] = l_IncDistance;

			for(i = 0; i < 2; i++)
			{
				DisSum += s_Sistance[i];
			}

			u32SumDis = DisSum/2;

			//591 project require the IFC display update cycle 100ms
#ifndef		IFC_FOR_591
			if(DEA_GetFilterVehicleSpeed() >= MOVESPEED)
#endif
    		{
    			s_lastDisValue = stIfc.IFC_u16InstCons;
				if(u32SumDis > 0)
    			{
    				stIfc.IFC_u16InstCons = (((u32SumFuel*1000)/(u32SumDis))+5)/10; //(NN.N L/100km change to NNN L/100km) expand 10 times
				}
				else if(u32SumFuel > 1)
				{
					stIfc.IFC_u16InstCons = IFC_MAXL_100KM;
				}
				else
				{
					stIfc.IFC_u16InstCons = 0;
				}
    			//ifc_tmp = (u32SumFuel*1000)/(u32SumDis)+5; //(NN.N L/100km change to NNN L/100km)
    			//stIfc.IFC_u16InstCons= ifc_tmp/10;
 #if 1   		
    			if(abs(stIfc.IFC_u16InstCons-s_lastDisValue) > 1)
    			{
    				s_lastDisValue = stIfc.IFC_u16InstCons;
    				s_moveFlag = 1;
    			}
    			else 
    			{
    				if(s_moveFlag == 0)
    				{
    					stIfc.IFC_u16InstCons = s_lastDisValue;
    				}
    				else
    				{
    					s_moveFlag = 0;
    				}
    			}
#endif
    			
    			if(stIfc.IFC_u16InstCons > IFC_MAXL_100KM)
    			{
    				stIfc.IFC_u16InstCons = IFC_MAXL_100KM;
    			}
    			
    			stIfc.IFC_u8Unit = IFC_UNIT_L_100KM;
    			stIfc.IFC_boValid = IFC_True;
    		}
#ifndef	IFC_FOR_591
    		else if(DEA_GetFilterVehicleSpeed() < STOPSPEED)
    		{
    			stIfc.IFC_u16InstCons = (u32SumFuel*36)/(CYCLE*TIMEREACH);	//now CYCLE*TIMEREACH=2000ms, *10(NN.N L/H change to NNN L/H) NNN mean NN.N L/H (2s cycle)
    			if(stIfc.IFC_u16InstCons > IFC_MAXL_H)
    			{
    				stIfc.IFC_u16InstCons = IFC_MAXL_H;
    			}
    			
    			stIfc.IFC_u8Unit = IFC_UNIT_L_H;
    			stIfc.IFC_boValid = IFC_True;
    		}
#endif
		}/*! signal invalid */
		else
		{
			stIfc.IFC_u16InstCons = s_lastDisValue;	//when abnormal hapend ,the IFC should keep the last value
			stIfc.IFC_boValid = IFC_False;
		}
		if((DEA_GetFilterEngineRevolution() <= 170) || (0 == DEA_GetFilterEngineRevolutionValid())) //yangxl 2019-11-26 14:30:38
		{
			stIfc.IFC_u16InstCons = 0u;
		}
		IFC_vWrite(stIfc);
	}
	else
	{
		if(ign_state == IGN_ON)
		{
			ign_state = IGN_OFF;
		}
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************



