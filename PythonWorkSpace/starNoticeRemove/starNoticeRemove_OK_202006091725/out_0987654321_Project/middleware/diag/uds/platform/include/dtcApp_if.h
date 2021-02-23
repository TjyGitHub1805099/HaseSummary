#ifndef _DIAG_APP_IF_H
#define _DIAG_APP_IF_H
#include"dtcApp_cfg.h"

void dtcAppReportBusOffState(boolean isBusOff);

extern void dtcAppInit();
extern void dtcAppLoop();
extern void dtcAppPartInit();
extern void dtcAppReDetect(uint8 *data,uint16 len);


extern void IVI_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag);

extern void IC_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag);

extern void AVM_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag);

extern boolean dtcApp_getVehicleSpeedTimeoutFlag();
extern void dtcApp_SetVehicleSpeedTimeoutFlag(boolean flag);
extern void dtcApp_setVehicleRpmTimeoutFlag(boolean flag);
extern boolean dtcApp_getVehicleRpmTimeoutFlag();
extern boolean dtcAppGetAvmMsgState(uint8 *status,uint8 len);

#endif
