/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: System_Task_Cfg.c
Author: Stephen Du
Version: V1.0
Timestamp: 2019-01-30 11:11:46
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
#include "Std_Types.h"
#include "System_Task.h"
#include "System_Task_Cfg.h"

SystemTaskListType SystemTaskList[SYSTEM_TASK_ID_MAX];
const uint16 System_TaskMaxId = SYSTEM_TASK_ID_MAX;

extern void Task_Alarm(void * pvParameters);
extern void Task_General(void * pvParameters);
extern void Task_Diag(void * pvParameters);
extern void Task_DiagApp(void * pvParameters);
extern void Task_Com(void * pvParameters);
extern void Task_PowerModing(void * pvParameters);
extern void Task_SpiCoreRead(void * pvParameters);
extern void Task_SpiCoreRx0(void * pvParameters);
extern void Task_Nm(void * pvParameters);
extern void Task_IpcApp(void * pvParameters);
extern void Task_BC(void * pvParameters);
extern void Task_GyroAcc(void * pvParameters);
extern void Task_Audio(void * pvParameters);
extern void Task_SpiCoreRead_M(void * pvParameters);
extern void Task_SpiCoreRx0_M(void * pvParameters);
extern void Task_M_IpcApp(void * pvParameters);
extern void Task_DigitalAMP(void * pvParameters);

const char Task_Name_Alarm[] = {"Alarm"};
const char Task_Name_General[] = {"General"};
const char Task_Name_Diag[] = {"Diag"};
const char Task_Name_DiagApp[] = {"DiagApp"};
const char Task_Name_Com[] = {"Com"};
const char Task_Name_PowerModing[] = {"PowerModing"};
const char Task_Name_SpiCoreRead[] = {"SpiCoreRead"};
const char Task_Name_SpiCoreRx0[] = {"SpiCoreRx0"};
const char Task_Name_Nm[] = {"Nm"};
const char Task_Name_IpcApp[] = {"IpcApp"};
const char Task_Name_BC[] = {"BC"};
const char Task_Name_GyroAcc[] = {"GyroAcc"};
const char Task_Name_Audio[] = {"Audio"};
const char Task_Name_SpiCoreRead_M[] = {"SpiCoreRead_M"};
const char Task_Name_SpiCoreRx0_M[] = {"SpiCoreRx0_M"};
const char Task_Name_M_IpcApp[] = {"M_IpcApp"};
const char Task_Name_DigitalAMP[] = {"DigitalAMP"};

SystemTaskConfigType SystemTaskConfig[SYSTEM_TASK_ID_MAX] =
{
	//taskName, priority, stackSize, taskMainFuncPtr, taskParameter, active, queueSize
	{&Task_Name_Alarm[0], 13, 64, Task_Alarm, NULL, STD_ON, 3},
	{&Task_Name_General[0], 7, 512, Task_General, NULL, STD_ON, 3},
	{&Task_Name_Diag[0], 8, 512, Task_Diag, NULL, STD_ON, 3},
	{&Task_Name_DiagApp[0], 8, 512, Task_DiagApp, NULL, STD_ON, 3},
	{&Task_Name_Com[0], 8, 640, Task_Com, NULL, STD_ON, 3},
	{&Task_Name_PowerModing[0], 8, 384, Task_PowerModing, NULL, STD_ON, 3},
	{&Task_Name_SpiCoreRead[0], 9, 512, Task_SpiCoreRead, NULL, STD_ON, 1},
	{&Task_Name_SpiCoreRx0[0], 7, 512, Task_SpiCoreRx0, NULL, STD_ON, 1},
	{&Task_Name_Nm[0], 8, 384, Task_Nm, NULL, STD_ON, 1},
	{&Task_Name_IpcApp[0], 7, 384, Task_IpcApp, NULL, STD_ON, 1},
	{&Task_Name_BC[0], 8, 300, Task_BC, NULL, STD_ON, 1},
	{&Task_Name_GyroAcc[0], 7, 384, Task_GyroAcc, NULL, STD_ON, 1},
	{&Task_Name_Audio[0], 8, 512, Task_Audio, NULL, STD_ON, 3},
	{&Task_Name_SpiCoreRead_M[0], 8, 384, Task_SpiCoreRead_M, NULL, STD_ON, 1},
	{&Task_Name_SpiCoreRx0_M[0], 7, 512, Task_SpiCoreRx0_M, NULL, STD_ON, 1},
	{&Task_Name_M_IpcApp[0], 8, 512, Task_M_IpcApp, NULL, STD_ON, 1},
	{&Task_Name_DigitalAMP[0], 8, 384, Task_DigitalAMP, NULL, STD_ON, 1},
};
/*=========================== END OF FILE: System_Task_Cfg.c ===========================*/
