//*****************************************************************************
// file			: afc.h
// Description	: average fuel consumption module .h file
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09         eason             Modify for HC Cluster
//****************************************************************************

#ifndef AFC_H
#define AFC_H
#include "Std_Types.h"
//*****************************************************************************
//
//! \addtogroup afc
//! @{
//
//*****************************************************************************

/**
 * Definition of pool data type "AFC_tstDpoolData"
 */
typedef struct
{
	//unsigned int AFC_u32DistanceA;
	//unsigned int AFC_u32FuelConsumA;
	uint8_t AFC_boValidA;
	//unsigned short AFC_u16AvgConsA;
	uint16_t AFC_u16AvgConsA;

	//unsigned int AFC_u32DistanceB;
	//unsigned int AFC_u32FuelConsumB;
	uint8_t AFC_boValidB;
	uint16_t AFC_u16AvgConsB;
}AFC_Data_STRU;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
void AFC_vResetA(void);
void AFC_vResetB(void);
void AFC_vMain(void);
void AFC_vInit(void);

uint16 GetAfcShow(void);
uint8 GetAfcValid(void);


                          
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  AFC_H

