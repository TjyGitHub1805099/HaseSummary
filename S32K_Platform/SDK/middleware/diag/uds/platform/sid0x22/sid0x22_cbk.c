#include"sid0x22_if.h"
#include"sid0x22_cfg.h"
#include"sid0x10_if.h"

extern const t0x22DidInfo g0x22DidInfor[];
extern uint16  g0x22DidTotalNum;

//===========================================
extern uint8  UdsTestDid[];
extern uint16 gSystemSupplierEcuHardwareNum;
extern uint8  gECUSerialNumbe[];
extern uint8  gVINDataID[];
extern uint8  gSupplierHWRefGuide[];
extern uint8  gDiagSerNum[];
extern uint8  gResetRecord[];
extern uint8  gBodyCfgInf[];
extern uint8  gPPKValue[];
extern uint8  gSoftwareProgramDate[];
extern uint8  gEcuInstallDate[];
//============================================
tUdsNRCType sid0x22CheckPreCondition(const uint8*data,uint16 len)
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}


tUdsNRCType sid0x2eCheckPreCondition(const uint8*data,uint16 len)
{
	//2TODO:
	return NRC_NONE;
}




//========================================================================
//========================================================================
//========================================================================


/* 1 vehicleManufacturerSparePartNumberDataIdentifier - ASCII*/
uint16 sid0x22ReadVehicleManufacturerSparePartNum(uint8*respData)
{
	uint8 gVehicleManufNum[DID_VEHICLE_MANUFACTURER_PART_NUM_LEN] = {'7','9','1','1','1','5','0','L','4','0','0','0'};
	if(respData!=NULL)
		diagCpyData(respData,gVehicleManufNum,DID_VEHICLE_MANUFACTURER_PART_NUM_LEN);
	return DID_VEHICLE_MANUFACTURER_PART_NUM_LEN;
}
tUdsNRCType sid0x2eVerifygVehicleManufacturerSparePartNum(uint16 didNum,const uint8*didValue,uint16 len)
{
	if(didValue==NULL || len!=DID_VEHICLE_MANUFACTURER_PART_NUM_LEN)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	//if(verifyDate(didNum,didValue, len) != NRC_NONE)
	//	return NRC_REQUEST_OUT_OF_RANGE;
	return NRC_NONE;
}

/* 2 systemSupplierIdentifierDataIdentifier  - ASCII + BCD*/
uint16 sid0x22ReadSupplierDIDNum(uint8*respData)
{
	uint8 gSystemSupplierDID[DID_SYSTEM_SUPPLIER_DID_LEN]	 = {'0','0','0','0','L','2','2','0','3','1'};
	if(respData!=NULL)
		diagCpyData(respData,gSystemSupplierDID,DID_SYSTEM_SUPPLIER_DID_LEN);
	return DID_SYSTEM_SUPPLIER_DID_LEN;
}

/* 3 SystemName  - ASCII */
uint16 sid0x22ReadSysName(uint8*respData)
{
	uint8 gSystemName[DID_SYSTEM_NAME_LEN]={'M','P','5','0','0','0' };
	if(respData!=NULL)
		diagCpyData(respData,gSystemName,DID_SYSTEM_NAME_LEN);
	return DID_SYSTEM_NAME_LEN;
}

/* 4 systemSupplierECUHardwareVersionNumberDataIdentifier  - BCD*/
/*
uint16 sid0x22ReadSupplierEcuHwNum(uint8*respData)
{
	//this Software version is set by supplier internally
	if(respData!=NULL)
		diagCpyData(respData,SysVersionInfo.SupplierSwNum,SYS_LEN_SUPPLIER_SW);	
	return 0;
}
*/
/* 5 SystemSupplierECUSoftwareVersionNumberDataIdentifier  - BCD*/
uint16 sid0x22ReadSupplierEcuSwNum(uint8*respData)
{
	//this Software version is set by supplier internally
	uint8 gSystemSupplierEcuSoftwareNum[DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM_LEN]={0x01,0x01};
	if(respData!=NULL)
		diagCpyData(respData,gSystemSupplierEcuSoftwareNum,DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM_LEN);	
	return DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM_LEN;
}

/* 6 ECUSerialNumberDataIdentifier  - ASCII*/
/*
uint16 sid0x22ReadEcuSertNum(uint8*respData)
{
	if(respData!=NULL)
		diagCpyData(respData,gECUSerialNumbe,DID_ECU_SER_NUM_LEN);
	
	return DID_ECU_SER_NUM_LEN;
}
*/

//len==12
//ecu serial num write by ame,eg:171225000001 12 bytes
//save in mcu eg:1712250001 10 bytes
boolean sid0x2eWriteEcuSerialNum(uint16 didNum,const uint8 *value,uint16 len)
{
	uint8 i=0,j=0;
	for(i=0;i<DID_ECU_SER_NUM_LEN + 2;i++)
	{
		if((6 == i)||(7 == i))
		{
			continue;
		}
		gECUSerialNumbe[j] = value[i];
		j++;
	}
	//NvM_WriteBlock(gECUSerialNumbe_NvMBlockID, NULL);
	return NRC_NONE;
}

/* 7 VIN DataIdentifier -ASCII */
tUdsNRCType sid0x2eVerifyVINDataDIDValue(uint16 didNum,const uint8*didValue,uint16 len)
{
	
	if(didValue==NULL||len!=DID_VIN_DATA_ID_LEN)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	// TODO:	
	return NRC_NONE;
}

/* 8 供应商ECU引导软件参考*/
uint16 sid0x22ReadSupplierSWRefGuide(uint8*respData)
{
	uint8 gSupplierSWRefGuide[DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE_LEN]={'V','1','.','0'};
	if(respData!=NULL)
		diagCpyData(respData,gSupplierSWRefGuide,DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE_LEN);
	return DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE_LEN;
}

/* 9 these DIDs are running time states  - Hex(Unsigned)*/
uint16 sid0x22ReadActiveDiagSession(uint8 *p)
{
	tUdsSessionInnerType innerSessionType;
	tUdsSessionExternalType extSessionType;
	if(p!=NULL)
	{
		innerSessionType = sid0x10GetCurSession();
		switch(innerSessionType)
		{
		case UDS_DEFAULT_SESSION:
			extSessionType = UDS_EXTERNAL_DEFAULT_SESSION;
			break;
		case UDS_PROGRAMMING_SESSION:
			extSessionType= UDS_EXTERNAL_PROGRAMMING_SESSION;
			break;
		case UDS_EXTENDED_DIAG_SESSION:
			extSessionType = UDS_EXTERNAL_EXTENDED_DIAG_SESSION;
			break;
		case UDS_CODING_SESSION:
			extSessionType = UDS_EXTERNAL_CODING_SESSION;
			break;
		//case UDS_DEVELOPMENT_SESSION:
		default:
			return NRC_STOP_PROCESS;
		}
		*p = extSessionType;
	}

	return 1;
}

/* 10 供应商ECU引导硬件参考*/
/*
uint16 sid0x22ReadSupplierHWRefGuide(uint8*respData)
{	
	return 0;
}
*/
/* 13 下载软件完整性状态*/	
uint16 sid0x22ReadDownloadSoftwareIntegrity(uint8*respData)
{
	uint8 temp=0x01;
	if(respData!=NULL)
		diagCpyData(respData,&temp,DID_DOWNLOAD_SOFTWARE_INTEGRITY_LEN);
	return DID_DOWNLOAD_SOFTWARE_INTEGRITY_LEN;
}

/* 14 下载软件兼容性状态*/
uint16 sid0x22ReadDownloadSoftwareCompatibility(uint8*respData)
{
	uint8 temp=0x00;
	if(respData!=NULL)
		diagCpyData(respData,&temp,DID_DOWNLOAD_SOFTWARE_COMPATIBILITY_LEN);

	return DID_DOWNLOAD_SOFTWARE_COMPATIBILITY_LEN;
}

/* 15 软件有效标志*/
uint16 sid0x22ReadSoftwareValidity(uint8*respData)
{
	uint8 temp=0x01;
	if(respData!=NULL)
		diagCpyData(respData,&temp,DID_SOFTWARE_VALIDITY_LEN);
	return DID_SOFTWARE_VALIDITY_LEN;
}


/* 17 整车配置信息*/ 
boolean sid0x2eWriteDidBodyCfgInf(uint16 didNum,uint8 *value,uint16 len)
{
	uint8 m = 0;

	if(value == NULL)
		return FALSE;
	
	for(m=0;m<g0x22DidTotalNum;m++)
	{
		if(g0x22DidInfor[m].id == didNum)
		{
			if(g0x22DidInfor[m].nvmAddr != NULL && len <= g0x22DidInfor[m].dataLen)
			{
				diagCpyData(g0x22DidInfor[m].nvmAddr,value,len);
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	return FALSE;

}



tUdsNRCType verifyDate(uint16 didNum,const uint8*date,uint16 len)
{
	uint8 year;
	uint8 month;
	uint8 day;
	
	if(date == NULL || len<3)
		return FALSE;
	year = date[0];
	month = date[1];
	day = date[2];

	if(year>0x99||month==0||month>0x12||day==0||day>0x31)
		return NRC_REQUEST_OUT_OF_RANGE;

	return NRC_NONE;
}


