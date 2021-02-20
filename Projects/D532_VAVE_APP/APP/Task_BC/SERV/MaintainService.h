#ifndef _MAINTAIN_SERVICE_H_
#define _MAINTAIN_SERVICE_H_

#include "MaintainServiceCfg.h"

extern int32_t SERV_GetDistance(void);
extern SERV_RESET_RET_ENUM ServReset(uint16_t ServiceValue);
extern void vMaintianServMain(void);
extern void vMaintianServInit(void);
extern void SERV_SetAccomplishFlag(uint8_t stat);


#endif
