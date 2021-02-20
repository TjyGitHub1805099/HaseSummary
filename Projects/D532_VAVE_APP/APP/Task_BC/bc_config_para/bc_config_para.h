#ifndef _BC_CONFIG_PARA_H_
#define _BC_CONFIG_PARA_H_

#include "Std_Types.h"
#include "Vel.h"

typedef struct
{
	uint8_t u8ValidFlag;
	//G-15-1
	uint8_t BestFuelT1;
	uint16_t VelModuleRc;
	uint16_t VelModuleRm1;
	uint16_t VelModuleRm2;
	uint16_t DteModuleDR;
	uint16_t DteModuleSM;

	//C-1-7-3-C
	uint16_t TachoA;
	uint16_t TachoB;
	uint16_t TachoC;
	uint16_t TachoD;
	uint16_t TachoE;

	
	//G-16-1
	uint8_t EchoRankT1;
	uint8_t EchoRankE1;
	uint8_t EchoRankE2;
	uint8_t EchoRankE3;
	uint8_t EchoRankE4;
	uint8_t Reserved[65];
}BC_CONFIG_PARA_STRU;

extern void bc_config_para_init(void);

#endif

