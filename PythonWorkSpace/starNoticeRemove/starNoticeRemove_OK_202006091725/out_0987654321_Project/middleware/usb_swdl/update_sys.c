
	
#include"update_sys.h"
#include "Flash_if.h"  
#include "Spi_Ipc_gw_api.h"  
#include "system_S32K148.h"
#include "Eeprom.h"
#include "trace.h"
#include "System_Portable.h"
#include "BoardDefines.h"
#include "interrupt_manager.h"
#include "System_Task.h"
#include "wdg_pal_mapping.h"
#include "wdg_pal.h"
#include "Com_If.h"
#include "BattVoltage.h"
#include "PowerSupply.h"
#include "System_Tick.h"
#include "Delay.h"
#if (UPDATER_VERSION == UPDATE_BOOTLOADER_FLAG)
#include "diagCommon.h"

#endif

extern void Com_RxStop();

static uint32 gBackupSysResult = BACKUP_SYS_RET_NONE;
static uint32 gRestoreSysResult = RESTORE_SYS_RET_NONE;
static boolean gBackupSysActiveFlag = FALSE;
static boolean gRestoreSysActiveFlag = FALSE;
static uint8 gRestoreSysTryCounter = 0;

static request_context_t gUpdateSysIpcCtx;

static uint8 gUpdateSysCompressMethod = 0;
static uint8 gUpdateSysEncryptionMethod = 0;
static uint32 gUpdateSysFlashBaseAddr = 0;
static uint32 gUpdateSysBinFileSize = 0;
static boolean gUpdateSysGotDownloadReqFlag = FALSE;
static boolean gUpdateSysWaitingExitReqFlag = FALSE;
static boolean gUpdateSysWriteFlashIsStartedFlag = FALSE;
static uint16 gUpdateSysExpectedSeq = 1;
static uint32 gUpdateSysCurDownloadTotalDataLen = 0;


static uint16 gUpdateSysMaxNumOfBlock = 0;
static uint16 gUpdateSysMaxSeq = 0;
static uint16 gUpdateSysCurSeq = 0;
static uint16 gUpdateSysDataRate=0;

static uint8 gUpdateSysBigBuffer[FLASH_MINI_BLOCK_SIZE] = {1};
static uint8 gUpdateSysIPCRxBuffer[UPDATE_SYS_IPC_RX_BUFF_LEN] = {1};
static uint8 gUpdateSysRxDataFlag = FALSE;
static uint16 gUpdateSysRxDataLen = 0;

static uint32 gClearUpdataFlag = 0;
static uint16 gUpdateSysType = 0;
static boolean gUpdateSysActiveFlag = FALSE;
static boolean gFirstTimeWriteFlashFlag = TRUE;



static boolean gUpdateSysDoneFlag = FALSE;
static boolean gOtherResourceDisableFlag = FALSE;

const uint8 gFlashCfgData[16] = {0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,	0xFE,0x7F,0xFF,0xFF};

uint16 g_sUpdateSysCurFunctionID = 0;  
uint32 gUpdateSysPreTick = 0;

static tUdsNRCType updateSys_WriteData(boolean sameBlockFlag,const uint8* data, uint16 len);
static void updateSys_ResetStatus();


void updateSys_disableSysResource()
{
#if (UPDATER_VERSION==UPDATE_BOOTLOADER_FLAG)
	WDG_Deinit(WDG_OVER_WDOG_INSTANCE);
	Com_RxStop();
	Com_TxStop();

	INT_SYS_DisableIRQ(CAN0_ORed_0_15_MB_IRQn);
	INT_SYS_DisableIRQ(CAN0_ORed_16_31_MB_IRQn);
	INT_SYS_DisableIRQ(CAN1_ORed_0_15_MB_IRQn);
	INT_SYS_DisableIRQ(CAN1_ORed_16_31_MB_IRQn);

	System_Task_Deactivate(SYSTEM_TASK_ID_DIAG);
	System_Task_Deactivate(SYSTEM_TASK_ID_DIAGAPP);
	System_Task_Deactivate(SYSTEM_TASK_ID_COM);
	System_Task_Deactivate(SYSTEM_TASK_ID_POWERMODE);


	System_Task_Deactivate(SYSTEM_TASK_ID_GENERAL);
	System_Task_Deactivate(SYSTEM_TASK_ID_NM);
	System_Task_Deactivate(SYSTEM_TASK_ID_BC);
	System_Task_Deactivate(SYSTEM_TASK_ID_AUDIO);
	System_Task_Deactivate(SYSTEM_TASK_ID_DIGITALAMP);
	System_Task_Deactivate(SYSTEM_TASK_ID_IPCAPP);
	System_Task_Deactivate(SYSTEM_TASK_ID_SPICOREREAD);
	System_Task_Deactivate(SYSTEM_TASK_ID_SPICORERX0);

	INT_SYS_DisableIRQ(ADC0_IRQn);
	INT_SYS_DisableIRQ(ADC1_IRQn);
	INT_SYS_DisableIRQ(LPUART2_RxTx_IRQn);
	INT_SYS_DisableIRQ(LPI2C0_Master_IRQn);
	INT_SYS_DisableIRQ(LPI2C1_Master_IRQn);

	ipc_gw_unregister(IPC_CHANNEL_DIAG);
	ipc_gw_unregister(IPC_CHANNEL_CANAPP);
	ipc_gw_unregister(IPC_CHANNEL_AUDIO);


	ipc_gw_unregister(IPC_CHANNEL_KEY);


	ipc_gw_unregister(IPC_CH_REQ);
	ipc_gw_unregister(IPC_CH_NTF);
	ipc_gw_unregister(IPC_CH_CAN);
	ipc_gw_unregister(IPC_CH_AUDIO);
	ipc_gw_unregister(IPC_CH_UPDATE);
	ipc_gw_unregister(IPC_GW_CHANNEL_SWDL);
	ipc_gw_unregister(IPC_GW_CHANNEL_CANAPP);
	ipc_gw_unregister(IPC_GW_CHANNEL_SYSTEM);
	ipc_gw_unregister(IPC_GW_CHANNEL_POWERMODING);
	ipc_gw_unregister(IPC_GW_CHANNEL_AVM);

#else

#endif
}

uint16 updateCpyData(uint8*dst,const uint8*src, uint16 srcLen)
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

boolean updateSys_RestoreCurSys(uint8 counter)
{
	static uint8 *flashAddr = (uint8*)BOOTLOADER_BACKUP_FLASH_START_ADDR;
	uint16 i = 0;
	uint16 j = 0;
	uint8 ret = 0;
	static uint32 dstAddr = 0;
	static boolean firstTimeFlag = TRUE;

	

	for(i=0;i<FLASH_MINI_BLOCK_SIZE;i++)
		gUpdateSysBigBuffer[i] = *((uint8*)flashAddr+i);
	
	if(firstTimeFlag == TRUE)
	{
		
		for(j=0;j<16;j++)
		{
			
			if(gFlashCfgData[j] != gUpdateSysBigBuffer[0x400+j])
			{
				TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Err:Restore:FlashCfgData mismatch!!!!\r\n");
				dstAddr = 0;
				flashAddr = (uint8*)BOOTLOADER_BACKUP_FLASH_START_ADDR;

				for(i=0;i<16;i++)
					TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Cfg:0x%x\r\n",gUpdateSysBigBuffer[0x400+i]);
		
				return FALSE;
			}
		}

		firstTimeFlag = FALSE;
	}
	
	
	portENTER_CRITICAL();

	ret = Flash_Sector_Programing(dstAddr,gUpdateSysBigBuffer,FLASH_MINI_BLOCK_SIZE);

	portEXIT_CRITICAL();
	if(ret!=0)
		return FALSE;
	flashAddr+=FLASH_MINI_BLOCK_SIZE;
	dstAddr += FLASH_MINI_BLOCK_SIZE;

	
	

	return TRUE;
}


boolean updateSys_BackupCurSys(uint8 counter)
{
	static uint8 *flashAddr = (uint8*)0;
	uint16 i = 0;
	uint16 j = 0;
	uint8 ret = 0;
	static uint32 dstAddr = BOOTLOADER_BACKUP_FLASH_START_ADDR;
	static boolean firstTimeFlag = TRUE;
	
	for(i=0;i<FLASH_MINI_BLOCK_SIZE;i++)
		gUpdateSysBigBuffer[i] = *((uint8*)flashAddr+i);

	
	if(firstTimeFlag == TRUE)
	{
		for(j=0;j<16;j++)
		{
			if(gFlashCfgData[j] != gUpdateSysBigBuffer[0x400+j])
			{
				TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Err:Backup:FlashCfgData mismatch!!!!\r\n");
				dstAddr = BOOTLOADER_BACKUP_FLASH_START_ADDR;
				flashAddr = (uint8*)0;

				for(i=0;i<16;i++)
					TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Cfg:0x%x\r\n",gUpdateSysBigBuffer[0x400+i]);	

				return FALSE;
			}
		}

		firstTimeFlag = FALSE;
	}
	
	
	
	portENTER_CRITICAL();

	ret = Flash_Sector_Programing(dstAddr,gUpdateSysBigBuffer,FLASH_MINI_BLOCK_SIZE);

	portEXIT_CRITICAL();
	
	if(ret!=0)
		return FALSE;
	flashAddr+=FLASH_MINI_BLOCK_SIZE;
	dstAddr += FLASH_MINI_BLOCK_SIZE;

	if(counter == (BOOTLOADER_FLASH_MAX_BLOCK_NUM-1))
	{
		dstAddr = BOOTLOADER_BACKUP_FLASH_START_ADDR;
		for(i=0;i<FLASH_MINI_BLOCK_SIZE;i++)
			gUpdateSysBigBuffer[i] = *((uint8*)dstAddr+i);
		for(j=0;j<16;j++)
		{
			if(gFlashCfgData[j] != gUpdateSysBigBuffer[0x400+j])
			{
				TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Err:Backup:FlashCfgData mismatch!!!!\r\n");
				dstAddr = BOOTLOADER_BACKUP_FLASH_START_ADDR;
				flashAddr = (uint8*)0;
				for(i=0;i<16;i++)
					TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Cfg:0x%x\r\n",gUpdateSysBigBuffer[0x400+i]);	

				return FALSE;
			}
		}
	}
	
	

	return TRUE;
}

boolean updateSys_BackupSysProcess()
{
	static uint8 backupSyscounter = 0;
	static uint8 intervalTimerBackup = 0;
	boolean cpySysRet = FALSE;
	
	if(gBackupSysResult == BACKUP_SYS_RET_OK)
	{
		
		

		gBackupSysActiveFlag = FALSE;
	}
	else if(intervalTimerBackup++%5 == 0)
	{
		if(backupSyscounter <BOOTLOADER_FLASH_MAX_BLOCK_NUM)
		{
			cpySysRet = updateSys_BackupCurSys(backupSyscounter);
			if(cpySysRet == FALSE)
			{
				backupSyscounter = 0;
				gBackupSysActiveFlag = FALSE;
				gBackupSysResult = BACKUP_SYS_RET_NOT_OK;
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERR: fail to backup sys\r\n");
			}
			else
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:BACKUP:%d\r\n",backupSyscounter*2);

			backupSyscounter++;
		}
		else
		{
			backupSyscounter = 0;
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:BACKUP:100\r\n");
			
			gBackupSysActiveFlag = FALSE;
			gBackupSysResult = BACKUP_SYS_RET_OK;

		
		#if 0
			for(j=0;j<FLASH_MINI_BLOCK_SIZE;j++)
				gUpdateSysBigBuffer[j] = *((uint8*)0+j);
		#endif
		}
	}

	return TRUE;
}


boolean updateSys_RestoreSysProcess()
{
	
	static uint8 restoreSyscounter = 0;
	static uint8 intervalTimerRestore = 0;
	boolean cpySysRet = FALSE;
	
	if(intervalTimerRestore++%5 == 0)
	{
		if(restoreSyscounter <BOOTLOADER_FLASH_MAX_BLOCK_NUM)
		{
			cpySysRet = updateSys_RestoreCurSys(restoreSyscounter);
			if(cpySysRet == FALSE)
			{
				gRestoreSysResult = RESTORE_SYS_RET_NOT_OK;
				gRestoreSysActiveFlag = FALSE;
				restoreSyscounter = 0;
				gRestoreSysTryCounter++;
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERR: fail to restore sys\r\n");
			}
			else
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:RESTORE:%d\r\n",restoreSyscounter*2);

			restoreSyscounter++;
		}
		else
		{
			restoreSyscounter = 0;
			gRestoreSysResult = RESTORE_SYS_RET_OK;
			gRestoreSysActiveFlag = FALSE;
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:RESTORE:100\r\n");
		}

	}

	return TRUE;
}

#if (UPDATER_VERSION == UPDATE_APP_FLAG)
void SetUsbSWDLPortFlag()
{
	
	Port_SetOutLevel(IOC_BOOT_MODE, PORT_LOW);

}
#endif

void updateSys_Init()
{
	gUpdateSysMaxNumOfBlock = DATA_MAX_SIZE_PER_FRAME+3+CRC_VALUE_BYTE_SIZE;
	gUpdateSysWriteFlashIsStartedFlag = FALSE;
	gUpdateSysGotDownloadReqFlag = FALSE;
	gUpdateSysExpectedSeq = 1;
	gUpdateSysCurDownloadTotalDataLen = 0;
	gUpdateSysMaxSeq = 0;
	gUpdateSysWaitingExitReqFlag = FALSE;
	
	ipc_gw_register(IPC_GW_CHANNEL_SWDL_APP, updateSys_IPCCallback, NULL);
#if (UPDATER_VERSION == UPDATE_APP_FLAG)
	SetUsbSWDLPortFlag();
#endif
}


tUdsNRCType updateSys_PreCondition()
{	
	uint16 sysVol = 0;

	BattVolt_GetRawValue(&sysVol);
	if(sysVol < SYS_CUR_LVI)
	{
		return NRC_VOLTAGE_TOO_LOW;	
	}
	else if(sysVol > SYS_CUR_HVI) 
	{
		return NRC_VOLTAGE_TOO_HIGH;	
	}

	return NRC_NONE;
}

boolean updateSys_ClearUpdateFlag()
{
  uint8 i = 0;
  boolean ret = FALSE;
  
  for(i=0;i<5;i++)
  {
    gClearUpdataFlag = 0;
    Eep_Write(0,(uint8 *)&gClearUpdataFlag,sizeof(gClearUpdataFlag));
    DelayMs(10);
    Eep_Read(0,(uint8 *)&gClearUpdataFlag,sizeof(gClearUpdataFlag));
    if(gClearUpdataFlag == 0)
    {
      ret = TRUE;
      break;
    }
    else
      TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Warning: FAIL to clear update flag, try again:%d\r\n",i);
  }

  return ret;
}


tUdsNRCType updateSys_VerifyData(const uint8* data, uint16 len)
{
	
	
	uint16 lenMemSize = 0;
	uint16 lenMemAddr = 0;
	uint32 rest = 0;
	

	if(data == NULL || len <11)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	gUpdateSysCompressMethod   = (0xF0&data[1])>>4; 
	gUpdateSysEncryptionMethod = 0x0F&data[1];      

	if(gUpdateSysCompressMethod !=0 || gUpdateSysEncryptionMethod != 0)
		return NRC_REQUEST_OUT_OF_RANGE;

	lenMemAddr = (0xF0&data[2])>>4;
	lenMemSize = 0xF&data[2];

	if(lenMemAddr<=4 && lenMemSize<=4)
	{
		switch(lenMemAddr)
		{
		case 1:
			gUpdateSysFlashBaseAddr = data[3];
			break;
		case 2:
			gUpdateSysFlashBaseAddr = (uint16)(data[3]<<8|data[4]);
			break;
		case 4:
			gUpdateSysFlashBaseAddr = (uint32)(data[3]<<24|data[4]<<16|data[5]<<8|data[6]);
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}

		switch(lenMemSize)
		{
		case 1:
			gUpdateSysBinFileSize = data[7];
			break;
		case 2:
			gUpdateSysBinFileSize = (uint16)(data[7]<<8|data[8]);
			break;
		case 4:
			gUpdateSysBinFileSize = (uint32)(data[7]<<24|data[8]<<16|data[9]<<8|data[10]);
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
	}
	else
		return NRC_REQUEST_OUT_OF_RANGE;

	
	if(gUpdateSysFlashBaseAddr%2 != 0)
		return NRC_REQUEST_OUT_OF_RANGE;

	#if 0
	if(gUpdateSysBinFileSize>gDownloadCfgInfor.downloadDataTotalLen)
		return NRC_REQUEST_OUT_OF_RANGE;
	#endif
	if(gUpdateSysType == UPDATE_BOOTLOADER_FLAG && gUpdateSysBinFileSize>=BOOTLOADER_BIN_FILE_MAX_LEN)
	{
		TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: Err: sysType:%d,BinFileSize[%d] exceeds\r\n",gUpdateSysType,gUpdateSysBinFileSize);
		return NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED;
	}
	
	updateSys_ResetStatus();

	gUpdateSysMaxSeq = gUpdateSysBinFileSize/DATA_MAX_SIZE_PER_FRAME;
	rest = gUpdateSysBinFileSize%DATA_MAX_SIZE_PER_FRAME;
	if(rest !=0)
		gUpdateSysMaxSeq += 1;
	gUpdateSysGotDownloadReqFlag = TRUE;
	gUpdateSysExpectedSeq = 1;
	gUpdateSysCurDownloadTotalDataLen = 0;
	gUpdateSysWaitingExitReqFlag = FALSE;
	TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: BinFileSize=%d,maxSeq=%d\r\n",gUpdateSysBinFileSize,gUpdateSysMaxSeq);
	return NRC_NONE;	
}


Std_ReturnType updateSys_CheckCRC(const uint8* data, uint16 len)
{
	#if 0	
	if(cal_crc16_table(data,len-1) == data[len-1])
		return E_OK;
	else
		return E_NOT_OK;
	#else
		return E_OK;
	#endif
}


tUdsNRCType updateSys_WriteData(boolean sameBlockFlag,const uint8* data, uint16 len)
{
	
	
	static uint16 sBufferNum = 0;
	static uint16 sPreBuffNum = 0;
	static uint32 sDstAddr = 0;
	uint8 ret=0;
	uint8 i = 0;
	boolean flashCfgDataNotMatchFlag = FALSE;

	if(data == NULL || len==0)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	#if 0
	if(gUpdateSysWriteFlashIsStartedFlag == FALSE)
	{
		if(gUpdateSysType == UPDATE_BOOTLOADER_FLAG)
			nextMemAddr = BOOTLOADER_FLASH_START_ADDR;
		else
			nextMemAddr = APP_FLASH_START_ADDR;
			
		nextMemSize = len;
		
		gUpdateSysWriteFlashIsStartedFlag = TRUE;
	}
	else
	{
		if(sameBlockFlag == FALSE)
		{
			nextMemAddr += nextMemSize;
		}
		nextMemSize = len;
	}
	#endif
	
	
	
	
	
	
	#ifdef USING_OS_FREERTOS
	if(gUpdateSysWriteFlashIsStartedFlag == FALSE)
	{
		sBufferNum = 0;
		sPreBuffNum = 0;
		if(gUpdateSysType == UPDATE_BOOTLOADER_FLAG)
			sDstAddr = BOOTLOADER_FLASH_START_ADDR;
		else
			sDstAddr = APP_FLASH_START_ADDR;
		gUpdateSysWriteFlashIsStartedFlag = TRUE;
	}

	if(sameBlockFlag == TRUE)
	{
		if(sPreBuffNum>0)
			sPreBuffNum--;
		sBufferNum = sPreBuffNum;
	}
	
    if(sBufferNum<(FLASH_MINI_BLOCK_SIZE/DATA_MAX_SIZE_PER_FRAME))
    {
		updateCpyData(gUpdateSysBigBuffer+sBufferNum*DATA_MAX_SIZE_PER_FRAME,data,len);
		sBufferNum++;
		sPreBuffNum = sBufferNum;
		if(gUpdateSysExpectedSeq<=gUpdateSysMaxSeq)
		{
			if(sBufferNum>=(FLASH_MINI_BLOCK_SIZE/DATA_MAX_SIZE_PER_FRAME))
				sBufferNum = 0;
			else
				return NRC_NONE;
		}
	}

	

	if(gFirstTimeWriteFlashFlag == TRUE)
	{
		gFirstTimeWriteFlashFlag = FALSE;
		
		for(i=0;i<16;i++)
		{
			if(gUpdateSysBigBuffer[0x400+i] != gFlashCfgData[i])
			{
				flashCfgDataNotMatchFlag = TRUE;
				break;
			}
		}
		
		if(flashCfgDataNotMatchFlag == TRUE)
		{			
			for(i=0;i<16;i++)
				TracePrintf(GENERAL_TRACE, TRACE_ERRO, "US: flashCfg:0x%x\r\n",gUpdateSysBigBuffer[0x400+i]);
			TracePrintf(GENERAL_TRACE, TRACE_ERRO, "US: Err:flashCfgData NOT matched!!!!\r\n");
			
			updateSys_ResetStatus();
			return NRC_GENERAL_PROGRAMMING_FAILURE;
		}
	}

#if UPDATE_SYS_DUBUG
	TracePrintf(GENERAL_TRACE,TRACE_WARNING,"GetTickCount programing before:%ld\r\n", xTaskGetTickCount());
#endif

	portENTER_CRITICAL();
	ret = Flash_Sector_Programing(sDstAddr,gUpdateSysBigBuffer,FLASH_MINI_BLOCK_SIZE);
	portEXIT_CRITICAL();

#if UPDATE_SYS_DUBUG
	TracePrintf(GENERAL_TRACE,TRACE_WARNING,"GetTickCount programing after:%ld\r\n", xTaskGetTickCount());
#endif

	if(ret != 0)
	{
	    TracePrintf(GENERAL_TRACE, TRACE_ERRO, "US: ERR:Fail to Write Flash[%d]!\r\n",ret);
		updateSys_ResetStatus();
		return NRC_GENERAL_PROGRAMMING_FAILURE;
	}
	
	sDstAddr+= FLASH_MINI_BLOCK_SIZE;
	
	#endif

	return NRC_NONE;
		
}


void updateSys_ResetStatus()
{
	gUpdateSysWriteFlashIsStartedFlag = FALSE;
	gUpdateSysGotDownloadReqFlag = FALSE;
	gUpdateSysExpectedSeq = 1;
	gUpdateSysCurDownloadTotalDataLen = 0;
	gUpdateSysMaxSeq = 0;
	gUpdateSysWaitingExitReqFlag = FALSE;
	gFirstTimeWriteFlashFlag = TRUE;
	gUpdateSysCurSeq = 0;
	gUpdateSysDoneFlag = FALSE;
}


tUdsNRCType updateSys_MainProcess(const uint8* data, uint16 len)
{
	uint16 blockSeq = 0;
	tUdsNRCType ret = NRC_NONE;
	uint16 curFrameValidDataLen = 0;
	uint32 curTempTotalLen = 0;
	
	if(data == NULL || len <(3+CRC_VALUE_BYTE_SIZE))
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	curFrameValidDataLen = len - 3 - CRC_VALUE_BYTE_SIZE;
	blockSeq = (uint16)(data[1]|(data[2]<<8));
	gUpdateSysCurSeq  = blockSeq;
	
	if(curFrameValidDataLen+gUpdateSysCurDownloadTotalDataLen>gUpdateSysBinFileSize)
	{
		TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: ERR:invalid data len,seq=%d,len=%d,curTotalLen=%d\r\n",blockSeq,curFrameValidDataLen,gUpdateSysCurDownloadTotalDataLen);
		ret = NRC_GENERAL_PROGRAMMING_FAILURE;
		return ret;
	}
	
	if(gUpdateSysExpectedSeq>1 && (blockSeq == (gUpdateSysExpectedSeq-1)))
	{
		TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: rx repeat Seq = %d\r\n",blockSeq);
		if((blockSeq<gUpdateSysMaxSeq && len!=gUpdateSysMaxNumOfBlock)
			||(blockSeq==gUpdateSysMaxSeq && ((gUpdateSysCurDownloadTotalDataLen+curFrameValidDataLen)!=gUpdateSysBinFileSize)))
		{
			return NRC_INCORRECT_MESSAGE_LENGTH;
		}
		
		if((ret = updateSys_WriteData(TRUE,data+3,curFrameValidDataLen)) != NRC_NONE)
			return ret;
	}
	else
	{
		if(gUpdateSysWaitingExitReqFlag == FALSE)
		{
			
			if(blockSeq != gUpdateSysExpectedSeq)
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: rx unexpected Seq[%d],expected Seq=%d\r\n",blockSeq,gUpdateSysExpectedSeq);
				return NRC_WRONG_BLOCK_SEQ_COUNTER;
			}
			
			if(blockSeq<gUpdateSysMaxSeq && curFrameValidDataLen!=(gUpdateSysMaxNumOfBlock-3-CRC_VALUE_BYTE_SIZE))
			{
				
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: wrong datalen before rx the last Seq\r\n");
				return NRC_INCORRECT_MESSAGE_LENGTH;
			}

			curTempTotalLen = gUpdateSysCurDownloadTotalDataLen+curFrameValidDataLen;
			if((blockSeq<gUpdateSysMaxSeq &&  curTempTotalLen>= gUpdateSysBinFileSize)
				|| (gUpdateSysMaxSeq == blockSeq && curTempTotalLen != gUpdateSysBinFileSize))
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: wrong datalen,need to RESET\r\n");
				return NRC_TRANSFER_DATA_SUSPENDED;
			}

			gUpdateSysCurDownloadTotalDataLen += curFrameValidDataLen;
			gUpdateSysExpectedSeq += 1;
			
			if((ret = updateSys_WriteData(FALSE,data+3,curFrameValidDataLen)) != NRC_NONE)
				return ret;
			else
			{
				if(gUpdateSysCurDownloadTotalDataLen == gUpdateSysBinFileSize)
				{
					gUpdateSysWaitingExitReqFlag = TRUE;
					gUpdateSysDoneFlag = TRUE;
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: got all datas,len = %d\r\n",gUpdateSysCurDownloadTotalDataLen);
				}
				
			}
		}
		else
			return NRC_REQUEST_SEQUENCE_ERROR;
	}
	
	return NRC_NONE;
}

tUdsNRCType updateSys_ReqExit(const uint8*data, uint16 len)
{
	if(data == NULL || len != 1)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(gUpdateSysWaitingExitReqFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;

	
	
	return NRC_NONE;
}

void SWDl_IPC_OtherCbk(const void *ctx,uint8 *rx_data, sint32 len)
{
	request_context_t *l_ctx = (request_context_t*)ctx;
	uint8 tempData[4] = {0x11,0,0x22,0x33};
    switch(l_ctx->function_id) {
	case 1:
		updateCpyData(gUpdateSysIPCRxBuffer,tempData,4);
		gUpdateSysRxDataFlag = true;
		gUpdateSysRxDataLen = len;
		gUpdateSysIpcCtx = *((request_context_t*)ctx);
		break;
	default:
		break;
    }
}

void updateSys_IPCCallback(uint16 function, uint8 *rx_data, uint32 len)
{
	uint8 nrcBuff[3];
	
	if((len <= UPDATE_SYS_IPC_RX_BUFF_LEN) && (NULL != rx_data))
	{
		updateCpyData(gUpdateSysIPCRxBuffer,rx_data,len);
		gUpdateSysRxDataFlag = true;
		gUpdateSysRxDataLen = len;

		g_sUpdateSysCurFunctionID = function;

#if UPDATE_SYS_DUBUG
		TracePrintf(GENERAL_TRACE,TRACE_WARNING,"64B receiveTick:%ld\r\n", xTaskGetTickCount());
		TracePrintf(GENERAL_TRACE, TRACE_INFO, "in ipccbk len = %d\r\n", len);
#endif

	}
	else
	{
		nrcBuff[0] = 0x7f;
		nrcBuff[1] = rx_data[0];
		nrcBuff[2] = NRC_INCORRECT_MESSAGE_LENGTH;
		TracePrintf(GENERAL_TRACE, TRACE_WARNING, "ERROR:Data len exceed buffer:%d\r\n",len);
		ipc_send_notification(SPI_HW_IPC_NUM1, IPC_GW_CHANNEL_SWDL_APP, function, (const uint8 *)&nrcBuff, 3);
	}
}

void SWDL_IPCSendAck(uint8 *Data, uint16 function, uint16 Len)
{

	ipc_send_notification(SPI_HW_IPC_NUM1, IPC_GW_CHANNEL_SWDL_APP, function, Data, Len);
}

void SWDL_IPCSendNAck(uint8 Cmd, uint16 function, uint8 Ecode)
{
	uint8 buff[3]={0};
	
	buff[0] = 0x7f;
	buff[1] = Cmd;
	buff[2] = Ecode;

	ipc_send_notification(SPI_HW_IPC_NUM1, IPC_GW_CHANNEL_SWDL_APP, function, buff, 3);
}
void Soc_PowerDown()
{
	
	if(gUpdateSysType == UPDATE_APP_FLAG)
		PowerOff();
}

uint16 updateSys_GetStatus()
{
	return gUpdateSysType;
}


void updateSys_Loop()
{
	uint16 Cmd = 0;
	uint8  RespondData[16] = {0};
	tUdsNRCType ret = NRC_NONE;
	
	uint16 updateSysType = 0;
	uint16 subFunction = 0;
	uint16 updateProgressRate = 0;
	
	

#if (UPDATER_VERSION == UPDATE_BOOTLOADER_FLAG)	
	if(TRUE == gBackupSysActiveFlag)
	{
		updateSys_BackupSysProcess();
	}

	if(TRUE == gRestoreSysActiveFlag)
	{
		updateSys_RestoreSysProcess();

		
		if(RESTORE_SYS_RET_NOT_OK == gRestoreSysResult && gRestoreSysTryCounter < RESTORE_BOOTLOADER_MAX_COUNTER)
			gRestoreSysActiveFlag = TRUE;
		else if (gRestoreSysTryCounter >= RESTORE_BOOTLOADER_MAX_COUNTER)
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERR:!!!GAME OVER!!!NO WAY TO RECOVERY SYS NOW!!!\r\n");
	}

	if(TRUE == gBackupSysActiveFlag || TRUE == gRestoreSysActiveFlag)
		return;
#endif

	
	if(gUpdateSysActiveFlag == TRUE)
	{
		if(System_GetTickValue()-gUpdateSysPreTick > SYSUPDATE_IPC_TIMEOUT)
		{
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:IPC TIMEOUT,START TO RESTORE\r\n");
			updateSys_ResetStatus();
			gRestoreSysActiveFlag = TRUE;
			gUpdateSysActiveFlag = FALSE;
			gBackupSysResult = BACKUP_SYS_RET_NONE;
			return;
		}
	}

	if(FALSE == gUpdateSysRxDataFlag)
		return;

	gUpdateSysPreTick = System_GetTickValue();

	gUpdateSysRxDataFlag = false;
	Cmd = gUpdateSysIPCRxBuffer[0];
	switch(Cmd)
	{
		case SID_ECU_RESET:
			gBackupSysResult = BACKUP_SYS_RET_OK;
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Rx ECU_RESET\r\n",
				gUpdateSysIPCRxBuffer[0],gUpdateSysIPCRxBuffer[1],gUpdateSysIPCRxBuffer[2],gUpdateSysIPCRxBuffer[3]);
			subFunction = (uint16)(gUpdateSysIPCRxBuffer[2]<<8|gUpdateSysIPCRxBuffer[3]);
			if(0x2233 == subFunction)
			{
				if(TRUE == gUpdateSysDoneFlag)
				{
					vTaskSuspendAll();
					WDG_Deinit(WDG_OVER_WDOG_INSTANCE);
					DelayMs(100);
					Soc_PowerDown();
					SystemSoftwareReset(PM_RESET, FALSE);
				}
				#if (UPDATER_VERSION == UPDATE_BOOTLOADER_FLAG)
				else
				{
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:update not Done!Restore sys now...\r\n");
					if(BACKUP_SYS_RET_OK == gBackupSysResult)
					{
						ipc_gw_unregister(IPC_GW_CHANNEL_SWDL_APP);
						gRestoreSysActiveFlag = TRUE;
					}
				}
				#else
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Warning:update not Done!Can't RESET!!!\r\n");
					SWDL_IPCSendNAck(Cmd, IVI_IPC_UPDATE_SID_ECU_RESET, NRC_GENERAL_PROGRAMMING_FAILURE);
				#endif
			}
			else
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Warning: invalid subFunction[0x%x]\r\n",subFunction);
				SWDL_IPCSendNAck(Cmd, IVI_IPC_UPDATE_SID_ECU_RESET, NRC_SUB_FUNCTION_NOT_SUPPORTED);
			}
			break;
		
		case SID_WRITE_DATA_BY_DID:
			updateSysType = (uint16)(gUpdateSysIPCRxBuffer[1]<<8|gUpdateSysIPCRxBuffer[2]);
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Rx WRITE_DATA_BY_DID[0x%x]\r\n",updateSysType);
			if(UPDATE_APP_FLAG == updateSysType || UPDATE_BOOTLOADER_FLAG == updateSysType)
			{
				if(UPDATER_VERSION != updateSysType)
				{
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Warning: UPDATER VERSION[0x%x] mismatch with sysType[0x%x]\r\n",UPDATER_VERSION,updateSysType);
					SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED);
				}
				else
				{
					gUpdateSysActiveFlag = TRUE;
					gUpdateSysType = updateSysType;
					RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
					RespondData[1] = gUpdateSysIPCRxBuffer[1];
					RespondData[2] = gUpdateSysIPCRxBuffer[2];
					SWDL_IPCSendAck(RespondData, g_sUpdateSysCurFunctionID, 3);

					if(FALSE == gOtherResourceDisableFlag)
					{
						gOtherResourceDisableFlag = TRUE;
						updateSys_disableSysResource();
						EnterPowerMode(ePowerMode_RUN);

						#if 0
							if(gUpdateSysType == UPDATE_BOOTLOADER_FLAG)
								ipc_gw_register(IPC_CHANNEL_POWERMODING,NULL,SWDl_IPC_OtherCbk);
						#endif
					}
				#if (UPDATER_VERSION == UPDATE_BOOTLOADER_FLAG)
					gBackupSysActiveFlag = TRUE;
				#endif
				}
			}
			else
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Warning: invalid sysType[0x%x]\r\n",updateSysType);
				SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_SUB_FUNCTION_NOT_SUPPORTED);
			}
			break;	
			
		case SID_ROUTINE_CONTROL:
			if(gUpdateSysRxDataLen<4)
				SWDL_IPCSendNAck(Cmd, IVI_IPC_UPDATE_SID_ROUTINE_CONTROL, NRC_INCORRECT_MESSAGE_LENGTH);
			else
			{
				TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:Rx ROUTINE_CONTROL!!!\r\n");
				subFunction = (uint16)(gUpdateSysIPCRxBuffer[2]<<8|gUpdateSysIPCRxBuffer[3]);
				switch(gUpdateSysIPCRxBuffer[1])
				{
				case 2:
					if(subFunction == 0x0102)
					{
						TracePrintf(GENERAL_TRACE,TRACE_WARNING,"US:erase update flag!!!\r\n");
						if(gUpdateSysType == UPDATE_APP_FLAG)
							updateSys_ClearUpdateFlag();
						RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
						SWDL_IPCSendAck(RespondData, IVI_IPC_UPDATE_SID_ROUTINE_CONTROL, 4);						
					}
					else
						SWDL_IPCSendNAck(Cmd, IVI_IPC_UPDATE_SID_ROUTINE_CONTROL, NRC_REQUEST_OUT_OF_RANGE);
					break;
				default:
					SWDL_IPCSendNAck(Cmd, IVI_IPC_UPDATE_SID_ROUTINE_CONTROL, NRC_SUB_FUNCTION_NOT_SUPPORTED);
					break;
				}
			}
			break;
			
		case SID_REQUEST_DOWNLOAD:
			if(TRUE == gUpdateSysActiveFlag)
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:Rx REQUEST_DOWNLOAD\r\n");
				ret = updateSys_VerifyData(gUpdateSysIPCRxBuffer,gUpdateSysRxDataLen);
				if(NRC_NONE != ret)
				{
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:fail to verify data, NRC=%d\r\n",ret);
					SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, ret);
				}
				else
				{
#if (UPDATER_VERSION == UPDATE_BOOTLOADER_FLAG)
					if(gBackupSysResult != BACKUP_SYS_RET_OK)
					{
						SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_GENERAL_PROGRAMMING_FAILURE);
					}
					else
#endif
					{
						RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
						RespondData[1] = gUpdateSysIPCRxBuffer[1];
						RespondData[2] = gUpdateSysIPCRxBuffer[2];
						RespondData[3] = 0;

						SWDL_IPCSendAck(RespondData, g_sUpdateSysCurFunctionID, 4);
					}
				}
			}
			else
			{
				ret = NRC_REQUEST_SEQUENCE_ERROR;
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERR:rx 0x34 before got sys type\r\n",ret);
				SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, ret);
			}
			break;
			
		case SID_TRANSFER_DATA:
			if(TRUE == gUpdateSysActiveFlag)
			{
				if(gUpdateSysGotDownloadReqFlag == TRUE)
				{
					
					if(E_OK == updateSys_CheckCRC(&gUpdateSysIPCRxBuffer[1], gUpdateSysRxDataLen-1))
					{

					#if UPDATE_SYS_DUBUG
						TracePrintf(GENERAL_TRACE,TRACE_WARNING,"64B before:%ld\r\n", xTaskGetTickCount());
					#endif
						
						ret = updateSys_MainProcess(gUpdateSysIPCRxBuffer,gUpdateSysRxDataLen);
						if(NRC_NONE == ret)
						{
							RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
							RespondData[1] = (uint8)((gUpdateSysExpectedSeq-1) & 0x00ff);
							RespondData[2] = (uint8)(((gUpdateSysExpectedSeq-1) & 0xff00) >> 8);
							RespondData[3] = 0;

							SWDL_IPCSendAck(RespondData, g_sUpdateSysCurFunctionID, 4);

						#if UPDATE_SYS_DUBUG
							TracePrintf(GENERAL_TRACE,TRACE_WARNING,"64B after:%ld\r\n", xTaskGetTickCount());
							TracePrintf(GENERAL_TRACE,TRACE_WARNING,"================\r\n");
						#endif

							updateProgressRate = gUpdateSysCurSeq*100/gUpdateSysMaxSeq;
							if(gUpdateSysDataRate != updateProgressRate)
							{
								gUpdateSysDataRate = updateProgressRate;
								TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:%d%\r\n",gUpdateSysDataRate);
							}
						}
						else
						{
							TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: ERR:fail to write data!!!\r\n");
							SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, ret);

							if(NRC_GENERAL_PROGRAMMING_FAILURE == ret)  
							{
								updateSys_ResetStatus();
								gRestoreSysActiveFlag = TRUE;
							}	
						}
					}
					else
					{
						TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERR: CRC err\r\n");
						SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_INVALID_KEY);					
					}
				}
				else
				{
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERROR:Rx 0x36 before 0x34\r\n");
					SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_REQUEST_SEQUENCE_ERROR);
				}

			}
			else
			{
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US:ERROR:Rx 0x36 before got sys type\r\n");
				SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, NRC_REQUEST_SEQUENCE_ERROR);
			}
			break;
			
		case SID_REQUEST_TRANSFER_EXIT:
			ret = updateSys_ReqExit(gUpdateSysIPCRxBuffer,gUpdateSysRxDataLen);
			if(NRC_NONE != ret)
			{
				SWDL_IPCSendNAck(Cmd, g_sUpdateSysCurFunctionID, ret);
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: ERR:got EXIT req,but need more datas!!!\r\n");
			}
			else
			{
				RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
				RespondData[1] = 0;
				RespondData[2] = 0;
				RespondData[3] = 0;

				SWDL_IPCSendAck(RespondData, g_sUpdateSysCurFunctionID, 4);
				if(gUpdateSysType == UPDATE_APP_FLAG)
					updateSys_ClearUpdateFlag();
				
				TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: UPDATE SYS DONE[sysType=0x%x]!!!\r\n",gUpdateSysType);
				updateSys_ResetStatus();
				gUpdateSysDoneFlag = TRUE;
			}
			break;

		case SID_READ_DATA_BY_DID:  
				TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[M_IPC-I]:Read DID\r\n");
				RespondData[0] = gUpdateSysIPCRxBuffer[0] + 0x40;
				RespondData[1] = gUpdateSysIPCRxBuffer[1];
				RespondData[2] = gUpdateSysIPCRxBuffer[2];
				RespondData[3] = SYS_APP_STATUS;
				ipc_send_notification(SPI_HW_IPC_NUM1, IPC_GW_CHANNEL_SWDL_APP, g_sUpdateSysCurFunctionID, (const uint8 *)&RespondData, 4);
			break;
			
		default:
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "US: Warning:rx invalid SID[0x%x]!!!\r\n",Cmd);
			break;
	}
}





