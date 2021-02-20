//******************************************************************************
//file		  		: tii.c 
//Description	  	: calculate fuel consumption from injection in cycle,cycle may be 10ms or others
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			        Author		    Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup tii
//! @{
//
//*****************************************************************************
#include "tii_cfg.h"           
#include "tii.h"
#include "dea.h"  
#include "string.h"  

/** define fuel consumption variable */
TII_Data_STRU stTII; 

/**
  * \brief Initial dpool data
  *
  *\param None  
  *   
  * \return None
  */
void TII_vInit(void)
{
	memset( &stTII, 0, sizeof( stTII));
	TII_vWrite(stTII);				//update DPOOL value	
}

/**
  * \brief Deinitial dpool data
  *
  *\param None  
  *   
  * \return None
  */
void TII_vDeinit(void)
{
	TII_vInit();
}

/**
  * \brief get fule comsumption every cycle
  *
  *\param None  
  *   
  * \return None
  */
void TII_vMain(void)
{  
	//static uint32_t s_remainder = 0;
	static TII_InputInfor s_PreviousFuelConsInput = {0};	//last recived CAN Tii data
	TII_InputInfor l_GetRxFuelConsInput = {0}; 				//Previous recived CAN Tii data

	if(DEA_GetIgnStat() == IGN_ON)
	{	
	#if 1
		l_GetRxFuelConsInput = TII__GetRxFCO(); 

		if((l_GetRxFuelConsInput.FuelConsValid == TII_True) && (s_PreviousFuelConsInput.FuelConsValid == TII_True) && (l_GetRxFuelConsInput.FuelConsTimeOut == TII_False))
		{
			if(l_GetRxFuelConsInput.FuelConsInput < s_PreviousFuelConsInput.FuelConsInput) //overflow happened
			{
				stTII.TII_u16FuelConsum = (l_GetRxFuelConsInput.FuelConsInput + 256 - s_PreviousFuelConsInput.FuelConsInput)*80;
			}
			else
			{
				stTII.TII_u16FuelConsum = (l_GetRxFuelConsInput.FuelConsInput - s_PreviousFuelConsInput.FuelConsInput)*80; 
			}
			stTII.TII_u8ValidFuelConsum = TII_True;
		}
		else
		{
			stTII.TII_u8ValidFuelConsum = TII_False;
		}
		
		s_PreviousFuelConsInput = l_GetRxFuelConsInput; //update fuel consumption

	#else

		
		l_GetRxFuelConsInput = TII__GetRxFCO(); 
	
		if((l_GetRxFuelConsInput.FuelConsValid == 1) && (l_GetRxFuelConsInput.FuelConsTimeOut == 0))
		{
			stTII.TII_u16FuelConsum = (l_GetRxFuelConsInput.FuelConsInput + s_remainder); //0.001ml  0~20400ul
			//s_remainder = (DEA_GetInstFuelConsmpRate()+s_remainder)%36;	//get the remainder
			
			stTII.TII_u8ValidFuelConsum = 1;		
		}
		else
		{
			stTII.TII_u16FuelConsum = 0; 	//0.001ml  
			s_remainder = 0;
			
			stTII.TII_u8ValidFuelConsum = 0;
		}
#endif		
		
		TII_vWrite(stTII); //write to DPOOL
	}
	else
	{
		//s_remainder = 0;
		if(stTII.TII_u16FuelConsum != 0)
		{
			stTII.TII_u16FuelConsum = 0;
		}

		stTII.TII_u8ValidFuelConsum = 0;

		TII_vWrite(stTII); //write to DPOOL
	}
	
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************



