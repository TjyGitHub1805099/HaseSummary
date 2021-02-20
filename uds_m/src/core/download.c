/**************************
description: public general process APIs for sid 0x34/0x36/0x37 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/11
**************************/
	
#include"download_if.h"
#include"sid0x27_if.h"
#include"uds_if.h"
//#include "System_Task.h"
#ifdef USING_OS_FREERTOS
#include"freeRtos.h"
#include"flash_if.h"
#endif

typedef enum{
	DOWNLOAD_CBK_INDEX_DISABLE_RESOURCE = 0,
	DOWNLOAD_CBK_MAX_INDEX,
}tDownloadFuncCbkIndex;


extern const tUdsCfgInfor gUdsCfgInfor;
extern const tDownloadCfgInfor gDownloadCfgInfor;

#define DATA_SLOT_LEN 1024
#define FLASH_MIN_BLOCK_LEN 4096
#define SID0X36_DATA_HEAD_LEN 2
#define SID0X36_DATA_CRC_LEN 1
static uint8 gCompressMethod = 0;
static uint8 gEncryptionMethod = 0;
static uint32 gMemoryAddress = 0;
static uint32 gMemorySize = 0;
static boolean gReqDownloadRxFlag = FALSE;
static boolean gTransferExitRxFlag = FALSE;
static boolean gWaitforTransferExitReqFlag = FALSE;
static boolean gWriteFlashIsStartedFlag = FALSE;
static uint8 gEptDownloadBlockSeq = 1;
static uint32 gCurDownloadDataLen = 0;
static uint8 gDownloadBufferBlockSeq = 0;

const uint16 gDownloadMaxNumOfBlock = DATA_SLOT_LEN;
static uint16 gMaxSeq = 0;
static uint16 gBlockCounter = 0;
static uint32 gUpgradeNextMemAddr = 0;

//extern boolean gProgPreConditionFullFilled;
//extern boolean gProgrammingFingerprintUpdated;
//static uint8 gDownloadRxBuffer[FLASH_MIN_BLOCK_LEN] = {1};
static uint8 *gDownloadRxBuffer = NULL;

typedef void (*tDownloadFuncCbk)(); 
static tDownloadFuncCbk gDownloadFuncCbk[DOWNLOAD_CBK_MAX_INDEX] = {0};


static tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len);

const uint8 CRC8_J1850_TABLE[256] =
{
	0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53,
	0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
	0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E,
	0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
	0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4,
	0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
	0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19,
	0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
	0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40,
	0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
	0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D,
	0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
	0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7,
	0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
	0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A,
	0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
	0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75,
	0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
	0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8,
	0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
	0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2,
	0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
	0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F,
	0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
	0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66,
	0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
	0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB,
	0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
	0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1,
	0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
	0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C,
	0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4
};


void udsDownload_register_cbk(uint8 index,void* cbk)
{
	if(index < DOWNLOAD_CBK_MAX_INDEX)
		gDownloadFuncCbk[index] = (tDownloadFuncCbk)cbk;
}

uint8 cal_crc8(const uint8*data,uint16 dataLen)
{
	uint8 crc8Value = 0xff;
	uint16 i = 0;
	
	if(data == NULL)
		return 0;
	
	for(i=0;i<dataLen;i++)
	{
		crc8Value = CRC8_J1850_TABLE[crc8Value ^ data[i]];
	}
	crc8Value = crc8Value^0xFF;
	return crc8Value;
}

void downloadInit()
{
	gWriteFlashIsStartedFlag = FALSE;
	gReqDownloadRxFlag = FALSE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gMaxSeq = 0;
	gWaitforTransferExitReqFlag = FALSE;
	gDownloadBufferBlockSeq = 0;
	gDownloadRxBuffer = NULL;
}


tUdsNRCType sid0x34VerifyData(tUdsReqInfor *reqInfor)
{
	//uint8 compressMethod = 0;
	//uint8 encryptionMethod = 0;
	uint16 lenMemSize = 0;
	uint16 lenMemAddr = 0;
	uint32 rest = 0;
	uint32 ch = reqInfor->ch;
	const uint8* data = reqInfor->data;
	uint16 len = reqInfor->len;

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

	downloadResetStatus(ch);

	gMaxSeq = gMemorySize/DATA_SLOT_LEN;//UDS_PHY_BUFF_LEN;
	rest = gMemorySize%DATA_SLOT_LEN;//UDS_PHY_BUFF_LEN;
	if(rest !=0)
		gMaxSeq += 1;
	gReqDownloadRxFlag = TRUE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gWaitforTransferExitReqFlag = FALSE;

	if(gDownloadRxBuffer == NULL)
	{
		gDownloadRxBuffer = diagMallocMem(FLASH_MIN_BLOCK_LEN);
		if(gDownloadRxBuffer == NULL)
			return NRC_GENERAL_PROGRAMMING_FAILURE;
	}

	if(gDownloadFuncCbk[DOWNLOAD_CBK_INDEX_DISABLE_RESOURCE] != NULL)
		gDownloadFuncCbk[DOWNLOAD_CBK_INDEX_DISABLE_RESOURCE]();
	return NRC_NONE;
}


tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len)
{
	boolean ret = FALSE;
	uint16 realDataLen = len-SID0X36_DATA_CRC_LEN;
	
	if(data == NULL || len==0)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		//sid0x36ResetStatus();
		return NRC_CONDITION_NOT_CORRECT;
	}
	
	#ifdef USING_OS_FREERTOS
	//if(gUpgradeNextMemAddr == 0)
	//	gUpgradeNextMemAddr = gDownloadCfgInfor.flashBaseAddr;

	diagCpyData(gDownloadRxBuffer+gDownloadBufferBlockSeq*DATA_SLOT_LEN,data,realDataLen);

	//for s32k,we can only write 4096*N per time,but the max uds len is 4095,so here we should combine two block[2048*2]
	if(gDownloadBufferBlockSeq <3)
	{
		if(sameBlockFlag == FALSE)
			gDownloadBufferBlockSeq += 1;

		return NRC_NONE;
	}
	else
	{
		gDownloadBufferBlockSeq = 0;
	}

	portENTER_CRITICAL();
	ret = Flash_Sector_Programing(gUpgradeNextMemAddr,gDownloadRxBuffer,FLASH_MIN_BLOCK_LEN);
	portEXIT_CRITICAL();
	
	if(ret != 0)
		return NRC_GENERAL_PROGRAMMING_FAILURE;
	gUpgradeNextMemAddr += FLASH_MIN_BLOCK_LEN;
	#endif

	return NRC_NONE;
		
}


void downloadResetStatus(uint32 ch)
{
	gWriteFlashIsStartedFlag = FALSE;
	gReqDownloadRxFlag = FALSE;
	gEptDownloadBlockSeq = 1;
	gCurDownloadDataLen = 0;
	gMaxSeq = 0;
	gWaitforTransferExitReqFlag = FALSE;
	gDownloadBufferBlockSeq = 0;
	gBlockCounter = 0;
	gUpgradeNextMemAddr = 0;
}

tUdsNRCType sid0x36MainProcess(tUdsReqInfor *reqInfor)
{
	uint16 blockSeq = 0;
	tUdsNRCType ret = NRC_NONE;
	uint16 realDataLen = 0;
	uint32 curTempTotalLen = 0;
	uint8 cal_crc_value = 0;
	uint8 rx_data_crc = 0;
	const uint8* data = reqInfor->data;
	uint16 len = reqInfor->len;
	
	if(data == NULL || len <(SID0X36_DATA_HEAD_LEN+SID0X36_DATA_CRC_LEN+1)) // 1 is for the mini real data len
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	realDataLen = len-SID0X36_DATA_HEAD_LEN-SID0X36_DATA_CRC_LEN;
	blockSeq = data[1];

	if(gReqDownloadRxFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;
	
	if((ret=sid0x36CheckMainPrecondition()) != NRC_NONE)
		return ret;

	//check CRC
	rx_data_crc = data[len-1];
	cal_crc_value = cal_crc8(data+SID0X36_DATA_HEAD_LEN,realDataLen);
	
	if(rx_data_crc!=cal_crc_value)
		return NRC_TRANSFER_DATA_SUSPENDED;
	
	//SRS_flashing_specification_M85_V1.12_20161220.pdf Pg40
	if(gEptDownloadBlockSeq>1 && (blockSeq == (gEptDownloadBlockSeq-1)))//rx repeated last block data
	{
		if((blockSeq<gMaxSeq && realDataLen!=(gDownloadMaxNumOfBlock))
			||(blockSeq==gMaxSeq && (gCurDownloadDataLen-realDataLen)!=(gMemorySize-realDataLen)))//maxNumberOfBlockLength
		{
			return NRC_INCORRECT_MESSAGE_LENGTH;
		}
		
		if((ret = sid0x36WriteData(TRUE,data+SID0X36_DATA_HEAD_LEN,len-SID0X36_DATA_HEAD_LEN)) != NRC_NONE)
			return ret;
	}
	else
	{
		if(gWaitforTransferExitReqFlag == FALSE)
		{
			
			if(blockSeq != gEptDownloadBlockSeq)
				return NRC_WRONG_BLOCK_SEQ_COUNTER;
			
			gBlockCounter++;
			if((gBlockCounter<gMaxSeq && realDataLen!=(gDownloadMaxNumOfBlock))
					|| (gBlockCounter==gMaxSeq && realDataLen>(gDownloadMaxNumOfBlock))
			)
			{
				//downloadResetStatus();
				gBlockCounter--;
				return NRC_INCORRECT_MESSAGE_LENGTH;
			}

			curTempTotalLen = gCurDownloadDataLen+realDataLen;
			if((gBlockCounter<gMaxSeq &&  curTempTotalLen>= gMemorySize)
				|| (gMaxSeq == gBlockCounter && curTempTotalLen != gMemorySize))
			{
				gBlockCounter--;
				return NRC_TRANSFER_DATA_SUSPENDED;
			}

			gCurDownloadDataLen += realDataLen;
			gEptDownloadBlockSeq += 1;
			if((ret = sid0x36WriteData(FALSE,data+2,len-SID0X36_DATA_HEAD_LEN)) != NRC_NONE)
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

tUdsNRCType sid0x37MainProcess(tUdsReqInfor *reqInfor)
{
	const uint8*data = reqInfor->data;
	uint16 len = reqInfor->len;
	
	if(data == NULL || len != 1)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(/*gReqDownloadRxFlag == FALSE || */gWaitforTransferExitReqFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;

	downloadResetStatus(reqInfor->ch);
	
	return NRC_NONE;
}
