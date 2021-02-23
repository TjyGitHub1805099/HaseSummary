

#ifndef _FLASH_IF_H_
#define _FLASH_IF_H_

#include "flash_driver.h"
#include "power_manager.h"

void Flash_If_Init();
boolean Flash_Sector_Programing(uint32 addr, uint8 *data, uint16 wr_size );

void DevAssert2(volatile bool x,uint8 checkPoint);

#define DEV_ASSERT2(x,y) DevAssert2(x,y)

typedef enum
{
	ePowerMode_HSRUN = 0,
	ePowerMode_VLPS = 1,
	ePowerMode_RUN = 2
}PowerMode;


void EnterPowerMode(uint8 cType);

/*! @}*/
#endif /* _FLASH_IF_H_ */


/*******************************************************************************
 * EOF
 ******************************************************************************/
