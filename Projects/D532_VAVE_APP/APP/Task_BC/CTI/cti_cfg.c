//******************************************************************************
//file		  	: cti_cfg.c 
//Description	: configure input source of cti module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/5/8         eason             First draft version
//2017/6/1         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup cti_cfg
//! @{
//
//*****************************************************************************
#include "dea.h"
#include "cti_cfg.h" 

const uint8_t CTI_CoolantToSegTable[] = {650, 760, 830, 1160, 1180, 1200, 1270};	//the data have expand 10 times
/**
  * \brief Get vel input information
  *
  *\param None  
  *   
  * \return  Input information
  */
CTI_InputInfor CTI_stUpdateCtiInfor(void)
{
	uint16_t l_cti = 0;
	CTI_InputInfor stCtiInfor =  {0};

	
	if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus) !=0)
	{
		stCtiInfor.u8CtiTimeoutFlag = CTI_True;
	}
	else
	{
		stCtiInfor.u8CtiTimeoutFlag = CTI_False;
	}
	
	l_cti = DEA_GetCoolantTempRaw()*10;	//expand 10 times
	
	if(l_cti <= 0xFE*10) //valid
	{	
		//only to show the tempreture above zero
		if(l_cti <= 0)	//coolant temperature below zero //591 project requirement ,expand 10 times
		{
			stCtiInfor.u8CtiValue = 0;
		}
		else
		{
			stCtiInfor.u8CtiValue = l_cti - 400;	//offset -40 ,expand 10 times
		}
		stCtiInfor.u8CtiValid = 1;//signal valid
	}
	else
	{
		stCtiInfor.u8CtiValue = 0;
		stCtiInfor.u8CtiValid = 0;//signal invalid
	}

	return stCtiInfor;

}

uint8_t CTI_CalSeg(uint8_t coolant)
{
	uint8_t l_cnt;
	for(l_cnt = 0; l_cnt < (sizeof(CTI_CoolantToSegTable)/sizeof(CTI_CoolantToSegTable[0])); l_cnt++)
	{
		if(coolant < CTI_CoolantToSegTable[l_cnt])
		{
			break;
		}
	}
	return (l_cnt+1);
}


/**
  * \brief Write dpool data
  *
  *\param stCti cti module ouput data  
  *   
  * \return None
  */
void CTI_vWrite(CTI_Data_STRU stCti)
{
	//DPOOL_enSetPoolData(DPOOL_nDataCTI_stDpoolData, &stCti);
	DEA_SetCoolantTempValid(stCti.CTI_boTempValid);
	DEA_SetCoolantTemp(stCti.CTI_u8CoolTempValue);
	DEA_SetCoolantIndAngle(stCti.CTI_AngleValue);	//591 require
	//DEA_SetCoolantSeg(stCti.CTI_SegValue);//segement value
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




