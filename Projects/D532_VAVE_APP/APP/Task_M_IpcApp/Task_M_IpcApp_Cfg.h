/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_M_IpcApp_Cfg.h
Author: Stephen Du
Version: V1.0
Timestamp: 2018-12-04 11:08:55
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _TASK_M_IPCAPP_CFG_H_
#define _TASK_M_IPCAPP_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "SpiIpc_cfg.h"
#include "TraceStore_Cfg.h"

#define TASK_M_IPCAPP_DEBUG_TRACE STD_ON
#define TASK_M_IPCAPP_STACK_MONITOR STD_OFF

#define IOC_MONITOR_WITH_SOC_ENABLE	STD_ON//used to ioc monitor with soc

#define IPC_CH_DA_PWR	(IPC_GW_CHANNEL_POWERMODING)
#if 1
typedef enum
{
     IPC_RX_SYS_UPDATE=0x01,      //IMX sends this to notify SH2 that user decides to update system
	 IPC_RX_SYS_GETSYSINFO,       //2
	 IPC_RX_SYS_GETMODE,          //3
     IPC_RX_GET_ACCSTATUS,        //4
     IPC_RX_GET_IGNSTATUS,        //5
     IPC_RX_GET_TCUACCSTATUS,     //6
     IPC_RX_GET_LCDSTATUS,        //7
     IPC_RX_GET_AMPSTATUS,        //8
     IPC_RX_SET_LCDONOFF,         //9
     IPC_RX_SET_AMPONOFF,         //10
	 IPC_RX_GET_REVERSE_STATUS,   //11
	 IPC_RX_GET_VOLTAGE_STATUS,    //12
	 IPC_RX_SET_FAN_SPEED,         //13
	 IPC_RX_GET_FAN_SPEED,         //14
	 IPC_RX_ON_TOUCH_EVENT,         //15
	 IPC_RX_GET_RTC_TIME,         //16  
	 IPC_RX_SET_RVM_ONOFF,        //17
	 IPC_RX_GET_RVM_STATUS,        //18
	// IPC_RX_SYS_SETFACTORY,
    // IPC_RX_ANIMATION_START,
   //  IPC_RX_ANIMATION_END,
     //IPC_RX_CALL_ACTIVE,
	 //IPC_RX_CALL_INACTIVE,
	// IPC_RX_HMI_MODE,
	
	// IPC_RX_AMPSTATUS,
	// IPC_RX_SYS_PRE_SETFACTORY,
	// IPC_RX_SET_USB_PORT,
	// IPC_RX_GET_SYSTEM_TIME,
	// IPC_RX_GET_BSZ,
	IPC_RX_RESTART = 19,  //reset IOC
 }DA_IPC_PM_ReqEType;

#endif


//used by tjy
typedef enum {
	#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)
		IPC_SW_KEYSERVICE_FUCTION_MONITOR	= 65,/* ioc monitor with soc function */
	#endif

	#if (STD_ON == TRACE_STORE_USING )
		IPC_IOC_TRACE_STORE_SEND = 55,/* trace store: send trace to soc */
		IPC_IOC_TRACE_STORE_RECV = 45,/* trace store: recv soc store ready */
	#endif

	IPC_SW_KEYSERVICE_FUCTION_ACCT	= 15,/* ac panel control function */
}ipc_gw_KEYSERVICE_function_id;
typedef enum {
	IPC_SW_SYSTEM_FUCTION_GET_GPS_INFO	= 71,/* soc get EEPROM stord gps infomation*/
	IPC_SW_SYSTEM_FUCTION_SET_GPS_INFO = 70,/* soc set EEPROM stord gps infomation */
	IPC_SW_SYSTEM_FUCTION_GET_SERIAL_NUMBER	= 60,/* soc get EEPROM stord own serial num */
	IPC_SW_SYSTEM_FUCTION_SET_SERIAL_NUMBER = 61,/* soc set EEPROM stord own serial num */
	IPC_SW_SYSTEM_FUCTION_RTCCT_READ	= 25,/* soc read ioc RTC control function */
	IPC_SW_SYSTEM_FUCTION_RTCCT_WRITE 	= 35,/* soc wtire ioc RTC control function */
	IPC_SW_SYSTEM_FUCTION_GYROCT		= 45,/* gyro control function */
	IPC_SW_SYSTEM_FUCTION_TEMPCT		= 55,/* temp control function */
	IPC_SW_SYSTEM_FUCTION_GYROCT_START	= 1,/* gyro control function start transfer gyro,acce,temp data*/
	IPC_SW_SYSTEM_FUCTION_GYROCT_STOP	= 2,/* gyro control function stop transfer gyro,acce,temp data*/
	IPC_SW_SYSTEM_FUCTION_GET_HARDWARE_VERSION = 3, /*soc get MB hardware version */
	IPC_SW_SYSTEM_FUCTION_GET_VARIANT_CTRL_VERSION = 4,/*soc get MB VariantCtrl version */
	IPC_SW_SYSTEM_FUCTION_GET_IOC_SW_VERSION = 5,      /*soc get IOC SW version */
    IPC_SW_SYSTEM_FUCTION_GET_AVM_PROG_DATE = 6,
	IPC_SW_SYSTEM_FUCTION_GET_AVM_SW_VERSION = 7,
	IPC_SW_SYSTEM_FUCTION_OUTSIDEPM25 = 100,
	IPC_SW_SYSTEM_FUCTION_OUTSIDETEMP = 101,
	IPC_SW_SYSTEM_FUCTION_LIGHTCTL = 102,
	IPC_SW_SYSTEM_FUCTION_GET_IT_MASTER_VERSION = 1006      /*soc get IT-MASTER version */
}ipc_gw_SYSTEM_function_id;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: Task_M_IpcApp_Cfg.h ===========================*/
