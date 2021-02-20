/**************************
description: DTC Core
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/08
**************************/
#include"dtcCore_if.h"
#include"sid0x14_if.h"
#include"uds_sid_if.h"

static void dtcCoreResetStatusBitByPowerOn();
static void dtcCoreResetDTCInfor(uint8 i, tDTCResetReason reason);
static boolean dtcCoreTxData(boolean finishFlag,tUdsNRCType respCode,const uint8 *respData, uint16 len);
static void dtcCoreUpdateInforInNvm(uint8 innerDtcId);


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
static void dtcCoreCleanStoredSnapshot(uint8 innerDtcId);
static void dtcCoreCpyFreezeFrameFromRamToNvm(uint8 nvmIndex,uint8 ramIndex);
#endif

extern const tDtcCoreCfgInfor gDtcCoreCfgInfor;
extern const tDtcMixInfor gDtcMixInfor[];
extern tDtcVmInfor gDtcVmInforList[];
extern tDtcStatusAndExtInfor gDtcStatusAndExtInforArray[];
extern uint16 gDtcTotalNum;

#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
extern tDtcStoredInfor gDtcStoredInfor[];
extern tDtcSnapshot gRuntimeSnapshotList[];
extern const uint8 gNvmBlockIdListForDtcStoredInfor[];
extern uint16 gAvailableDtcSpaceNumInNvm;

#else
extern const uint8 gNvmBlockIdListForRuntimeSnapshot[]; // map to gRuntimeSnapshotList
extern const uint8 gNvmBlockIdListForDtcExtInfor[]; // map to gDtcStatusAndExtInforArray

#endif
static boolean gDtcCore_IgnOnFlag = FALSE;
static boolean gDtcCore_IgnStatusSwitchFlag = FALSE;
static boolean gDtcCore_SysVoltSwitchFlag = FALSE;

//uint8 gKL30Status = DIAG_KL30_STOP_CRANK;
//uint8 gAccStatus = DIAG_ACC_OFF;
uint8 gDtcAppTaskIdList[DTC_APP_TASKID_LIST_LEN] = {0};


static boolean gEnableUpdateDtcBitStatusFlag = FALSE;
//static tDiagMutex gDtcMutex1 = 0; 

static tDiagMutex gDtcMutex1 = 0;
static tDiagMutex gDtcMutex2 = 0;
static boolean gDtcInitDoneFlag = FALSE;

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

void dtcCoreChangeUpdateDtcStatusFlag(boolean enableOrDisable)
{
	gEnableUpdateDtcBitStatusFlag = enableOrDisable;
}


void dtcCoreResetStatusBitByPowerOn()
{
	uint16 i = 0;
	boolean needUpdateNvm = FALSE;
	
	for(i=0;i<gDtcTotalNum;i++)
	{
		if(gDtcVmInforList[i].enableDtc == TRUE)
		{
			if(gDtcCoreCfgInfor.logLevel >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:status:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
			if(gDtcVmInforList[i].unlearningByAgingCounterFlag== TRUE)// && gDtcMixInfor[i].operCycType == DTC_OPERATION_CLCLIC_BY_POWER_ON)
			{
				//if in last operation cycle,there was no error, then update agingCounter
				if(((gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_CONFIRMED)!=0)
					&& ((gDtcStatusAndExtInforArray[i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE
					|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0))
				{
					gDtcStatusAndExtInforArray[i].extData.agingCounter += 1;
					needUpdateNvm = TRUE;
					
					if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
						DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"power on,dtc[%d],agingCounter++:%d\r\n",i,gDtcStatusAndExtInforArray[i].extData.agingCounter);
				}
			}
			//if(gDtcVmInforList[i].unlearningByAgingCounterFlag== TRUE)
			{
				// iso14229-2013,PG373
				//in last operation cycle,if there was no testFailed or testNotCompleted,then reset pendingDTC bit to 0
				if((gDtcStatusAndExtInforArray[i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0)
				{
					gDtcStatusAndExtInforArray[i].statusBit &= ~DTC_STATUS_BIT_PENDING;
					needUpdateNvm = TRUE;
				}
				
				if(gDtcStatusAndExtInforArray[i].extData.agingCounter >= DTC_AGING_COUNTER_LIMIT)
				{
					dtcCoreResetDTCInfor(i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
				}

				gDtcStatusAndExtInforArray[i].statusBit |= DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
				gDtcStatusAndExtInforArray[i].statusBit &= ~(DTC_STATUS_BIT_TEST_FAILED | DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE /*|DTC_STATUS_BIT_TEST_FAILED_SINCE_LAST_CLEAR*/);
				
			}
			
			if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:status:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
		}

		if(needUpdateNvm == TRUE)
			dtcCoreUpdateInforInNvm(i);
		needUpdateNvm = FALSE;
	}
}

void dtcCoreInit()
{

	if(gDtcInitDoneFlag == FALSE)
	{
		dtcCorePartInit();
		sid0x14Init();

		gDtcMutex1 = diagCreatMutex();
		gDtcMutex2 = diagCreatMutex();

		if(gDtcMutex1 == 0 || gDtcMutex2 == 0)
			return;
		
		diagResetBuffer(gDtcAppTaskIdList,0,DTC_APP_TASKID_LIST_LEN);

		dtcCoreResetStatusBitByPowerOn();
		
		gDtcCore_IgnOnFlag = FALSE;
		gDtcCore_IgnStatusSwitchFlag = FALSE;

		gEnableUpdateDtcBitStatusFlag = TRUE;

		//1TODO:
		//diagSetNvmBlockId(gDtcNvmInforList_NvMBlockID,STD_ON);

		gDtcInitDoneFlag = TRUE;
	}
}


boolean dtcCoreGetInitStatus()
{
	return gDtcInitDoneFlag;
}

tUdsNRCType dtcCoreClearDtc(const uint8 *data, uint16 len)
{

	uint16 i= 0;
	uint32 dtcGroup = 0;
	tUdsNRCType ret = NRC_NONE;
	uint8 restartDtcDetectReq[4] = {DIAG_CMD_RESTART_DETECTING_DTC_REQ,0xFF,0xFF,0xff};
	boolean validDTCGroup = FALSE;
	
	if(data==NULL || len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(gDtcInitDoneFlag == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	
	if(sid0x14CheckPreCondition()!=NRC_NONE)
		return NRC_CONDITION_NOT_CORRECT;
	
	dtcGroup = (uint32)(data[0]<<16 | data[1] <<8 | data[2]);
	
	if(dtcGroup == DTC_GROUP_ALL)//ALL dtc
	{
		validDTCGroup = TRUE;
		for(i=0;i<gDtcTotalNum;i++)
		{
			if(gDtcVmInforList[i].enableDtc == TRUE)
			{
				diagMutexLock(gDtcMutex1);
				dtcCoreResetDTCInfor(i,DTC_RESET_BY_SID_0X14);
				diagMutexUnlock(gDtcMutex1);
			}
		}

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
			
			for(i=0;i<gDtcTotalNum;i++)
			{
				if((gDtcMixInfor[i].dtcGroup == dtcGroup)&& (gDtcVmInforList[i].enableDtc == TRUE))
				{
					diagMutexLock(gDtcMutex1);
					dtcCoreResetDTCInfor(i,DTC_RESET_BY_SID_0X14);
					diagMutexUnlock(gDtcMutex1);
					DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d],del dtc group:0x%x\r\n",i,dtcGroup);
					validDTCGroup = TRUE;
				}
			}
			break;
		default:
			// specify single DTC
			for(i=0;i<gDtcTotalNum;i++)
			{
				if((gDtcVmInforList[i].dtc_id == dtcGroup)&& (gDtcVmInforList[i].enableDtc == TRUE))
				{
					diagMutexLock(gDtcMutex1);
					dtcCoreResetDTCInfor(i,DTC_RESET_BY_SID_0X14);
					diagMutexUnlock(gDtcMutex1);
					DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d],del dtc group:0x%x\r\n",i,dtcGroup);
					validDTCGroup = TRUE;
					break;
				}
			}
		}
	}

	if(validDTCGroup == TRUE)
	{
		restartDtcDetectReq[1] = data[0];
		restartDtcDetectReq[2] = data[1];
		restartDtcDetectReq[3] = data[2];

		for(i=0;i<DTC_APP_TASKID_LIST_LEN;i++)
		{
			if(gDtcAppTaskIdList[i] != 0)
			{
				udsSendMsg(gDtcAppTaskIdList[i],restartDtcDetectReq,4);
			}
		}
	}
	else
	{
		ret = NRC_REQUEST_OUT_OF_RANGE;
	}
	return ret;
}



void dtcCoreResetDTCInfor(uint8 i, tDTCResetReason reason)
{
	//diagMutexLock(gDtcMutex1);
	gDtcVmInforList[i].preResetReason = reason;
	if(reason == DTC_RESET_BY_SID_0X14)
		gDtcStatusAndExtInforArray[i].statusBit = DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
	else if (reason == DTC_RESET_BY_NOT_CODED)
		gDtcStatusAndExtInforArray[i].statusBit = DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
	else if(reason == DTC_RESET_BY_AGING_COUNTER_EXCEED)
		gDtcStatusAndExtInforArray[i].statusBit = DTC_STATUS_BIT_NONE;
	
	//PG29,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf
	//When a DTC is cleared via the DTC aging mechanism, the same parameters except for the
	//aged counter will be cleared. The aged counter is retained in ECU long term memory to allow
	//the rectifier to determine if that particular fault was previously deleted due to the ageing mechanism.
	if(reason == DTC_RESET_BY_AGING_COUNTER_EXCEED)
	{
		if(gDtcVmInforList[i].unlearningByAgingCounterFlag == TRUE)
		{
			if(gDtcStatusAndExtInforArray[i].extData.agedCounter <254)
				gDtcStatusAndExtInforArray[i].extData.agedCounter += 1; // 255 means invalid value or DTC can't be deleted autonomously
		}
		else
			gDtcStatusAndExtInforArray[i].extData.agedCounter = 255;
	}
	else// if(reason == DTC_RESET_BY_SID_0X14 || reason==DTC_RESET_BY_NOT_CODED)
		gDtcStatusAndExtInforArray[i].extData.agedCounter = 0;
	gDtcStatusAndExtInforArray[i].extData.agingCounter = 0;
	gDtcStatusAndExtInforArray[i].extData.occCounter = 0;

	dtcCoreResetSnapshot(i);
#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
	if(gDtcStatusAndExtInforArray[i].storedInNvmFlag == TRUE)
	{
		dtcCoreCleanStoredSnapshot(i);
		gDtcStatusAndExtInforArray[i].storedInNvmFlag = FALSE;
	}
#endif
	//diagMutexLock(gDtcMutex2);
	gDtcVmInforList[i].passiveOrActive = DTC_PRE_INIT;
	//diagMutexUnlock(gDtcMutex2);
	
	gDtcVmInforList[i].curTimer.type = DTC_TIMER_NONE;
	gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
	//gDtcVmInforList[i].curTimer.tick = 0;
	gDtcVmInforList[i].curTimer.preTick = 0;

	dtcCoreStoreSnapshotToNvm(i);

	//diagMutexUnlock(gDtcMutex1);

}


void dtcCoreMainLoop()
{
	uint16 i= 0;
	uint32 curTick = 0;
	uint32 tickInterval = 0;
	uint8 delayDtcDetectReq[4] = {DIAG_CMD_DELAY_DETECT_DTC_REQ,0x00,0x00,0x00};
	boolean needUpdateNvm = FALSE;
	boolean IsCurSysVoltOkFlag = TRUE;
	uint8 curIgnStatus = 0;
	static uint8 sPreIgnStatus = 0xFF;
	static boolean isStableSysVoltOkFlag = 0xFF;

	if(gDtcInitDoneFlag == FALSE)
		return;

	if(diagGetCurTick()<300)
		return;

	diagGetKL15Status(&curIgnStatus);

	IsCurSysVoltOkFlag = diagCheckIsSysVoltOK();

	if(IsCurSysVoltOkFlag != isStableSysVoltOkFlag)
	{
		isStableSysVoltOkFlag = IsCurSysVoltOkFlag;
		gDtcCore_SysVoltSwitchFlag = TRUE;
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"[dtcCore]sysVoltStatus:%d\r\n",isStableSysVoltOkFlag);
	}


	if(curIgnStatus != sPreIgnStatus)
	{
		gDtcCore_IgnStatusSwitchFlag = TRUE;
		sPreIgnStatus = curIgnStatus;
		gDtcCore_IgnOnFlag = (sPreIgnStatus == DIAG_KL15_ON)?TRUE:FALSE;
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_WARNING,"[dtcCore]IGN: %s\r\n",((sPreIgnStatus == DIAG_KL15_ON)?"ON":"OFF"));

		#if 0
		if(gDtcCore_IgnStatusSwitchFlag == TRUE)
		{
			// inform app layer to delay 1200ms for some DTCs
			for(i=0;i<DTC_APP_TASKID_LIST_LEN;i++)
			{
				if(gDtcAppTaskIdList[i] != 0)
				{
					udsSendMsg(gDtcAppTaskIdList[i],delayDtcDetectReq,4);
				}
			}
		}
		#endif
	}	


	if(gEnableUpdateDtcBitStatusFlag == TRUE)
	{	
		diagMutexLock(gDtcMutex1);
		
		for(i=0;i<gDtcTotalNum;i++)
		{
			//diagMutexLock(gDtcMutex1);
			if(gDtcVmInforList[i].enableDtc == TRUE)
			{

				if(gDtcMixInfor[i].operCycType == DTC_OPERATION_CLCLIC_BY_IGN_ON)
				{
					
					gDtcVmInforList[i].continueToDetectFlag = gDtcCore_IgnOnFlag;
					if(gDtcMixInfor[i].isStoppedByHiLowSysVoltage == TRUE)
					{
						if(isStableSysVoltOkFlag == FALSE)
							gDtcVmInforList[i].continueToDetectFlag = FALSE;
						else
							gDtcVmInforList[i].continueToDetectFlag &= TRUE;

						//if(gDtcCore_SysVoltSwitchFlag == TRUE)
							//DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_WARNING,"dtc[%d] ign %d,detectFlag:%d\r\n",i,gDtcCore_IgnOnFlag,gDtcVmInforList[i].continueToDetectFlag);
					}

					if(gDtcCore_IgnStatusSwitchFlag == TRUE)
					{
						if(gDtcCore_IgnOnFlag == TRUE)
						{
							if(gDtcVmInforList[i].unlearningByAgingCounterFlag== TRUE)
							{
	
								//if(gDtcLogLevel >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"ign on,dtc[%d],status:%d\r\n",i,gDtcNvmInforList[i].statusBit);
								
								//if in last operation cycle,there was no error, then update agingCounter
								if(((gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_CONFIRMED)!=0)
									&& ((gDtcStatusAndExtInforArray[i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE
									|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0))
								{
									gDtcStatusAndExtInforArray[i].extData.agingCounter += 1;
									
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"IGN On,dtc[%d],agingCounter++:%d\r\n",i,gDtcStatusAndExtInforArray[i].extData.agingCounter);
									if(gDtcStatusAndExtInforArray[i].extData.agingCounter > DTC_AGING_COUNTER_LIMIT)
									{
										dtcCoreResetDTCInfor(i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
									}										
								}
							}
						
							if((gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_PENDING)!=0)
							{
								if((gDtcStatusAndExtInforArray[i].statusBit & (DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)) == 0)
								{
									gDtcStatusAndExtInforArray[i].statusBit &= ~DTC_STATUS_BIT_PENDING;
								}
							}
							gDtcStatusAndExtInforArray[i].statusBit |= DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE;
							gDtcStatusAndExtInforArray[i].statusBit &= ~(DTC_STATUS_BIT_TEST_FAILED | DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE );
							gDtcVmInforList[i].curTimer.type = DTC_TIMER_NONE;
							gDtcVmInforList[i].curTimer.preTick = diagGetCurTick();
							gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
							if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
								DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d],S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
							needUpdateNvm = TRUE;
						}
						
					}

				}
				else if(gDtcCore_SysVoltSwitchFlag == TRUE && gDtcMixInfor[i].isStoppedByHiLowSysVoltage == TRUE)
				{
					gDtcVmInforList[i].continueToDetectFlag = isStableSysVoltOkFlag;
					//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_WARNING,"dtc[%d] sysVolt change:%d\r\n",i,gDtcVmInforList[i].continueToDetectFlag);
				}

				if(gDtcVmInforList[i].continueToDetectFlag == FALSE)
					gDtcStatusAndExtInforArray[i].statusBit &= ~DTC_STATUS_BIT_TEST_FAILED;

				if(gDtcVmInforList[i].continueToDetectFlag == TRUE && (gDtcVmInforList[i].passiveOrActive != DTC_PRE_INIT))
				{

					
					if(gDtcStatusAndExtInforArray[i].statusBit & (DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE))
					{
						needUpdateNvm = TRUE;
						gDtcStatusAndExtInforArray[i].statusBit &= ~(DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR
														|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE);
					}
					
					if(gDtcVmInforList[i].unlearningByAgingCounterFlag == TRUE)
					{	
							
							if((gDtcVmInforList[i].passiveOrActive == DTC_PRE_PASSIVE) && gDtcVmInforList[i].curTimer.type != DTC_TIMER_DISQUALIFY)
							{
								gDtcVmInforList[i].curTimer.type = DTC_TIMER_DISQUALIFY;
								//gDtcVmInforList[i].curTimer.preTick = gDtcVmInforList[i].disqual_timer;
								gDtcVmInforList[i].curTimer.preTick = diagGetCurTick();
								gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:NA,S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
							}
							else if ((gDtcVmInforList[i].passiveOrActive == DTC_PRE_ACTIVE) && gDtcVmInforList[i].curTimer.type != DTC_TIMER_QUALIFY)
							{
								gDtcVmInforList[i].curTimer.type = DTC_TIMER_QUALIFY;
								//gDtcVmInforList[i].curTimer.tick = gDtcVmInforList[i].qual_timer;
								gDtcVmInforList[i].curTimer.preTick = diagGetCurTick();
								gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
								
								
								//pg28,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf:
								//A: It will be reset to “0” if there is any failed test result during an ignition cycle.
								//B: If the DTC can’t be deleted autonomously, ECU must save the aging counter status 255dec in the error memory.
								gDtcStatusAndExtInforArray[i].extData.agingCounter = 0;
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"dtc[%d]:agingCounter = 0\r\n",i);
								//华晨M8需求: Pg28,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf:
								//Fault occurrence counter will be incremented by 1 
								//when the “testFailedThisOperationCycle” bit is set to '1' from '0'. 
								if((gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE) == 0)
								{
									if(gDtcStatusAndExtInforArray[i].extData.occCounter<254)//the max value is 254
									{
										gDtcStatusAndExtInforArray[i].extData.occCounter += 1;
										if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
											DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:occCounter=%d\r\n",i,gDtcStatusAndExtInforArray[i].extData.occCounter);
									}
								}

								needUpdateNvm = TRUE;
								gDtcStatusAndExtInforArray[i].statusBit |= (DTC_STATUS_BIT_TEST_FAILED
																|DTC_STATUS_BIT_PENDING
																|DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE);
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:A,S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);		
							}
							

					}
					else
					{
							if((gDtcVmInforList[i].passiveOrActive == DTC_PRE_PASSIVE) && gDtcVmInforList[i].curTimer.type != DTC_TIMER_DISQUALIFY)
							{
								dtcCoreResetDTCInfor(i,DTC_RESET_BY_AGING_COUNTER_EXCEED);
								gDtcVmInforList[i].curTimer.type = DTC_TIMER_DISQUALIFY;
								//gDtcVmInforList[i].curTimer.tick = gDtcVmInforList[i].disqual_timer;
								gDtcVmInforList[i].curTimer.preTick = diagGetCurTick();
								gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
								
								gDtcStatusAndExtInforArray[i].statusBit = DTC_STATUS_BIT_NONE;
							}
							else if((gDtcVmInforList[i].passiveOrActive == DTC_PRE_ACTIVE) && gDtcVmInforList[i].curTimer.type != DTC_TIMER_QUALIFY)
							{
								gDtcStatusAndExtInforArray[i].statusBit = (DTC_STATUS_BIT_TEST_FAILED
																|DTC_STATUS_BIT_PENDING
																|DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE);

								gDtcVmInforList[i].curTimer.type = DTC_TIMER_QUALIFY;
								//gDtcVmInforList[i].curTimer.tick = gDtcVmInforList[i].qual_timer;
								gDtcVmInforList[i].curTimer.preTick = diagGetCurTick();;
								gDtcVmInforList[i].curTimer.timeoutFlag = FALSE;
								
							}
							
							needUpdateNvm = TRUE;							
					}

					
					if(gDtcVmInforList[i].curTimer.type != DTC_TIMER_NONE && gDtcVmInforList[i].curTimer.timeoutFlag == FALSE)
					{	
						tickInterval = (gDtcVmInforList[i].curTimer.type == DTC_TIMER_DISQUALIFY)?gDtcVmInforList[i].disqual_timer:gDtcVmInforList[i].qual_timer;
						curTick = diagGetCurTick();

						if((curTick-gDtcVmInforList[i].curTimer.preTick) >= tickInterval)
						{
							gDtcVmInforList[i].curTimer.timeoutFlag = TRUE;
							
							if(gDtcVmInforList[i].curTimer.type == DTC_TIMER_DISQUALIFY)
							{
								gDtcStatusAndExtInforArray[i].statusBit &= ~DTC_STATUS_BIT_TEST_FAILED;
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:Disqualify,S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
								
							}
							else
							{
								if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
									DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d]:Qualify,S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
								gDtcStatusAndExtInforArray[i].statusBit |= DTC_STATUS_BIT_CONFIRMED;
								if(gDtcVmInforList[i].preResetReason == DTC_RESET_BY_SID_0X14)
								{
									gDtcStatusAndExtInforArray[i].statusBit |= DTC_STATUS_BIT_TEST_FAILED_SINCE_LAST_CLEAR;
									if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
										DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d] failed again,S:0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
								}
								
								dtcCoreCaptureGlobalSnapshot(i);
								dtcCoreStoreSnapshotToNvm(i);
							}

							//gDtcVmInforList[i].preResetReason = DTC_RESET_BY_INIT;
						}
					}

				}
			}
			//diagMutexUnlock(gDtcMutex1);
			if(needUpdateNvm == TRUE)
				dtcCoreUpdateInforInNvm(i);
			needUpdateNvm = FALSE;
		}
		diagMutexUnlock(gDtcMutex1);

	}
	


	gDtcCore_IgnStatusSwitchFlag = FALSE;
	gDtcCore_SysVoltSwitchFlag = FALSE;

}

boolean dtcCoreSetDtcStatus(uint16 i,tDtcPassOrActive passiveOrActive, uint8*localSnapshot, uint16 len)
{
	boolean ret = FALSE;
	
	if(gDtcInitDoneFlag == TRUE)
	{
		//diagMutexLock(gDtcMutex2);
		if(gDtcVmInforList[i].passiveOrActive != passiveOrActive)
		{
			gDtcVmInforList[i].passiveOrActive = passiveOrActive;
			if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[i].traceLogEnableFlag == TRUE)
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"dtc[%d] update:%d\r\n",i,passiveOrActive);
			if(passiveOrActive == DTC_PRE_ACTIVE && localSnapshot)
				dtcCoreCaptureLocalSnapshot(i,localSnapshot,len);
		}
		//diagMutexUnlock(gDtcMutex2);
		ret = TRUE;
	}
	
	return ret;
}

tUdsNRCType dtcCoreReadDtcInfor(uint8 *data, uint16 len)
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
	const tDtcDataBuffer *dataBuffer=NULL;

	if(data == NULL || len<2 || len>32)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	#if 0
	if(diagCheckPrecondition(NULL) == FALSE)
		dtcCoreTxData(TRUE,NRC_CONDITION_NOT_CORRECT,NULL,0);
	#endif
	
	diagResetBuffer(localBuffer,0,32);
	reportType = data[1];
	DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"dtcCore:read dtc,report type:0x%x\r\n",reportType);

	if(gDtcInitDoneFlag == FALSE)
	{
		dtcCoreTxData(TRUE,NRC_CONDITION_NOT_CORRECT,NULL,0);
		return NRC_CONDITION_NOT_CORRECT;
	}
	
	switch(reportType)
	{
	case REPORT_NUM_OF_DTC_BY_STATUS_MASK:
		if(gDtcCoreCfgInfor.enable1901 == FALSE)
		{
			dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
		/*
		req:
		byte 0: 0x19
		byte 1: REPORT_NUM_OF_DTC_BY_STATUS_MASK
		byte 2: status mask
		*/
		if(len!=3)
			dtcCoreTxData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		else
		{
			localBuffer[0] = data[1];
			localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;
			statusMask = data[2] & gDtcCoreCfgInfor.bitStatusMask;
			
			for(i=0;i<gDtcTotalNum;i++)
			{
				if(gDtcVmInforList[i].enableDtc == TRUE && (gDtcStatusAndExtInforArray[i].statusBit &statusMask)!=0)
				{
					count+=1 ;
				}
			}
			localBuffer[2] = 0;//DTC format:华晨需求是设置成0,SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf,但是ISO-14229_1 pg208，是1
			localBuffer[3] = (uint8)((count&0xFF00)>>8);
			localBuffer[4] = (uint8)(count&0xFF);
			dtcCoreTxData(TRUE,NRC_NONE,localBuffer,5);
		}
		break;
	case REPORT_DTC_BY_STATUS_MASK:
		if(gDtcCoreCfgInfor.enable1902 == FALSE)
		{
			dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
		/*
		req:
		byte 0: 0x19
		byte 1: REPORT_DTC_BY_STATUS_MASK
		byte 2: status mask
		*/
		if(len != 3)
			dtcCoreTxData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		else
		{
			diagCpyData(localBuffer,data+1,len-1);
			localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;
			statusMask = data[2]&gDtcCoreCfgInfor.bitStatusMask;
			dtcCoreTxData(FALSE,NRC_NONE,localBuffer,2);
			
			for(i=0;i<gDtcTotalNum;i++)
			{
				if(gDtcVmInforList[i].enableDtc == TRUE && (gDtcStatusAndExtInforArray[i].statusBit &statusMask)!=0)
				{
					dtcId = gDtcVmInforList[i].dtc_id;

					localBuffer[0] = (uint8)((dtcId&0xFF0000)>>16);
					localBuffer[1] = (uint8)((dtcId&0xFF00)>>8);
					localBuffer[2] = (uint8)(dtcId&0xFF);
					localBuffer[3] = gDtcStatusAndExtInforArray[i].statusBit & gDtcCoreCfgInfor.bitStatusMask;
					TracePrintf(UDS_TRACE,TRACE_INFO,"DTC %x,%x,%x:%x\r\n",localBuffer[0],localBuffer[1],localBuffer[2],localBuffer[3]);
					dtcCoreTxData(FALSE,NRC_NONE,localBuffer,4);					
				}
			}
			
			TracePrintf(UDS_TRACE,TRACE_WARNING,"\r\n");

			//dtcCoreTxData(TRUE,NRC_NONE,data+1,2+4*count);
			dtcCoreTxData(TRUE,NRC_NONE,NULL,0);
		}
		break;
	#if 1
	case REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUM:
		if(gDtcCoreCfgInfor.enable1904 == FALSE)
		{
			dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
		/*
		req:
		byte 0: 0x19
		byte 1: REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUM
		byte 2~byte 4: DTC Num
		byte 5: DTC SnapshotRecordNumber. 在华晨M8项目中，该byte值为1
		*/
		if(len != 6)
			dtcCoreTxData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		else
		{
			diagCpyData(localBuffer,&data[1],4);
			
			dtcId = (uint32)(data[2]<<16 | data[3]<<8 | data[4]);
			snapshotRecordNum = data[5];
			if(dtcCoreCheckSnapshotRecordNum(snapshotRecordNum) == FALSE)
				dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
			else
			{
				for(i=0;i<gDtcTotalNum;i++)
				{
					if(gDtcVmInforList[i].enableDtc == TRUE && (gDtcVmInforList[i].dtc_id==dtcId))
					{
						gotFlag = TRUE;
						DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"snapshot:id:%d,status0x%x\r\n",i,gDtcStatusAndExtInforArray[i].statusBit);
						localBuffer[4] = gDtcStatusAndExtInforArray[i].statusBit & gDtcCoreCfgInfor.bitStatusMask;

						if(gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_CONFIRMED)
						{
							if((dataBuffer=dtcCoreGetSnapshot(i,snapshotRecordNum))==NULL)
								dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
							else
							{
								dtcCoreTxData(FALSE,NRC_NONE,localBuffer,5);
								dtcCoreTxData(TRUE,NRC_NONE,dataBuffer->buffer,dataBuffer->len);
							}
						}
						else// if DTC is not confirmed, return empty snapshot record datas
						{
							dtcCoreTxData(TRUE,NRC_NONE,localBuffer,5);
						}
						break;
					}
				}

				if(gotFlag == FALSE)
				{
					dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
				}

			}

		}
		break;
		
	case REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUM:
		if(gDtcCoreCfgInfor.enable1906 == FALSE)
		{
			dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
		/*
		req:
		byte 0: 0x19
		byte 1: REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUM
		byte 2~byte 4: DTC Num
		byte 5: extended data recored num // 华晨M8中设置为1
		*/
		if(len != 6)
			dtcCoreTxData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		else
		{
			diagCpyData(localBuffer,data+1,4);
			
			extendedRecordNum = data[5];
			dtcId = (uint32)(data[2]<<16 | data[3]<<8 | data[4]);

			if(dtcCoreCheckExtDataRecordNum(extendedRecordNum) == TRUE)
			{			
				for(i=0;i<gDtcTotalNum;i++)
				{
					if(gDtcVmInforList[i].enableDtc == TRUE && gDtcVmInforList[i].dtc_id == dtcId)
					{
						gotFlag = TRUE;
						localBuffer[4] = gDtcStatusAndExtInforArray[i].statusBit  & gDtcCoreCfgInfor.bitStatusMask;

						if(gDtcStatusAndExtInforArray[i].statusBit & DTC_STATUS_BIT_CONFIRMED)
						{
							if((dataBuffer = dtcCoreGetExtendDataRecord(i,extendedRecordNum))==NULL)
								dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
							else
							{
								dtcCoreTxData(FALSE,NRC_NONE,localBuffer,5);
								dtcCoreTxData(TRUE,NRC_NONE,dataBuffer->buffer,dataBuffer->len);
							}
						}
						else// if DTC is not confirmed, return empty extended data record
						{
							dtcCoreTxData(TRUE,NRC_NONE,localBuffer,5);
						}
						break;
					}
				}

				if(gotFlag == FALSE)
					dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
			}
			else
				dtcCoreTxData(TRUE,NRC_REQUEST_OUT_OF_RANGE,NULL,0);
		}
		break;
	
	case REPORT_SUPPORTED_DTC:
		if(gDtcCoreCfgInfor.enable190a == FALSE)
		{
			dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}
		/*
		req:
		byte 0: 0x19
		byte 1: REPORT_SUPPORTED_DTC
		*/
		if(len!=2)
			dtcCoreTxData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		else
		{
			localBuffer[0] = data[1];
			localBuffer[1] = gDtcCoreCfgInfor.bitStatusMask;;
			dtcCoreTxData(FALSE,NRC_NONE,&localBuffer[0],2);
			for(i=0;i<gDtcTotalNum;i++)
			{
				if(gDtcVmInforList[i].enableDtc == TRUE)
				{
					dtcId = gDtcVmInforList[i].dtc_id;
					localBuffer[0] = (uint8)((dtcId&0xFF0000)>>16);//DTC High Byte
					localBuffer[1] = (uint8)((dtcId&0xFF00)>>8);//DTC Middle Byte
					localBuffer[2] = (uint8)((dtcId&0xFF));//DTC Low Byte
					localBuffer[3] = gDtcStatusAndExtInforArray[i].statusBit  & gDtcCoreCfgInfor.bitStatusMask;
					dtcCoreTxData(FALSE,NRC_NONE,localBuffer,4);
				}
			}

			//dtcCoreTxData(TRUE,NRC_NONE,data+1,2+count*4);
			dtcCoreTxData(TRUE,NRC_NONE,NULL,0);
		}
		break;
	#endif
	default:
		dtcCoreTxData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return NRC_NONE;
}


boolean dtcCoreTxData(boolean finishFlag,tUdsNRCType respCode,const uint8 *respData, uint16 len)
{
	return diagSidResp(FALSE,finishFlag,respCode,respData,len);
}

void dtcCoreUpdateInforInNvm(uint8 innerDtcId)
{	
	#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
	uint8 nvmBlockId = 0;
	uint8 i = 0;
	if(gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag == TRUE) // this dtc snapshot had been in the Nvm,just update it
	{
		for(i=0;i<gDtcCoreCfgInfor.totalDtcNumStoredInNvm;i++)
		{
			if(gDtcStoredInfor[i].innerDtcId == innerDtcId)
			{
				dtcCoreCpyFreezeFrameFromRamToNvm(i,innerDtcId);
				diagSetNvmBlockId(gNvmBlockIdListForDtcStoredInfor[i],STD_ON);
				break;
			}
		}
	}
	#else
		gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag = TRUE;
		diagSetNvmBlockId(gNvmBlockIdListForDtcExtInfor[innerDtcId],STD_ON);
	#endif
}


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM

void dtcCoreCleanStoredSnapshot(uint8 innerDtcId)
{
	uint8 i = 0;
	
	for(i=0;i<gDtcCoreCfgInfor.totalDtcNumStoredInNvm;i++)
	{
		if(gDtcStoredInfor[i].innerDtcId == innerDtcId)
		{
			diagResetBuffer((uint8*)&gDtcStoredInfor[i],0,sizeof(tDtcStoredInfor));
			gDtcStoredInfor[i].innerDtcId = DTC_NVM_SPACE_IS_EMPTY;
			gDtcStoredInfor[i].dtcStatusAndExtInfor.storedInNvmFlag = FALSE;
			gAvailableDtcSpaceNumInNvm++;
			diagSetNvmBlockId(gNvmBlockIdListForDtcStoredInfor[i],STD_ON);
			break;
		}
	}
}

void dtcCoreCpyFreezeFrameFromRamToNvm(uint8 nvmIndex,uint8 ramIndex)
{
	diagCpyData((uint8*)&gDtcStoredInfor[nvmIndex].dtcStatusAndExtInfor,(uint8*)&gDtcStatusAndExtInforArray[ramIndex],sizeof(tDtcStatusAndExtInfor));
	diagCpyData((uint8*)&gDtcStoredInfor[nvmIndex].snapshot,(uint8*)&gRuntimeSnapshotList[ramIndex],sizeof(tDtcSnapshot));
}

boolean dtcCoreStoreSnapshotToNvm(uint8 innerDtcId)
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
		
	if(gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag == TRUE) // this dtc snapshot had been in the Nvm,just update it
	{
		for(i=0;i<gDtcCoreCfgInfor.totalDtcNumStoredInNvm;i++)
		{
			if(gDtcStoredInfor[i].innerDtcId == innerDtcId)
			{
				dtcCoreCpyFreezeFrameFromRamToNvm(i,innerDtcId);
				nvmBlockId = gNvmBlockIdListForDtcStoredInfor[i];
				ret = TRUE;
				break;
			}
		}
	}
	else
	{
		if(gAvailableDtcSpaceNumInNvm == 0) // no space now
		{
			//check dtc priority
			priority = gDtcStatusAndExtInforArray[innerDtcId].extData.priority;
			minPriority = gDtcStatusAndExtInforArray[innerDtcId].extData.priority;
			dtcIdIndexList.len = 0;
			for(i=0;i<gDtcCoreCfgInfor.totalDtcNumStoredInNvm;i++)
			{
				
				if(gDtcStoredInfor[i].dtcStatusAndExtInfor.extData.priority>minPriority )//the value bigger,the priority lower
				{
					minPriority = gDtcStoredInfor[i].dtcStatusAndExtInfor.extData.priority;
					dtcIdIndexList.len = 1;
					dtcIdIndexList.buffer[0] = i;
					j = 0;
				}
				else if(gDtcStoredInfor[i].dtcStatusAndExtInfor.extData.priority == minPriority && (priority!=minPriority))
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
					if(gDtcStoredInfor[oldestAgingCounterIndex].dtcStatusAndExtInfor.extData.agingCounter <gDtcStoredInfor[dtcIdIndexList.buffer[i+1]].dtcStatusAndExtInfor.extData.agingCounter)
						oldestAgingCounterIndex = dtcIdIndexList.buffer[i+1];
				}
				gDtcStatusAndExtInforArray[gDtcStoredInfor[oldestAgingCounterIndex].innerDtcId].storedInNvmFlag = FALSE;//the old one will be mark as NotStoredInNvm
				gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag = TRUE;// the new one will be mark as StoredInNvm
				dtcCoreCpyFreezeFrameFromRamToNvm(oldestAgingCounterIndex,innerDtcId);
				
				nvmBlockId = gNvmBlockIdListForDtcStoredInfor[oldestAgingCounterIndex];
				ret = TRUE;
			}
		}
		else//find the available space index
		{
			for(i=0;i<gDtcCoreCfgInfor.totalDtcNumStoredInNvm;i++)
			{
				if(gDtcStoredInfor[i].innerDtcId == DTC_NVM_SPACE_IS_EMPTY)//this space is availabe
				{
					gDtcStoredInfor[i].innerDtcId = innerDtcId;
					gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag = TRUE;
					dtcCoreCpyFreezeFrameFromRamToNvm(i,innerDtcId);
					nvmBlockId = gNvmBlockIdListForDtcStoredInfor[i];
					gAvailableDtcSpaceNumInNvm--;
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
boolean dtcCoreStoreSnapshotToNvm(uint8 innerDtcId)
{
	gDtcStatusAndExtInforArray[innerDtcId].storedInNvmFlag = TRUE;
	diagSetNvmBlockId(gNvmBlockIdListForDtcExtInfor[innerDtcId],STD_ON);
	diagSetNvmBlockId(gNvmBlockIdListForRuntimeSnapshot[innerDtcId],STD_ON);
	return TRUE;
}

#endif

