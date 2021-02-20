/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#include"diagCommon_if.h"
#include"diagCommon_cfg.h"
#include"NvM_if.h"
#include"battVoltage.h"

#include"system_tick.h"
#include"system_mutex.h"
#include"system_task.h"
#include"system_signal.h"

#ifdef USING_OS_FREERTOS
#include "freertos.h"
#endif


#ifdef SW_PHASE_APPLICATION

uint8 * diagMallocMem(uint16 memLen)
{
	uint8 *memAddr = NULL;
	#ifdef USING_OS_FREERTOS
	memAddr = (uint8 *)pvPortMalloc(memLen);
	#endif

	return memAddr;
}


tDiagMutex diagCreatMutex()
{
	tDiagMutex l_mutex = NULL;
	
	System_Mutex_Create(&l_mutex);
	
	return l_mutex;
}

boolean diagMutexLock(tDiagMutex mutex)
{
    System_Mutex_Lock(mutex,FALSE);
	return TRUE;
}

void diagMutexUnlock(tDiagMutex mutex)
{
    System_Mutex_Unlock(mutex,FALSE);
}

uint32 diagGetRandomNum()
{
	uint32 seed = 0xf1f2f3f4;
	
	#ifdef DIAG_OS_UCOS
	seed = Random();
	#else
	//1TODO:
	//seed = Math_GenRandomNum();
	#endif

	return seed;
}

void diagReqSysReset()
{
	#ifdef DIAG_OS_UCOS
	Sys_SoftReset();
	#else
	//1TODO: TBD
	//NvM_ResetForWriteAll();
	//NvM_SetRamBlockStatus(21,STD_ON);
	/*NvM_WriteAll();
	NvM_MainFunction();
	Watchdog_SysReset();*/
	//PM_Mode_Restart();
	#endif
}

uint32 diagGetCurTick()
{
	uint32 curTick = 0;

	curTick = System_GetTickValue();

	return curTick;
}

boolean diagGetCanTime(tDiagCANTime *curCANTime)
{
	#ifdef DIAG_OS_UCOS

	TIME_STRU curTime;
	
	DEA_GetTimeValue(&curTime);
	if(curCANTime!=NULL)
	{
		curCANTime->Year = curTime.Year;
		curCANTime->Month = curTime.Month;
		curCANTime->Day = curTime.Day;
		curCANTime->Hour = curTime.Hour;
		curCANTime->Min = curTime.Min;
		curCANTime->Sec = curTime.Sec;

		return TRUE;
	}
	#elif defined DIAG_OS_SH2
	// TODO:  done
	float32 l_TimeValueF32;
	uint16  l_TimeValueU16;
	uint8   l_TimeValueU8;
	if(curCANTime!=NULL)
	{
		Com_TxSig_mp5_year(COM_GET_SIG_VALUE, &l_TimeValueU16);		
		curCANTime->Year = (uint8)(l_TimeValueU16-1985);
		Com_TxSig_mp5_month(COM_GET_SIG_VALUE, &l_TimeValueU8);	
		curCANTime->Month = l_TimeValueU8;
		Com_TxSig_mp5_day(COM_GET_SIG_VALUE,&l_TimeValueF32);	
		curCANTime->Day = (uint8)(l_TimeValueF32*4);
		Com_TxSig_mp5_hours(COM_GET_SIG_VALUE, &l_TimeValueU8);	
		curCANTime->Hour = l_TimeValueU8;
		Com_TxSig_mp5_minutes(COM_GET_SIG_VALUE, &l_TimeValueU8);	
		curCANTime->Min = l_TimeValueU8;
		Com_TxSig_mp5_seconds(COM_GET_SIG_VALUE, &l_TimeValueF32);	
		curCANTime->Sec = (uint8)(l_TimeValueF32*4);

		return TRUE;
	}
	#endif
	return FALSE;
}

boolean diagGetODO(uint32 *odo)
{
	if(odo!=NULL)
	{
	#ifdef DIAG_OS_UCOS
		if(DEA_GetOdoValid())
		{
			*odo = DEA_GetOdo();
		}
		else
		{
			*odo = 0xFFFFFF;
		}
		return TRUE;
	#endif
	}
	
	return FALSE;
}


#endif


void diagSetNvmBlockId(uint16 BlockId,boolean BlockChanged)
{
	NvM_SetRamBlockStatus(BlockId,BlockChanged);
}

void diagNvmWriteSync(uint16 BlockId, uint8 * NvM_SrcPtr)
{
	NvM_WriteBlock(BlockId,NvM_SrcPtr);
}

void udsSendSignal(uint32 sigId,uint32 value)
{
	#ifdef SW_PHASE_APPLICATION
	//1TODO:
		System_Signal_Send(sigId, value, STD_OFF);
	#endif
}

void udsSendMsg(uint32 taskId,uint8 *data, uint16 len)
{
#ifdef SW_PHASE_APPLICATION
#if 1
	SystemTaskMsgInfoType reqMsg;
	
	reqMsg.msgId = SYSTEM_SIG_ID_DIAGCMD;
	reqMsg.size = len;
	reqMsg.parBufPtr = data;
	System_Task_SendMessage(taskId,&reqMsg,STD_OFF);
#endif
#endif
}


//only support 8421  BCD code,and value<=99
boolean DiagConvertToBCDcode8bit(uint8 srcNum, uint8*dstNum)
{
	if(srcNum>99 || dstNum == NULL)
		return FALSE;
	else
	{
		*dstNum = ((srcNum/10)<<4 | srcNum%10);
		return TRUE;
	}
}

boolean diagCmp(const uint8*data1,const uint8*data2,uint16 len)
{
	uint16 i = 0;
	if(data1 != NULL && data2 !=NULL && len!=0 && data1!=data2)
	{
		if(data1>data2 && (data1-data2<len))
			return FALSE;
		if(data2>data1 && (data2-data1<len))
			return FALSE;
		
		for(i=0;i<len;i++)
		{
			if(data1[i] != data2[i])
				return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


//if the data in dst will be sent out from BUS,must consider the big/little endian problem
uint16 diagCpyData(uint8*dst,const uint8*src, uint16 srcLen)
{
	uint16 i = 0;
	
	if(!(dst && src) || (dst == src))
	{
		return 0;
	}
	
	while(i<srcLen)
	{
		*dst++ = *src++;
		i++;
	}
	
	return srcLen;
}

boolean diagResetBuffer(uint8* buffer,uint8 padding,uint16 len)
{
	uint16 i =0;
	if(buffer==NULL)
		return FALSE;
	while(i<len)
	{
		buffer[i++] = padding;
	}
	
	return TRUE;
	
}


void diagAppOnResponse(const uint8* data, uint8 len)
{
	
	
}


boolean diagCheckPrecondition(tDiagStopReason *stopReason)
{
	uint8 kl15Status = 0;
	uint8 accStatus = 0;
	uint8 kl30Status = 0;
	tDiagSysVoltageStatus sysVoltageStatus;

	static uint16 sysVoltage = 0;
#ifdef SW_PHASE_APPLICATION
	uint32 nmState = 0;
	#ifdef DIAG_OS_UCOS
	NetworkHandleType channelNum = 0;
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;
	#endif
#endif

	//Voltage drop during engine start: Safe interruption of diagnostic communication is
	//allowed, ECU has to stay in diagnostic mode (not allowed to drop out)
	
	sysVoltageStatus = diagCheckSysVoltageStatus();
	if(sysVoltageStatus == DIAG_SYS_VOLTAGE_STATUS_LOW)
	{
		if(diagGetKL30Status(&kl30Status) == TRUE)
		{
			if(kl30Status == DIAG_KL30_START_CRANK)
				return TRUE;
			else
			{
				if(stopReason!=NULL)
					*stopReason = DIAG_STOP_REASON_LOW_VOLT;
				return FALSE;
			}
			
		}
		else
		{
			if(stopReason!=NULL)
				*stopReason = DIAG_STOP_REASON_LOW_VOLT;
			return FALSE;
		}
	}
	else if(sysVoltageStatus == DIAG_SYS_VOLTAGE_STATUS_HIGH)
	{
		if(stopReason!=NULL)
			*stopReason = DIAG_STOP_REASON_HIGH_VOLT;
		return FALSE;
	}
	else if(diagGetKL15Status(&kl15Status) == TRUE)
	{
		#ifdef SW_PHASE_APPLICATION
		//Kl.15 is turn off but the network communication is steady and reliable.
		if(kl15Status == DIAG_KL15_OFF)
		{
			#ifdef DIAG_OS_UCOS
			if(CanNm_GetState(channelNum,&nmStatePtr,&nmModePtr)== NM_E_OK)
			{
				if(nmModePtr == NM_MODE_NETWORK)
					return TRUE;
			}
			else
			{
				if(stopReason!=NULL)
					*stopReason = DIAG_STOP_REASON_NM_OFF;
				return FALSE;
			}
			#elif defined DIAG_OS_SH2
			// TODO:
			return TRUE;
			#endif
		}
		#endif

		return TRUE;
	}


	#if 1
	if(stopReason!=NULL)
		*stopReason = DIAG_STOP_REASON_OTHER;

	return FALSE;
	#else
	return TRUE;
	#endif
}

boolean diagCheckPreCondtionGroup2()
{
	if(diagCheckIfVehicleIsStatic() == FALSE)
		return FALSE;
	else if(diagCheckIfEngineIsRunning() == TRUE)
		return FALSE;
	else if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
	{
		return FALSE;
	}

	return TRUE;
}

boolean diagGetAccStatus(uint8* accStatus)
{
	boolean ret = FALSE;
	
	if(accStatus!=NULL)
	{
	#ifdef DIAG_OS_UCOS
		if(DEA_GetAccStat() == ACC_ON)
			*accStatus = DIAG_ACC_ON;
		else
			*accStatus = DIAG_ACC_OFF;

		ret = TRUE;
	#else
		// TODO: need API
		*accStatus = DIAG_ACC_ON;
		ret = TRUE;
	#endif
	}
	
	return ret;
}

boolean diagGetKL15Status(uint8*kl15Status)
{
	uint8 klStatus = 0;
	boolean ret = FALSE;
	
	if(kl15Status!=NULL)
	{
		//1TODO: need API
		*kl15Status = DIAG_KL15_ON;
		ret = TRUE;
	}

	return ret;
}


boolean diagGetKL30Status(uint8*kl30Status)
{
	boolean ret = FALSE;
	
	if(kl30Status!=NULL)
	{
	#ifdef DIAG_OS_UCOS
		if(DEA_GetEngineStartStat() == ENGINE_START_ON)
			*kl30Status = DIAG_KL30_START_CRANK;
		else
			*kl30Status = DIAG_KL30_STOP_CRANK;
		ret = TRUE;
	#else
	// TODO: need API
		*kl30Status = DIAG_KL30_STOP_CRANK;
		ret = TRUE;
	#endif
	}

	return ret;
}


boolean diagGetSysVoltage(uint16 *voltage)
{
	boolean ret = FALSE;
	
	if(voltage)
	{
		if(BattVolt_GetVoltage(voltage) == E_OK)
			ret = TRUE;
	}
	
	return ret;
}

tDiagSysVoltageStatus diagCheckSysVoltageStatus()
{
	static boolean lowVol = FALSE;
	static boolean highVol = FALSE;
	static uint16 sysVoltage = 0;

	diagGetSysVoltage(&sysVoltage);

	//1TODO:
	sysVoltage = 1200;
	
	if(sysVoltage < DIAG_LOW_VOLTAGE_THRESHOLD_VALUE)
		lowVol = TRUE;
	else if(sysVoltage >DIAG_LOW_VOLTAGE_FILTER_THRESHOLD_VALUE)
		lowVol = FALSE;	

	if(sysVoltage > DIAG_HIGH_VOLTAGE_THRESHOLD_VALUE)
		highVol = TRUE;
	else if(sysVoltage <DIAG_HIGH_VOLTAGE_FILTER_THRESHOLD_VALUE)
		highVol = FALSE;

	if(lowVol== TRUE)
		return DIAG_SYS_VOLTAGE_STATUS_LOW;
	else if(highVol == TRUE)
		return DIAG_SYS_VOLTAGE_STATUS_HIGH;
	else
		return DIAG_SYS_VOLTAGE_STATUS_NORMAL;
}


boolean diagGetVehicleSpeed(uint16 *vehicleSpeed)
{
	boolean ret = FALSE;
	uint16 speedU16 = 0;
	
	if(vehicleSpeed!=NULL)
	{
		//1TODO: need API
		*vehicleSpeed = 0;
		ret = TRUE;
	}

	return ret;
}

boolean diagCheckIfVehicleIsStatic()
{
	uint16 speed = 0;
	
	diagGetVehicleSpeed(&speed);
	return (speed<DIAG_STATIC_VEHICLE_SPEED_VALUE);
}

boolean diagCheckIfEngineIsRunning()
{
	uint16 engineRPM = 0;
	static boolean engineIsRunning = FALSE;
	
	if(diagGetEngineRPM(&engineRPM) == TRUE)
	{
		if(engineRPM > DIAG_ENGINE_IS_RUNNING_THRESHOLD_VALUE)
			engineIsRunning = TRUE;
		else if(engineRPM<DIAG_ENGINE_IS_RUNNING_FILTER_THRESHOLD_VALUE)
			engineIsRunning = FALSE;
	}

	return engineIsRunning;
}

boolean diagGetEngineRPM(uint16 *rpm)
{
	uint16 u16_rpm =0;
	boolean ret = FALSE;
	
	if(rpm!=NULL)
	{
		//1TODO: need API
		*rpm = u16_rpm;
		ret = TRUE;
	}

	return ret;
}

void diagDisableComTx()
{
#ifdef SW_PHASE_APPLICATION
	//1TODO:
	//Com_TxStop();
#endif
}

void diagDisableComRx()
{
#ifdef SW_PHASE_APPLICATION
	//1TODO:
	//Com_RxStop();
#endif
}

void diagEnableComTx()
{
#ifdef SW_PHASE_APPLICATION
	//1TODO:
	//Com_TxStart();
#endif
}

void diagEnableComRx()
{
#ifdef SW_PHASE_APPLICATION
	//1TODO:
	//Com_RxStart();
#endif
}

void diagDisableNMTx()
{
#ifdef SW_PHASE_APPLICATION
#ifdef DIAG_OS_UCOS
	Nm_DisableCommunication(0);
#else
	// TODO:
#endif

#endif
}

void diagDisableNMRx()
{
#ifdef SW_PHASE_APPLICATION
#ifdef DIAG_OS_UCOS
	CanNm_DisabletRx();
#else
	// TODO:
#endif

#endif
}

void diagEnableNMTx()
{
#ifdef SW_PHASE_APPLICATION
#ifdef DIAG_OS_UCOS
	Nm_EnableCommunication(0);//enable tx
#else
	// TODO:
#endif

#endif
}

void diagEnableNMRx()
{
#ifdef SW_PHASE_APPLICATION
#ifdef DIAG_OS_UCOS
	CanNm_EnableRx();
#else
	// TODO:
#endif

#endif
}
