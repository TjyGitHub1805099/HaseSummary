//*****************************************************************************
// file			: vel.h
// Description	: Velocity module .h file
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/6/1         eason             Modify for HC Cluster
//****************************************************************************

#ifndef VEL_H
#define VEL_H
//*****************************************************************************
//
//! \addtogroup vel
//! @{
//
//*****************************************************************************
/** Vel processing  state structure*/

typedef enum 
{
   VEL_Initial = 0,	/**< Initial State */
   VEL_Normal,		/**< Normal State */
   VEL_Exception,	/**< Exception State */		
} VEL_Mode;

/** input speed information */
typedef struct  
{
	unsigned short u16SpeedValue;      	/**< velocity signal value */ 
	unsigned char  u8SpeedValid; 		/**< velocity signal validity : True valid , False not valid */ 
	unsigned char  u8SpeedAlive;		/**< velocity signal alive : True alive , False not alive */
	unsigned char  u8SpeedTimeoutFlag;	/**< velocity signal timeout : False not timeout , True timeout */
}VEL_InputInfor;



//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

VEL_InputInfor VEL_stUpdateSpeedInfor(void);

void VEL_vMain(void);

void VEL_vInit(void);


VEL_Mode VEL_vGetVelMode(void);




                      
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  VEL_H

