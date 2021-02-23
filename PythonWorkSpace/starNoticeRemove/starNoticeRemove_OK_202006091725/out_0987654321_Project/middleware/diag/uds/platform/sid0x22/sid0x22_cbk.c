#include "sid0x22_if.h"
#include "sid0x22_cfg.h"
#include "sid0x10_if.h"
#include "adc_wrapper_if.h"
#include "uds_if.h"
#include "DeaDiag.h"
#include "pin_wrapper_if.h"
#include "pmStateMachine.h"
#include "BattVoltage.h"
#include "dtcApp_cfg.h"
#include "Key.h"
#include "odo.h"
#include "HardwareVersion.h"
#include "SWVersion.h"
#include "diagAppWllr_cfg.h"
#include "NvM_Cfg.h"
#include "string.h"
#include "MaintainService.h"
#include "SecurityAlgorithm.h"
#include "SpiIpc_cfg.h"
#include "diagAppIpc_cbk.h"
#include "trace.h"
#include "Spi_Ipc_gw_api.h"
#include "Delay.h"
#include "diagCommon_if.h"
#include "diagCommon_cfg.h"
#include "main.h"
#include "uds_cfg.h"
#include "diagAppIpc_cbk.h"
#include "main.h"
#include "dtcCore_cfg.h"


extern uint8* DateHexToDateAscii(uint8* hex, uint8 hexLen, uint8* asc, uint8 ascLen);






extern uint8 g_AVM_EncryptedIC_Status[];
extern uint8 g_AVM_Original_Status[];

#if 0
extern uint8 g_AVM_CanDiagMuxState[];


extern uint8 g_AVM_OperationalReference[];
extern uint8 g_AVM_SysSupplierNum[];

extern uint8 g_AVM_DiagIdCode[];
extern uint8 g_AVM_BootVersion[];
#endif






tUdsNRCType sid0x22CheckPreCondition(const uint8* data, uint16 len)
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}


tUdsNRCType sid0x2eCheckPreCondition(const uint8* data, uint16 len)
{
	
	return NRC_NONE;
}



tUdsNRCType sid0x22Check0xDF(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 did_high_or_low = 0;
	uint8 i=0;
	uint8 byteValue = 0;
	uint8 byteNum = 0;
	uint8 bitNum = 0;
	uint8 ch = reqInfor->ch;
	uint8* dataPtr = reqInfor->data;
	uint16 len = reqInfor->len;
	boolean matchFlag = FALSE;
	

	if(g0x22DidInforArray[ch] != NULL)
	{
		if(dataPtr[1] == 0xDF)
		{
			if(len == 3)
			{
				diagResetBuffer(dataPtr+3, 0, 40);
				did_high_or_low = dataPtr[2];
				
				for(i=0; i<g0x22DidTotalNum[ch]; i++)
				{
					if(g0x22DidInforArray[ch][i].privateFlag == FALSE)   
					{
						if((g0x22DidInforArray[ch][i].rdwrFlag & DID_READ) != 0)
						{
							if(did_high_or_low == 0xDF)    
							{
								byteValue = (uint8)((g0x22DidInforArray[ch][i].id & 0xFF00)>>8);
								matchFlag = TRUE;
							}
							else	 
							{
								if(did_high_or_low == (uint8)((g0x22DidInforArray[ch][i].id & 0xFF00)>>8))
								{
									byteValue = (uint8)(g0x22DidInforArray[ch][i].id & 0xFF);
									matchFlag = TRUE;
								}
							}
						
							if(matchFlag == TRUE)
							{
								byteNum = byteValue/8;
								bitNum = 7-byteValue%8;
								dataPtr[3+byteNum] |= 1<<bitNum;
								matchFlag = FALSE;
								
							}

						}
					}
				}

				
				if(did_high_or_low == 0xDF)
				{
					byteNum = 0xDF/8;
					bitNum = 7-byteValue%8;
					dataPtr[3+byteNum] |= 1;
					
				}
				
				gRespDataLen[ch] = 34;
			}
			else
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
		}
		else
			ret = NRC_REQUEST_OUT_OF_RANGE;

	}
	else
		ret = NRC_STOP_PROCESS;

	return ret;
}



tUdsNRCType sid0x2eCheck0xDF(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 did_high_byte = 0;
	uint8 i=0;
	uint8 byteValue = 0;
	uint8 byteNum = 0;
	uint8 bitNum = 0;
	uint8 ch = reqInfor->ch;
	uint8* dataPtr = reqInfor->data;
	uint16 len = reqInfor->len;
	boolean matchFlag = FALSE;
	
	
	if(g0x22DidInforArray[ch] != NULL)
	{
		if(dataPtr[1] == 0xDF)
		{
			if(len == 3)
			{
				diagResetBuffer(dataPtr+3, 0, 32);
				did_high_byte = dataPtr[2];
				
				for(i=0; i<g0x22DidTotalNum[ch]; i++)
				{
					if((g0x22DidInforArray[ch][i].rdwrFlag & DID_WRITE) != 0)
					{
						if(did_high_byte == 0xDF)    
						{
							byteValue = (uint8)((g0x22DidInforArray[ch][i].id & 0xFF00)>>8);
							matchFlag = TRUE;
						}
						else      
						{
							if(did_high_byte == (uint8)((g0x22DidInforArray[ch][i].id & 0xFF00)>>8))
							{
								byteValue = (uint8)(g0x22DidInforArray[ch][i].id & 0xFF);
								matchFlag = TRUE;
							}
						}

						if(matchFlag == TRUE)
						{
							byteNum = byteValue/8;
							bitNum = 7-byteValue%8;
							dataPtr[3+byteNum] |= 1<<bitNum;
							matchFlag = FALSE;
							
						}
					}
				}

				#if 0
				
				byteNum = 0xDF/8;
				bitNum = 7-byteValue%8;
				dataPtr[3+byteNum] |= 1<<bitNum;
				counter++;
				#endif
				
				gRespDataLen[ch] = 34;
			}
			else
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
		}
		else
			ret = NRC_REQUEST_OUT_OF_RANGE;

	}
	else
		ret = NRC_STOP_PROCESS;

	return ret;
}







uint16 IVI_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_UdsInnerTestDid, IVI_DID_UDS_INNER_TEST_LEN);
	
	return IVI_DID_UDS_INNER_TEST_LEN;
}


#if 0

uint16 IVI_sid0x22ReadSparePartsNumNissan(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_SparePartsNumNissan, IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN);
	
	return IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN;
}
#endif

#if 0
tUdsNRCType IVI_sid0x2eWriteSparePartsNumNissan(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_SparePartsNumNissan[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif

#if 0

uint16 IVI_sid0x22ReadSparePartsNumRenault(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_SparePartsNumRenault, IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN);
	
	return IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN;
}


tUdsNRCType IVI_sid0x2eWriteSparePartsNumRenault(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_SparePartsNumRenault[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif


uint16 IVI_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_SysSupplierNum, IVI_DID_SYS_SUPPLIER_NUM_LEN);
	
	return IVI_DID_SYS_SUPPLIER_NUM_LEN;
}


tUdsNRCType IVI_sid0x2eWriteSysSupplierNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_SYS_SUPPLIER_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_SysSupplierNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}

#if 1

uint16 IVI_sid0x22ReadEcuSerialNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, socSerialNum, IVI_DID_ECU_SERIAL_NUM_LEN);
	
	return IVI_DID_ECU_SERIAL_NUM_LEN;
}


tUdsNRCType IVI_sid0x2eWriteEcuSerialNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0;
	
	if(value==NULL_PTR || len!=IVI_DID_ECU_SERIAL_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		socSerialNum[i] = value[i];
	}
	
	diagNvmWriteSync(socSerialNumNvMBlockID, socSerialNum);
	return NRC_NONE;
}
#endif



uint16 IVI_sid0x22ReadVin(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_VIN, IVI_DID_VIN_LEN);
	
	return IVI_DID_VIN_LEN;
}


tUdsNRCType IVI_sid0x2eWriteVin(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i = 0;
	
	if(value==NULL_PTR || len!=IVI_DID_VIN_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_VIN[i] = value[i];
	}
	
	diagNvmWriteSync(g_IVI_VINNvMBlockID, g_IVI_VIN);

	ipc_send_notification(1, IPC_GW_CHANNEL_DIAG_IVI, IVI_IPC_UDS_GET_VIN, g_IVI_VIN, IVI_DID_VIN_LEN);
	
	return NRC_NONE;
}




#if 1

uint16 IVI_sid0x22ReadVehicleManufEcuHwNum(uint32 ch, uint8* respData)
{
	uint8 i = 0;
	uint8 tempBuf[IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN] = {0};

	tempBuf[0] = 'H';
	tempBuf[1] = getCircuitVerion();
	tempBuf[2] = 'B';
	tempBuf[3] = 'L';

	for(i=0; i<IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN-4; i++)
	{
		tempBuf[4+i] = gBootloaderSWVersionHex[i];
	}

	if(respData != NULL_PTR)
		diagCpyData(respData, tempBuf, IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN);
	
	return IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN;
}
#endif

#if 0
tUdsNRCType IVI_sid0x2eWriteVehicleManufEcuHwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_VehicleManufEcuHwNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif


uint16 IVI_sid0x22ReadSysSupplierEcuSwNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
	{
		respData[0] = '2';
		respData[1] = '0';
		diagCpyData(&respData[2], gIocSWVersion, IOC_SOFTWARE_VERSION_LEN);
	}

	return IVI_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN;
}

#if 0
tUdsNRCType IVI_sid0x2eWriteSysSupplierEcuSwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_SysSupplierEcuSwNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif



uint16 IVI_sid0x22ReadSysSupplierEcuSwVersionNum(uint32 ch, uint8* respData)
{
	uint8 date_asc[IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2] = {0};
	
	DateHexToDateAscii(gBootloaderSWVersionHex, IOC_SOFTWARE_VERSION_ROM_HEX_LEN, date_asc, IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2);

	if(respData != NULL_PTR)
	{
		respData[0] = '2';
		respData[1] = '0';
		diagCpyData(&respData[2], date_asc, IOC_SOFTWARE_VERSION_LEN-1);
	}

	return IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN;	
}

#if 0
tUdsNRCType IVI_sid0x2eWriteSysSupplierEcuSwVersionNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IVI_SysSupplierEcuSwVersionNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif



uint16 IVI_sid0x22ReadOperationalReference(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_OperationalReference, IVI_DID_OPERATIONAL_REFERENCE_LEN);
	
	return IVI_DID_OPERATIONAL_REFERENCE_LEN;
}



uint16 IVI_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_DiagIdCode, IVI_DID_DIAG_ID_CODE_LEN);
	
	return IVI_DID_DIAG_ID_CODE_LEN;
}


uint16 IVI_sid0x22ReadCanDiagMuxState(uint32 ch, uint8* respData)
{
	

	diagResetBuffer(respData,0xFF,140);

	respData[0] = 0x33;
	respData[1] = 0x31;
	
	respData[2] = (uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_BUS_OFF]&0xFF);
	respData[3] = (uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_TX_MUTE]&0xFF);
	respData[4] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_ECM]&0xFF));
	respData[5] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC]&0xFF));
	respData[6] = ((gDiagMuxOnCurState==0) ? 0xFF:0);
	respData[7] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_BCM]&0xFF));
	respData[11] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_HVAC]&0xFF));

	respData[12] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_CVT]&0xFF));
	respData[13] = ((gDiagMuxOnCurState==0) ? 0xFF:0);

	
	respData[17] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_STRG]&0xFF));
	
	respData[21] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_EPS]&0xFF));
	respData[23] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_USM]&0xFF));
	respData[24] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_TPMS]&0xFF));

	respData[35] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_IKEY]&0xFF));
	
	respData[41] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_TCU]&0xFF));
	respData[44] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_UPA]&0xFF));
	
	respData[53] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIVICanFaultBitFlagArray[IVI_FAULT_INDEX_RX_CAN_MSG_ABD]&0xFF));
	
	
	
	
	return IVI_DID_CAN_DIAG_MUX_STATE_LEN;
}


uint16 IVI_sid0x22ReadInputLines(uint32 ch, uint8* respData)
{
	uint8 state = 0;
	
	respData[0] = 0;
	
	if(PORT_READ_PIN_VALUE(PORTGP_IOC_PKB, PINNUM_IOC_PKB) !=0)
		respData[0] |= 2;
	diagGetKL15Status(&state);
	respData[0] |= (state&1);
	
	
	respData[1] = 0;
	diagGetAccStatus(&state);
	respData[1] |= (state<<6);
	
	
	
	respData[1] |= (state&4);
	respData[1] |= (state&2);
	

	return IVI_DID_INPUT_LINES_LEN;
}


uint16 IVI_sid0x22ReadTcuAcc(uint32 ch, uint8* respData)
{
	uint8 state = GetPinValue_TCU_ACC_DET();

	respData[0] = (state == PM_TCU_ACC_ON) ? 1:0;
	return IVI_DID_TCU_ACC_LEN;
}


uint16 IVI_sid0x22ReadBatteryLevel(uint32 ch, uint8* respData)
{
	uint16 state = 0;

	BattVolt_GetRawValue(&state);

	respData[0] = (uint8)(0.05*state);
	return IVI_DID_BATTERY_LEVEL_LEN;
}


uint16 IVI_sid0x22ReadSpeedInformation(uint32 ch, uint8* respData)
{
	uint16 state = 0;
	
	
	state = DEA_GetRawVehicleSpeedCanOutput();

	
	respData[0] = (uint8)((state&0xFF00)>>8);
	respData[1] = (uint8)((state&0xFF));
	return IVI_DID_SPEED_INFORMATION_LEN;
}





uint16 IC_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_UdsInnerTestDid, IC_DID_UDS_INNER_TEST_LEN);

	return IC_DID_UDS_INNER_TEST_LEN;
}



uint16 IC_sid0x22ReadSupportedDid(uint32 ch, uint8* respData)
{
	return 0;
}



uint16 IC_sid0x22ReadSparePartsNumNissan(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_SparePartsNumNissan, IC_DID_SPARE_PARTS_NUM_NISSAN_LEN);
	
	return IC_DID_SPARE_PARTS_NUM_NISSAN_LEN;
}


tUdsNRCType IC_sid0x2eWriteSparePartsNumNissan(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_SPARE_PARTS_NUM_NISSAN_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_SparePartsNumNissan[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}


#if 0

uint16 IC_sid0x22ReadSparePartsNumRenault(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_SparePartsNumRenault, IC_DID_SPARE_PARTS_NUM_RENAULT_LEN);
	
	return IC_DID_SPARE_PARTS_NUM_RENAULT_LEN;
}


tUdsNRCType IC_sid0x2eWriteSparePartsNumRenault(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_SPARE_PARTS_NUM_RENAULT_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_SparePartsNumRenault[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif


uint16 IC_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_DiagIdCode, IC_DID_DIAG_ID_CODE_LEN);
	
	return IC_DID_DIAG_ID_CODE_LEN;
}


tUdsNRCType IC_sid0x2eWriteDiagIdCode(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_DIAG_ID_CODE_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_DiagIdCode[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}



uint16 IC_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_SysSupplierNum, IC_DID_SYS_SUPPLIER_NUM_LEN);
	
	return IC_DID_SYS_SUPPLIER_NUM_LEN;
}


tUdsNRCType IC_sid0x2eWriteSysSupplierNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_SYS_SUPPLIER_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_SysSupplierNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}



uint16 IC_sid0x22ReadSysSupplierSwNum(uint32 ch, uint8* respData)
{
	


	
	uint8 date_asc[IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2] = {0};
	
	DateHexToDateAscii(gBootloaderSWVersionHex, IOC_SOFTWARE_VERSION_ROM_HEX_LEN, date_asc, IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2);

	if(respData != NULL_PTR)
	{	
		diagCpyData(&respData[0], date_asc, IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2);
	}

	return IC_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN*2;

	
}

#if 0
tUdsNRCType IC_sid0x2eWriteSysSupplierSwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_SysSupplierEcuSwNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif


uint16 IC_sid0x22ReadSysSupplierSwVersionNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, gIocSWVersion, IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN);
	
	return IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN;
}

#if 0
tUdsNRCType IC_sid0x2eWriteSysSupplierSwVersionNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_SysSupplierEcuSwVersionNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}
#endif


uint16 IC_sid0x22ReadLatestSparePartsNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IC_LatestSparePartsNum, IC_DID_LATEST_SPARE_PARTS_NUM_LEN);
	
	return IC_DID_LATEST_SPARE_PARTS_NUM_LEN;
}


tUdsNRCType IC_sid0x2eWriteLatestSparePartsNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	
	if(value==NULL_PTR || len!=IC_DID_LATEST_SPARE_PARTS_NUM_LEN)
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		g_IC_LatestSparePartsNum[j] = value[i];
		j++;
	}
	
	
	
	return NRC_NONE;
}



uint16 IC_sid0x22ReadSpeedOmeter(uint32 ch, uint8* respData)
{
	uint16 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_F40D_Speed_ometer((uint8*)&receive);

	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&receive, receiveLen);

	return receiveLen;
}



uint16 IC_sid0x22ReadSpeedOutput(uint32 ch, uint8* respData)
{
	uint16 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E00_Speed_output((uint8*)&receive);

	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&receive, receiveLen);

	return receiveLen;
}



uint16 IC_sid0x22ReadOdometerOutput(uint32 ch, uint8* respData)
{
	tDid_0E01_ODO_output Did_0E01_ODO_output = {0};
	uint16 receiveLen = sizeof(Did_0E01_ODO_output);

	DEA_Diag_GetDid_0E01_ODO_output((uint8*)&Did_0E01_ODO_output);

	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E01_ODO_output, receiveLen-1);

	return IC_DID_ODOMETER_OUTPUT_LEN;
}



uint16 IC_sid0x22ReadOdometer(uint32 ch, uint8* respData)
{
	uint16 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E02_Tachometer((uint8*)&receive);

	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&receive, receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadFuelMeter(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E03_Fuel_meter(&receive);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadWaterTemperature(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E04_Water_temperature(&receive);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
	
	return receiveLen;
}



#if 0


uint16 IC_sid0x22ReadDid0E07Info(uint32 ch, uint8* respData)
{
	tDid_0E07_Info Did_0E07_Info = {0};
	uint16 receiveLen = sizeof(Did_0E07_Info);
	
	DEA_Diag_GetDid_0E07_Info((uint8*)&Did_0E07_Info);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E07_Info, receiveLen);
	
	return receiveLen;
}
#endif


uint16 IC_sid0x22ReadDistanceToEmpty(uint32 ch, uint8* respData)
{
	uint16 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E08_Distance_To_Empty((uint8*)&receive);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&receive, receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadAmbientTemperature(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E09_Ambient_temperature(&receive);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E0AInfo(uint32 ch, uint8* respData)
{
	tDid_0E0A_Info Did_0E0A_Info = {0};
	uint16 receiveLen = sizeof(Did_0E0A_Info);
	
	DEA_Diag_GetDid_0E0A_Info((uint8*)&Did_0E0A_Info);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E0A_Info), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadGeneralPowerMeter(uint32 ch, uint8* respData)
{
	tDid_0E0B_General_power_meter Did_0E0B_General_power_meter = {0};
	uint16 receiveLen = sizeof(Did_0E0B_General_power_meter);
	
	DEA_Diag_GetDid_0E0B_General_power_meter((uint8*)&Did_0E0B_General_power_meter);
		
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E0B_General_power_meter, receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadSocMeter(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E0C_SOC_meter(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadVoltMeter(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E0D_VOLT_meter(&receive);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadBrakePad(uint32 ch, uint8* respData)
{
	tDid_0E0E_General_Brake_pad Did_0E0E_General_Brake_pad = {0};
	uint16 receiveLen = sizeof(Did_0E0E_General_Brake_pad);
	
	DEA_Diag_GetDid_0E0E_Brake_pad((uint8*)&Did_0E0E_General_Brake_pad);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E0E_General_Brake_pad, receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E0FInfo(uint32 ch, uint8* respData)
{
	tDid_0E0F_Info Did_0E0F_Info = {0};
	uint16 receiveLen = sizeof(Did_0E0F_Info);
	
	DEA_Diag_GetDid_0E0F_Info((uint8*)&Did_0E0F_Info);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E0F_Info), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadEngShutTimer(uint32 ch, uint8* respData)
{
	tDid_0E10_ENG_shut_timer Did_0E10_ENG_shut_timer = {0};
	uint16 receiveLen = sizeof(Did_0E10_ENG_shut_timer);
	
	DEA_Diag_GetDid_0E10_ENG_shut_timer((uint8*)&Did_0E10_ENG_shut_timer);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E10_ENG_shut_timer, receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadTorqueDistribution(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E11_Torque(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E12Info(uint32 ch, uint8* respData)
{
	tDid_0E12_Info Did_0E12_Info = {0};
	uint16 receiveLen = sizeof(Did_0E12_Info);
	
	DEA_Diag_GetDid_0E12_Info((uint8*)&Did_0E12_Info);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E12_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadCcSlReq(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E13_CC_SL_REQ(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadScrRemaining(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E14_SCR_REMAINING(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadScrDistance(uint32 ch, uint8* respData)
{
	tDid_0E15_SCR_DISTANCE Did_0E15_SCR_DISTANCE = {0};
	uint16 receiveLen = sizeof(Did_0E15_SCR_DISTANCE);
	
	DEA_Diag_GetDid_0E15_SCR_DISTANCE((uint8*)&Did_0E15_SCR_DISTANCE);
		
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E15_SCR_DISTANCE, receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadOilDistance(uint32 ch, uint8* respData)
{
	tDid_0E16_OIL_DISTANCE Did_0E16_OIL_DISTANCE = {0};
	uint16 receiveLen = sizeof(Did_0E16_OIL_DISTANCE);
	
	DEA_Diag_GetDid_0E16_OIL_DISTANCE((uint8*)&Did_0E16_OIL_DISTANCE);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E16_OIL_DISTANCE, receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadOilReset(uint32 ch, uint8* respData)
{
	

	return 0;
}



uint16 IC_sid0x22ReadAbsoluteTime(uint32 ch, uint8* respData)
{
	tDid_0E18_ABSOLUTE_TIME Did_0E18_ABSOLUTE_TIME = {0};
	uint16 receiveLen = sizeof(Did_0E18_ABSOLUTE_TIME);
	
	DEA_Diag_GetDid_0E18_ABSOLUTE_TIME((uint8*)&Did_0E18_ABSOLUTE_TIME);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E18_ABSOLUTE_TIME, receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E19Info(uint32 ch, uint8* respData)
{
	tDid_0E19_Info Did_0E19_Info = {0};
	uint16 receiveLen = sizeof(Did_0E19_Info);
	
	DEA_Diag_GetDid_0E19_Info((uint8*)&Did_0E19_Info);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E19_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadAsstChg(uint32 ch, uint8* respData)
{
	tDid_0E1A_ASST_CHG Did_0E1A_ASST_CHG = {0};
	uint16 receiveLen = sizeof(Did_0E1A_ASST_CHG);
	
	DEA_Diag_GetDid_0E1A_ASST_CHG((uint8*)&Did_0E1A_ASST_CHG);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1A_ASST_CHG), receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadEvIndicator(uint32 ch, uint8* respData)
{
	tDid_0E1B_EV_IND Did_0E1B_EV_IND = {0};
	uint16 receiveLen = sizeof(Did_0E1B_EV_IND);
	
	DEA_Diag_GetDid_0E1B_EV_IND((uint8*)&Did_0E1B_EV_IND);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1B_EV_IND), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadApoDisplay(uint32 ch, uint8* respData)
{
	tDid_0E1C_APO_DISPLAY Did_0E1C_APO_DISPLAY = {0};
	uint16 receiveLen = sizeof(Did_0E1C_APO_DISPLAY);
	
	DEA_Diag_GetDid_0E1C_APO_DISPLAY((uint8*)&Did_0E1C_APO_DISPLAY);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1C_APO_DISPLAY), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadSocDisplay(uint32 ch, uint8* respData)
{
	tDid_0E1D_SOC_DISPLAY Did_0E1D_SOC_DISPLAY = {0};
	uint16 receiveLen = sizeof(Did_0E1D_SOC_DISPLAY);
	
	DEA_Diag_GetDid_0E1D_SOC_DISPLAY((uint8*)&Did_0E1D_SOC_DISPLAY);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1D_SOC_DISPLAY), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadEcoApo(uint32 ch, uint8* respData)
{
	tDid_0E1E_ECO_APO Did_0E1E_ECO_APO = {0};
	uint16 receiveLen = sizeof(Did_0E1E_ECO_APO);
	
	DEA_Diag_GetDid_0E1E_ECO_APO((uint8*)&Did_0E1E_ECO_APO);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1E_ECO_APO), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadRegenBrakeTrq(uint32 ch, uint8* respData)
{
	tDid_0E1F_REGEN_BRAKE_TRQ Did_0E1F_REGEN_BRAKE_TRQ = {0};
	uint16 receiveLen = sizeof(Did_0E1F_REGEN_BRAKE_TRQ);
	
	DEA_Diag_GetDid_0E1F_REGEN_BRAKE_TRQ((uint8*)&Did_0E1F_REGEN_BRAKE_TRQ);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E1F_REGEN_BRAKE_TRQ), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadMechaBrakeTrq(uint32 ch, uint8* respData)
{
	tDid_0E20_MECHA_BRAKE_TRQ Did_0E20_MECHA_BRAKE_TRQ = {0};
	uint16 receiveLen = sizeof(Did_0E20_MECHA_BRAKE_TRQ);
	
	DEA_Diag_GetDid_0E20_MECHA_BRAKE_TRQ((uint8*)&Did_0E20_MECHA_BRAKE_TRQ);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E20_MECHA_BRAKE_TRQ), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadEnergyFlow(uint32 ch, uint8* respData)
{
	tDid_0E21_ENERGY_FLOW Did_0E21_ENERGY_FLOW = {0};
	uint16 receiveLen = sizeof(Did_0E21_ENERGY_FLOW);
	
	DEA_Diag_GetDid_0E21_ENERGY_FLOW((uint8*)&Did_0E21_ENERGY_FLOW);
	
	if(respData != NULL_PTR)
		diagReveCpyData(respData, (uint8*)&Did_0E21_ENERGY_FLOW, receiveLen);
	
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E22Info(uint32 ch, uint8* respData)
{
	tDid_0E22_Info Did_0E22_Info = {0};
	uint16 receiveLen = sizeof(Did_0E22_Info);
	
	DEA_Diag_GetDid_0E22_Info((uint8*)&Did_0E22_Info);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E22_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadOilDistK9k(uint32 ch, uint8* respData)
{
	uint16 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E23_OIL_DIST_K9K((uint8*)&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E24Info(uint32 ch, uint8* respData)
{
	tDid_0E24_Info Did_0E24_Info = {0};
	uint16 receiveLen = sizeof(Did_0E24_Info);
	
	DEA_Diag_GetDid_0E24_Info((uint8*)&Did_0E24_Info);
	
	if(respData != NULL_PTR)
			diagCpyData(respData, (const uint8*)(&Did_0E24_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadTirePressFr(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E25_Tire_Press_FR(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadTirePressFl(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E26_Tire_Press_FL(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadTirePressRr(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E27_Tire_Press_RR(&receive);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
			
	return receiveLen;
}



uint16 IC_sid0x22ReadTirePressRl(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E28_Tire_Press_RL(&receive);
			
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
				
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E29Info(uint32 ch, uint8* respData)
{
	tDid_0E29_Info Did_0E29_Info = {0};
	uint16 receiveLen = sizeof(Did_0E29_Info);
	
	DEA_Diag_GetDid_0E29_Info((uint8*)&Did_0E29_Info);
		
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E29_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadRamErrorData(uint32 ch, uint8* respData)
{
	uint8 receive = 0;
	uint16 receiveLen = sizeof(receive);
	
	DEA_Diag_GetDid_0E2A_RAM_error_data(&receive);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&receive), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E2BInfo(uint32 ch, uint8* respData)
{
	tDid_0E2B_Info Did_0E2B_Info = {0};
	uint16 receiveLen = sizeof(Did_0E2B_Info);
	
	DEA_Diag_GetDid_0E2B_Info((uint8*)&Did_0E2B_Info);
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E2B_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDid0E2CInfo(uint32 ch, uint8* respData)
{
	tDid_0E2C_Info Did_0E2C_Info = {0};
	uint16 receiveLen = sizeof(Did_0E2C_Info);
	
	DEA_Diag_GetDid_0E2C_Info((uint8*)&Did_0E2C_Info);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E2C_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadDisplayNondisplayInfo(uint32 ch, uint8* respData)
{
	tDid_0E80_Info Did_0E80_Info = {0};
	uint16 receiveLen = sizeof(Did_0E80_Info);
	DEA_Diag_GetDid_0E80_Info((uint8*)&Did_0E80_Info);

	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(&Did_0E80_Info), receiveLen);
		
	return receiveLen;
}



uint16 IC_sid0x22ReadWarningLampLightingRecords(uint32 ch, uint8* respData)
{
	uint8 i = 0;
	uint8 bufTemp[INNER_WL_ID_MAX] = {0};
	for(i=0; i<INNER_WL_ID_MAX; i++)
	{
		bufTemp[i] = gWarningLampLightingRecords[i].ignCounter;
	}
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)bufTemp, INNER_WL_ID_MAX);
			
	return INNER_WL_ID_MAX;
}



uint16 IC_sid0x22ReadWarningLampLightingMileage(uint32 ch, uint8* respData)
{
	uint8 i = 0;
	uint8* p = NULL;
	uint32 temp = 0;
	uint8 bufTemp[INNER_WL_ID_MAX*3] = {0};
	for(i=0; i<INNER_WL_ID_MAX; i++)
	{
		temp = gWarningLampLightingRecords[i].mileage;
		p = (uint8*)(&temp);
		bufTemp[i*3]   = *(p+2);
		bufTemp[i*3+1] = *(p+1);
		bufTemp[i*3+2] = *p;
	}
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)bufTemp, INNER_WL_ID_MAX*3);
	
	return INNER_WL_ID_MAX*3;
}



uint16 IC_sid0x22ReadCanCommunicationSupportInfo(uint32 ch, uint8* respData)
{
	
	


	diagResetBuffer(respData,0xFF,140);

	respData[0] = 0x33;	
	respData[1] = 0x31;	

	respData[2] = (uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_BUS_OFF]&0xFF);
	respData[3] = (uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_TX_MUTE]&0xFF);
	respData[4] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_ECM]&0xFF));
	respData[5] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC]&0xFF));
	respData[6] = 0x00;
	respData[7] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_BCM]&0xFF));
	respData[8] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_ACU]&0xFF));
	
	respData[10] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_RADAR]&0xFF));
	
	respData[12] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_CVT]&0xFF));
	
	respData[16] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_VDC]&0xFF));

	respData[21] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_EPS]&0xFF));
	respData[23] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_IPDM]&0xFF));
	respData[24] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_TPMS]&0xFF));
	respData[35] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_IKEY]&0xFF));
	respData[44] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_BSW]&0xFF));
	respData[45] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA]&0xFF));
	respData[85] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_BSW]&0xFF));
	respData[97] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_SCU]&0xFF));
	respData[134] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_LCMR]&0xFF));
	respData[135] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gIcCanFaultBitFlagArray[IC_FAULT_INDEX_RX_CAN_MSG_LCML]&0xFF));

	return IC_DID_CAN_COMMUNICATION_SUPPORT_INFO_LEN;

}



uint16 IC_sid0x22ReadSsdTotalMileage(uint32 ch, uint8* respData)
{
	return TRUE;
}



uint16 IC_sid0x22ReadSsdBatVoltage(uint32 ch, uint8* respData)
{
	return TRUE;
}



uint16 IC_sid0x22ReadSsdIgnVoltage(uint32 ch, uint8* respData)
{
	return TRUE;
}



uint16 IC_sid0x22ReadSsdAgingCounter(uint32 ch, uint8* respData)
{
	return TRUE;
}



uint16 IC_sid0x22ReadSsdVehicleSpeed(uint32 ch, uint8* respData)
{
	return TRUE;
}

uint16 IC_sid0x22ReadUUID(uint32 ch, uint8* respData)
{
	uint8 temp[3] = {0x20, 0x20, 0x20};
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (const uint8*)temp, 3);
		diagCpyData(respData+3, (const uint8*)(socSerialNum), IC_DID_SSD_SYS_UUID_LEN);
	}

	return IC_DID_SSD_SYS_UUID_LEN+3;
}

uint16 IC_sid0x22ReadEQPIN(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (const uint8*)(&gITMasterCtrlVersion), IC_DID_SSD_SYS_EQPINL_LEN);
	}
	return IC_DID_SSD_SYS_EQPINL_LEN;

}






uint16 AVM_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)g_AVM_UdsInnerTestDid, AVM_DID_UDS_INNER_TEST_LEN);
		
	return AVM_DID_UDS_INNER_TEST_LEN;
}



uint16 AVM_sid0x22ReadProductionLine(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)g_AVM_ProductionLine, AVM_DID_PRODUCTION_LINE_LEN);

	return AVM_DID_PRODUCTION_LINE_LEN;
}

tUdsNRCType AVM_sid0x2eWriteProductionLine(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 i=0, j=0;
	uint8 uReqData[2];
	if(value==NULL_PTR || len!=AVM_DID_PRODUCTION_LINE_LEN)
		return FALSE;

	for(i=0; i<len; i++)
	{
		g_AVM_ProductionLine[j] = value[i];
		j++;
	}
	diagNvmWriteSync(g_AVM_ProductionLineNvMBlockID, g_AVM_ProductionLine);


	uReqData[0] = 0;
	uReqData[1] = g_AVM_ProductionLine[0];
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_DIAGNOSTIC_CTRL_REQ, uReqData, 2); 

	return NRC_NONE;
}



uint16 AVM_sid0x22ReadDataMonitorServiceRequest(uint32 ch, uint8* respData)
{
	
	uint8 retval;
	uint8 data = 0;

	if(respData != NULL_PTR)
	{
		diagResetBuffer(respData, 0, 16);
		respData[0] |= ((gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_STRG]&AVM_RX_CAN_MSG_MASK_STRG_0X2 ? 0:1))<<7;	

		if(gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_CVT]&AVM_RX_CAN_MSG_MASK_CVT_0X421)	
			respData[0] |= 0;
		else
		{
			Com_RxSig_GetIND_RNG(&data);
			if(data ==2)
				respData[0] |= 1<<6;
			else
				respData[0] |= 0;
		}

		respData[0] |= (gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_ABS_VDC]&AVM_RX_CAN_MSG_MASK_ABS_0X284 ? 0:1)<<5;
		
		
		
		
		
		if(gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_BCM]&AVM_RX_CAN_MSG_MASK_BCM_0X60D)
			respData[0] |= 0;
		else
		{
			
			Com_RxSig_GetTAIL_L_REQ(&data);
			if(data ==1)
				respData[0] |= 1<<4;
			else
				respData[0] |= 0;

			
			retval = Com_RxSig_GetTURN_IND(&data);
			if(retval == E_NOT_OK || data>2)
				respData[0] |= 3;
			else
				respData[0] |= data;
		}

		
		
		
		
		
		if(gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_CVT]&AVM_RX_CAN_MSG_MASK_CVT_0X421)	
			respData[1] |= 0;
		else
		{
			Com_RxSig_GetIND_RNG(&data);
			if((data ==4) || (data == 5) || (data &0x10))
				respData[1] |= 1<<5;
			else
				respData[1] |= 0;
		}

		
		
		
		
		
		respData[2] |= (((gDtcStatusAndExtInforArray[UDS_AVM][AVM_DTC_ID_INDEX_FRONT_CAMERA_VIDEO_INPUT].statusBit & DTC_STATUS_BIT_TEST_FAILED) == 0)?0:1)<<7; 
		respData[2] |= (((gDtcStatusAndExtInforArray[UDS_AVM][AVM_DTC_ID_INDEX_REAR_CAMERA_VIDEO_INPUT].statusBit & DTC_STATUS_BIT_TEST_FAILED) == 0)?0:1)<<6;  
		respData[2] |= (((gDtcStatusAndExtInforArray[UDS_AVM][AVM_DTC_ID_INDEX_LEFT_CAMERA_VIDEO_INPUT].statusBit & DTC_STATUS_BIT_TEST_FAILED) == 0)?0:1)<<5;  
		respData[2] |= (((gDtcStatusAndExtInforArray[UDS_AVM][AVM_DTC_ID_INDEX_RIGHT_CAMERA_VIDEO_INPUT].statusBit & DTC_STATUS_BIT_TEST_FAILED) == 0)?0:1)<<4; 
		
	}

	
	
	return AVM_DID_DATA_MONITOR_SERVICE_REQUEST_LEN;
}



uint16 AVM_sid0x22ReadCalibrationState(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)gAVM_CalibrationState, AVM_DID_CALIBRATION_STATE_LEN);
			
	return AVM_DID_CALIBRATION_STATE_LEN;
}



uint16 AVM_sid0x22ReadProgramingDate(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(g_AVM_ProgramingDate), AVM_DID_PROGRAMING_DATE_LEN);
				
	return AVM_DID_PROGRAMING_DATE_LEN;
}


uint16 AVM_sid0x22ReadCanDiagMuxState(uint32 ch, uint8* respData)
{
	diagResetBuffer(respData,0xFF,140);

	respData[0] = 0x53;  
	respData[1] = 0x34;  
	
	respData[2] = (uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_BUS_OFF]&0xFF);  			
	
	respData[3] = 0; 

	respData[4] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_HCM]&0xFF));       

	respData[5] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_ABS_VDC]&0xFF));   

	respData[7] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_BCM]&0xFF));       

	respData[12] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_CVT]&0xFF));		
	respData[13] = ((gDiagMuxOnCurState==0) ? 0xFF:0);

	
	respData[17] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_STRG]&0xFF));		
	
	respData[44] = ((gDiagMuxOnCurState==0) ? 0xFF:(uint8)(gAvmCanFaultBitFlagArray[AVM_FAULT_INDEX_RX_CAN_MSG_BSW]&0xFF));		
			
	return AVM_DID_CAN_DIAG_MUX_STATE_LEN;
}

#if 0

uint16 AVM_sid0x22ReadSparePartsNumNissan(uint32 ch, uint8* respData)
{
	uint8 ampVersion = 0;
	if(respData != NULL_PTR)
	{
		ampVersion = getVariantCtrlVerion();
		if(N1210 == ampVersion)
			diagCpyData(respData, "259152GS0A;", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); 
		else
			diagCpyData(respData, "259152GS1A;", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); 
	}
	return AVM_DID_SPARE_PARTS_NUM_NISSAN_LEN;
}
#endif

#if 0

uint16 AVM_sid0x22ReadVehicleManufEcuHwNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_SparePartsNumNissan, IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); 
						
	return AVM_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN;
}
#endif


#if 0
uint16 AVM_sid0x22ReadOperationalReference(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_AVM_OperationalReference, AVM_DID_OPERATIONAL_REFERENCE_LEN);
							
	return AVM_DID_OPERATIONAL_REFERENCE_LEN;
}
#endif


#if 0
uint16 AVM_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_AVM_SysSupplierNum, AVM_DID_SYS_SUPPLIER_NUM_LEN);
							
	return AVM_DID_SYS_SUPPLIER_NUM_LEN;
}
#endif


#if 0
uint16 AVM_sid0x22ReadEcuSerialNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_IVI_EcuSerialNum, AVM_DID_ECU_SERIAL_NUM_LEN); 
							
	return AVM_DID_ECU_SERIAL_NUM_LEN;
}
#endif


uint16 AVM_sid0x22ReadSysSupplierEcuSwVersionNum(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(g_AVM_SysSupplierEcuSwVersionNum), AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN);
							
	return AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN;
}


uint16 AVM_sid0x22ReadEncryptICStatus(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(g_AVM_EncryptedIC_Status), AVM_DID_ENCRYPT_IC_STATUS_LEN);

	return AVM_DID_ENCRYPT_IC_STATUS_LEN;
}


uint16 AVM_sid0x22ReadOriginalStatus(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, (const uint8*)(g_AVM_Original_Status), AVM_DID_ORIGINAL_STATUS_LEN);

	return AVM_DID_ORIGINAL_STATUS_LEN;
}


#if 0
uint16 AVM_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData)
{
	if(respData != NULL_PTR)
		diagCpyData(respData, g_AVM_DiagIdCode, AVM_DID_DIAG_ID_CODE_LEN);
							
	return AVM_DID_DIAG_ID_CODE_LEN;
}
#endif

#if 0

uint16 AVM_sid0x22ReadBootVersion(uint32 ch, uint8* respData)
{
	
		
							
	return AVM_DID_BOOT_VERSION_LEN;
}
#endif



tUdsNRCType verifyDate(uint16 didNum, const uint8* date, uint16 len)
{
	uint8 year;
	uint8 month;
	uint8 day;
	
	if(date == NULL_PTR || len<3)
		return FALSE;
	year = date[0];
	month = date[1];
	day = date[2];

	if(year>0x99 || month==0 || month>0x12 || day==0 || day>0x31)
		return NRC_REQUEST_OUT_OF_RANGE;

	return NRC_NONE;
}



tUdsNRCType HUD_sid0x2eWriteMainSWONOFF(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{

	uint8 uReqData[len+3];

	

	uReqData[0] = 0x2E;
	uReqData[1] = 0x01;
	uReqData[2] = (didNum & 0xFF);
	uReqData[3] = value[0];
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $2E Turn HUD main SW ON/OFF command send to HUD via IPC!\r\n");
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_MAIN_SW_ON_OFF_REQ, uReqData, len+3); 

	return NRC_NONE;
}

tUdsNRCType HUD_sid0x2eWriteCalibrationData(uint32 ch, uint16 didNum, const uint8* value, uint16 len)
{
	uint8 uDataLen = len/2;
	uint8 uReqData_1[uDataLen+3];
	uint8 uReqData_2[uDataLen+3];

	

	uReqData_1[0] = 0x2E;
	uReqData_1[1] = 0x01;
	uReqData_1[2] = 0xA2;
	diagCpyData(uReqData_1+3, (const uint8*)value,uDataLen);

	uReqData_2[0] = 0x2E;
	uReqData_2[1] = 0x01;
	uReqData_2[2] = 0xA2;
	diagCpyData(uReqData_2+3, (const uint8*)(value+uDataLen),uDataLen);
	
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $2E Write calibration data command send to HUD via IPC!\r\n");
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_WRITE_CAL_DATA_REQ, uReqData_1, uDataLen+3); 

	DelayMs(10);
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_WRITE_CAL_DATA_REQ, uReqData_2, uDataLen+3);

	return NRC_NONE;

}

