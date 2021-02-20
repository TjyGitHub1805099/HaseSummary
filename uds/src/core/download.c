/**************************
description: public general process APIs for sid 0x34/0x36/0x37 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/11
**************************/
	
#include"download_if.h"
#include"sid0x27_if.h"
#include"uds_if.h"

extern const tUdsCfgInfor gUdsCfgInfor;
extern const tDownloadCfgInfor gDownloadCfgInfor;


uint8 gCompressMethod = 0;
uint8 gEncryptionMethod = 0;
uint32 gMemoryAddress = 0;
uint32 gMemorySize = 0;
boolean gReqDownloadRxFlag = FALSE;
boolean gTransferExitRxFlag = FALSE;
boolean gWaitforTransferExitReqFlag = FALSE;
boolean gWriteFlashIsStartedFlag = FALSE;
uint8 gEptDownloadBlockSeq = 1;
uint32 gCurDownloadDataLen = 0;

uint16 gDownloadMaxNumOfBlock = 0;
uint16 gMaxSeq = 0;
//extern boolean gProgPreConditionFullFilled;
//extern boolean gProgrammingFingerprintUpdated;

static tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len);

void downloadInit()
{
	gDownloadMaxNumOfBlock = gUdsCfgInfor.udsRxTxBufferLen;//0x802
	gWriteFlashIsStartedFlag = FALSE;
	gReqDownloadRxFlag = FALSE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gMaxSeq = 0;
	gWaitforTransferExitReqFlag = FALSE;
}


tUdsNRCType sid0x34VerifyData(const uint8* data, uint16 len)
{
	//uint8 compressMethod = 0;
	//uint8 encryptionMethod = 0;
	uint16 lenMemSize = 0;
	uint16 lenMemAddr = 0;
	uint32 rest = 0;
	

	if(data == NULL || len <11)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	gCompressMethod = (0xF0&data[1])>>4;// 0--uncompressed,1--vector compress
	gEncryptionMethod = 0x0F&data[1];//0 -- unEncrypted

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

	if(gMemorySize>gDownloadCfgInfor.downloadDataTotalLen)
		return NRC_REQUEST_OUT_OF_RANGE;

	downloadResetStatus();

	gMaxSeq = gMemorySize/2048;//UDS_PHY_BUFF_LEN;
	rest = gMemorySize%2048;//UDS_PHY_BUFF_LEN;
	if(rest !=0)
		gMaxSeq += 1;
	gReqDownloadRxFlag = TRUE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gWaitforTransferExitReqFlag = FALSE;
	
	return NRC_NONE;	
}


tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len)
{
	static uint32 nextMemAddr = 0;
	static uint32 nextMemSize = 0;

	if(data == NULL || len==0)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		//sid0x36ResetStatus();
		return NRC_CONDITION_NOT_CORRECT;
	}

	if(gWriteFlashIsStartedFlag == FALSE)
	{
		nextMemAddr = gMemoryAddress;
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
	#if 0
	if(Flash_Sector_Programing(nextMemAddr,(uint16*)(data),len/2)!=E_OK)
	{
		sid0x36ResetStatus();
		return NRC_GENERAL_PROGRAMMING_FAILURE;
	}
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
	
	realDataLen = len-2;
	blockSeq = data[1];

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
		
		if((ret = sid0x36WriteData(TRUE,data+2,realDataLen)) != NRC_NONE)
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
			if((ret = sid0x36WriteData(FALSE,data+2,realDataLen)) != NRC_NONE)
				return ret;
			else
			{
				if(gCurDownloadDataLen == gMemorySize)
				{
					gWaitforTransferExitReqFlag = TRUE;
				}
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
