/**************************
description: DTC Core
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/08
**************************/
#include"dtcCore_if.h"
#include"sid0x14_if.h"
#include"uds_sid_if.h"
#include"trace.h"

static void dtcCoreResetStatusBitByPowerOn();
static void dtcCoreUpdateInforInNvm(uint8 ch,uint8 innerDtcId);
static boolean dtcCoreFillTxBuffer(const tUdsReqInfor*reqInfor,uint8 *respData, uint16 len);

#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
static void dtcCoreCleanStoredSnapshot(uint8 ch,uint8 innerDtcId);
#endif

extern const tDtcCoreCfgInfor gDtcCoreCfgInfor;
extern const tDtcMixInfor *gDtcMixInforArray[];
extern tDtcVmInfor *gDtcVmInforArray[];
extern tDtcStatusAndExtInfor *gDtcStatusAndExtInforArray[];
extern uint16 gDtcTotalNum[];
extern tDiagMutex gDtcMutex1[];
extern uint16 gRespDataLen[];

#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
extern uint8 *gDtcTotalNumStoredInNvmArray;
extern tDtcStoredInfor *gDtcStoredInforArray[];
extern const uint8 *gNvmBlockIdListForDtcStoredInforArray[];
extern uint16 gDtcAvailableSpaceNumInNvmArray[];

#else
extern const uint8 *gNvmBlockIdListForRuntimeSnapshotArray[]; // map to gRuntimeSnapshotArray
extern const uint8 *gNvmBlockIdListForDtcExtInforArray[]; // map to gDtcStatusAndExtInforArray

#endif

static boolean gDtcIgnOnFlag = FALSE;
static boolean gIgnStatusSwitchFlag = FALSE;
static boolean gSysVolStatusIsSwitched = FALSE;
static boolean gDtcMonitorStatus = TRUE;

uint8 gDtcCurIgnStatus = DIAG_KL15_OFF;
uint8 gDtcKL30Status = DIAG_KL30_STOP_CRANK;
uint8 gDtcAccStatus = DIAG_ACC_OFF;
uint8 gDtcAppTaskIdList[DTC_APP_TASKID_LIST_LEN] = {0};


static boolean gEnableUpdateDtcBitStatusFlag = FALSE;
//static tDiagMutex gDtcMutex1 = 0; 


//static tDiagMutex gDtcMutex2 = 0;

boolean dtcCoreGetIgnOnFlag()
{
	return gDtcIgnOnFlag;
}

//only for SID 0x85?
boolean dtcCoreCheckCommonPrecondition()
{
	//if KL15 off?
	//return FALSE;
	
	// if vehicle speed != 0?
	//return FALSE;
	
	// if rpm !=0? 
	// return FALSE;

	//if voltage too high ?
	//return FALSE;

	//if voltage too low ?
	//return FALSE;

	return TRUE;
}


boolean dtcCoreRegAppTaskId(uint8 taskid)
{	
	uint8 i = 0;

	for(i=0;i<DTC_APP_TASKID_LIST_LEN;i++)
	{
		if(gDtcAppTaskIdList[i] == 0)
		{
			gDtcAppTaskIdList[i] = taskid;
			return TRUE;
		}
	}

	return FALSE;
}

void dtcCoreChangeUpdateDtcStatusFlag(uint32 ch,boolean enableOrDisable)
{
	gEnableUpdateDtcBitStatusFlag = enableOrDisable;
}


void dtcCoreResetStatusBitByPowerOn()
{
	uint8 ch = 0;
	uint16 i = 0;
	boolean needUpdateNvm = FALSE;

	for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
	{
		for(i=0;i<gDtcTotalNum[ch];i++)
			{
				if(gDtcVmInforArray[ch][i].enableDtc == TRUE)
				{
					if(gDtcCoreCfgInfor.logLevel >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
						DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"ch:%d,dtc[%d]:status:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
					if(gDtcVmInforArray[ch][i].unlearningByAgingCounterFlag== TRUE)// && gDtcMixInforArray[i].operCycType == DTC_OPERATION_CLCLIC_BY_POWER_ON)
					{
						//if in last operation cycle,there was no error, then update agingCounter
						if(((gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_CONFIRMED)!=0)
							&& ((gDtcStatusAndExtInforArray[ch][i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE
							|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0))
						{
							if(gDtcStatusAndExtInforArray[ch][i].extData.agingCounter < gDtcCoreCfgInfor.agingCounterLimitValue)
							{
								gDtcStatusAndExtInforArray[ch][i].extData.agingCounter += 1;
								needUpdateNvm = TRUE;
									
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"power on,ch:%d,dtc[%d],agingCounter++:%d\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].extData.agingCounter);
							}
						}
					}
					//if(gDtcVmInforArray[i].unlearningByAgingCounterFlag== TRUE)
					{
						// iso14229-2013,PG373
						//in last operation cycle,if there was no testFailed or testNotCompleted,then reset pendingDTC bit to 0
						if((gDtcStatusAndExtInforArray[ch][i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0)
						{
							gDtcStatusAndExtInforArray[ch][i].statusBit &= ~DTC_STATUS_BIT_PENDING;
							needUpdateNvm = TRUE;
						}

						#if 0 // in DNTC,no requirement to reset dtc if aging counter exceeded
						if(gDtcStatusAndExtInforArray[ch][i].extData.agingCounter >= DTC_AGING_COUNTER_LIMIT)
						{
							dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
						}
						#endif
		
						gDtcStatusAndExtInforArray[ch][i].statusBit |= DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
						gDtcStatusAndExtInforArray[ch][i].statusBit &= ~(DTC_STATUS_BIT_TEST_FAILED | DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE /*|DTC_STATUS_BIT_TEST_FAILED_SINCE_LAST_CLEAR*/);
						
					}
					
					if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
						DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"ch:%d,dtc[%d]:status:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
				}
		
				if(needUpdateNvm == TRUE)
					dtcCoreUpdateInforInNvm(ch,i);
				needUpdateNvm = FALSE;
			}

	}
	
}

void dtcCoreInit()
{
	uint8 ch= 0;

	for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
	{
		gDtcMutex1[ch] = 0;
		gDtcMutex1[ch] = diagCreatMutex();
		if(gDtcMutex1[ch] == 0)
			return;
	}
	
	dtcCorePartInit();
	sid0x14Init();
		
	diagResetBuffer(gDtcAppTaskIdList,0,DTC_APP_TASKID_LIST_LEN);

	dtcCoreResetStatusBitByPowerOn();
	
	gDtcIgnOnFlag = FALSE;
	gIgnStatusSwitchFlag = FALSE;

	gEnableUpdateDtcBitStatusFlag = TRUE;

	//1TODO:
	//diagSetNvmBlockId(gDtcNvmInforList_NvMBlockID,STD_ON);

}

tUdsNRCType dtcCoreClearDtc(const tUdsReqInfor*reqInfor)
{

	uint16 i= 0;
	uint32 dtcGroup = 0;
	tUdsNRCType ret = NRC_NONE;
	uint8 restartDtcDetectReq[16];
	boolean validDTCGroup = FALSE;
	uint8 ch = reqInfor->ch;
	const uint8 *data = reqInfor->data+1;
	uint16 len = reqInfor->len-1;
	//boolean reqFromFuncAddrFlag = reqInfor->reqFromFuncAddrFlag;
	
	if(data==NULL || len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(sid0x14CheckPreCondition()!=NRC_NONE)
		return NRC_CONDITION_NOT_CORRECT;
	
	dtcGroup = (uint32)(data[0]<<16 | data[1] <<8 | data[2]);
	
	if(dtcGroup == DTC_GROUP_ALL)//ALL dtc
	{
		validDTCGroup = TRUE;

		//for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
		//{
			//if(reqFromFuncAddrFlag == FALSE && (ch!=reqInfor.ch))
				//continue;
			for(i=0;i<gDtcTotalNum[ch];i++)
			{
				if(gDtcVmInforArray[ch][i].enableDtc == TRUE)
				{
					diagMutexLock(gDtcMutex1[ch]);
					dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_SID_0X14);
					diagMutexUnlock(gDtcMutex1[ch]);
				}
			}
		//}
		
	}
	else
	{
		switch(dtcGroup)
		{
		case DTC_GROUP_BODY:
		case DTC_GROUP_CHASSIS:
		case DTC_GROUP_EMISSIONS_RELATED:
		case DTC_GROUP_NETWORK:
		case DTC_GROUP_POWERTRAIN:
			
				
			//for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
			//{
				//if(reqFromFuncAddrFlag == FALSE && (ch!=reqInfor.ch))
					//continue;
				for(i=0;i<gDtcTotalNum[ch];i++)
				{
					if((gDtcMixInforArray[ch][i].dtcGroup == dtcGroup)&& (gDtcVmInforArray[ch][i].enableDtc == TRUE))
					{
						diagMutexLock(gDtcMutex1[ch]);
						dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_SID_0X14);
						diagMutexUnlock(gDtcMutex1[ch]);
						DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"ch:%d,dtc[%d],del dtc group:0x%x\r\n",ch,i,dtcGroup);
						validDTCGroup = TRUE;
					}
				}
			//}
			
			break;
		default:
			// specify single DTC
			//for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
			//{
				//if(reqFromFuncAddrFlag == FALSE && (ch!=reqInfor.ch))
					//continue;
				for(i=0;i<gDtcTotalNum[ch];i++)
				{
					if((gDtcVmInforArray[ch][i].dtc_id == dtcGroup)&& (gDtcVmInforArray[ch][i].enableDtc == TRUE))
					{
						diagMutexLock(gDtcMutex1[ch]);
						dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_SID_0X14);
						diagMutexUnlock(gDtcMutex1[ch]);
						DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"ch:%d,dtc[%d],del dtc group:0x%x\r\n",ch,i,dtcGroup);
						validDTCGroup = TRUE;
						break;
					}
				}
			//}
		}
	}

	if(validDTCGroup == TRUE)
	{
		restartDtcDetectReq[0] = (uint8)(reqInfor->ch&0xFF);
		restartDtcDetectReq[1] = (uint8)((reqInfor->reqTick&0xFF000000)>>24);
		restartDtcDetectReq[2] = (uint8)((reqInfor->reqTick&0xFF0000)>>16);
		restartDtcDetectReq[3] = (uint8)((reqInfor->reqTick&0xFF00)>>8);
		restartDtcDetectReq[4] = (uint8)(reqInfor->reqTick&0xFF);
		restartDtcDetectReq[5] = 0x14;//reqInfor->data[0];//sid
		//DTC GROUP
		restartDtcDetectReq[6] = reqInfor->data[1];
		restartDtcDetectReq[7] = reqInfor->data[2];
		restartDtcDetectReq[8] = reqInfor->data[3];

		for(i=0;i<DTC_APP_TASKID_LIST_LEN;i++)
		{
			if(gDtcAppTaskIdList[i] != 0)
			{
				udsSendMsg(gDtcAppTaskIdList[i],restartDtcDetectReq,9);
			}
		}
	}
	else
	{
		ret = NRC_REQUEST_OUT_OF_RANGE;
	}
	return ret;
}



void dtcCoreResetDTCInfor(uint8 ch,uint8 i, tDTCResetReason reason)
{
	
	gDtcVmInforArray[ch][i].preResetReason = reason;
	if(reason == DTC_RESET_BY_SID_0X14)
		gDtcStatusAndExtInforArray[ch][i].statusBit = DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
	else if (reason == DTC_RESET_BY_NOT_CODED)
		gDtcStatusAndExtInforArray[ch][i].statusBit = DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
	else if(reason == DTC_RESET_BY_AGING_COUNTER_EXCEED)
		gDtcStatusAndExtInforArray[ch][i].statusBit = DTC_STATUS_BIT_NONE;
	
	//PG29,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf
	//When a DTC is cleared via the DTC aging mechanism, the same parameters except for the
	//aged counter will be cleared. The aged counter is retained in ECU long term memory to allow
	//the rectifier to determine if that particular fault was previously deleted due to the ageing mechanism.
	if(reason == DTC_RESET_BY_AGING_COUNTER_EXCEED)
	{
		if(gDtcVmInforArray[ch][i].unlearningByAgingCounterFlag == TRUE)
		{
			if(gDtcStatusAndExtInforArray[ch][i].extData.agedCounter <254)
				gDtcStatusAndExtInforArray[ch][i].extData.agedCounter += 1; // 255 means invalid value or DTC can't be deleted autonomously
		}
		else
			gDtcStatusAndExtInforArray[ch][i].extData.agedCounter = 255;
	}
	else// if(reason == DTC_RESET_BY_SID_0X14 || reason==DTC_RESET_BY_NOT_CODED)
		gDtcStatusAndExtInforArray[ch][i].extData.agedCounter = 0;
	gDtcStatusAndExtInforArray[ch][i].extData.agingCounter = gDtcCoreCfgInfor.agingCounterInitValue;
	gDtcStatusAndExtInforArray[ch][i].extData.occCounter = 0;

	dtcCoreResetSnapshot(ch,i);
#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
	if(gDtcStatusAndExtInforArray[ch][i].storedInNvmFlag == TRUE)
	{
		dtcCoreCleanStoredSnapshot(ch,i);
		gDtcStatusAndExtInforArray[ch][i].storedInNvmFlag = FALSE;
	}
#endif
	//diagMutexLock(gDtcMutex1[ch]);
	gDtcVmInforArray[ch][i].passiveOrActive = DTC_PRE_INIT;
	
	gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_NONE;
	gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
	//gDtcVmInforArray[ch][i].curTimer.tick = 0;
	gDtcVmInforArray[ch][i].curTimer.preTick = 0;
	dtcCoreStoreSnapshotToNvm(ch,i);
	//diagMutexUnlock(gDtcMutex1[ch]);

}


void dtcCoreMainLoop()
{
	uint8 ch = 0;
	uint16 i= 0;
	uint32 curTick = 0;
	uint32 tickInterval = 0;
	tDiagSysVoltageStatus curSysVolStatus;
	boolean needUpdateNvm = FALSE;
	static tDiagSysVoltageStatus sysVolStatus = DIAG_SYS_VOLTAGE_STATUS_NORMAL;
	static uint8 gDtcStableIgnStatus = DIAG_KL15_OFF;

	diagGetAccStatus(&gDtcAccStatus);
	diagGetKL15Status(&gDtcCurIgnStatus);
	diagGetKL30Status(&gDtcKL30Status);

	curSysVolStatus = diagCheckSysVoltageStatus();

	if(curSysVolStatus != sysVolStatus)
	{
		sysVolStatus = curSysVolStatus;
		gSysVolStatusIsSwitched = TRUE;
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"[dtc]VOL STATUS:%s\r\n",(sysVolStatus == DIAG_SYS_VOLTAGE_STATUS_NORMAL)?"Normal":"not Normal");
	}

	#if 1
	if(gDtcStableIgnStatus != gDtcCurIgnStatus)
	{
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"[dtc]IGN STATUS:%s\r\n",(gDtcCurIgnStatus == DIAG_KL15_ON)?"ON":"OFF");
		gDtcStableIgnStatus = gDtcCurIgnStatus;
		gIgnStatusSwitchFlag = TRUE;
	}

	#else
	if(DIAG_KL15_ON == gDtcCurIgnStatus)
	{
		if(gDtcIgnOnFlag == FALSE)
		{
			gDtcIgnOnFlag = TRUE;
			gIgnStatusSwitchFlag = TRUE;
			DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"IGN ON:%d\r\n",diagGetCurTick());

			#if 1
			if(firstIgnitionOnFLag == FALSE)
			{
				// inform app layer to delay 1200ms for some DTCs
				#if 1

				for(i=0;i<DTC_APP_TASKID_LIST_LEN;i++)
				{
					if(gDtcAppTaskIdList[i] != 0)
					{
						udsSendMsg(gDtcAppTaskIdList[i],delayDtcDetectReq,4);
					}
				}
				#else
					// or callback function directly for effectively
					dtcAppReDelayDetect();
				#endif
			}

			if(firstIgnitionOnFLag == TRUE)
				firstIgnitionOnFLag = FALSE;
			#endif
			
		}
	}
	else
	{
		if(gDtcIgnOnFlag == TRUE)
		{
			DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"IGN Off:%d\r\n",diagGetCurTick());
			gDtcIgnOnFlag = FALSE;
			gIgnStatusSwitchFlag = TRUE;
		}
	}
	#endif

	gDtcIgnOnFlag = (gDtcStableIgnStatus == DIAG_KL15_ON)?TRUE:FALSE;
	
	if(gEnableUpdateDtcBitStatusFlag == TRUE)
	{	
		for(ch=0;ch<gDtcCoreCfgInfor.udsChMaxNum;ch++)
		{
			diagMutexLock(gDtcMutex1[ch]);
			for(i=0;i<gDtcTotalNum[ch];i++)
			{
				//diagMutexLock(gDtcMutex1);
				if(gDtcVmInforArray[ch][i].enableDtc == TRUE)
				{

					if(gDtcMixInforArray[ch][i].operCycType == DTC_OPERATION_CLCLIC_BY_IGN_ON)
					{
						gDtcVmInforArray[ch][i].continueToDetectFlag = gDtcIgnOnFlag;
						if(gDtcMixInforArray[ch][i].isStoppedByHiLowSysVoltage == TRUE)
						{
							if(DIAG_SYS_VOLTAGE_STATUS_NORMAL == sysVolStatus)
								gDtcVmInforArray[ch][i].continueToDetectFlag &=	TRUE;
							else
								gDtcVmInforArray[ch][i].continueToDetectFlag = FALSE;
						}

						if(gIgnStatusSwitchFlag == TRUE && gDtcIgnOnFlag == TRUE)
						{
							if(gDtcVmInforArray[ch][i].unlearningByAgingCounterFlag== TRUE)
							{

								//if(gDtcLogLevel >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[i].traceLogEnableFlag == TRUE)
									//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_WARNING,"ign on,ch:%d,dtc[%d],status:%d\r\n",ch,i,gDtcNvmInforList[i].statusBit);
								
								//if in last operation cycle,there was no error, then update agingCounter
								if(((gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_CONFIRMED)!=0)
									&& ((gDtcStatusAndExtInforArray[ch][i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE
									|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0))
								{
									if(gDtcStatusAndExtInforArray[ch][i].extData.agingCounter < gDtcCoreCfgInfor.agingCounterLimitValue)
									{
										gDtcStatusAndExtInforArray[ch][i].extData.agingCounter += 1;
										needUpdateNvm = TRUE;
										if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
											DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"IGN On,ch:%d,dtc[%d],agingCounter++:%d\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].extData.agingCounter);

									}
									#if 0
									if(gDtcStatusAndExtInforArray[ch][i].extData.agingCounter > gDtcCoreCfgInfor.agingCounterLimitValue)
									{
										dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
									}
									needUpdateNvm = TRUE;
									#endif
										
								}
							}
						
							if((gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_PENDING)!=0)
							{
								if((gDtcStatusAndExtInforArray[ch][i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0)
								{
									gDtcStatusAndExtInforArray[ch][i].statusBit &= ~DTC_STATUS_BIT_PENDING;
								}
							}
							gDtcStatusAndExtInforArray[ch][i].statusBit |= DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
							gDtcStatusAndExtInforArray[ch][i].statusBit &= ~(DTC_STATUS_BIT_TEST_FAILED | DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE );
							gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_NONE;
							gDtcVmInforArray[ch][i].curTimer.preTick = diagGetCurTick();
							gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
							
							if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
								DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"IgnOn,ch:%d,dtc[%d],statusBit:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
							needUpdateNvm = TRUE;
						}
					}
					else if(/*gSysVolStatusIsSwitched == TRUE &&*/ gDtcMixInforArray[ch][i].isStoppedByHiLowSysVoltage == TRUE)
					{
						gDtcVmInforArray[ch][i].continueToDetectFlag = ((DIAG_SYS_VOLTAGE_STATUS_NORMAL == sysVolStatus)?TRUE:FALSE);
					}

					if(gDtcVmInforArray[ch][i].continueToDetectFlag == TRUE && (gDtcVmInforArray[ch][i].passiveOrActive != DTC_PRE_INIT))
					{
						needUpdateNvm = TRUE;
						gDtcStatusAndExtInforArray[ch][i].statusBit &= ~(DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR
														|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE);
						if(gDtcVmInforArray[ch][i].unlearningByAgingCounterFlag == TRUE)
						{	
								
								if((gDtcVmInforArray[ch][i].passiveOrActive == DTC_PRE_PASSIVE) && gDtcVmInforArray[ch][i].curTimer.type != DTC_TIMER_DISQUALIFY)
								{
									gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_DISQUALIFY;
									//gDtcVmInforArray[ch][i].curTimer.preTick = gDtcVmInforArray[ch][i].disqual_timer;
									gDtcVmInforArray[ch][i].curTimer.preTick = diagGetCurTick();
									gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d]:NA,S:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
									#if 0
									if(gDtcVmInforArray[i].timerCalculatedByUpperLayerFlag == TRUE)
									{
										if(gDtcVmInforArray[i].preResetReason!= DTC_RESET_BY_SID_0X14)
										{
											gDtcVmInforArray[i].curTimer.tick = 0;
										}
									}
									#endif
								}
								else if ((gDtcVmInforArray[ch][i].passiveOrActive == DTC_PRE_ACTIVE) && gDtcVmInforArray[ch][i].curTimer.type != DTC_TIMER_QUALIFY)
								{
									gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_QUALIFY;
									//gDtcVmInforArray[i].curTimer.tick = gDtcVmInforArray[i].qual_timer;
									gDtcVmInforArray[ch][i].curTimer.preTick = diagGetCurTick();
									gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
									
									
									//pg28,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf:
									//A: It will be reset to ¡°0¡± if there is any failed test result during an ignition cycle.
									//B: If the DTC can¡¯t be deleted autonomously, ECU must save the aging counter status 255dec in the error memory.
									gDtcStatusAndExtInforArray[ch][i].extData.agingCounter = 0;
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d]:agingCounter = 0\r\n",ch,i);
									//»ª³¿M8ÐèÇó: Pg28,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf:
									//Fault occurrence counter will be incremented by 1 
									//when the ¡°testFailedThisOperationCycle¡± bit is set to '1' from '0'. 
									if((gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE) == 0)
									{
										if(gDtcStatusAndExtInforArray[ch][i].extData.occCounter<254)//the max value is 254
										{
											gDtcStatusAndExtInforArray[ch][i].extData.occCounter += 1;
											if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
												DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"ch:%d,dtc[%d]:occCounter=%d\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].extData.occCounter);
										}
									}

									needUpdateNvm = TRUE;
									gDtcStatusAndExtInforArray[ch][i].statusBit |= (DTC_STATUS_BIT_TEST_FAILED
																	|DTC_STATUS_BIT_PENDING
																	|DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE);
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d]:A,S:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
									/*
									if(gDtcVmInforArray[ch][i].timerCalculatedByUpperLayerFlag == TRUE)
									{
										if(gDtcVmInforArray[ch][i].preResetReason!= DTC_RESET_BY_SID_0X14)
										{
											gDtcVmInforArray[ch][i].curTimer.tick = 0;
										}
									}
									*/
								}
								

						}
						else
						{
								if((gDtcVmInforArray[ch][i].passiveOrActive == DTC_PRE_PASSIVE) && gDtcVmInforArray[ch][i].curTimer.type != DTC_TIMER_DISQUALIFY)
								{
									dtcCoreResetDTCInfor(ch,i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
									gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_DISQUALIFY;
									//gDtcVmInforArray[ch][i].curTimer.tick = gDtcVmInforArray[i].disqual_timer;
									gDtcVmInforArray[ch][i].curTimer.preTick = diagGetCurTick();
									gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
									
									gDtcStatusAndExtInforArray[ch][i].statusBit = DTC_STATUS_BIT_NONE;
								}
								else if((gDtcVmInforArray[ch][i].passiveOrActive == DTC_PRE_ACTIVE) && gDtcVmInforArray[ch][i].curTimer.type != DTC_TIMER_QUALIFY)
								{
									gDtcStatusAndExtInforArray[ch][i].statusBit = (DTC_STATUS_BIT_TEST_FAILED
																	|DTC_STATUS_BIT_PENDING
																	|DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE);

									gDtcVmInforArray[ch][i].curTimer.type = DTC_TIMER_QUALIFY;
									//gDtcVmInforArray[ch][i].curTimer.tick = gDtcVmInforArray[i].qual_timer;
									gDtcVmInforArray[ch][i].curTimer.preTick = diagGetCurTick();;
									gDtcVmInforArray[ch][i].curTimer.timeoutFlag = FALSE;
									
								}
								
								needUpdateNvm = TRUE;							
						}

						
						if(gDtcVmInforArray[ch][i].curTimer.type != DTC_TIMER_NONE && gDtcVmInforArray[ch][i].curTimer.timeoutFlag == FALSE)
						{	
							tickInterval = (gDtcVmInforArray[ch][i].curTimer.type == DTC_TIMER_DISQUALIFY)?gDtcVmInforArray[ch][i].disqual_timer:gDtcVmInforArray[ch][i].qual_timer;
							curTick = diagGetCurTick();
							#if 0
							if(gDtcVmInforArray[ch][i].timerCalculatedByUpperLayerFlag == TRUE)
							{
								if(gDtcVmInforArray[ch][i].preResetReason!= DTC_RESET_BY_SID_0X14)
								{
									tickInterval = 0;
								}
							}
							#endif
							if((curTick-gDtcVmInforArray[ch][i].curTimer.preTick) >= tickInterval)
							{
								gDtcVmInforArray[ch][i].curTimer.timeoutFlag = TRUE;
								
								if(gDtcVmInforArray[ch][i].curTimer.type == DTC_TIMER_DISQUALIFY)
								{
									gDtcStatusAndExtInforArray[ch][i].statusBit &= ~DTC_STATUS_BIT_TEST_FAILED;
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_1 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d]:Disqualify,S:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
									
								}
								else
								{
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_1 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d]:Qualify,S:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
									gDtcStatusAndExtInforArray[ch][i].statusBit |= DTC_STATUS_BIT_CONFIRMED;
									if(gDtcVmInforArray[ch][i].preResetReason == DTC_RESET_BY_SID_0X14)
									{
										gDtcStatusAndExtInforArray[ch][i].statusBit |= DTC_STATUS_BIT_TEST_FAILED_SINCE_LAST_CLEAR;
										if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_1 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
											DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d] failed again,S:0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
									}
									
									dtcCoreCaptureGlobalSnapshot(ch,i);
									dtcCoreStoreSnapshotToNvm(ch,i);
								}
								needUpdateNvm = TRUE;

								//gDtcVmInforArray[ch][i].preResetReason = DTC_RESET_BY_INIT;
							}
						}

					}
				}
				//diagMutexUnlock(gDtcMutex1[ch]);
				if(needUpdateNvm == TRUE)
					dtcCoreUpdateInforInNvm(ch,i);
				needUpdateNvm = FALSE;
			}
		diagMutexUnlock(gDtcMutex1[ch]);
		}
		
		
		

	}
	


	gIgnStatusSwitchFlag = FALSE;
	gSysVolStatusIsSwitched = FALSE;

}

boolean dtcCoreSetDtcStatus(uint8 ch,uint16 i,tDtcPassOrActive passiveOrActive, uint8*localSnapshot, uint16 len)
{
	if(ch >= gDtcCoreCfgInfor.udsChMaxNum)
		return FALSE;
	
	//diagMutexLock(gDtcMutex2);
	if(gDtcVmInforArray[ch][i].passiveOrActive != passiveOrActive)
	{
		gDtcVmInforArray[ch][i].passiveOrActive = passiveOrActive;
		if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_1 && gDtcMixInforArray[ch][i].traceLogEnableFlag == TRUE)
			DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,dtc[%d] update:%d\r\n",ch,i,passiveOrActive);
		if(passiveOrActive == DTC_PRE_ACTIVE )
		{
			gDtcStatusAndExtInforArray[ch][i].extData.agingCounter = 0;
			
			if(localSnapshot)
				dtcCoreCaptureLocalSnapshot(ch,i,localSnapshot,len);
		}
	}
	//diagMutexUnlock(gDtcMutex2);
	return TRUE;
}

tUdsNRCType dtcCoreReadDtcInfor(const tUdsReqInfor*reqInfor)
{
	uint16 i= 0;
	static uint32 dtcId = 0;
	uint8 extendedRecordNum = 0;
	tReportDTCInforType reportType;
	uint8 statusMask;
	uint16 count = 0;
	boolean gotFlag = FALSE;
	uint8	localBuffer[32];
	uint8 snapshotRecordNum = 0;
	tDtcDataBuffer *dataBuffer=NULL;
	uint32 ch = reqInfor->ch;
	uint8 *data = reqInfor->data;
	uint16 len = reqInfor->len;
	uint16 externalDataLen = 0;
	uint8 *externalDataPtr = NULL;
	tUdsNRCType ret = NRC_NONE;

	if(data == NULL || len<2 || len>32)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	diagResetBuffer(localBuffer,0,32);
	reportType = data[1];
	DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtcCore:ch:%d,0x19 0x%x\r\n",ch,reportType);
	switch(reportType)
	{
	case REPORT_NUM_OF_DTC_BY_STATUS_MASK:
		if(gDtcCoreCfgInfor.enable1901 == TRUE)
		{
			
			/*
			req:
			byte 0: 0x19
			byte 1: REPORT_NUM_OF_DTC_BY_STATUS_MASK
			byte 2: status mask
			*/
			if(len!=3)
			{
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				localBuffer[0] = data[1];
				localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;
				statusMask = data[2] & gDtcCoreCfgInfor.bitStatusMask;
				
				for(i=0;i<gDtcTotalNum[ch];i++)
				{
					if(gDtcVmInforArray[ch][i].enableDtc == TRUE && (gDtcStatusAndExtInforArray[ch][i].statusBit &statusMask)!=0)
					{
						count+=1 ;
					}
				}
				localBuffer[2] = 0;//DTC format:14229_1 pg208
				localBuffer[3] = (uint8)((count&0xFF00)>>8);
				localBuffer[4] = (uint8)(count&0xFF);
				dtcCoreFillTxBuffer(reqInfor,localBuffer,5);
			}
		}
		else
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		break;
	case REPORT_DTC_BY_STATUS_MASK:
		if(gDtcCoreCfgInfor.enable1902 == TRUE)
		{
			/*
			req:
			byte 0: 0x19
			byte 1: REPORT_DTC_BY_STATUS_MASK
			byte 2: status mask
			*/
			if(len != 3)
			{
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				statusMask = data[2]&gDtcCoreCfgInfor.bitStatusMask;
				diagCpyData(localBuffer,data+1,len-1);
				localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;
				dtcCoreFillTxBuffer(reqInfor,localBuffer,2);

				for(i=0;i<gDtcTotalNum[ch];i++)
				{
					if(gDtcVmInforArray[ch][i].enableDtc == TRUE && (gDtcStatusAndExtInforArray[ch][i].statusBit &statusMask)!=0)
					{
						dtcId = gDtcVmInforArray[ch][i].dtc_id;

						localBuffer[0] = (uint8)((dtcId&0xFF0000)>>16);
						localBuffer[1] = (uint8)((dtcId&0xFF00)>>8);
						localBuffer[2] = (uint8)(dtcId&0xFF);
						localBuffer[3] = gDtcStatusAndExtInforArray[ch][i].statusBit & gDtcCoreCfgInfor.bitStatusMask;
						dtcCoreFillTxBuffer(reqInfor,localBuffer,4);
					}
				}
				externalDataLen = dtcCoreGetExternalReport(reqInfor,&externalDataPtr);
				if(externalDataLen>0 && externalDataPtr!= NULL)
					dtcCoreFillTxBuffer(reqInfor,externalDataPtr,externalDataLen);
			}
		}
		else
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		break;
	#if 1
	case REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUM:
		if(gDtcCoreCfgInfor.enable1904 == TRUE)
		{
			/*
			req:
			byte 0: 0x19
			byte 1: REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUM
			byte 2~byte 4: DTC Num
			byte 5: DTC SnapshotRecordNumber.
			*/
			if(len != 6)
			{
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				diagCpyData(localBuffer,&data[1],4);
				
				dtcId = (uint32)(data[2]<<16 | data[3]<<8 | data[4]);
				snapshotRecordNum = data[5];
				if(dtcCoreCheckSnapshotRecordNum(ch,snapshotRecordNum) == FALSE)
				{
					ret = NRC_REQUEST_OUT_OF_RANGE;
				}
				else
				{
					for(i=0;i<gDtcTotalNum[ch];i++)
					{
						if(gDtcVmInforArray[ch][i].enableDtc == TRUE && (gDtcVmInforArray[ch][i].dtc_id==dtcId))
						{
							gotFlag = TRUE;
							DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"ch:%d,snapshot:id:%d,status0x%x\r\n",ch,i,gDtcStatusAndExtInforArray[ch][i].statusBit);
							localBuffer[4] = gDtcStatusAndExtInforArray[ch][i].statusBit & gDtcCoreCfgInfor.bitStatusMask;

							if(gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_CONFIRMED)
							{
								if((dataBuffer=dtcCoreGetSnapshot(ch,i,snapshotRecordNum))==NULL)
								{
									ret = NRC_REQUEST_OUT_OF_RANGE;
								}
								else
								{
									dtcCoreFillTxBuffer(reqInfor,localBuffer,5);
									dtcCoreFillTxBuffer(reqInfor,dataBuffer->buffer,dataBuffer->len);
								}
							}
							else// if DTC is not confirmed, return empty snapshot record datas
							{
								dtcCoreFillTxBuffer(reqInfor,localBuffer,5);
							}
							break;
						}
					}

					if(gotFlag == FALSE)
					{
						ret = NRC_REQUEST_OUT_OF_RANGE;
					}

				}

			}
		}
		else
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		break;
		
	case REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUM:
		if(gDtcCoreCfgInfor.enable1906 == TRUE)
		{
			/*
			req:
			byte 0: 0x19
			byte 1: REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUM
			byte 2~byte 4: DTC Num
			byte 5: extended data recored num 
			*/
			if(len != 6)
			{
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				diagCpyData(localBuffer,data+1,4);
				
				extendedRecordNum = data[5];
				dtcId = (uint32)(data[2]<<16 | data[3]<<8 | data[4]);

				if(dtcCoreCheckExtDataRecordNum(extendedRecordNum) == TRUE)
				{			
					for(i=0;i<gDtcTotalNum[ch];i++)
					{
						if(gDtcVmInforArray[ch][i].enableDtc == TRUE && gDtcVmInforArray[ch][i].dtc_id == dtcId)
						{
							gotFlag = TRUE;
							localBuffer[4] = gDtcStatusAndExtInforArray[ch][i].statusBit & gDtcCoreCfgInfor.bitStatusMask;

							if(gDtcStatusAndExtInforArray[ch][i].statusBit & DTC_STATUS_BIT_CONFIRMED)
							{
								if((dataBuffer = dtcCoreGetExtendDataRecord(ch,i,extendedRecordNum))==NULL)
								{
									ret = NRC_REQUEST_OUT_OF_RANGE;
								}
								else
								{
									dtcCoreFillTxBuffer(reqInfor,localBuffer,5);
								}
							}
							else// if DTC is not confirmed, return empty extended data record
							{
								dtcCoreFillTxBuffer(reqInfor,localBuffer,5);
							}
							break;
						}
					}

					if(gotFlag == FALSE)
					{
						ret = NRC_REQUEST_OUT_OF_RANGE;
					}
				}
				else
				{
					ret = NRC_REQUEST_OUT_OF_RANGE;
				}
			}
		}
		else
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		break;
	
	case REPORT_SUPPORTED_DTC:
		if(gDtcCoreCfgInfor.enable190a == TRUE)
		{
			/*
			req:
			byte 0: 0x19
			byte 1: REPORT_SUPPORTED_DTC
			*/
			if(len!=2)
			{
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				localBuffer[0] = data[1];
				localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;
				dtcCoreFillTxBuffer(reqInfor,localBuffer,2);

				for(i=0;i<gDtcTotalNum[ch];i++)
				{
					if(gDtcVmInforArray[ch][i].enableDtc == TRUE)
					{
						dtcId = gDtcVmInforArray[ch][i].dtc_id;
						localBuffer[0] = (uint8)((dtcId&0xFF0000)>>16);//DTC High Byte
						localBuffer[1] = (uint8)((dtcId&0xFF00)>>8);//DTC Middle Byte
						localBuffer[2] = (uint8)((dtcId&0xFF));//DTC Low Byte
						localBuffer[3] = gDtcStatusAndExtInforArray[ch][i].statusBit & gDtcCoreCfgInfor.bitStatusMask;
						dtcCoreFillTxBuffer(reqInfor,localBuffer,4);
					}
				}

			}
		}
		else
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		break;
	#endif
	default:
		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}


boolean dtcCoreFillTxBuffer(const tUdsReqInfor*reqInfor,uint8 *respData, uint16 len)
{
	#if 0
	tUdsRespInfor respInfor;
	
	respInfor.ch =reqInfor->ch;
	respInfor.finishFlag = finishFlag;
	respInfor.respCode = respCode;
	respInfor.data = respData;
	respInfor.len = len;
	respInfor.recordTick = reqInfor->reqTick;
	return diagSidResp(&respInfor);
	#else
	uint8 *respBuffPtr = &reqInfor->data[1];
	uint8 ch = reqInfor->ch;
	
	diagCpyData(respBuffPtr+gRespDataLen[ch], respData, len);
	gRespDataLen[ch] += len;

	return TRUE;
	#endif
}

void dtcCoreUpdateInforInNvm(uint8 ch,uint8 innerDtcId)
{	
	#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
	uint8 i = 0;
	if(gDtcStatusAndExtInforArray[ch][innerDtcId].storedInNvmFlag == TRUE) // this dtc snapshot had been in the Nvm,just update it
	{
		for(i=0;i<gDtcTotalNumStoredInNvmArray[ch];i++)
		{
			if(gDtcStoredInforArray[ch][i].innerDtcId == innerDtcId)
			{
				dtcCoreCpyFreezeFrameFromRamToNvm(ch,i,innerDtcId);
				diagSetNvmBlockId(gNvmBlockIdListForDtcStoredInforArray[ch][i],STD_ON);
				break;
			}
		}
	}
	#endif
}


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM

void dtcCoreCleanStoredSnapshot(uint8 ch,uint8 innerDtcId)
{
	uint8 i = 0;
	
	for(i=0;i<gDtcTotalNumStoredInNvmArray[ch];i++)
	{
		if(gDtcStoredInforArray[ch][i].innerDtcId == innerDtcId)
		{
			diagResetBuffer((uint8*)&gDtcStoredInforArray[ch][i],0,sizeof(tDtcStoredInfor));
			gDtcStoredInforArray[ch][i].innerDtcId = DTC_NVM_SPACE_IS_EMPTY;
			gDtcStoredInforArray[ch][i].dtcStatusAndExtInfor.storedInNvmFlag = FALSE;
			gDtcAvailableSpaceNumInNvmArray[ch]++;
			diagSetNvmBlockId(gNvmBlockIdListForDtcStoredInforArray[ch][i],STD_ON);
			break;
		}
	}
}
/*
void dtcCoreCpyFreezeFrameFromRamToNvm(uint8 nvmIndex,uint8 ramIndex)
{
	diagCpyData((uint8*)&gDtcStoredInforArray[nvmIndex].dtcStatusAndExtInfor,(uint8*)&gDtcStatusAndExtInforArray[ramIndex],sizeof(tDtcStatusAndExtInfor));
	diagCpyData((uint8*)&gDtcStoredInforArray[nvmIndex].snapshot,(uint8*)&gRuntimeSnapshotArray[ramIndex],sizeof(tDtcSnapshot));
}
*/
boolean dtcCoreStoreSnapshotToNvm(uint8 ch,uint8 innerDtcId)
{
	uint8 i = 0;
	boolean ret = FALSE;
	uint8 priority = 0;
	uint8 minPriority = 0;
	uint8 oldestAgingCounterIndex = 0;
	uint8 index = 0;
	tDtcDataBuffer dtcIdIndexList;
	uint8 j = 0;
	uint8 nvmBlockId = 0;
		
	if(gDtcStatusAndExtInforArray[ch][innerDtcId].storedInNvmFlag == TRUE) // this dtc snapshot had been in the Nvm,just update it
	{
		for(i=0;i<gDtcTotalNumStoredInNvmArray[ch];i++)
		{
			if(gDtcStoredInforArray[ch][i].innerDtcId == innerDtcId)
			{
				dtcCoreCpyFreezeFrameFromRamToNvm(ch,i,innerDtcId);
				nvmBlockId = gNvmBlockIdListForDtcStoredInforArray[ch][i];
				ret = TRUE;
				break;
			}
		}
	}
	else
	{
		if(gDtcAvailableSpaceNumInNvmArray[ch] == 0) // no space now
		{
			//check dtc priority
			priority = gDtcStatusAndExtInforArray[ch][innerDtcId].extData.priority;
			minPriority = gDtcStatusAndExtInforArray[ch][innerDtcId].extData.priority;
			dtcIdIndexList.len = 0;
			for(i=0;i<gDtcTotalNumStoredInNvmArray[ch];i++)
			{
				
				if(gDtcStoredInforArray[ch][i].dtcStatusAndExtInfor.extData.priority>minPriority )//the value bigger,the priority lower
				{
					minPriority = gDtcStoredInforArray[ch][i].dtcStatusAndExtInfor.extData.priority;
					dtcIdIndexList.len = 1;
					dtcIdIndexList.buffer[0] = i;
					j = 0;
				}
				else if(gDtcStoredInforArray[ch][i].dtcStatusAndExtInfor.extData.priority == minPriority && (priority!=minPriority))
				{
					dtcIdIndexList.len += 1;
					dtcIdIndexList.buffer[j++] = i;
				}
					
			}

			if(dtcIdIndexList.len>0)
			{
				//check agingCounter;
				 oldestAgingCounterIndex = dtcIdIndexList.buffer[0];
				for(i=0;i<dtcIdIndexList.len-1;i++)
				{
					if(gDtcStoredInforArray[ch][oldestAgingCounterIndex].dtcStatusAndExtInfor.extData.agingCounter <gDtcStoredInforArray[ch][dtcIdIndexList.buffer[i+1]].dtcStatusAndExtInfor.extData.agingCounter)
						oldestAgingCounterIndex = dtcIdIndexList.buffer[i+1];
				}
				gDtcStatusAndExtInforArray[ch][gDtcStoredInforArray[ch][oldestAgingCounterIndex].innerDtcId].storedInNvmFlag = FALSE;//the old one will be mark as NotStoredInNvm
				gDtcStatusAndExtInforArray[ch][innerDtcId].storedInNvmFlag = TRUE;// the new one will be mark as StoredInNvm
				dtcCoreCpyFreezeFrameFromRamToNvm(ch,oldestAgingCounterIndex,innerDtcId);
				
				nvmBlockId = gNvmBlockIdListForDtcStoredInforArray[ch][oldestAgingCounterIndex];
				ret = TRUE;
			}
		}
		else//find the available space index
		{
			for(i=0;i<gDtcTotalNumStoredInNvmArray[ch];i++)
			{
				if(gDtcStoredInforArray[ch][i].innerDtcId == DTC_NVM_SPACE_IS_EMPTY)//this space is availabe
				{
					gDtcStoredInforArray[ch][i].innerDtcId = innerDtcId;
					gDtcStatusAndExtInforArray[ch][innerDtcId].storedInNvmFlag = TRUE;
					dtcCoreCpyFreezeFrameFromRamToNvm(ch,i,innerDtcId);
					nvmBlockId = gNvmBlockIdListForDtcStoredInforArray[ch][i];
					gDtcAvailableSpaceNumInNvmArray[ch]--;
					ret = TRUE;
					break;
				}

			}
		}
	}

	if(ret == TRUE)
	{
		diagSetNvmBlockId(nvmBlockId,STD_ON);
	}

	return ret;
}

#else
boolean dtcCoreStoreSnapshotToNvm(uint8 ch,uint8 innerDtcId)
{
	gDtcStatusAndExtInforArray[ch][innerDtcId].storedInNvmFlag = TRUE;
	diagSetNvmBlockId(gNvmBlockIdListForRuntimeSnapshotArray[ch][innerDtcId],STD_ON);
	diagSetNvmBlockId(gNvmBlockIdListForDtcExtInforArray[ch][innerDtcId],STD_ON);
	return TRUE;
}

#endif

void dtcCoreSetMonitorFlag(boolean flag)
{
	gDtcMonitorStatus = flag;
}

boolean dtcCoreGetMonitorFlag()
{
	return gDtcMonitorStatus;
}
