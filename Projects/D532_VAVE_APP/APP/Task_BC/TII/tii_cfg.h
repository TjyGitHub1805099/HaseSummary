
//*****************************************************************************
// file			: tii_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09         eason             Modify for HC Cluster
//****************************************************************************
#ifndef TIICFG_H
#define TIICFG_H
//*****************************************************************************
//
//! \addtogroup tii_cfg
//! @{
//
//*****************************************************************************
#include "Std_Types.h"

#define TII_True 1
#define TII_False 0

#define TII_CanMax 0xFFFF

/** Definition of pool data type TII */
typedef struct
{
	uint16_t TII_u16FuelConsum;
	uint8_t TII_u8ValidFuelConsum;
}TII_Data_STRU;


/** input fuel consumption information */
typedef struct  
{
	unsigned short FuelConsInput;      	/**< fuel consumption value */ 
	unsigned char  FuelConsValid; 		/**<  fuel consumption validity : True valid , False not valid */
	unsigned char  FuelConsTimeOut; 	/**<  fuel consumption timeout : True time out , False not time out */
} TII_InputInfor;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
TII_InputInfor TII__GetRxFCO(void);
void TII_vWrite(TII_Data_STRU stTiiData);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  TIICFG_H


