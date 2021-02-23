

#ifndef _POWER_SUPPLY_H_
#define _POWER_SUPPLY_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "Std_Types.h"
#include "BoardDefines.h"
#include "pin_mux.h"
#include "clockManager_cfg.h"


#define UG8V_ERR_CHECK_EN	STD_OFF
#if ( STD_ON == UG8V_ERR_CHECK_EN )
extern void UG8V_Err_Check_Main();
#endif

//void PowerOn(void);
extern void PowerOff(void);
extern void Soc_Power_Onoff(boolean onoff);
extern void SysResetWrapper(uint32 moduleID);
extern void SysResetWrapper_AndStore(uint32 moduleID);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif


