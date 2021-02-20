
#ifndef		_MOTORSP_H_
#define		_MOTORSP_H_

#include "Std_Types.h"


#define		MSP_TRUE	1

#define		MSP_FALSE	0

#define		MOTORSP_MAX		52000


 

#define		MSP_s32MspPT1Rise 	2 //滤波递增步进值

#define		MSP_s32MspPT1Fall 	2 //滤波递减步进值






typedef struct
{
	  signed int	 s32MspValue;
	unsigned char  	 u8MspValid; 		    
	unsigned char    u8MspAlive;			
	unsigned char    u8MspTimeoutFlag;	
	
}MotorSp_InputInfor;

typedef enum
{
	MSP_Normal = 0,		/**< Normal State */
    MSP_Exception,	    /**< Exception State */	
    
}MSP_Mode;



void MotorSp_Init();

void MotorSp_MAIN();

signed int FilteringHandle(signed int s32TargetPos,signed int s32CurrentPos,unsigned short u16RiseStep,unsigned short u16FallStep);

MotorSp_InputInfor UpdateReMotorSpInfor();






#endif
