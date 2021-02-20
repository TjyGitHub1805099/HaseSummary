//******************************************************************************
//file		  	: afc_cfg.c 
//Description	: configure input source of afc module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup afc_cfg
//! @{
//
//*****************************************************************************

#include "afc_cfg.h"
#include "dea.h"
//#include "signal_process_app.h"
//#include "can_app.h"

/**
  * \brief get next mileage value
  *
  *\param None
  *   
  * \return mileage value
  */
static unsigned short AFC_u16GetNextMileage(void)
{
	//return (g_sDriving_Module_GW.DistRCAvgDrvn);	//configured by CAN IL
	//return 100;
	return 0;
}

/**
  * \brief get next mileage alive
  *
  *\param None
  *   
  * \return mileage alive
  */
static unsigned char AFC_u8GetNextMileageAlive(void)
{
	return 1;
	
}

/**
  * \brief get next mileage valid
  *
  *\param None
  *   
  * \return mileage validity
  */
static unsigned char AFC_u8GetNextMileageValid(void)
{
	//if((g_sDriving_Module_GW.DistRCAvgDrvnV) == 0)
	//{
		return 1;
	//}
	//else
	//{
	//	return 0;	
	//}
}

/**
  * \brief get next mileage information include alive,valid and value
  *
  *\param None
  *   
  * \return next mileage information include alive,valid and value
  */
AFC_tstMileage AFC_stGetNextMileageInfor(void)
{
	AFC_tstMileage AFC_stNextMileage;
	
	AFC_stNextMileage.u16Mileage = AFC_u16GetNextMileage();
	AFC_stNextMileage.u8MileageAlive = AFC_u8GetNextMileageAlive();
	AFC_stNextMileage.u8MileageValid = AFC_u8GetNextMileageValid();

	return AFC_stNextMileage;
}

/**
  * \brief get  mileage time out flag
  *
  *\param None
  *   
  * \return 1 or 0
  */
unsigned char AFC_u8GetMileageTimeOut(void)
{
	return 0;

}

/**
  * \brief get  fuel consumption every cycle
  *
  *\param None
  *   
  * \return  fuel consumption
  */
TII_Data_STRU AFC_u16GetFuelInc(void)
{
	TII_Data_STRU l_TIIInput;
	l_TIIInput.TII_u16FuelConsum = DEA_GetTII_Value();
	l_TIIInput.TII_u8ValidFuelConsum = DEA_GetTII_Valid();
	return l_TIIInput;
}

/**
  * \brief publish average fuel consumption information
  *
  *\param stAfcData average fuel consumption information
  *   
  * \return None
  */
void AFC_vWriteData(AFC_Data_STRU stAfcData)
{
	//DPOOL_enSetPoolData(DPOOL_nDataAFC_stDpoolData, &stAfcData);
	DEA_SetAFC_A_Valid(stAfcData.AFC_boValidA);
	DEA_SetAFC_A_Vaule(stAfcData.AFC_u16AvgConsA);
	DEA_SetAFC_B_Valid(stAfcData.AFC_boValidB);
	DEA_SetAFC_B_Vaule(stAfcData.AFC_u16AvgConsB);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************



