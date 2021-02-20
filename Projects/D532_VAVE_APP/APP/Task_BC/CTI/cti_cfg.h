//*****************************************************************************
// file			: cti_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/6/1         eason             Modify for HC Cluster
//****************************************************************************
#ifndef CTICFG_H
#define CTICFG_H
//*****************************************************************************
//
//! \addtogroup cti_cfg
//! @{
//
//*****************************************************************************
#include "dea.h"

/** define bool 1 */
#define CTI_True    1

/** define bool 0 */
#define CTI_False   0

/** max tempture */
#define CTI_u8MaxCti  		2140  //591 project requirement ,expand 10 times

#define CTI_RspTimeReachMid		(1U)	

#define CTI_RspTimeReachSlow	(30U)	



/** input speed information */
typedef struct  
{
	int32_t u8CtiValue;      	/**< cti signal value */ 
	uint8_t u8CtiValid; 		/**< cti signal validity : True valid , False not valid */ 
	uint8_t u8CtiTimeoutFlag;	/**< cti signal timeout : False not timeout , True timeout */
} CTI_InputInfor;

/** Definition of pool data type cti */
typedef struct
{
	uint8_t CTI_boTempValid;
	int32_t CTI_u8CoolTempValue;
	uint8_t CTI_SegValue;
	uint16_t CTI_AngleValue;
} CTI_Data_STRU;


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
CTI_InputInfor CTI_stUpdateCtiInfor(void);
uint8_t CTI_CalSeg(uint8_t coolant);
void CTI_vWrite(CTI_Data_STRU stcti);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  CTICFG_H


