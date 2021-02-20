#include"dtcCore_if.h"
#include"dtcCore_cfg.h"
#include"nvm.h"


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM

extern tDtcStoredInfor gDtcStoredInfor[];
extern uint8 gDtcStoredInforNvmBlockIdList[];
extern uint16 gAvailableDtcSpaceNumInNvm;
#else
extern uint8 gNvmBlockIdListForRuntimeSnapshot[];
extern uint8 gNvmBlockIdListForDtcExtInfor[];

#endif

extern tDtcStatusAndExtInfor gDtcStatusAndExtInforArray[];
extern tDtcSnapshot gRuntimeSnapshotList[];
extern const tDtcCoreCfgInfor gDtcCoreCfgInfor;
extern const tDtcMixInfor gDtcMixInfor[];
extern uint16 gDtcTotalNum;

tDtcDataBuffer gDtcDataBuffer = {
	.len = 0,
	.buffer = {1},
	};

tUdsNRCType sid0x19CheckPreCondition(const uint8*data,uint16 len)
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}


boolean dtcCoreResetSnapshot(uint16 innerDtcId)
{
	diagResetBuffer(gRuntimeSnapshotList[innerDtcId].globalSnapshot.timeStamp,0,6);
	diagResetBuffer(gRuntimeSnapshotList[innerDtcId].globalSnapshot.odometer,0xff,3);
	gRuntimeSnapshotList[innerDtcId].globalSnapshot.supplyVoltage = 0;
	gRuntimeSnapshotList[innerDtcId].globalSnapshot.ignitionStatus = DIAG_IGN_STATUS_OFF;	

	//1TODO:reset localSnapshot if supported
	
	return TRUE;
}

boolean dtcCoreCaptureLocalSnapshot(uint16 dtcIndex,uint8*localSnapshot, uint16 len)
{
	
	return TRUE;
}


boolean dtcCoreCaptureGlobalSnapshot(uint16 dtcIndex)
{
	tDiagCANTime curTime;
	uint32 odo = 0;
	uint16 sysVol = 0xff;
	uint8 KL15Status = DIAG_KL15_OFF;
	uint8 KL30Status = DIAG_KL30_STOP_CRANK;
	uint8 AccStatus = DIAG_ACC_OFF;
	
	//global snapshot,extData
	//details,refer to Pg26 SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf

	diagGetCanTime(&curTime);
	
	DiagConvertToBCDcode8bit(curTime.Year,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[0]);
	DiagConvertToBCDcode8bit(curTime.Month,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[1]);
	DiagConvertToBCDcode8bit(curTime.Day,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[2]);
	DiagConvertToBCDcode8bit(curTime.Hour,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[3]);
	DiagConvertToBCDcode8bit(curTime.Min,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[4]);
	DiagConvertToBCDcode8bit(curTime.Sec,&gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp[5]);
	//if time is not available,set time to all 0xFF
	//diagResetBuffer(gDtcNvmInforList[i].globalSnapshot.timeStamp,0xFF,6);
	
	diagGetODO(&odo);
	//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],ODO:%d\r\n",dtcIndex,odo);
	gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer[0] = (uint8)((odo & 0xFF0000)>>16);
	gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer[1] = (uint8)((odo & 0xFF00)>>8);
	gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer[2] = (uint8)((odo & 0xFF));
	
	if(diagGetSysVoltage(&sysVol) == FALSE)
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage = 0xff;
	else
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage = (uint8)(sysVol/10);

	//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],voltage:%d\r\n",dtcIndex,sysVol);

	diagGetAccStatus(&AccStatus);
	diagGetKL15Status(&KL15Status);
	diagGetKL30Status(&KL30Status);
	//DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"DTC[%d],IGN:%d,Eng:%d,ACC:%d,%d\r\n",dtcIndex,gKL15Status,gKL30Status,gAccStatus);
	if(KL30Status == DIAG_KL30_START_CRANK)
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus = DIAG_IGN_STATUS_START;
	else if(KL15Status == DIAG_KL15_ON)
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus = DIAG_IGN_STATUS_ON;
	else if(AccStatus == DIAG_ACC_ON)
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus = DIAG_IGN_STATUS_ACC;
	else
		gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus = DIAG_IGN_STATUS_OFF;
	
	if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInfor[dtcIndex].traceLogEnableFlag == TRUE)
			DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"dtc[%d]:snapshot,kl15:%d,vol:%d,odo:%d\r\n",dtcIndex,gKL15Status,gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage,odo);

	return TRUE;

}


const tDtcDataBuffer *dtcCoreGetSnapshot(uint16 dtcIndex,uint32 snapshotRecordNum)
{
	gDtcDataBuffer.len = 0;

	switch(snapshotRecordNum)
	{
	case GLOBAL_SNAPSHORT_RECORED_NUM:
		gDtcDataBuffer.buffer[0] = GLOBAL_SNAPSHORT_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = 4;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
		gDtcDataBuffer.buffer[2] = 0xf1;
		gDtcDataBuffer.buffer[3] = 0x20;
		diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp,6);
		gDtcDataBuffer.buffer[10] = 0xf1;
		gDtcDataBuffer.buffer[11] = 0x21;
		diagCpyData(&gDtcDataBuffer.buffer[12],gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer,3);
		gDtcDataBuffer.buffer[15] = 0xf1;
		gDtcDataBuffer.buffer[16] = 0x22;
		gDtcDataBuffer.buffer[17] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage;
		gDtcDataBuffer.buffer[18] = 0xf1;
		gDtcDataBuffer.buffer[19] = 0x23;
		gDtcDataBuffer.buffer[20] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus;

		gDtcDataBuffer.len = 21;
		break;
	case ALL_SNAPSHORT_RECORD_NUM:
		//return all snapshot record infor
		gDtcDataBuffer.buffer[0] = GLOBAL_SNAPSHORT_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = 4;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
		gDtcDataBuffer.buffer[2] = 0xf1;
		gDtcDataBuffer.buffer[3] = 0x20;
		diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp,6);
		gDtcDataBuffer.buffer[10] = 0xf1;
		gDtcDataBuffer.buffer[11] = 0x21;
		diagCpyData(&gDtcDataBuffer.buffer[12],gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer,3);
		gDtcDataBuffer.buffer[15] = 0xf1;
		gDtcDataBuffer.buffer[16] = 0x22;
		gDtcDataBuffer.buffer[17] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage;
		gDtcDataBuffer.buffer[18] = 0xf1;
		gDtcDataBuffer.buffer[19] = 0x23;
		gDtcDataBuffer.buffer[20] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus;

		gDtcDataBuffer.len += 21;

		//1TODO: if support more snapshorts,add more here,same as above
		#if 0
		gDtcDataBuffer.buffer[21] = LOCAL_SNAPSHORT_RECORD_NUM;
		gDtcDataBuffer.buffer[22] = 2;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
		gDtcDataBuffer.buffer[23] = 0xf1;
		gDtcDataBuffer.buffer[24] = 0x24;
		diagCpyData(&gDtcDataBuffer.buffer[4],otherData,x);
		//....
		gDtcDataBuffer.len += n;
		#endif
		break;
	#if 0
	case LOCAL_SNAPSHORT_RECORD_NUM:
		//if the Dtc which is request doesn't support LOCAL_SNAPSHORT_RECORD_NUM,
		//just return NULL
		break;
	#endif
	default:
		return NULL;
	}
	return &gDtcDataBuffer;
}


boolean dtcCoreCheckSnapshotRecordNum(uint32 snapshotRecordNum)
{
	boolean ret = TRUE;
	
	switch(snapshotRecordNum)
	{
	case GLOBAL_SNAPSHORT_RECORED_NUM:
	case ALL_SNAPSHORT_RECORD_NUM:
	//case LOCAL_SNAPSHORT_RECORD_NUM:
		break;
	default:
		ret = FALSE;
	}

	return ret;
} 

boolean dtcCoreCheckExtDataRecordNum(uint32 extDataRecordNum)
{
	boolean ret = TRUE;
	
	switch(extDataRecordNum)
	{
	case GLOBAL_EXTENDED_DATA_RECORED_NUM:
	case ALL_EXTENDED_DATA_RECORD_NUM:
	//case LOCAL_EXTENDED_DATA_RECORD_NUM:
		break;
	default:
		ret = FALSE;
	}

	return ret;
} 



const tDtcDataBuffer *dtcCoreGetExtendDataRecord(uint16 dtcIndex,uint32 extDataRecordNum)
{
	gDtcDataBuffer.len = 0;

	switch(extDataRecordNum)
	{
	case GLOBAL_EXTENDED_DATA_RECORED_NUM:
		gDtcDataBuffer.buffer[0] = GLOBAL_EXTENDED_DATA_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = gDtcStatusAndExtInforArray[dtcIndex].extData.priority;
		gDtcDataBuffer.buffer[2] = gDtcStatusAndExtInforArray[dtcIndex].extData.agedCounter;
		gDtcDataBuffer.buffer[3] = gDtcStatusAndExtInforArray[dtcIndex].extData.agingCounter;
		gDtcDataBuffer.buffer[4] = gDtcStatusAndExtInforArray[dtcIndex].extData.occCounter;
		
		gDtcDataBuffer.len = 5;
		break;
	case ALL_EXTENDED_DATA_RECORD_NUM:
		//return all snapshot record infor
		gDtcDataBuffer.buffer[0] = GLOBAL_EXTENDED_DATA_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = gDtcStatusAndExtInforArray[dtcIndex].extData.priority;
		gDtcDataBuffer.buffer[2] = gDtcStatusAndExtInforArray[dtcIndex].extData.agedCounter;
		gDtcDataBuffer.buffer[3] = gDtcStatusAndExtInforArray[dtcIndex].extData.agingCounter;
		gDtcDataBuffer.buffer[4] = gDtcStatusAndExtInforArray[dtcIndex].extData.occCounter;
		
		gDtcDataBuffer.len += 5;

		//1TODO: if support more extended data,add more here,same as above
		#if 0
		gDtcDataBuffer.buffer[5] = LOCAL_SNAPSHORT_RECORD_NUM;
		gDtcDataBuffer.buffer[6] = 0;//other extData
		//....
		gDtcDataBuffer.len += n;
		#endif
		break;
	#if 0
	case LOCAL_EXTENDED_DATA_RECORD_NUM:
		//if the Dtc which is request doesn't support LOCAL_EXTENDED_DATA_RECORD_NUM,
		//just return NULL
		break;
	#endif
	default:
		return NULL;
	}
	return &gDtcDataBuffer;
}

