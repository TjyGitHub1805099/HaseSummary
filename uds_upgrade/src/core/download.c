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

extern boolean uds_write_flash(uint32 addr, uint8 *data, uint16 wr_size );

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
static uint8 gDownloadRxBuffer[FLASH_MIN_BLOCK_LEN] = {1};

static tUdsNRCType sid0x36WriteData(boolean sameBlockFlag,const uint8* data, uint16 len);



#if 0
static const uint32 crc32c_table[256] = {
	0x00000000L, 0xF26B8303L, 0xE13B70F7L, 0x1350F3F4L,
	0xC79A971FL, 0x35F1141CL, 0x26A1E7E8L, 0xD4CA64EBL,
	0x8AD958CFL, 0x78B2DBCCL, 0x6BE22838L, 0x9989AB3BL,
	0x4D43CFD0L, 0xBF284CD3L, 0xAC78BF27L, 0x5E133C24L,
	0x105EC76FL, 0xE235446CL, 0xF165B798L, 0x030E349BL,
	0xD7C45070L, 0x25AFD373L, 0x36FF2087L, 0xC494A384L,
	0x9A879FA0L, 0x68EC1CA3L, 0x7BBCEF57L, 0x89D76C54L,
	0x5D1D08BFL, 0xAF768BBCL, 0xBC267848L, 0x4E4DFB4BL,
	0x20BD8EDEL, 0xD2D60DDDL, 0xC186FE29L, 0x33ED7D2AL,
	0xE72719C1L, 0x154C9AC2L, 0x061C6936L, 0xF477EA35L,
	0xAA64D611L, 0x580F5512L, 0x4B5FA6E6L, 0xB93425E5L,
	0x6DFE410EL, 0x9F95C20DL, 0x8CC531F9L, 0x7EAEB2FAL,
	0x30E349B1L, 0xC288CAB2L, 0xD1D83946L, 0x23B3BA45L,
	0xF779DEAEL, 0x05125DADL, 0x1642AE59L, 0xE4292D5AL,
	0xBA3A117EL, 0x4851927DL, 0x5B016189L, 0xA96AE28AL,
	0x7DA08661L, 0x8FCB0562L, 0x9C9BF696L, 0x6EF07595L,
	0x417B1DBCL, 0xB3109EBFL, 0xA0406D4BL, 0x522BEE48L,
	0x86E18AA3L, 0x748A09A0L, 0x67DAFA54L, 0x95B17957L,
	0xCBA24573L, 0x39C9C670L, 0x2A993584L, 0xD8F2B687L,
	0x0C38D26CL, 0xFE53516FL, 0xED03A29BL, 0x1F682198L,
	0x5125DAD3L, 0xA34E59D0L, 0xB01EAA24L, 0x42752927L,
	0x96BF4DCCL, 0x64D4CECFL, 0x77843D3BL, 0x85EFBE38L,
	0xDBFC821CL, 0x2997011FL, 0x3AC7F2EBL, 0xC8AC71E8L,
	0x1C661503L, 0xEE0D9600L, 0xFD5D65F4L, 0x0F36E6F7L,
	0x61C69362L, 0x93AD1061L, 0x80FDE395L, 0x72966096L,
	0xA65C047DL, 0x5437877EL, 0x4767748AL, 0xB50CF789L,
	0xEB1FCBADL, 0x197448AEL, 0x0A24BB5AL, 0xF84F3859L,
	0x2C855CB2L, 0xDEEEDFB1L, 0xCDBE2C45L, 0x3FD5AF46L,
	0x7198540DL, 0x83F3D70EL, 0x90A324FAL, 0x62C8A7F9L,
	0xB602C312L, 0x44694011L, 0x5739B3E5L, 0xA55230E6L,
	0xFB410CC2L, 0x092A8FC1L, 0x1A7A7C35L, 0xE811FF36L,
	0x3CDB9BDDL, 0xCEB018DEL, 0xDDE0EB2AL, 0x2F8B6829L,
	0x82F63B78L, 0x709DB87BL, 0x63CD4B8FL, 0x91A6C88CL,
	0x456CAC67L, 0xB7072F64L, 0xA457DC90L, 0x563C5F93L,
	0x082F63B7L, 0xFA44E0B4L, 0xE9141340L, 0x1B7F9043L,
	0xCFB5F4A8L, 0x3DDE77ABL, 0x2E8E845FL, 0xDCE5075CL,
	0x92A8FC17L, 0x60C37F14L, 0x73938CE0L, 0x81F80FE3L,
	0x55326B08L, 0xA759E80BL, 0xB4091BFFL, 0x466298FCL,
	0x1871A4D8L, 0xEA1A27DBL, 0xF94AD42FL, 0x0B21572CL,
	0xDFEB33C7L, 0x2D80B0C4L, 0x3ED04330L, 0xCCBBC033L,
	0xA24BB5A6L, 0x502036A5L, 0x4370C551L, 0xB11B4652L,
	0x65D122B9L, 0x97BAA1BAL, 0x84EA524EL, 0x7681D14DL,
	0x2892ED69L, 0xDAF96E6AL, 0xC9A99D9EL, 0x3BC21E9DL,
	0xEF087A76L, 0x1D63F975L, 0x0E330A81L, 0xFC588982L,
	0xB21572C9L, 0x407EF1CAL, 0x532E023EL, 0xA145813DL,
	0x758FE5D6L, 0x87E466D5L, 0x94B49521L, 0x66DF1622L,
	0x38CC2A06L, 0xCAA7A905L, 0xD9F75AF1L, 0x2B9CD9F2L,
	0xFF56BD19L, 0x0D3D3E1AL, 0x1E6DCDEEL, 0xEC064EEDL,
	0xC38D26C4L, 0x31E6A5C7L, 0x22B65633L, 0xD0DDD530L,
	0x0417B1DBL, 0xF67C32D8L, 0xE52CC12CL, 0x1747422FL,
	0x49547E0BL, 0xBB3FFD08L, 0xA86F0EFCL, 0x5A048DFFL,
	0x8ECEE914L, 0x7CA56A17L, 0x6FF599E3L, 0x9D9E1AE0L,
	0xD3D3E1ABL, 0x21B862A8L, 0x32E8915CL, 0xC083125FL,
	0x144976B4L, 0xE622F5B7L, 0xF5720643L, 0x07198540L,
	0x590AB964L, 0xAB613A67L, 0xB831C993L, 0x4A5A4A90L,
	0x9E902E7BL, 0x6CFBAD78L, 0x7FAB5E8CL, 0x8DC0DD8FL,
	0xE330A81AL, 0x115B2B19L, 0x020BD8EDL, 0xF0605BEEL,
	0x24AA3F05L, 0xD6C1BC06L, 0xC5914FF2L, 0x37FACCF1L,
	0x69E9F0D5L, 0x9B8273D6L, 0x88D28022L, 0x7AB90321L,
	0xAE7367CAL, 0x5C18E4C9L, 0x4F48173DL, 0xBD23943EL,
	0xF36E6F75L, 0x0105EC76L, 0x12551F82L, 0xE03E9C81L,
	0x34F4F86AL, 0xC69F7B69L, 0xD5CF889DL, 0x27A40B9EL,
	0x79B737BAL, 0x8BDCB4B9L, 0x988C474DL, 0x6AE7C44EL,
	0xBE2DA0A5L, 0x4C4623A6L, 0x5F16D052L, 0xAD7D5351L
};
 
 
static uint32 calculate_CRC32 (void *pStart, uint32 uSize)
{
	#define INIT  0xffffffffL
	#define XOROT 0xffffffffL
 
	uint32 uCRCValue;
	uint8 *pData;
 
	uCRCValue = INIT;
	pData = pStart;
 
	while (uSize --)
	{
		uCRCValue = crc32c_table[(uCRCValue ^ *pData++) & 0xFFL] ^ (uCRCValue >> 8);
	}
 
	return uCRCValue ^ XOROT;
}
#endif

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

	gMaxSeq = gMemorySize/DATA_SLOT_LEN;//UDS_PHY_BUFF_LEN;
	rest = gMemorySize%DATA_SLOT_LEN;//UDS_PHY_BUFF_LEN;
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
	boolean ret = FALSE;
	uint16 realDataLen = len-SID0X36_DATA_CRC_LEN;
	
	if(data == NULL || len==0)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(diagCheckPreCondtionGroup2() == FALSE)
	{
		//sid0x36ResetStatus();
		return NRC_CONDITION_NOT_CORRECT;
	}
	
	//#ifdef USING_OS_FREERTOS
	if(gUpgradeNextMemAddr == 0)
		gUpgradeNextMemAddr = gDownloadCfgInfor.flashBaseAddr;

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
	
	ret = uds_write_flash(gUpgradeNextMemAddr,gDownloadRxBuffer,FLASH_MIN_BLOCK_LEN);

	if(ret != 0)
		return NRC_GENERAL_PROGRAMMING_FAILURE;
	gUpgradeNextMemAddr += FLASH_MIN_BLOCK_LEN;
	//#endif

	return NRC_NONE;
		
}


void downloadResetStatus()
{
	TracePrintf(GENERAL_TRACE,TRACE_WARNING,"downloadResetStatus\r\n");
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

tUdsNRCType sid0x36MainProcess(const uint8* data, uint16 len)
{
	uint16 blockSeq = 0;
	tUdsNRCType ret = NRC_NONE;
	uint16 realDataLen = 0;
	uint32 curTempTotalLen = 0;
	uint8 cal_crc_value = 0;
	uint8 rx_data_crc = 0;
	
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

tUdsNRCType sid0x37MainProcess(const uint8*data, uint16 len)
{
	if(data == NULL || len != 1)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(/*gReqDownloadRxFlag == FALSE || */gWaitforTransferExitReqFlag == FALSE)
		return NRC_REQUEST_SEQUENCE_ERROR;

	downloadResetStatus();
	download_ClearUpdateFlag();
	
	return NRC_NONE;
}
