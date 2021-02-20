/**************************
description: public general process APIs for sid 0x34/0x36/0x37 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/11
**************************/
	
#include"download.h"
//#include"gsc0x22.h"
//#include"uds_cfg.h"
//#include"codeflash.h"
//#include"gsc0x27.h"
#include "Flash_if.h"
#include "Spi_Ipc_gw_api.h"
#include "system_S32K148.h"
#include "Eeprom.h"
#include "Trace.h"
#include "system_portable.h"
#include "BoardDefines.h"



//extern flash_ssd_config_t pSSDConfig;

#define SID_ALL_NUM 6
#define SPIIPC_BUFF_LEN (69)
#define SPIIPCCHANNEL 2
#define SPIIPCHARDCHANNEL   1
#define SPIIPCFUNID 2
#define SPIIPCFEAID 2
#define DATAFRAMESIZE 64
#define DATAFRAMECRCSIZE 2


uint16 gFrameNumToWrite=0;
uint16 gRxFunId = 0;
request_context_t Rx_Ctx;

uint8 gCompressMethod = 0;
uint8 gEncryptionMethod = 0;
uint32 gMemoryAddress = 0;
uint32 gMemorySize = 0;
boolean gReqDownloadRxFlag = FALSE;
boolean gTransferExitRxFlag = FALSE;
boolean gWaitforTransferExitReqFlag = FALSE;
boolean gWriteFlashIsStartedFlag = FALSE;
uint16 gEptDownloadBlockSeq = 1;
uint32 gCurDownloadDataLen = 0;


uint16 gDownloadMaxNumOfBlock = 0;
uint16 gMaxSeq = 0;
uint16 gCurSeq = 0;
uint16 gUpdataRate=0;

//extern boolean gProgPreConditionFullFilled;
//extern boolean gProgrammingFingerprintUpdated;
uint8 gUpdataBigBuffer[4096] = {1};

uint8 gIPCBuffer[SPIIPC_BUFF_LEN]={0};
uint8 gGetIPCDataFlag=false;
uint16 gGetIPCDataLen=0;
request_context_t ReplyCtx;

uint32 gClearUpdataFlag = 0;

static tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len);

void SWDl_IPCCallback(uint16 function, uint8 *rx_data, uint32 len);
void SWDl_ReqIPCCallback(const void *ctx, uint8 *rx_data, uint32 len);
void SWDl_NotTestCallback(uint16 function, uint8 *rx_data, uint32 len);


typedef enum{
	SID_ECU_RESET = 0x11,
	SID_READ_DID_DATA = 0x22,
	SID_WRITE_DATA_BY_DID = 0x2E,
	SID_ROUTINE_CONTROL = 0x31,
	SID_REQUEST_DOWNLOAD = 0x34,
	SID_TRANSFER_DATA = 0x36,
	SID_REQUEST_TRANSFER_EXIT = 0x37,
}tSidType;

typedef enum{
	SYS_APP_STATUS = 0x00,
	SYS_USB_SWDL_STATUS=0x01,
	SYS_MFG_SWDL_STATUS=0x02
}SysStatus;
	
void SetUsbSWDLPortFlag()
{
	//PINS_DRV_SetPins(PORT_IOC_BOOT_MODE, (1 << PORT_PIN_IOC_BOOT_MODE));
	Port_SetOutLevel(IOC_BOOT_MODE, PORT_LOW);

}

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

void IPC_Test_callback(const void *ctx, uint8 *rx_data, uint32 len)
{
#if    1
	uint8 buff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

   // SYSTIM  l_curTime;
    uint16 curTime[2];

    request_context_t *l_ctx = (request_context_t*)ctx;
	
    switch(l_ctx->function_id) {
	
		case 101:
		  // StartSend = 1;
		   ipc_gw_send_reply(1,ctx,rx_data,len);
		//TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Task_Spi]:Reply = 6\r\n");
			break;


		case 102:
			//StartSend = 0;
			ipc_gw_send_reply(1,ctx,rx_data,len);
			break;
    }
#endif
}


void downloadInit()
{
	gDownloadMaxNumOfBlock = 64+2;//0x802
	gWriteFlashIsStartedFlag = FALSE;
	gReqDownloadRxFlag = FALSE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gMaxSeq = 0;
	gWaitforTransferExitReqFlag = FALSE;


	ipc_gw_register(SPIIPCCHANNEL, SWDl_IPCCallback,NULL);
	//ipc_gw_register(2, SWDl_NotTestCallback, IPC_Test_callback);
	//SetUsbSWDLPortFlag();
}


tUdsNRCType sid0x34CheckMainPreCondition()
{	

	//if(gProgrammingFingerprintUpdated == FALSE)
		//return NRC_CONDITION_NOT_CORRECT;
	#ifdef UDS_WITH_SID0X27
	if(sid0x27GetInnerSecurityLevel()!=UDS_INNER_SECURITY_LEVEL_4)
		return NRC_SECURITY_ACCESS_REQUEST;
	#endif
	
	return NRC_NONE;
}



tUdsNRCType sid0x34VerifyData(const uint8* data, uint16 len)
{
	//uint8 compressMethod = 0;
	//uint8 encryptionMethod = 0;
	uint16 lenMemSize = 0;
	uint16 lenMemAddr = 0;
	uint32 rest = 0;
	

	//return NRC_NONE;	

	if(data == NULL || len <11)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	gCompressMethod   = (0xF0&data[1])>>4; // 0--uncompressed,1--vector compress
	gEncryptionMethod = 0x0F&data[1];      //0 -- unEncrypted

	if(gCompressMethod !=0 || gEncryptionMethod != 0)
		return NRC_REQUEST_OUT_OF_RANGE;

	lenMemAddr = (0xF0&data[2])>>4;
	lenMemSize = 0xF&data[2];

	if(lenMemAddr<=4 && lenMemSize<=4)
	{
		switch(lenMemAddr)
		{
		case 1:
			gMemoryAddress = data[3];
			break;
		case 2:
			gMemoryAddress = (uint16)(data[3]<<8|data[4]);
			break;
		case 4:
			gMemoryAddress = (uint32)(data[3]<<24|data[4]<<16|data[5]<<8|data[6]);
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}

		switch(lenMemSize)
		{
		case 1:
			gMemorySize = data[7];
			break;
		case 2:
			gMemorySize = (uint16)(data[7]<<8|data[8]);
			break;
		case 4:
			gMemorySize = (uint32)(data[7]<<24|data[8]<<16|data[9]<<8|data[10]);
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
	}
	else
		return NRC_REQUEST_OUT_OF_RANGE;

	// check if mem addr is valid
	if(gMemoryAddress%2 != 0)//addr alignment with even number
		return NRC_REQUEST_OUT_OF_RANGE;

	#if 0
	if(gMemorySize>gDownloadCfgInfor.downloadDataTotalLen)
		return NRC_REQUEST_OUT_OF_RANGE;
	#endif
	
	downloadResetStatus();

	gMaxSeq = gMemorySize/DATAFRAMESIZE;//UDS_PHY_BUFF_LEN;
	rest = gMemorySize%DATAFRAMESIZE;//UDS_PHY_BUFF_LEN;
	if(rest !=0)
		gMaxSeq += 1;
	gReqDownloadRxFlag = TRUE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gWaitforTransferExitReqFlag = FALSE;
	TracePrintf(TRACE_TRACE, TRACE_INFO, "sid0x34VerifyData: gMemorySize = %d\r\n",gMemorySize);
	return NRC_NONE;	
}




tUdsNRCType sid0x36CheckMainPrecondition()
{
#if 0
	tDiagSysVoltageStatus sysVolStatus;

	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		sysVolStatus=diagCheckSysVoltageStatus();
		if(sysVolStatus== DIAG_SYS_VOLTAGE_STATUS_LOW)
			return NRC_VOLTAGE_TOO_LOW;
		else if(sysVolStatus== DIAG_SYS_VOLTAGE_STATUS_HIGH)
			return NRC_VOLTAGE_TOO_HIGH;
		else
			return NRC_CONDITION_NOT_CORRECT;
	}
#endif	
	return NRC_NONE;
}
Std_ReturnType sid0x36CheckCRC(const uint8* data, uint16 len)
{


	return E_OK;
}


tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len)
{
	static uint32 nextMemAddr = 0;
	static uint32 nextMemSize = 0;
	static uint16 bufferNum = 0;
	static uint32 dstAddr = 0;
	uint8 ret=0;
	
	if(data == NULL || len==0)
		return NRC_INCORRECT_MESSAGE_LENGTH;
#if 0
	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		//sid0x36ResetStatus();
		return NRC_CONDITION_NOT_CORRECT;
	}
#endif
	if(gWriteFlashIsStartedFlag == FALSE)
	{
		nextMemAddr = 0x32000;//pSSDConfig.PFlashBase;//gMemoryAddress;
		nextMemSize = len;
		
		gWriteFlashIsStartedFlag = TRUE;
	}
	else
	{
		if(sameBlockFlag == FALSE)
		{
			nextMemAddr += nextMemSize;
		}
		nextMemSize = len;
	}
	
	//here,to fix the address align issue, 
	//the platform system DATA is align to 1 byte in startup_MB91526.asm
	//and also change the type of global buffer 'sUDSRxBuffer' to uint16 [for even addr]
	// TODO: the len must be 2048*N bytes
	
	#ifdef USING_OS_FREERTOS
	if(dstAddr == 0)
		dstAddr = 0x32000;//pSSDConfig.PFlashBase;
	
    if(bufferNum<(4096/DATAFRAMESIZE))
    {
		diagCpyData(gUpdataBigBuffer+bufferNum*DATAFRAMESIZE,data,len);
		bufferNum++;
		if(gEptDownloadBlockSeq<=gMaxSeq)//not last fragment
		{
			if(bufferNum>=(4096/DATAFRAMESIZE))
				bufferNum = 0;
			else
				return NRC_NONE;
		}
	}

	//TracePrintf(TRACE_TRACE, TRACE_INFO, "sid0x36WriteData: bufferNum = %d\r\n",bufferNum);

	
	//TracePrintf(TRACE_TRACE, TRACE_ERRO, "curLen:%d,sumLen:%d!\r\n",gCurDownloadDataLen,gMemorySize);
	//if(gCurDownloadDataLen == gMemorySize)
		//TracePrintf(TRACE_TRACE, TRACE_ERRO, "got all datas!\r\n");
	portENTER_CRITICAL();
	ret = Flash_Sector_Programing(dstAddr,gUpdataBigBuffer,4096);
	portEXIT_CRITICAL();
	//TracePrintf(TRACE_TRACE, TRACE_INFO, "sid0x36WriteData Flash_Sector_Programing ret = %d\r\n",ret);
	if(ret != STATUS_SUCCESS)
	{
	    TracePrintf(TRACE_TRACE, TRACE_ERRO, "Write Flash Error!\r\n");
		bufferNum--;
		return NRC_GENERAL_PROGRAMMING_FAILURE;
	}
	dstAddr+= 4096;
	
	#endif

	return NRC_NONE;
		
}


void downloadResetStatus()
{
	gWriteFlashIsStartedFlag = FALSE;
	gReqDownloadRxFlag = FALSE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gMaxSeq = 0;
	gWaitforTransferExitReqFlag = FALSE;
}

tUdsNRCType sid0x36MainProcess(const uint8* data, uint16 len)
{
	uint16 blockSeq = 0;
	tUdsNRCType ret = NRC_NONE;
	uint16 realDataLen = 0;
	uint32 curTempTotalLen = 0;
	
	if(data == NULL || len <3)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	realDataLen = len - 3 - DATAFRAMECRCSIZE;//there no use crc
	blockSeq = (uint16)(data[1]|(data[2]<<8));
	gCurSeq  = blockSeq;
	//TracePrintf(TRACE_TRACE, TRACE_INFO, "sid0x36MainProcess: blockSeq = %d\r\n",blockSeq);
	if(gReqDownloadRxFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;
	
	if((ret=sid0x36CheckMainPrecondition()) != NRC_NONE)
		return ret;
	
	//SRS_flashing_specification_M85_V1.12_20161220.pdf Pg40
	if(gEptDownloadBlockSeq>1 && (blockSeq == (gEptDownloadBlockSeq-1)))//rx repeated last block data
	{
		if((blockSeq<gMaxSeq && realDataLen!=(gDownloadMaxNumOfBlock-2))
			||(blockSeq==gMaxSeq && (gCurDownloadDataLen-realDataLen)!=(gMemorySize-realDataLen)))//maxNumberOfBlockLength
		{
			return NRC_INCORRECT_MESSAGE_LENGTH;
		}
		
		if((ret = sid0x36WriteData(TRUE,data+3,realDataLen)) != NRC_NONE)
			return ret;
	}
	else
	{
		if(gWaitforTransferExitReqFlag == FALSE)
		{
			
			if(blockSeq != gEptDownloadBlockSeq)
				return NRC_WRONG_BLOCK_SEQ_COUNTER;
			
			if(blockSeq<gMaxSeq && realDataLen!=(gDownloadMaxNumOfBlock-2))//maxNumberOfBlockLength
			{
				//downloadResetStatus();
				return NRC_INCORRECT_MESSAGE_LENGTH;
			}

			curTempTotalLen = gCurDownloadDataLen+realDataLen;
			if((blockSeq<gMaxSeq &&  curTempTotalLen>= gMemorySize)
				|| (gMaxSeq == blockSeq && curTempTotalLen != gMemorySize))
			{
				return NRC_TRANSFER_DATA_SUSPENDED;
			}

			gCurDownloadDataLen += realDataLen;
			gEptDownloadBlockSeq += 1;
			if((ret = sid0x36WriteData(FALSE,data+3,realDataLen)) != NRC_NONE)
			{
				gCurDownloadDataLen -= realDataLen;
				gEptDownloadBlockSeq -= 1;
				return ret;
			}
			else
			{
				if(gCurDownloadDataLen == gMemorySize)
				{
					gWaitforTransferExitReqFlag = TRUE;
					TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: got all datas,len = %d\r\n",gCurDownloadDataLen);
				}
				//TracePrintf(TRACE_TRACE, TRACE_INFO, "sid0x36MainProcess: gCurDownloadDataLen = %d\r\n",gCurDownloadDataLen);
			}
		}
		else
			return NRC_REQUEST_SEQUENCE_ERROR;
	}
	
	return NRC_NONE;
}

tUdsNRCType sid0x37MainProcess(const uint8*data, uint16 len)
{
	if(data == NULL || len != 1)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(/*gReqDownloadRxFlag == FALSE || */gWaitforTransferExitReqFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;

	downloadResetStatus();
	
	return NRC_NONE;
}


void SWDl_IPCCallback(uint16 function, uint8 *rx_data, uint32 len)
{
	//ipc_send_notification(1,2,function,rx_data,len);
	gIPCBuffer[0]=0;
	diagCpyData(gIPCBuffer,rx_data,len);
	gGetIPCDataFlag = true;
	gGetIPCDataLen = len;
	gRxFunId = function;
	//TracePrintf(TRACE_TRACE, TRACE_INFO, "in ipccbk len = %d\r\n",len);
}

void SWDl_NotTestCallback(uint16 function, uint8 *rx_data, uint32 len)
{
	ipc_send_notification(1,2,function,rx_data,len);
	//TracePrintf(TRACE_TRACE, TRACE_INFO, "in ipccbk len = %d\r\n",len);
}


void SWDl_ReqIPCCallback(const void *ctx, uint8 *rx_data, uint32 len)
{
	uint8 buff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    uint16 curTime[2];

    request_context_t *l_ctx = (request_context_t*)ctx;
#if   0
	gIPCBuffer[0]=0;
	diagCpyData(gIPCBuffer,rx_data,len);
	gGetIPCDataFlag = true;
	gGetIPCDataLen = len;
	gRxFunId = l_ctx->function_id;
	Rx_Ctx = *l_ctx;

#else
	#if    0
    switch(l_ctx->function_id) {
	
		case 101:
		   ipc_gw_send_reply(1,ctx,rx_data,len);
		//TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Task_Spi]:Reply = 6\r\n");
			break;

		case 102:
			ipc_gw_send_reply(1,ctx,rx_data,len);
			break;
    }
	#else
	ipc_gw_send_reply(1,ctx,rx_data,len);
	#endif
#endif
}


void SWDL_IPCSendAck(uint8 *Data,uint16 Len)
{
	ipc_send_notification(SPIIPCHARDCHANNEL,SPIIPCCHANNEL,gRxFunId,Data,Len);
	//ipc_gw_send_reply(SPIIPCHARDCHANNEL,&Rx_Ctx.channel,Data,Len);	
	//TracePrintf(TRACE_TRACE, TRACE_INFO, "SWDL_IPCSendAck: Cmd= %x,Len = %x\r\n",Data[0],Len);
}

void SWDL_IPCSendNAck(uint8 Cmd,uint8 Ecode)
{
	uint8 buff[3]={0};
	buff[0] = 0x7f;
	buff[1] = Cmd;
	buff[2] = Ecode;
	ipc_send_notification(SPIIPCHARDCHANNEL,SPIIPCCHANNEL,gRxFunId,buff,3);
	//ipc_gw_send_reply(SPIIPCHARDCHANNEL,&Rx_Ctx.channel,buff,3);
	//TracePrintf(TRACE_TRACE, TRACE_ERRO, "SWDL_IPCSendNAck: Cmd= %x,Ecode = %x\r\n",Cmd,Ecode);
}

void PowerOff(void)
{
#if 1
	//======================================SOC : BOOT========================================
	//info soc in normal mode
	//Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);

	//======================================SOC : POWER OFF start=============================
	//======================================SOC : RESET=======================================
	//when power off soc , must first set reset then power off
	//if( gCircuitVersion > 0 )
		Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
	//else
	//	Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);

	//======================================SOC : OTHER=======================================
	Port_SetOutLevel(UDBATT_AD_EN,PORT_LOW);//batery voltage discontact to s32k adc pin
	Port_SetOutLevel(IOC_FUEL_EN, PORT_LOW);//disenable fuel
	Port_SetOutLevel(FAN_EN,PORT_LOW);//disenable fan control 
	Port_SetOutLevel(WDI_EN,PORT_HIGH); //disable WatchDog
	
	//======================================SOC : AMP=========================================
	//internal AMP
	Port_SetOutLevel(AMP_D_EN,PORT_LOW);
	Port_SetOutLevel(AMP_D_MUTEN,PORT_LOW);
	//external AMP
	Port_SetOutLevel(EX_AMP_EN,PORT_LOW);

	//======================================PCI : CAN1 CAN2 ======================================
	//remove here to solve can interface ack from sleep less than 45ms
	Port_SetOutLevel(CAN_STB1,PORT_LOW);
	Port_SetOutLevel(CAN_INH1,PORT_LOW);
	Port_SetOutLevel(CAN_EN1,PORT_LOW);
	Port_SetOutLevel(CAN_STB2,PORT_LOW);
	Port_SetOutLevel(CAN_INH2,PORT_LOW);
	Port_SetOutLevel(CAN_EN2,PORT_LOW);
	Port_SetOutLevel(CAN_WAKE1,PORT_HIGH);
	Port_SetOutLevel(CAN_WAKE2,PORT_HIGH);

	//======================================SOC : CAMMERA=====================================
	//hardware require : must lager than 9ms after UGPWR_ON set
	Port_SetOutLevel(UG_CAMMERA_ON,PORT_LOW);//close camera
	Port_SetOutLevel(IOC_AP_ON_OFF,PORT_LOW);
	Port_SetOutLevel(DSP_RST_N,PORT_LOW);//dsp reset disable
	Port_SetOutLevel(CHARGE_EN,PORT_LOW);
	Port_SetOutLevel(CHARGE_CTL3,PORT_LOW);

	//======================================SOC : GPS=========================================
	Port_SetOutLevel(GPS_ON,PORT_LOW);
	Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_LOW);
	Port_SetOutLevel(GPS_RST_Q,PORT_LOW);
	
	//======================================SOC : TUNER=======================================
	Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_LOW);
	
	//======================================SOC : TFT=========================================
	Port_SetOutLevel(TFT0_EN,PORT_LOW);
	Port_SetOutLevel(TFT1_EN,PORT_LOW);
	Port_SetOutLevel(TFT3_EN,PORT_LOW);

	//======================================SOC : BT==========================================
	Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);

	//======================================SOC : UG==========================================
	Port_SetOutLevel(UG3V3_ON,PORT_LOW);
	Port_SetOutLevel(PWR_SYNC,PORT_LOW);
	Port_SetOutLevel(UGPWR_ON,PORT_LOW);
	Port_SetOutLevel(UG6V5_ON,PORT_LOW);
	Port_SetOutLevel(UG8V_ON,PORT_LOW);
	Port_SetOutLevel(UG5V_ON,PORT_LOW);

	//======================================SOC : PIMC========================================
	Port_SetOutLevel(PMIC_EN,PORT_LOW);
	//last power off PMIC_PWR_ON
	Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);
#endif

}

void Soc_PowerDown()
{
	PowerOff();
#if  0
	Port_SetOutLevel(UG5V_ON, PORT_LOW);
	DelayMs(7);
	Port_SetOutLevel(UG3V3_ON, PORT_LOW);
	DelayMs(1);
	Port_SetOutLevel(UG1V5_ON, PORT_LOW);
	DelayMs(3);
	Port_SetOutLevel(DDR_VTT_ON, PORT_LOW);
	DelayMs(7);
#endif
}

void Download_Loop()
{
	uint16 Cmd = 0;
	uint8  RespondData[4]={0};
	tUdsNRCType ret;
	uint8 i=0;
	Std_ReturnType ee_ret=E_OK;
	
	if(false == gGetIPCDataFlag)
		return;
	gGetIPCDataFlag = false;
	Cmd = gIPCBuffer[0];
	switch(Cmd)
	{
		case SID_ECU_RESET:
			//RespondData[0] = gIPCBuffer[0] + 0x40;
			//RespondData[1] = gIPCBuffer[1];
			//RespondData[2] = gIPCBuffer[2];
			//SWDL_IPCSendAck(RespondData,3);
			TracePrintf(TRACE_TRACE, TRACE_INFO, "Rx ECU_RESET\r\n");
			Soc_PowerDown();
			DelayMs(1000);
			SystemSoftwareReset();
			break;
		
		case SID_WRITE_DATA_BY_DID:
			RespondData[0] = gIPCBuffer[0] + 0x40;
			RespondData[1] = gIPCBuffer[1];
			RespondData[2] = gIPCBuffer[2];			
			break;	

		case SID_READ_DID_DATA:
			TracePrintf(TRACE_TRACE, TRACE_INFO, "Rx ReadDID\r\n");
			RespondData[0] = gIPCBuffer[0] + 0x40;
			RespondData[1] = gIPCBuffer[1];
			RespondData[2] = gIPCBuffer[2];
			RespondData[3] = SYS_USB_SWDL_STATUS;
			SWDL_IPCSendAck(RespondData,4);
			if(RespondData[2] == 0x02)
			{
				Eep_Write(0,(uint8 *)(&gClearUpdataFlag),sizeof(gClearUpdataFlag));
			}
			break;
			
		case SID_ROUTINE_CONTROL:
			RespondData[0] = gIPCBuffer[0] + 0x40;
			RespondData[1] = gIPCBuffer[1];
			RespondData[2] = gIPCBuffer[2];
			SWDL_IPCSendAck(RespondData,3);
			break;
			
		case SID_REQUEST_DOWNLOAD:
			TracePrintf(TRACE_TRACE, TRACE_INFO, "Rx REQUEST_DOWNLOAD\r\n");
			ret = sid0x34VerifyData(gIPCBuffer,gGetIPCDataLen);
			if(NRC_NONE != ret)
			{
				SWDL_IPCSendNAck(Cmd,ret);
			}
			else
			{
				RespondData[0] = gIPCBuffer[0] + 0x40;
				RespondData[1] = gIPCBuffer[1];
				RespondData[2] = gIPCBuffer[2];
				RespondData[3] = 0;
				SWDL_IPCSendAck(RespondData,4);
			}
			//TracePrintf(TRACE_TRACE, TRACE_INFO, "Get 0x34\r\n");
			break;
			
		case SID_TRANSFER_DATA:
			//TracePrintf(TRACE_TRACE, TRACE_INFO, "Get 0x36\r\n");
			if(E_NOT_OK == sid0x36CheckCRC(&gIPCBuffer[1], gGetIPCDataLen-1))
			{
				SWDL_IPCSendNAck(Cmd,NRC_STOP_PROCESS);
				TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: ERR:CRC err!!!\r\n");
				return;
			}
			ret = sid0x36MainProcess(gIPCBuffer,(gGetIPCDataLen));
			if(NRC_NONE != ret)
			{
				SWDL_IPCSendNAck(Cmd,ret);
				TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: ERR:fail to write data!!!\r\n");
				return;
			}
			else
			{
				RespondData[0] = gIPCBuffer[0] + 0x40;
				RespondData[1] = 0;
				RespondData[2] = 0;
				RespondData[3] = 0;
				SWDL_IPCSendAck(RespondData,4);
				if(gUpdataRate != gCurSeq*100/gMaxSeq)
				{
					gUpdataRate = gCurSeq*100/gMaxSeq;
					TracePrintf(TRACE_TRACE, TRACE_INFO, "Usb update:%d%\r\n",gUpdataRate);
				}
			}
			break;
			
		case SID_REQUEST_TRANSFER_EXIT:
			ret = sid0x37MainProcess(gIPCBuffer,gGetIPCDataLen);
			if(NRC_NONE != ret)
			{
				SWDL_IPCSendNAck(Cmd,ret);
				TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: ERR:got EXIT req,but need more datas!!!\r\n");
			}
			else
			{
				RespondData[0] = gIPCBuffer[0] + 0x40;
				RespondData[1] = 0;
				RespondData[2] = 0;
				RespondData[3] = 0;
				SWDL_IPCSendAck(RespondData,4);
				ee_ret=Eep_Write(0,(uint8 *)(&gClearUpdataFlag),sizeof(gClearUpdataFlag));
				if(ee_ret !=E_OK)
				{
					Eep_Write(0,(uint8 *)(&gClearUpdataFlag),sizeof(gClearUpdataFlag));
				}
				//Port_SetOutLevel(IMX_GPIO2_15,PORT_LOW);// SOC exit update mode
				TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: UPDATE APP DONE!!!\r\n");
			}
			break;
	}
}


const uint8 TestBuff[4096]={0x55};
uint16 WriteCout=0;

void ProTest(void)
{
	uint8 i=0;
	static status_t ret=STATUS_ERROR;
	
	static uint32 dstAddr = 0x32000;
	if(WriteCout<300)
	{
		WriteCout++;
#if   0
		if((sid0x36WriteData(FALSE,TestBuff,69)) != NRC_NONE)
		{
			i++;
		}
#else
		portENTER_CRITICAL();
		ret=Flash_Sector_Programing(dstAddr,TestBuff,4096);
		if(ret != STATUS_SUCCESS)
		{
			i++;
		}
		portEXIT_CRITICAL();
		dstAddr+= 4096;
#endif
	}
}



