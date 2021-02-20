//*****************************************************************************
// file			: rev_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09         eason             Modify for HC Cluster
//****************************************************************************
#ifndef REVCFG_H
#define REVCFG_H
//*****************************************************************************
//
//! \addtogroup rev_cfg
//! @{
//
//*****************************************************************************
#include "dea.h"

/** define bool 1 */
#define REV_True    1

/** define bool 0 */
#define REV_False   0

/**591 project revlution range**/
#define RevA		1000
#define RevB		500
#define RevC		0
#define RevD		100
#define RevE		350



/** define configure switch open */
#define REV_On   	1

/** define configure switch close */
#define REV_Off   	0

/** damping rise factor */
#define REV_u16PT1Rise  		5

/** damping fall factor */
#define REV_u16PT1Fall  		5

/** min rev quota to qualify that engin is "running" */
#define REV_u16MinEnginRun  	400	 				

/** max rev quota to qualify that vehicle is "running" */
#define REV_u16MaxEnginRun   	500

/** max revolution */
#define REV_u16MaxRevolution  	8000 //591 project requirement

/** idle control switch */
#define REV__IdleControl_Cfg	REV_On

#define REV__nu8Hyst		 	20 


#if (REV__IdleControl_Cfg == REV_On)
/** damping idle control rise factor */
#define  REV__nu8IdleHyst		 80  

/** damping idle control rise factor */
#define  REV__nu8IdlePT1Hyst	60	

/** damping idle control rise factor */
#define  REV__nu16IdleMaxRPM	1000

/** damping idle control rise factor */
#define REV_u16IdlePT1Rise  	10

/** damping idle control fall factor */
#define REV_u16IdlePT1Fall  	10
#endif

/** input rpmspeed information */
typedef struct  
{
	uint16_t u16EnginValue;      	/**< Engin signal value */ 
	uint8_t  u8EnginValid; 		/**< Engin signal validity : True valid , False not valid */ 
	uint8_t  u8EnginAlive;		/**< Engin signal alive : True alive , False not alive */
	uint8_t  u8EnginTimeoutFlag;	/**< Engin signal timeout : False not timeout , True timeout */
} REV_InputInfor;

#if 0
typedef struct  
{
	uint16_t u16EnginValue;      	/**< Engin signal value */ 
	uint8_t  u8EnginValid; 		/**< Engin signal validity : True valid , False not valid */ 
	uint8_t  u8EnginAlive;		/**< Engin signal alive : True alive , False not alive */
	uint8_t  u8EnginTimeoutFlag;	/**< Engin signal timeout : False not timeout , True timeout */
} REV_InputInfor3EC;
#endif

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
REV_InputInfor REV_stUpdateEnginInfor(void);
//REV_InputInfor REV_stUpdateEnginInfor3EC(void);

uint8_t REV_u8GetDampOff(void);
void REV_vWrite(uint8_t engineRunning, uint16_t stRevolution,uint8_t valid);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  REVCFG_H


