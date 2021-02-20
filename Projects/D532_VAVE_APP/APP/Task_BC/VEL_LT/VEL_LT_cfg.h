#ifndef _VEL_LT_CFG_H_
#define _VEL_LT_CFG_H_

#include "Std_Types.h"
#include "Platform_Types.h"
#include "NvM.h"
#include "bc_common.h"
#include "dea.h"
//#include "BackRam.h"




#define VEL_LT_OK				(0U)
#define VEL_LT_ERROR			(1U)

#define VEL_LT_NVRAM_INDEX		(7)
#define VEL_LT_VALUE_DEFAULT	(3)	//unit:10km
#define VEL_LT_VALUE_MIN		(3)		
#define VEL_LT_VALUE_MAX		(26)	
#define VEL_LT_HYSTERESIS		(20)	//unit:0.1km
#define VEL_LT_INTERVAL			(100)	//unit:0.1km

#define VEL_LT_OFF		(0U)
#define VEL_LT_ON		(1U)

typedef struct
{
	uint8_t MagicByte;
	uint8_t VEL_LT_OnOff;
	uint16_t VEL_LT_ConfigValue;
}VEL_LT_CFG_STRU;

void VEL_LT_CFG_vInit(void);
uint8_t VEL_LT_GetOnOff(void);
uint16_t VEL_LT_GetConfigValue(void);
void VEL_LT_Write(uint8_t mode);
uint8_t VEL_LT_SetValue(uint16_t vel_config);




#endif
