#ifndef PMSTATEMACHINE_IF_H
#define PMSTATEMACHINE_IF_H
#include "pmStateMachine.h"

extern boolean socOnFlag;


extern void  PM_Main(uint32 event);
extern uint8 PM_GetCurrentMode(void);
extern uint8 PM_GetAccStatus(void);
extern uint8 PM_GetTcuAccStatus(void);
extern uint8 PM_GetIgnStatus(void);
extern uint8 PM_GetReverseStatus(void);
extern PMVoltStateType PM_GetVoltStatus(void);
extern PMMbTempStateType PM_GetMbTempStatus(void);
extern PMAmpTempStateType PM_GetAmpTempStatus(void);
extern uint8 PM_GetCircuitVersion(void);
extern uint8 PM_GetVariantCtrlVersion(void);
extern void  PM_AMP_OnOff(boolean OnOff);
extern void  PM_EX_AMP_OnOff(boolean OnOff);
extern uint8 PM_GetAmpStatus(void);

extern void  DA_TftBackLightOnOff(boolean OnOff);
extern uint8 DA_PM_GetLcdStatus(void);

extern void  IC_TftBackLightOnOff(boolean OnOff);
extern uint8 IC_PM_GetLcdStatus(void);

extern void PM_CheckRmtStartSignal(void);
extern void PM_CheckDAwakeupRequestSignal(void);
extern void PM_CheckAVM_Outdoor_Photo_RequestSignal(void);
extern void PM_CheckISS_ACTSignal(void);
extern void PM_CheckEngineStatusSignal(void);
extern void PM_CheckDoorSwitchesStateSignal();
extern void PM_SetTouchScreenStateSignal(void);
extern void PM_setSocStatus(void);
extern void RVM_CammeraOnOff(uint8 OnOff);

void setFactoryDefault(uint8 mode);
void setUsbPower(uint8 state);

#endif
