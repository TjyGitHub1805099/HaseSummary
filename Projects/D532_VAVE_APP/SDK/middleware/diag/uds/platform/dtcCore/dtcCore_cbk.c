/********************************************************************
File name: dtcCore_cbk.c
Author: Li Qi
Version: V1.0
Timestamp: 2018-10-31 14:29:08
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
#include"dtcCore_if.h"
#include"dtcCore_cfg.h"
#include"NvM.h"
#include"trace.h"

#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM

extern tDtcStoredInfor **gDtcStoredInforArray;
extern uint16 *gDtcAvailableSpaceNumInNvmArray;
#else
extern uint8 **gNvmBlockIdListForRuntimeSnapshotArray;
extern uint8 **gNvmBlockIdListForDtcExtInforArray;

#endif

extern tDtcStatusAndExtInfor *gDtcStatusAndExtInforArray[];
extern uint8 *gRuntimeSnapshotArray[];
extern const tDtcMixInfor *gDtcMixInforArray[];
extern uint16 gDtcTotalNum[];
extern uint8 gDtcSnapshotTypeLen[];

extern tDtcSnapshot_IVI gRuntimeSnapshot_IVI[];
extern tDtcSnapshot_IC gRuntimeSnapshot_IC[];
extern tDtcSnapshot_AVM gRuntimeSnapshot_AVM[];
extern tDtcSnapshot_HUD gRuntimeSnapshot_HUD[];


extern tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IVI[];
extern tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IC[];
extern tDtcStatusAndExtInfor gDtcStatusAndExtInfor_AVM[];

extern const tDtcCoreCfgInfor gDtcCoreCfgInfor;


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


boolean dtcCoreResetSnapshot(uint8 ch,uint16 innerDtcId)
{
	diagResetBuffer((uint8*)&gRuntimeSnapshotArray[ch][innerDtcId*gDtcSnapshotTypeLen[ch]],0,gDtcSnapshotTypeLen[ch]);

	#if 0
	if(ch == UDS_IVI)
		diagResetBuffer((uint8*)(&(((tDtcSnapshot_ivi*)gRuntimeSnapshotArray[ch])[innerDtcId])),0,gDtcSnapshotTypeLen[ch]);
	else if(ch == UDS_IC)
		diagResetBuffer((uint8*)(&(((tDtcSnapshot_ic*)gRuntimeSnapshotArray[ch])[innerDtcId])),0,gDtcSnapshotTypeLen[ch]);
	#endif
	//1TODO:reset localSnapshot if supported
	
	return TRUE;
}

boolean dtcCoreCaptureLocalSnapshot(uint8 ch,uint16 dtcIndex,uint8*localSnapshot, uint16 len)
{
	
	return TRUE;
}


boolean dtcCoreCaptureGlobalSnapshot(uint8 ch,uint16 dtcIndex)
{
	uint32 odo = 0;
	uint16 sysVol = 0xff;
	float vehicleSpeed = 0;
	uint16 vehicleSpeedU16 = 0;
	//global snapshot,extData
	//details,refer to Pg26 SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf
	diagGetODO(&odo);

	if(ch == UDS_IVI)
	{						
		
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],ODO:%d\r\n",dtcIndex,odo);
		diagResetBuffer((uint8*)&gRuntimeSnapshot_IVI[dtcIndex], 0, sizeof(tDtcSnapshot_IVI));
		gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.odometer[0] = (uint8)((odo & 0xFF0000)>>16);
		gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.odometer[1] = (uint8)((odo & 0xFF00)>>8);
		gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.odometer[2] = (uint8)((odo & 0xFF));
		
		if(diagGetSysVoltage(&sysVol) == FALSE)
			gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.supplyVoltage = 0xff;
		else
			gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.supplyVoltage = (uint8)(sysVol/10);
	
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],voltage:%d\r\n",dtcIndex,sysVol);
	
		
		if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][dtcIndex].traceLogEnableFlag == TRUE)
				DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"dtc[%d]:snapshot,kl15:%d,vol:%d,odo:%d\r\n",dtcIndex,dtcCoreGetIgnOnFlag(),gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.supplyVoltage,odo);

	}
	else if(ch == UDS_IC)
	{
	
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],ODO:%d\r\n",dtcIndex,odo);
		diagResetBuffer((uint8*)&gRuntimeSnapshot_IC[dtcIndex], 0, sizeof(tDtcSnapshot_IC));
		gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer[0] = (uint8)((odo & 0xFF0000)>>16);
		gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer[1] = (uint8)((odo & 0xFF00)>>8);
		gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer[2] = (uint8)((odo & 0xFF));
		
		if(diagGetSysVoltage(&sysVol) == FALSE)
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.batVoltage = 0xff;
		else
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.batVoltage = (uint8)(sysVol/10);

		//TODO: GET IGN VOLTAGE
		#if 0
		if(diagGetSysVoltage(&sysVol) == FALSE)
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.ignVoltage = 0xff;
		else
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.ignVoltage = (uint8)(sysVol/10);
		#endif

		if(diagGetVehicleSpeed(&vehicleSpeed) == TRUE)
		{
			vehicleSpeedU16 = (uint16)(vehicleSpeed*100);
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[0] = (uint8)((vehicleSpeedU16&0xFF00)>>8);
			gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[1] = (uint8)((vehicleSpeedU16&0xFF));
		}
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],voltage:%d\r\n",dtcIndex,sysVol);
	
		
		if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][dtcIndex].traceLogEnableFlag == TRUE)
				DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"dtc[%d]:snapshot,kl15:%d,vol:%d,odo:%d\r\n",dtcIndex,dtcCoreGetIgnOnFlag(),gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.batVoltage,odo);
	}
	else if(ch == UDS_AVM)
	{
		
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],ODO:%d\r\n",dtcIndex,odo);
		diagResetBuffer((uint8*)&gRuntimeSnapshot_AVM[dtcIndex], 0, sizeof(tDtcSnapshot_AVM));
		gRuntimeSnapshot_AVM[dtcIndex].globalSnapshot.odometer[0] = (uint8)((odo & 0xFF0000)>>16);
		gRuntimeSnapshot_AVM[dtcIndex].globalSnapshot.odometer[1] = (uint8)((odo & 0xFF00)>>8);
		gRuntimeSnapshot_AVM[dtcIndex].globalSnapshot.odometer[2] = (uint8)((odo & 0xFF));		
		
		if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][dtcIndex].traceLogEnableFlag == TRUE)
			DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"dtc[%d]:snapshot,kl15:%d,vol:%d,odo:%d\r\n",dtcIndex,dtcCoreGetIgnOnFlag(),sysVol,odo);
	}
	else if(ch == UDS_HUD)
	{
		
		//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"DTC[%d],ODO:%d\r\n",dtcIndex,odo);
		diagResetBuffer((uint8*)&gRuntimeSnapshot_HUD[dtcIndex], 0, sizeof(tDtcSnapshot_HUD));
		gRuntimeSnapshot_HUD[dtcIndex].globalSnapshot.odometer[0] = (uint8)((odo & 0xFF0000)>>16);
		gRuntimeSnapshot_HUD[dtcIndex].globalSnapshot.odometer[1] = (uint8)((odo & 0xFF00)>>8);
		gRuntimeSnapshot_HUD[dtcIndex].globalSnapshot.odometer[2] = (uint8)((odo & 0xFF));		
		
		if(gDtcCoreCfgInfor.logLevel  >= DTC_LOG_LEVEL_2 && gDtcMixInforArray[ch][dtcIndex].traceLogEnableFlag == TRUE)
			DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"dtc[%d]:snapshot,kl15:%d,vol:%d,odo:%d\r\n",dtcIndex,dtcCoreGetIgnOnFlag(),sysVol,odo);
	}
	return TRUE;

}


tDtcDataBuffer *dtcCoreGetSnapshot(uint8 ch,uint16 dtcIndex,uint32 snapshotRecordNum)
{
	gDtcDataBuffer.len = 0;

	switch(snapshotRecordNum)
	{
	case GLOBAL_SNAPSHORT_RECORED_NUM:
		gDtcDataBuffer.buffer[0] = GLOBAL_SNAPSHORT_RECORED_NUM;
		switch(ch)
		{
		case UDS_IVI:
			gDtcDataBuffer.buffer[1] = 3;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
			gDtcDataBuffer.buffer[2] = 0xf0;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.odometer,3);
			gDtcDataBuffer.buffer[7] = 0xf0;
			gDtcDataBuffer.buffer[8] = 0xd1;
			gDtcDataBuffer.buffer[9] = gDtcStatusAndExtInfor_IVI[dtcIndex].extData.occCounter;
			gDtcDataBuffer.buffer[10] = 0xf0;
			gDtcDataBuffer.buffer[11] = 0xd2;
			gDtcDataBuffer.buffer[12] = gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.supplyVoltage;

			gDtcDataBuffer.len = 13;
			break;
		case UDS_IC:
			gDtcDataBuffer.buffer[1] = 5;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.
			gDtcDataBuffer.buffer[2] = 0xf0;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer,3);
			gDtcDataBuffer.buffer[7] = 0xf0;
			gDtcDataBuffer.buffer[8] = 0xd1;
			gDtcDataBuffer.buffer[9] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.batVoltage;
			gDtcDataBuffer.buffer[10] = 0xf0;
			gDtcDataBuffer.buffer[11] = 0xd2;
			gDtcDataBuffer.buffer[12] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.ignVoltage;
			gDtcDataBuffer.buffer[13] = 0xf0;
			gDtcDataBuffer.buffer[14] = 0xd3;
			gDtcDataBuffer.buffer[15] = gDtcStatusAndExtInfor_IC[dtcIndex].extData.agingCounter;
			gDtcDataBuffer.buffer[16] = 0xf0;
			gDtcDataBuffer.buffer[17] = 0xd4;
			gDtcDataBuffer.buffer[18] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[0];
			gDtcDataBuffer.buffer[19] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[1];

			gDtcDataBuffer.len = 20;
			break;
		
		case UDS_AVM:
			gDtcDataBuffer.buffer[1] = 1;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf1d0
			gDtcDataBuffer.buffer[2] = 0xf1;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer,3);
			break;

		case UDS_HUD:
			gDtcDataBuffer.buffer[1] = 1;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf1d0
			gDtcDataBuffer.buffer[2] = 0xf1;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_HUD[dtcIndex].globalSnapshot.odometer,3);
			break;
		}
		break;
	#if 1
	case ALL_SNAPSHORT_RECORD_NUM:
		//return all snapshot record infor
		gDtcDataBuffer.buffer[0] = GLOBAL_SNAPSHORT_RECORED_NUM;
		#if 0
		gDtcDataBuffer.buffer[1] = 4;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
		gDtcDataBuffer.buffer[2] = 0xf1;
		gDtcDataBuffer.buffer[3] = 0x20;
		//diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshotList[dtcIndex].globalSnapshot.timeStamp,6);
		gDtcDataBuffer.buffer[10] = 0xf1;
		gDtcDataBuffer.buffer[11] = 0x21;
		//diagCpyData(&gDtcDataBuffer.buffer[12],gRuntimeSnapshotList[dtcIndex].globalSnapshot.odometer,3);
		gDtcDataBuffer.buffer[15] = 0xf1;
		gDtcDataBuffer.buffer[16] = 0x22;
		//gDtcDataBuffer.buffer[17] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.supplyVoltage;
		gDtcDataBuffer.buffer[18] = 0xf1;
		gDtcDataBuffer.buffer[19] = 0x23;
		//gDtcDataBuffer.buffer[20] = gRuntimeSnapshotList[dtcIndex].globalSnapshot.ignitionStatus;

		gDtcDataBuffer.len += 21;

		#endif

		switch(ch)
		{
		case UDS_IVI:
			gDtcDataBuffer.buffer[1] = 3;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf120,0xf121,0xf122,0xf123
			gDtcDataBuffer.buffer[2] = 0xf0;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.odometer,3);
			gDtcDataBuffer.buffer[7] = 0xf0;
			gDtcDataBuffer.buffer[8] = 0xd1;
			gDtcDataBuffer.buffer[9] = gDtcStatusAndExtInfor_IVI[dtcIndex].extData.occCounter;
			gDtcDataBuffer.buffer[10] = 0xf0;
			gDtcDataBuffer.buffer[11] = 0xd2;
			gDtcDataBuffer.buffer[12] = gRuntimeSnapshot_IVI[dtcIndex].globalSnapshot.supplyVoltage;

			gDtcDataBuffer.len = 13;
			break;
		case UDS_IC:
			gDtcDataBuffer.buffer[1] = 5;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.
			gDtcDataBuffer.buffer[2] = 0xf0;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer,3);
			gDtcDataBuffer.buffer[7] = 0xf0;
			gDtcDataBuffer.buffer[8] = 0xd1;
			gDtcDataBuffer.buffer[9] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.batVoltage;
			gDtcDataBuffer.buffer[10] = 0xf0;
			gDtcDataBuffer.buffer[11] = 0xd2;
			gDtcDataBuffer.buffer[12] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.ignVoltage;
			gDtcDataBuffer.buffer[13] = 0xf0;
			gDtcDataBuffer.buffer[14] = 0xd3;
			gDtcDataBuffer.buffer[15] = gDtcStatusAndExtInfor_IC[dtcIndex].extData.agingCounter;
			gDtcDataBuffer.buffer[16] = 0xf0;
			gDtcDataBuffer.buffer[17] = 0xd4;
			gDtcDataBuffer.buffer[18] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[0];
			gDtcDataBuffer.buffer[19] = gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.vehicleSpeed[1];

			gDtcDataBuffer.len = 20;
			break;
		
		case UDS_AVM:
			gDtcDataBuffer.buffer[1] = 1;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf1d0
			gDtcDataBuffer.buffer[2] = 0xf1;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_IC[dtcIndex].globalSnapshot.odometer,3);
			break;

		case UDS_HUD:
			gDtcDataBuffer.buffer[1] = 1;//DTCSnapshotRecordNumboerOfIdentifiers. means how many sub types of snapshot.here are 0xf1d0
			gDtcDataBuffer.buffer[2] = 0xf1;
			gDtcDataBuffer.buffer[3] = 0xd0;
			diagCpyData(&gDtcDataBuffer.buffer[4],gRuntimeSnapshot_HUD[dtcIndex].globalSnapshot.odometer,3);
			break;
		}




		
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
	#endif
	default:
		return NULL;
	}
	
	return &gDtcDataBuffer;
}


boolean dtcCoreCheckSnapshotRecordNum(uint8 ch,uint32 snapshotRecordNum)
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


tDtcDataBuffer *dtcCoreGetExtendDataRecord(uint8 ch,uint16 dtcIndex,uint32 extDataRecordNum)
{
	gDtcDataBuffer.len = 0;

	switch(extDataRecordNum)
	{
	case GLOBAL_EXTENDED_DATA_RECORED_NUM:
		gDtcDataBuffer.buffer[0] = GLOBAL_EXTENDED_DATA_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.priority;
		gDtcDataBuffer.buffer[2] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.agedCounter;
		gDtcDataBuffer.buffer[3] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.agingCounter;
		gDtcDataBuffer.buffer[4] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.occCounter;
		
		gDtcDataBuffer.len = 5;
		break;
	case ALL_EXTENDED_DATA_RECORD_NUM:
		//return all snapshot record infor
		gDtcDataBuffer.buffer[0] = GLOBAL_EXTENDED_DATA_RECORED_NUM;
		gDtcDataBuffer.buffer[1] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.priority;
		gDtcDataBuffer.buffer[2] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.agedCounter;
		gDtcDataBuffer.buffer[4] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.occCounter;
		gDtcDataBuffer.buffer[3] = gDtcStatusAndExtInforArray[ch][dtcIndex].extData.agingCounter;
		
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


uint16 dtcCoreGetExternalReport(const tUdsReqInfor*reqInfor,uint8 **retBuffer)
{
	uint16 retDataLen = 0;
	uint8 ch = reqInfor->ch;
	uint8 *data = reqInfor->data;
	uint8 len = reqInfor->len;
	uint8 statusMask = 0;
	uint8 i = 0;
	uint8 testDtcStatusBuffer[2][4] = {{1,1,1,0x7},{2,2,2,0x7}};
	static uint8 tmpBuffer[16];
		
	switch(data[1])
	{
	case REPORT_DTC_BY_STATUS_MASK://DNTC only req to support this item
		#if 0
		if(ch == UDS_AVM)
		{
			statusMask = data[2]&DTC_STATUS_AVAILABILITY_MASK;
			for(i=0;i<2;i++)
			{
				if(testDtcStatusBuffer[i][3] & statusMask)
				{
					diagCpyData(tmpBuffer+(i*4),testDtcStatusBuffer[i],4);
					retDataLen += 4;
				}
			}
		}
		#endif
		break;
	default:
		break;
	}
	
	*retBuffer = tmpBuffer;
	
	return retDataLen;
}


void dtcCoreClearAll()
{
	uint8 ch=0;
	uint8 index = 0;
	
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
	{
		for(index=0;index<gDtcTotalNum[ch];index++)
		{
			dtcCoreResetDTCInfor(ch,index,DTC_RESET_BY_SID_0X14);
			dtcCoreStoreSnapshotToNvm(ch,index);
		}
	}
}
