//******************************************************************************
//file		  	: vel_cfg.c 
//Description	: configure input source of vel module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/5/8         eason             First draft version
//2017/6/1         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup vel_cfg
//! @{
//
//*****************************************************************************

//#include "signal_process_app.h"
//#include "can_app.h"
#include "Vel_cfg.h" 
#include "dea.h"
//#include "CanApp_AliveProc.h"
#include "Ea.h"
#include "bc_common.h"
#include "NvM.h"
#include "MemMap.h"
#include "NvM_if.h"



//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED VEL_CODING_STRU g_VEL_Coding = {0};
//#pragma segment BACK_RAM_END


/**金康项目车速信号有效性**/
uint8_t VchSpValueValid = 0;


/**
  * \brief VEL coding init
  *
  *\param None  
  *   
  * \return  result
  */

uint8_t VEL_InitCoding(void)
{
	
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		if(g_VEL_Coding.u8ValidFlag != MAGIC_BYTE)
		{
			g_VEL_Coding.u8ValidFlag = MAGIC_BYTE;
			g_VEL_Coding.u8Factor = 3;
			g_VEL_Coding.u8Offset1 = 0;
			g_VEL_Coding.u8Offset2 = 1;
			g_VEL_Coding.u8Offset3 = 2;
			g_VEL_Coding.u8Offset4 = 3;
			g_VEL_Coding.u16MaxVelocity = VEL_u16MaxVelocity;
			NvM_WriteBlock(VEL_CODING_NVRAM_INDEX, (uint8_t *)&g_VEL_Coding);
		}
	}	
	return 0;
}

/**
  * \brief Get vel input information
  *
  *\param None  
  *   
  * \return  Input information
  */
VEL_InputInfor VEL_stUpdateSpeedInfor(void)
{
	VEL_InputInfor stVelInfor;
	
	
	if((DEA_GetRawVehicleSpeedErrorStat() == 0))// && (DEA_GetRawVehicleSpeed() != 0x12c0)) //speed valid
	{
		stVelInfor.u8SpeedValid = VEL_True;
	}
	else
	{
		stVelInfor.u8SpeedValid = VEL_False;
	}

		
	stVelInfor.u8SpeedAlive = VEL_True;
#if 0
	if(DEA_GetRawVehicleSpeed() <= 29)	//input value <= 3km/h ->value = 0
	{
		stVelInfor.u16SpeedValue = 0;
	}
	else 
	{
		stVelInfor.u16SpeedValue = DEA_GetRawVehicleSpeed();
	}
#else
	stVelInfor.u16SpeedValue = DEA_GetRawVehicleSpeed();
#endif
	
	if(DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) != 0)
	{
		stVelInfor.u8SpeedTimeoutFlag = VEL_True;
	}
	else
	{
		stVelInfor.u8SpeedTimeoutFlag = VEL_False;
	}

	
	return stVelInfor;

}





/**skon_106 project speed valid value**/

uint8_t GetVchSpValidValue()
{
	if((DEA_GetRawVchSpValidValue()) && ( DEA_GetVchSpValidErrStat() ==0)) //validvalue = 1;valid = 1;
	{
		VchSpValueValid = 1;
	}
	else
	{
		VchSpValueValid = 0;
	}

	return VchSpValueValid;
}


/**
  * \brief Get damp off  information
  *
  *\param None  
  *   
  * \return   damp off  information
  */
unsigned char VEL_u8GetDampOff(void)
{
	return VEL_False;
}

/**
  * \brief Write dpool data
  *
  *\param stSpeedo vel module ouput data  
  *   
  * \return None
  */

void VEL_vWrite(uint16 stSpeedo,uint8_t stSpeedoValid)
{
#if 0
	if(DEA_GetVehicleSpeedDiagMode() == 0)
	{
		
	}
	else
	{
		DEA_SetFilterVehicleSpeedDiag(stSpeedo);
		/*if((DEA_GetRawVehicleSpeedErrorStat() == 0) && (CanApp_GetAliveErrorStat(ALIVE_ID_EPBi_V_VEH) == 0) && (DEA_GetCanTimeOutStat(IPDU_HND_EPBi_V_VEH) == 0))
		{
			DEA_SetFilterVehicleSpeed(DEA_GetRawVehicleSpeed());
		}
		else
		{
			DEA_SetFilterVehicleSpeed(0);
		}*/
	}
#endif

	DEA_SetFilterVehicleSpeed(stSpeedo);	
	DEA_SetFilterVehicleSpeedValid(stSpeedoValid);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




