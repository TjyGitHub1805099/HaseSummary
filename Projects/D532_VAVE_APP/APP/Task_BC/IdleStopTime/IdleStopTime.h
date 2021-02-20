#ifndef _IDLESTOPTIME_H_
#define _IDLESTOPTIME_H_

#include "Std_Types.h"


#define PARAMETER_ISFUEL	18	//expand 100 times

#define IDLEFUEL_MAX	999999

typedef enum
{
	OFF = 1,
	COUNT_ON,
	COUNT_OFF,
	COUNTVALUE_RESET,
	
}IDLETIME_STATE;



void IDLE_FUEL_CumulativeReset(void);

void IDLE_FUEL_TripReset(void);

void IDLE_FUEL_Init(void);

void IDLE_FUEL_vMAIN(void);




#endif

