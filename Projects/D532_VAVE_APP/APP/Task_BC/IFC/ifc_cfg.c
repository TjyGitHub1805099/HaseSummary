//******************************************************************************
//file		  	: ifc_cfg.c 
//Description	: configure input source of ifc module 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup ifc_cfg
//! @{
//
//*****************************************************************************
#include "ifc_cfg.h"
/**
  * \brief get tii information
  *
  *\param None
  *   
  * \return tii information
  */
TII_Data_STRU IFC_stGetTII(void)
{
	TII_Data_STRU stTIIInput; 
	stTIIInput.TII_u8ValidFuelConsum = DEA_GetTII_Valid();
	stTIIInput.TII_u16FuelConsum = DEA_GetTII_Value();
	return stTIIInput;
}

/**
  * \brief publish ifc information
  *
  *\param None
  *   
  * \return tii information
  */
void IFC_vWrite(IFC_Data_STRU stIfcdata)
{
	DEA_SetIFC_Valid(stIfcdata.IFC_boValid);
	DEA_SetIFC_Unit(stIfcdata.IFC_u8Unit);
	DEA_SetIFC_Vaule(stIfcdata.IFC_u16InstCons);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


