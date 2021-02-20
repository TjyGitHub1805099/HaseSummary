//*****************************************************************************
// file			: vel_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/6/1         eason             Modify for HC Cluster
//****************************************************************************
#ifndef VELCFG_H
#define VELCFG_H
//*****************************************************************************
//
//! \addtogroup vel_cfg
//! @{
//
//*****************************************************************************
#include "Std_Types.h"
#include "Vel.h"
//#include "BackRam.h"



#define VEL_CODING_NVRAM_INDEX	(6)

/** define bool 1 */
#define VEL_True    1

/** define bool 0 */
#define VEL_False   0

/** damping rise factor */
#define VEL_u16VelPT1Rise  		2	//591 project require step 1

/** damping fall factor */			
#define VEL_u16VelPT1Fall  		2	//591 project require step 1

/** Hysterisis factor for qualifying that vehicle is "moving" */
#define VEL_nHyst  				5

/** min velocity quota to qualify that vehicle is "moving" */
#define VEL_u16MinMove  			10	 				

/** max velocity quota to qualify that vehicle is "moving" */
#define VEL_u16MaxMove   		((unsigned char)(VEL_u16MinMove + (unsigned short)VEL_nHyst)) 

/** max velocity */
#define VEL_u16MaxVelocity  		3400  //the max threshold speed expand 10 times 

/** velocity adjust constant , maximum lead of 3% +4km/h */
#define VEL_u8AdjustConstant 		103  //adjust parmetera

#if 0
#define VEL_Rm1		3377000	//expand 10000*1000times(0.3377)
#define VEL_Rc		3350	//expand 10000times(0.335)
#else //yangxl 2019-8-29 10:11:35
#define VEL_Rm2		3510000	//expand 1000*1000times(0.351)//2020-3-20 11:47:50
#define VEL_Rm1		3510000	//expand 1000*1000times(0.351)
#define VEL_Rc		3510	    //expand 1000times(0.351)
#define VEL_B	(VEL_Rm2/VEL_Rc) // B = Rm_2 / Rc 
#endif

typedef struct
{
	uint8_t u8ValidFlag;
	uint8_t u8Factor;	/*! %1 */
	uint8_t u8Offset1;
	uint8_t u8Offset2;
	uint8_t u8Offset3;
	uint8_t u8Offset4;
	uint8_t u8PointerOffset;
	uint8_t u8Reserved;
	uint16_t u16MaxVelocity;
}VEL_CODING_STRU;

//#pragma segment BACK_RAM_START
extern VEL_CODING_STRU g_VEL_Coding;
//#pragma segment BACK_RAM_END


#define VEL_u8GetFactor() 		(g_VEL_Coding.u8Factor)
#define VEL_u8GetOffset1() 		(g_VEL_Coding.u8Offset1)
#define VEL_u8GetOffset2() 		(g_VEL_Coding.u8Offset2)
#define VEL_u8GetOffset3() 		(g_VEL_Coding.u8Offset3)
#define VEL_u8GetOffset4() 		(g_VEL_Coding.u8Offset4)
#define VEL_u16GetMaxVelocity()	(g_VEL_Coding.u16MaxVelocity)


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

unsigned char VEL_u8GetDampOff(void);
void VEL_vWrite(uint16 stSpeedo,uint8_t stSpeedoValid);
uint8_t VEL_InitCoding(void);
VEL_InputInfor VEL_stUpdateSpeedInfor(void);

uint8_t GetVchSpValidValue(void);/**skon_106 project requirement**/




//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  VELCFG_H


