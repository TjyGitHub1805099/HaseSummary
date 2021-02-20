#ifndef _MAINTAIN_SERVICE_CFG_H_
#define _MAINTAIN_SERVICE_CFG_H_
#include "Std_Types.h"
//#include "BackRam.h"


#define SERV_DISABLE 			(0)
#define SERV_ENABLE				(1)

#define SERV_BACK_RAM_INDEX		(8)
#define SERV_DEFAULT_INTERVAL	(8000) //定义保养里程的数值

typedef struct
{
	uint8_t u8ValidFlag;
	uint8_t AlarmFlag;
	uint16_t u16ServValue; /* Maintian Service Value at  Last coding */
	uint32_t u32OdoValueLastReset;	/* Odometer Value at Last reset time */
}SERV_CODING_STRU;

typedef enum
{
	SERV_RESET_OK,
	SERV_RESET_NVM_ERROR,
}SERV_RESET_RET_ENUM;

//#pragma segment BACK_RAM_START
extern SERV_CODING_STRU g_SERV_Coding;
//#pragma segment BACK_RAM_END


extern uint8_t InitServCfg(void);
extern SERV_RESET_RET_ENUM ServReset(uint16_t ServiceValue);
extern SERV_RESET_RET_ENUM ServResetExt(void);
extern void ServAlarmFunctionFlagSet(uint8_t stat);
extern uint8_t SERV_GetAlarmFlag(void);

#endif
