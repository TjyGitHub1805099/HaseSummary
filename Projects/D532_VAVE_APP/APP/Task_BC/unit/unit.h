#ifndef _UNIT_H_
#define _UNIT_H_
#include "Std_Types.h"
//#include "BackRam.h"


#define UNIT_NVRAM_INDEX	(9)

typedef struct
{
	uint8_t MagicByte;
	uint8_t TimeFormat			:1;	// 0-24H 1-12H
	uint8_t FuelConsumptionUnit	:1;	// 0-liter per hour (l/h)
	uint8_t LanguageFormat		:1; // 0-Chinese 1-English
	uint8_t Reserved			:5;
	uint16_t DriveTimeFlag		:2;	//Driver Time interrupt on off flag
	uint16_t DriveTimeThreshold	:14;	//Driver Time interrupt Threshold minutes
}UNIT_STRU;


//#pragma segment BACK_RAM_START
extern UNIT_STRU g_Unit_Coding;
//#pragma segment BACK_RAM_END


extern void UNIT_Init(void);
extern void UNIT_vMain(void);
extern uint8_t UNIT_GetDriveTimeFlag(void);
extern uint8_t UNIT_GetDriveTimeThreshold(void);

#endif
