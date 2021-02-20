#ifndef _DTE_H_
#define _DTE_H_
#include "Std_Types.h"


#if 0
typedef struct
{
	uint32_t dteValue;
	uint8_t dteValid;
	uint8_t dteDisplayValid;
	uint16_t dteDisplayValue;
	//uint16_t lastFuelCons;	/*! uint:ul */
	uint32_t fuelCons;			/*! uint:ul */
	uint32_t dteMileage;
	uint32_t dteMileageOffset;
}DTE_STRU;

#endif


//591 project struct data require
typedef struct
{
	uint32_t dteValue;
	uint8_t dteValid;
	uint32_t dteValueDisplay;
	
}DTE_STRU;

typedef enum
{
	state_one = 1,	//F_VOL = TIM_VOL200ms
	state_two,		//F_VOL = 1.5 * TIM_VOL200ms
	state_three,	//F_VOL = 2.5 * TIM_VOL200ms
	state_four,		//F_VOL = 0.25 * TIM_VOL200ms
	state_five,		//F_VOL = 0.5 * TIM_VOL200ms
	
}F_VOL_STATE;

typedef enum
{
	DTE_DIS_NORMAL,
	DTE_DIS_PRE,
	DTE_DIS_SECOND,
	DTE_DIS_INVALID,
}DTE_DIS_ENUM;


void DTE_Init(void);

uint8_t Get_DteState(void);

void DTE_Main(void);

uint32_t Get_R_AFC_Value(void);

uint32_t Get_R_AFV_Value(void);		//get the remian fuel vol


void DTE_SetDebugFlag(uint8_t stat);


#endif
