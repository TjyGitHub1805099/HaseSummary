
#include "Std_Types.h"
#include "Platform_Types.h"

#ifndef _GEAR_H_
#define _GEAR_H_
/***********************[I N C L U D E S]***********************/

#ifdef __cplusplus
extern "C" {
#endif

#if 0
/**传统车-手动档/自动档**/
typedef struct
{
	uint8  Vail:1;
	uint8 Data:7;
}GEAR_STRU;

typedef enum
{
	GEAR_DCT = 0,
	GEAR_MT,
	GEAR_AT,
	GEAR_MAX
}GEAR_TYPE;

typedef enum
{
	SHIFT_NONE = 0,
	SHIFT_MODE
}GEAR_SHIFT_MODE;

enum{
	DATA_VALID=0,
	DATA_INVALID
};

enum{
	GEAR_RUN_OFF=0x00,
	GEAR_CHECK_MODE,
	GERA_RUN_ON
};

typedef struct
{
	uint8  Type;
	uint8 ShiftMode;
}GEAR_Nvm;

void GEAR_GearProc(void);


#ifdef __cplusplus
}
#endif

#endif


typedef enum	//section 1
{
	METER_OFF = 0,
	METER_ON,
	METER_BLINK,
	
}GEAR_MODE;

typedef enum	//section 2
{
	Show_OFF2 = 0,
	Show_M,
	Show_IT,
}Grar_SHOW2;

typedef enum	//section 3
{
	Show_OFF = 0,
	Show_UP,
	Show_DOWN,
	Show_S,
	
}Grar_MTSHOW;


typedef struct
{
	unsigned char u8GearValue;
	unsigned char u8GearTm_Type;
	unsigned char u8GearIndicateAllow;
	unsigned char u8GearBlinkValue;
	
}Gear_Infor;






Grar_SHOW2 Get_GearAtShow2();


Grar_MTSHOW Get_GearAtShow3();


GEAR_MODE Get_GearMode();


Grar_MTSHOW Get_GearMTMode();


void Gear_Init(void);
void Gear_MAIN(void);





#endif



