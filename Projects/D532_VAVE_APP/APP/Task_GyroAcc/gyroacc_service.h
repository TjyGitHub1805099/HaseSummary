#ifndef __GYROACC_SERVICE_H
#define __GYROACC_SERVICE_H

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Task_M_IpcApp_Cfg.h" 


/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
#define MEMS_CYCLE_DELAY_TOLERANCE	1		/* MEMS cycle timer tolerace : ms */
#define MEMS_INITIAL_DELAY_COUNT	(200-MEMS_CYCLE_DELAY_TOLERANCE)
#define MEMS_CYCLE_DELAY_COUNT		(40-MEMS_CYCLE_DELAY_TOLERANCE)

#define GYROCT_IPC_HW_CHANNEL		1//SPI1
#define GYROCT_IPC_SW_CHANNEL		IPC_GW_CHANNEL_SYSTEM//   IPC_GW_CHANNEL_SYSTEM = 8, /* include time info, sys info, version info, back light*/
#define GYROCT_IPC_SW_FUNCTION_ID	IPC_SW_SYSTEM_FUCTION_GYROCT
#define GYROCT_IPC_SW_TEMP_FUNCTION_ID	IPC_SW_SYSTEM_FUCTION_TEMPCT




#define GYROCT_RECV_IPC_SW_HMICMD_LEN	12//second minute hour week day month year

#define GYROCT_DEBUG_TRACE STD_ON


#define SOC_GPS_INFO_DATA_LEN	8 // example:VHT09A18A0010001
extern uint8 gpsInfoHandleFlag;
extern uint8 gpsInfoFromSoc[];

/************************************************************************************************/
void GyroAccServiceInit(void);
void GyroAccDataCapture(void);
void GyroAccTimerTransmit(void);

/************************************************************************************************/
/*
 * Brief               Main function of the GyroAcc which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/************************************************************************************************/
void GyroAcc_MainFunction(void);

#endif // GYROACC_SERVICE

