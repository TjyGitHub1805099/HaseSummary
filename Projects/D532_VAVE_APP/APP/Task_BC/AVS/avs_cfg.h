#ifndef __AVS_CFG_H__
#define __AVS_CFG_H__

#include "Std_Types.h"
#define AVS_CONTROL_CFG STD_ON
#define AVS_DISPLAY_MILLAGE_THRESHOLD		(500)	//unit: meter(591 project range 500m)
#define AVS_CAR_MOVING_THRESHOLD			(30)	//unit: 0.1 km/h
#define AVS_MAX_VALUE						(340)	//unit: 1km/h (the 591 project speed limit 340km/h)
#define AVS_MAXDIS 							4200000/** define maximum value */

#define AVS_MAX_S	100000000ul	//10W km
#define AVS_MAX_T	3600000000UL	//100000h



/**金康项目新增**/
#define AVS_True   1

#define AVS_False  0




typedef struct  
{
	unsigned char  u8VcuValue;      	/**< Vcu signal value */ 
	unsigned char  u8VcuValid; 			/**< Vcu signal validity : True valid , False not valid */ 
	unsigned char  u8VcuAlive;			/**< Vcu signal alive : True alive , False not alive */
	unsigned char  u8VcuTimeoutFlag;	/**< Vcu signal timeout : False not timeout , True timeout */
}Vcu_InputInfor;






#endif

