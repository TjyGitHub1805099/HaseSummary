//******************************************************************************
//file		  	: tii_cfg.c 
//Description	: configure input source of tii module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup tii_cfg
//! @{
//
//*****************************************************************************

//#include "signal_process_app.h"
#include "dea.h"
#include "tii_cfg.h"  

/**
  * \brief Get tii input information
  *
  *\param None  
  *   
  * \return  Input information
  */
TII_InputInfor TII__GetRxFCO(void)
{
	TII_InputInfor stFuelConsum;
	
	if(DEA_GetRawFuelInjectionErrStat() == 0)
	{
		stFuelConsum.FuelConsValid = 1;
	}
	else
	{
		stFuelConsum.FuelConsValid = 0;
	}

	if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus) != 0)
	{
		stFuelConsum.FuelConsTimeOut = 1;
	}
	else
	{
		stFuelConsum.FuelConsTimeOut = 0;
	}
	
	stFuelConsum.FuelConsInput = DEA_GetFuelInjection();

	//stFuelConsum.FuelConsTimeOut = DEA_GetCanTimeOutStat(IPDU_HND_GTW_ecuAndGcu_info);
		
	return stFuelConsum;
}

/**
  * \brief Write dpool data
  *
  *\param stSpeedo TII module ouput data  
  *   
  * \return None
  */
void TII_vWrite(TII_Data_STRU stTiiData)
{
	DEA_SetTII_Valid(stTiiData.TII_u8ValidFuelConsum);
	DEA_SetTII_Value(stTiiData.TII_u16FuelConsum);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************









