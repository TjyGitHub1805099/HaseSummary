/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#include"diagCommon_if.h"
#include"diagCommon_cfg.h"
#include"NvM_if.h"
#include"BattVoltage.h"

#include"System_Tick.h"
#include"System_Mutex.h"
#include"System_Task.h"
#include"System_Signal.h"
#include"pin_wrapper_if.h"
#include"ExternRTC.h"

#include"dtcApp_if.h"

#ifdef USING_OS_FREERTOS
#include "FreeRTOS.h"
#endif
#include "Com_Cfg.h"
#include "Com_If.h"

#ifdef SW_PHASE_APPLICATION
#include "dea.h"
#include "pmStateMachine_If.h"


extern void PM_Mode_Restart();
extern uint8 BattVolt_GetStatus(void);

volatile uint32 gDiagRandomNumber = 0;


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
	seed = System_GetTickValue() + (gDiagRandomNumber<<16);
	#endif
	
	return seed;
}

void diagReqSysReset()
{
	#ifdef DIAG_OS_UCOS
	Sys_SoftReset();
	#else
	PM_Mode_Restart();
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
	//#ifdef DIAG_OS_UCOS
		if(DEA_GetOdoValid())
		{
			*odo = DEA_GetOdo();
		}
		else
		{
			*odo = 0xFFFFFF;
		}
		return TRUE;
	//#endif
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
/*****************************************************************************************************
* Convert hexadecimal number to ASCII code 
* Input: 6 Byte (year, month, day, hour, minute, second)
* Output: 12 Byte
******************************************************************************************************/
uint8* DateHexToDateAscii(uint8* hex, uint8 hexLen, uint8* asc, uint8 ascLen)
{
	uint8 year = 0;
	uint8 month = 0;
	uint8 day = 0;
	uint8 hour = 0;
	uint8 min = 0;
	uint8 sec = 0;

	uint8 year_high = 0;
	uint8 year_low = 0;
	uint8 month_high = 0;
	uint8 month_low = 0;
	uint8 day_high = 0;
	uint8 day_low = 0;
	uint8 hour_high = 0;
	uint8 hour_low = 0;
	uint8 min_high = 0;
	uint8 min_low = 0;
	uint8 sec_high = 0;
	uint8 sec_low = 0;

	if(hex == NULL || asc == NULL || hexLen != 6 || ascLen != 12)
		return NULL;
	
	year  = (hex[0]>>4)*16 + (hex[0]&0xf);
	month = (hex[1]>>4)*16 + (hex[1]&0xf);
	day   = (hex[2]>>4)*16 + (hex[2]&0xf);
	hour  = (hex[3]>>4)*16 + (hex[3]&0xf);
	min   = (hex[4]>>4)*16 + (hex[4]&0xf);
	sec   = (hex[5]>>4)*16 + (hex[5]&0xf);

	year_high  = year/10;
	year_low   = year%10;
	month_high = month/10;
	month_low  = month%10;
	day_high   = day/10;
	day_low    = day%10;
	hour_high  = hour/10;
	hour_low   = hour%10;
	min_high   = min/10;
	min_low    = min%10;
	sec_high   = sec/10;
	sec_low    = sec%10;
	
	asc[0]  = year_high  + 0x30;
	asc[1]  = year_low   + 0x30;
	asc[2]  = month_high + 0x30;
	asc[3]  = month_low  + 0x30;
	asc[4]  = day_high   + 0x30;
	asc[5]  = day_low    + 0x30;
	asc[6]  = hour_high  + 0x30;
	asc[7]  = hour_low   + 0x30;
	asc[8]  = min_high   + 0x30;
	asc[9]  = min_low    + 0x30;
	asc[10] = sec_high   + 0x30;
	asc[11] = sec_low    + 0x30;

	return asc;
	
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


uint16 diagReveCpyData(uint8* dst, uint8* src, uint16 srcLen)
{
    uint16 i = 0;

    uint8* tempDst = NULL;
    uint8* tempSrc = NULL;

    if(!(dst && src) || (dst == src))
    {
        return 0;
    }

    tempDst = dst;
    tempSrc = src+srcLen-1;

    while(i < srcLen)
    {
        *tempDst++ = *tempSrc--;
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

//if TRUE means OK,else uds stops to run
boolean diagCheckGlobalPrecondition()
{
	boolean ret = (diagCheckSysVoltageStatus() == DIAG_SYS_VOLTAGE_STATUS_NORMAL);
	return ret;
}

boolean diagCheckPrecondition(tDiagStopReason *stopReason)
{
	uint8 kl15Status = 0;
	//uint8 accStatus = 0;
	uint8 kl30Status = 0;
	tDiagSysVoltageStatus sysVoltageStatus;

	//static uint16 sysVoltage = 0;
#ifdef SW_PHASE_APPLICATION
	//uint32 nmState = 0;
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
#if 0
	if(diagCheckIfVehicleIsStatic() == FALSE)
		return FALSE;
	else if(diagCheckIfEngineIsRunning() == TRUE)
		return FALSE;
#endif

	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
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
		//*accStatus = (GetPinValue_ACC() == ACC_ON)? DIAG_ACC_ON : DIAG_ACC_OFF;
		*accStatus = (PM_GetAccStatus() == STD_ON)? DIAG_ACC_ON : DIAG_ACC_OFF;
		
		ret = TRUE;
	#endif
	}
	
	return ret;
}

boolean diagGetKL15Status(uint8* kl15Status)
{
	boolean ret = FALSE;
	
	if(kl15Status != NULL)
	{
		//*kl15Status = (GetPinValue_IGN() == PM_IGN_ON)? DIAG_KL15_ON : DIAG_KL15_OFF;
		*kl15Status = (PM_GetIgnStatus() == STD_ON)? DIAG_KL15_ON : DIAG_KL15_OFF;
		ret = TRUE;
	}

	return ret;
}


boolean diagGetKL30Status(uint8* kl30Status)
{
	boolean ret = FALSE;
	
	if(kl30Status != NULL)
	{	
		if(DEA_GetEngineStartStat() == ENGINE_START_ON)
			*kl30Status = DIAG_KL30_START_CRANK;
		else
			*kl30Status = DIAG_KL30_STOP_CRANK;
		ret = TRUE;
	}

	return ret;
}


boolean diagGetSysVoltage(uint16 *voltage)
{
	boolean ret = FALSE;
	//uint16 adc_value = 0;
	
	if(voltage)
	{
		#if 0
		if(BattVolt_GetVoltage(voltage) == E_OK)
			ret = TRUE;
		#else
		if(BattVolt_GetRawValue(voltage) == E_OK)
			ret = TRUE;
		#endif
	}
	
	return ret;
}

tDiagSysVoltageStatus diagCheckSysVoltageStatus()
{
	uint8 status = 0;
	#if 0
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
	#else
	status = BattVolt_GetStatus();
	
	switch(status)
	{
	case BATTVOLT_STATE_ULTRALOW:
    case BATTVOLT_STATE_LOW:
		return DIAG_SYS_VOLTAGE_STATUS_LOW;
    case BATTVOLT_STATE_NORMAL:
		return DIAG_SYS_VOLTAGE_STATUS_NORMAL;
    case BATTVOLT_STATE_HIGH:
	case BATTVOLT_STATE_ULTRAHIGH:
		return DIAG_SYS_VOLTAGE_STATUS_HIGH;
	default:
		;
	}

	return BATTVOLT_STATE_NORMAL;
	#endif
}


boolean diagGetVehicleSpeed(float *vehicleSpeed)
{
	boolean ret = FALSE;
	//uint16 speedU16 = 0;
	//float speedFloat = 0;
	
	if(vehicleSpeed != NULL_PTR)
	{
		//1TODO: need API
	#if 0
		if(dtcApp_getVehicleSpeedTimeoutFlag() == FALSE)
		{
			//Com_ReceiveSignal(SIG_HND_VSO, &speedU16);
			Com_RxSig_GetVSO(&speedFloat);
			
			#if 0
			if((uint16)speedFloat == 65535)	// no signal
				*vehicleSpeed = 0;
			else
			#endif
				*vehicleSpeed = speedFloat;
		}
	#else
		if(dtcApp_getVehicleSpeedTimeoutFlag() == FALSE)
		{
			*vehicleSpeed = (float32)(DEA_GetRawVehicleSpeedCanOutput()/100.0);
		}
		else
		{
			*vehicleSpeed = 0;
		}
	#endif
		ret = TRUE;
	}

	return ret;
}

boolean diagCheckIfVehicleIsStatic()
{
	//uint16 speed = 0;
	float32 speed = 0;
	diagGetVehicleSpeed(&speed);
	return ((uint16)speed < DIAG_STATIC_VEHICLE_SPEED_VALUE);
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
	//uint16 u16_rpm =0;
	boolean ret = FALSE;
	
	if(rpm!=NULL)
	{
		//1TODO: need API
		if(dtcApp_getVehicleRpmTimeoutFlag()==FALSE)
		{
			*rpm = DEA_GetFilterEngineRevolution();
		}
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
