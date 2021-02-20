//*****************************************************************************
// file			: rev.h
// Description	: revolution module .h file
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09         eason             Modify for HC Cluster
//****************************************************************************

#ifndef REV_H
#define REV_H
#include "Std_Types.h"
//*****************************************************************************
//
//! \addtogroup rev
//! @{
//
//*****************************************************************************
typedef struct
{
	uint8_t REV_boIsValidEngineRevolution;
	uint8_t REV_boEngineRun;
	uint16_t REV_u16Revolution;
}REV_DATA_STRU;




typedef enum
{
	State_0 = 0,
	State_1,
	State_2,
}REV_MODE;
//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
void REV_vInit(void);
void REV_vMain(void);
                          
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  REV_H

