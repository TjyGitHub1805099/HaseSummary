#ifndef _CAN_APP_TX_MSG_PROC_H_
#define _CAN_APP_TX_MSG_PROC_H_

#include "Std_Types.h"

#define CAN_OUTPUT_RC (335)		//0.335
#define CAN_OUTPUT_RM2 (3387)	//0.3387

extern void CanApp_TxMsgInit(void);

extern void CanApp_TxMsgProc(void);

extern void CanApp_TxSonarSetting(uint8_t status);

extern void CanApp_TxITS_BSWSetting(uint8_t status);

extern void CanApp_TxITS_LDWSetting(uint8_t status);

extern void CanApp_TxITS_FEBSetting(uint8_t status);

extern void CanApp_TxEAPMSetting(uint8_t status);

extern void CanApp_TxSonarCancelRequest(void);

extern void VehicleResettingSetting(void);

extern void CanApp_TxITS_LKASetting(uint8_t status);//yangxl 2019-5-30 10:53:20
extern void CanApp_TxITS_ISSSetting(uint8_t status);//yangxl 2019-5-30 15:17:34



#endif
