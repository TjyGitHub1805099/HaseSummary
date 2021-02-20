//******************************************************************************
//file		  	: rev_cfg.c 
//Description	: configure input source of rev module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup rev_cfg
//! @{
//
//*****************************************************************************

#include "dea.h"
//#include "CanApp_AliveProc.h"
#include "Rev_cfg.h" 

/**
  * \brief Get vel input information
  *
  *\param None  
  *   
  * \return  Input information
  */
REV_InputInfor REV_stUpdateEnginInfor(void)	//591 project require ID:180
{
	REV_InputInfor stRevInfor;
	
	if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0)
	{
		stRevInfor.u8EnginTimeoutFlag = REV_False;
	}
	else
	{
		stRevInfor.u8EnginTimeoutFlag = REV_True;
	}

	stRevInfor.u8EnginAlive = REV_True;

	if(DEA_GetRawEngineRevolutionErrorStat() == 0)
	{
		stRevInfor.u8EnginValid = REV_True;
	}
	else
	{
		stRevInfor.u8EnginValid = REV_False;
	}
		
	stRevInfor.u16EnginValue = DEA_GetRawEngineRevolution();

	return stRevInfor;
}

#if 0
REV_InputInfor REV_stUpdateEnginInfor3EC(void)	//591 project require	ID:3EC
{
	REV_InputInfor stRevInfor;
	
	if(DEA_GetCanTimeOutStat(IPDU_HND_ATCVT_to_MMI_N1) == 0)
	{
		stRevInfor.u8EnginTimeoutFlag = REV_False;
	}
	else
	{
		stRevInfor.u8EnginTimeoutFlag = REV_True;
	}

	stRevInfor.u8EnginAlive = REV_True;

	if(DEA_GetRawEngineRevolutionErrorStat_3EC() == 0)
	{
		stRevInfor.u8EnginValid = REV_True;
	}
	else
	{
		stRevInfor.u8EnginValid = REV_False;
	}
		
	stRevInfor.u16EnginValue = DEA_GetRawEngineRevolution_3EC();

	return stRevInfor;
}
#endif

/**
  * \brief Get damp off  information
  *
  *\param None  
  *   
  * \return   damp off  information
  */
uint8_t REV_u8GetDampOff(void)
{
	return REV_False;
}

/**
  * \brief Write dpool data
  *
  *\param stSpeedo vel module ouput data  
  *   
  * \return None
  */

void REV_vWrite(uint8_t engineRunning, uint16_t stRevolution,uint8_t valid)
{
	DEA_SetEngineRunningStat(engineRunning);
	DEA_SetFilterEngineRevolution(stRevolution);
	DEA_SetFilterEngineRevolutionValid(valid);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




