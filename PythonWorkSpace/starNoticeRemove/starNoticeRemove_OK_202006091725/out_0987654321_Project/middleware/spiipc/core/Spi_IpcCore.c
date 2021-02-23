

#include "SpiIpc_cfg.h"
#include "hashmap.h"
#include "Spi_IpcCore.h"
#include "Spi_if.h"
#include "SpiIpc.h"
#include "trace.h"
#include "SPI_CircularQueue.h"
#include "status.h"
#include "spiSem.h"
#include "FreeRTOS.h"
#include "task.h"


#define SPI_IPC_MSG_HEADER            	(2)
#define SPI_IPC_CMD_POS               	(0)
#define SPI_IPC_LEN_POS               	(1)

#define SPI_IPC_MASTER_READ            (0xA1)
#define SPI_IPC_MASTER_WRITE           (0xA2)
#define SPI_IPC_MASTER_READ_EXT        (0xA3)
#define SPI_IPC_MASTER_WRITE_EXT       (0xA4)

#define SPI_IPC_INVALID_CH			   (0x3F)


typedef union {
   uint32_t u32;
   uint8_t  u[4];
}d4b_t;


#define CRC_CHECK_ORGAMIZ(SpiCrcSupport,pack_len,ch) \
if(SpiCrcSupport)\
{\
	(pack_len) += 4;		 \
	(ch) |= IPC_CRC_MASK;\
}\

boolean  Spi_MComFLag = FALSE;
static uint8 TempSpiReadBuf[HW_SPI_IPC_MAX_NUM][SPI_MAX_FRAME_SIZE];
static uint8 TempSpiWriteBuf[HW_SPI_IPC_MAX_NUM][SPI_MAX_FRAME_SIZE];

static uint8 WairtSpiReadComplex(uint8 HwChannel)
{
	return GetCurSpiStatus(HwChannel);
}

sint32 SetupAndProcessMasterWriteExt(uint8 HwChannel,uint8 *pBuff)
{
	uint8 err = 0;
	uint8 ReadLen;
	uint8 Ch;
	uint16 pos = 0;

	d4b_t CrcValue; 

	ReadLen = pBuff[0];

	
	{
		Ch = TempSpiReadBuf[HwChannel][2]&IPC_CHANNEL_MASK;

		if(1 == HwChannel)	
		{
			Spi_MComFLag = TRUE;
		}

		if(ReadLen < 4)
		{
			return E_NOT_OK;	
		}
		CrcValue.u[0] = TempSpiReadBuf[HwChannel][ReadLen+SPI_IPC_MSG_HEADER-1];
		CrcValue.u[1] = TempSpiReadBuf[HwChannel][ReadLen+SPI_IPC_MSG_HEADER-2];
		CrcValue.u[2] = TempSpiReadBuf[HwChannel][ReadLen+SPI_IPC_MSG_HEADER-3];
		CrcValue.u[3] = TempSpiReadBuf[HwChannel][ReadLen+SPI_IPC_MSG_HEADER-4];
		if(CrcValue.u32 != crc32(&TempSpiReadBuf[HwChannel][2], ReadLen-4))
		{
			return E_NOT_OK;	
		}
		ReadLen -= 4;
		if (Ch == IPC_FLOWCTRL_CHANNEL)
		{
			if(ReadLen > 1)
			{
				
				SPI_CQueue_Push(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), Ch,  &TempSpiReadBuf[HwChannel][1+SPI_IPC_MSG_HEADER], ReadLen-1);
				SpiMasterReq(HwChannel);
			}
		}
		else if (IPC_FLOWCTRL_CHANNEL != Ch)
		{
			pos = SPI_IPC_MSG_HEADER;
			while(1)
			{
				Ch = TempSpiReadBuf[HwChannel][pos]&IPC_CHANNEL_MASK;
				pos += 1;
				err = SPI_CQueue_Push(HwChannel, SPI_CQueueGetRxManagerHandle(HwChannel), Ch, &TempSpiReadBuf[HwChannel][1+pos], TempSpiReadBuf[HwChannel][pos]);	
				if(err != E_OK)
					break;
				spiSemPost(HwChannel);
				pos += TempSpiReadBuf[HwChannel][pos] + 1;	
				if(pos >= ReadLen)
				{
					break;
				}
			}
		}
	}
	return err;
}

uint8_t PrepareWriteMessae(uint8 HwChannel)
{
	uint16 qlen = 0;
	uint8 ch = 0;
	uint8 pack_len;
	d4b_t CrcValue;

	SPI_CQueue_GetTotalLen(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		SPI_CQueueGlobalLock(HwChannel);
		for(ch=0;ch < SpiIpcUserCfg[HwChannel].ChannelNum;ch++)
		{
			if(SPI_CQueue_GetLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), ch, &qlen) == E_OK)
			{
				if(qlen > 0)
				{
					break;
				}
			}
		}
		SPI_CQueueGlobalUnLock(HwChannel);

		if(ch >= SpiIpcUserCfg[HwChannel].ChannelNum)
		{
			return 0;
		}
		else
		{
			if(SpiIsChMerge(HwChannel) == 0)
			{
				pack_len = (uint8)SPI_CQueue_PopMulti(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), ch, &TempSpiWriteBuf[HwChannel][2], sizeof(TempSpiWriteBuf[0])-SPI_IPC_MSG_HEADER-SPI_MAX_CRC_SIZE);
			}
			else
			{
				pack_len = (uint8)SPI_CQueue_PopMultiChannel(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), &TempSpiWriteBuf[HwChannel][2], sizeof(TempSpiWriteBuf[0])-SPI_IPC_MSG_HEADER-SPI_MAX_CRC_SIZE);
			}

			if(pack_len <= 1)	
			{
				return 0;
			}
			CRC_CHECK_ORGAMIZ(1, pack_len, ch);
			TempSpiWriteBuf[HwChannel][0] = SPI_IPC_MASTER_READ;
			TempSpiWriteBuf[HwChannel][1] = pack_len;
			


			CrcValue.u32 = crc32(&TempSpiWriteBuf[HwChannel][SPI_IPC_MSG_HEADER], pack_len-4);
			TempSpiWriteBuf[HwChannel][pack_len+SPI_IPC_MSG_HEADER-1] = CrcValue.u[0];
			TempSpiWriteBuf[HwChannel][pack_len+SPI_IPC_MSG_HEADER-2] = CrcValue.u[1];
			TempSpiWriteBuf[HwChannel][pack_len+SPI_IPC_MSG_HEADER-3] = CrcValue.u[2];
			TempSpiWriteBuf[HwChannel][pack_len+SPI_IPC_MSG_HEADER-4] = CrcValue.u[3];
			SPI_CQueueGlobalLock(HwChannel);
			SPI_CQueue_GetTotalLen(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
			if(qlen > 0)
			{
				SpiMasterReq(HwChannel);
				UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
			}
			else
			{
				TempSpiWriteBuf[HwChannel][0] = SPI_IPC_MASTER_READ_EXT;
			}
			SPI_CQueueGlobalUnLock(HwChannel);
			return pack_len;
		}
	}
	else
	{
		return 0;
	}
}

void HandleSPI_IPC_Messages(uint8 HwChannel)
{
	uint8 err = 0xFF;

	
	if(WairtSpiReadComplex(HwChannel) != SPI_STATUS_BUSY)
	{
		memset(&TempSpiReadBuf[HwChannel], 0, 2);
  		if(!PrepareWriteMessae(HwChannel))
  		{
  			TempSpiWriteBuf[HwChannel][0] = SPI_IPC_MASTER_READ;
			TempSpiWriteBuf[HwChannel][1] = 1;
			TempSpiWriteBuf[HwChannel][2] = SPI_IPC_INVALID_CH;
  		}
		err  = SpiTransData(HwChannel, &TempSpiReadBuf[HwChannel][0], &TempSpiWriteBuf[HwChannel][0], SPI_MAX_FRAME_SIZE, STD_ON);
	}

	if((err == E_OK) && (GetCurSpiStatus(HwChannel) == SPI_STATUS_SUCESS))
	{
		if((TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS] == SPI_IPC_MASTER_WRITE)
			||(TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS] == SPI_IPC_MASTER_WRITE_EXT))
		{
			if((TempSpiReadBuf[HwChannel][1] >= (SPI_MAX_FRAME_SIZE-SPI_HEAD_SIZE))|| (TempSpiReadBuf[HwChannel][1] == 0))
			{
				return;
			}
			err = SetupAndProcessMasterWriteExt(HwChannel, &TempSpiReadBuf[HwChannel][1]);
		}
		else if(TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS] == SPI_IPC_MASTER_READ)
		{
			return;
		}
		else 	
		{
			TracePrintf(IPC_TRACE, TRACE_NONMASK, "[IPC] ch:%d %x %x\r\n", HwChannel,  
				TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS], TempSpiReadBuf[HwChannel][SPI_IPC_LEN_POS]);
			
			SpiSetLastTransErrorState(HwChannel, 1);
			vTaskDelay(1U); 
		}
	}
	else
	{
		vTaskDelay(1U);	
		SpiAbortTransfer(HwChannel);
	}
}

#define OLD_SPI_SUPPORT
#ifdef OLD_SPI_SUPPORT
#if SPI_IPC_CRC_SUPPORT
static uint8 s_SpiCrcSupport[HW_SPI_IPC_MAX_NUM] = {FALSE};
#endif
sint32 SetupAndProcessMasterWrite(uint8 HwChannel,uint8 *pBuff)
{
	uint8 err;
	uint8 ReadLen;
	uint8 Ch;
#if SPI_IPC_MERGE_SUPPORT
	uint8 MultiFlag;
	uint16 pos = 0;
#endif

#if SPI_IPC_CRC_SUPPORT
	uint8 CrcFlag;
	d4b_t CrcValue; 
#endif

	ReadLen = pBuff[0];
	
	
	err = SpiReadData(HwChannel, &TempSpiReadBuf[HwChannel][0], ReadLen, STD_OFF);
	if(err != E_OK)
	{
		return E_NOT_OK;
	}
	if(err == E_OK)
	{
		Ch = TempSpiReadBuf[HwChannel][0]&IPC_CHANNEL_MASK;
#if SPI_IPC_MERGE_SUPPORT
		MultiFlag = TempSpiReadBuf[HwChannel][0]>>IPC_MULTI_SHIFT;
#endif

#if SPI_IPC_CRC_SUPPORT
		CrcFlag = TempSpiReadBuf[HwChannel][0]>>IPC_CRC_SHIFT;

		if(s_SpiCrcSupport[HwChannel] == FALSE)
		{
			if(CrcFlag)
			{
				s_SpiCrcSupport[HwChannel] = TRUE;
			}
		}
#endif

		if(1 == HwChannel)	
		{
			Spi_MComFLag = TRUE;
		}
#if SPI_IPC_CRC_SUPPORT
		if(CrcFlag)
		{
			if(ReadLen < 4)
			{
				return E_NOT_OK;	
			}
			CrcValue.u[0] = TempSpiReadBuf[HwChannel][ReadLen-1];
			CrcValue.u[1] = TempSpiReadBuf[HwChannel][ReadLen-2];
			CrcValue.u[2] = TempSpiReadBuf[HwChannel][ReadLen-3];
			CrcValue.u[3] = TempSpiReadBuf[HwChannel][ReadLen-4];
			if(CrcValue.u32 != crc32(&TempSpiReadBuf[HwChannel][0], ReadLen-4))
			{
				return E_NOT_OK;	
			}
			ReadLen -= 4;
		}

		if (Ch == IPC_FLOWCTRL_CHANNEL)
		{
			if(ReadLen > 1)
			{
				
				SPI_CQueue_Push(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), Ch,  &TempSpiReadBuf[HwChannel][1], ReadLen-1);
				SpiMasterReq(HwChannel);
			}
		}
		else
#endif
		if (IPC_FLOWCTRL_CHANNEL != Ch)
		{
#if SPI_IPC_MERGE_SUPPORT
			if(MultiFlag == 0)
			{
				SPI_CQueue_Push(HwChannel, SPI_CQueueGetRxManagerHandle(HwChannel), Ch, &TempSpiReadBuf[HwChannel][1], ReadLen-1);	
				spiSemPost(HwChannel);
			}
			else if(HwChannel == 0)
			{
				pos = 1;	
				while(1)
				{
					err = SPI_CQueue_Push(HwChannel, SPI_CQueueGetRxManagerHandle(HwChannel), Ch, &TempSpiReadBuf[HwChannel][1+pos], TempSpiReadBuf[HwChannel][pos]);	
					if(err != E_OK)
						break;
					spiSemPost(HwChannel);
					pos += TempSpiReadBuf[HwChannel][pos] + 1;	
					if(pos >= ReadLen)
					{
						break;
					}
				}
			}
#else
			SPI_CQueue_Push(HwChannel, SPI_CQueueGetRxManagerHandle(HwChannel), Ch, &TempSpiReadBuf[HwChannel][1], ReadLen-1);	
			spiSemPost(HwChannel);
#endif
		}
	}
	else
	{
		err = E_NOT_OK;
	}
	return err;
}


static void MasterReadComplete(uint8 HwChannel,uint8* msg_ptr, uint8 ch, uint8 len)
{
	boolean err;
	uint8 hdr_msg[SPI_IPC_MSG_HEADER];
	uint16 qlen = 0;
#if SPI_IPC_CRC_SUPPORT
	d4b_t CrcValue;
#endif

	if(len > SPI_MAX_FRAME_SIZE)
	{
		return;
	}

	SPI_CQueue_GetTotalLen(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	hdr_msg[SPI_IPC_CMD_POS] = SPI_IPC_MASTER_READ;
	hdr_msg[SPI_IPC_LEN_POS] = len;

	
	err = SpiSendData(HwChannel, &hdr_msg[0], SPI_IPC_MSG_HEADER, STD_OFF);	
	if(err != E_OK)
	{
		return;
	}

	if(ch&IPC_MULTI_MASK)
	{
		msg_ptr[0] |= IPC_MULTI_MASK;
		msg_ptr[0] |= ch&IPC_CRC_MASK;
	}
	else
	{
		msg_ptr[0] = ch;
	}
	
#if SPI_IPC_CRC_SUPPORT
	if((ch&IPC_CRC_MASK))
	{
		CrcValue.u32 = crc32(msg_ptr, len-4);
		msg_ptr[len-1] = CrcValue.u[0];
		msg_ptr[len-2] = CrcValue.u[1];
		msg_ptr[len-3] = CrcValue.u[2];
		msg_ptr[len-4] = CrcValue.u[3];
	}
#endif

	
	SpiSendData(HwChannel, msg_ptr, len, STD_OFF);
	if(qlen > 0)
	{
		SpiMasterReq(HwChannel);
		UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
	}
}

static uint8 SetupAndProcessMasterRead(uint8 HwChannel)
{
	uint8 ch;
	uint16 len;
	uint8 err;
	uint8 pack_len;
	uint8 crc_flag;

	err = E_OK;
	SPI_CQueueGlobalLock(HwChannel);
	for(ch=0;ch < SpiIpcUserCfg[HwChannel].ChannelNum;ch++)
	{
		if(SPI_CQueue_GetLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), ch, &len) == E_OK)
		{
			if(len > 0)
			{
				break;
			}
		}
	}
	SPI_CQueueGlobalUnLock(HwChannel);

	if(ch >= SpiIpcUserCfg[HwChannel].ChannelNum)
	{
		TempSpiWriteBuf[HwChannel][0] = 0;
		MasterReadComplete(HwChannel,&TempSpiWriteBuf[HwChannel][0], IPC_CHANNEL_MASK, 1);
		err = E_NOT_OK;
	}
	else
	{
		#if SPI_IPC_CRC_SUPPORT
		if(ch == IPC_FLOWCTRL_CHANNEL)
		{
			pack_len = (uint8)SPI_CQueue_Pop(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), ch, &TempSpiWriteBuf[HwChannel][1], sizeof(TempSpiWriteBuf[0])-SPI_MAX_CRC_SIZE);
			CRC_CHECK_ORGAMIZ(1, pack_len, ch);
			pack_len += 1;	
		}
		else
		#endif
#if SPI_IPC_MERGE_SUPPORT
		if(SpiIsChMerge(HwChannel) == 0)
		{
			if(len == 1)
			{
				pack_len = (uint8)SPI_CQueue_Pop(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), ch, &TempSpiWriteBuf[HwChannel][1], sizeof(TempSpiWriteBuf[0])-SPI_MAX_CRC_SIZE);
				pack_len += 1;	
			}
			else
			{
				pack_len = (uint8)SPI_CQueue_PopMulti(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), ch, &TempSpiWriteBuf[HwChannel][0], sizeof(TempSpiWriteBuf[0])-SPI_MAX_CRC_SIZE);
				ch |= IPC_MULTI_MASK;
			}
		}
		else
		{
			pack_len = (uint8)SPI_CQueue_PopMultiChannel(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), &TempSpiWriteBuf[HwChannel][0], sizeof(TempSpiWriteBuf[0])-SPI_MAX_CRC_SIZE);
			ch |= IPC_MULTI_MASK;
		}
#else
		pack_len = (uint8)SPI_CQueue_Pop(HwChannel, SPI_CQueueGetTxManagerHandle(HwChannel), ch, &TempSpiWriteBuf[HwChannel][1], sizeof(TempSpiWriteBuf[0])-SPI_MAX_CRC_SIZE);
		pack_len += 1;	
#endif
		if(pack_len <= 1)	
		{
			return E_NOT_OK;
		}
		#if SPI_IPC_CRC_SUPPORT
		crc_flag = (s_SpiCrcSupport[HwChannel]);
		CRC_CHECK_ORGAMIZ(crc_flag, pack_len, ch);
		#endif
		MasterReadComplete(HwChannel, &TempSpiWriteBuf[HwChannel][0], ch, pack_len);
	}
	return( err );
}

void HandleSPI_IPC_Messages_Old(uint8 HwChannel)
{
	uint8 err = 0xFF;
	
	
	if(WairtSpiReadComplex(HwChannel) != SPI_STATUS_BUSY)
	{
		memset(&TempSpiReadBuf[HwChannel], 0, 2);
      	err  = SpiReadData(HwChannel, &TempSpiReadBuf[HwChannel][0], SPI_IPC_MSG_HEADER, STD_ON);
	}

	if((err == E_OK) && (GetCurSpiStatus(HwChannel) == SPI_STATUS_SUCESS))
	{
		if(TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS] == SPI_IPC_MASTER_WRITE)
		{
			if(TempSpiReadBuf[HwChannel][1] >= SPI_MAX_FRAME_SIZE)
			{
				return;
			}
			err = SetupAndProcessMasterWrite(HwChannel, &TempSpiReadBuf[HwChannel][1]);
		}
		else if (TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS] == SPI_IPC_MASTER_READ)
		{
			err = SetupAndProcessMasterRead(HwChannel);
		}
		else	
		{
			TracePrintf(IPC_TRACE, TRACE_NONMASK, "[IPC] ch:%d %x %x\r\n", HwChannel,  
				TempSpiReadBuf[HwChannel][SPI_IPC_CMD_POS], TempSpiReadBuf[HwChannel][SPI_IPC_LEN_POS]);
			
			SpiSetLastTransErrorState(HwChannel, 1);
			
			vTaskDelay(1U); 
		}
	}
	else
	{
		vTaskDelay(1U);	
		SpiAbortTransfer(HwChannel);
	}
}

#endif

