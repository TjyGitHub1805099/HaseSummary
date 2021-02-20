/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Ea.c
**          Modifly:brown
**			author: weltry wang
**          Date  : 2017-06-20
**          
**          History:
**          1. Date:2018-02-26
**             Author:weltry wang
**             Modification:
**
******************************************************************************/

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/

/*====================[I N C L U D E S]====================*/

#include "Ea.h" 
#include "MemIf_types.h"

/*====================[V A R I A N T S]====================*/

extern EaBlockConfigType EaBlockConfiguration[];
extern const uint16      EA_VERSION;
extern const uint8       EA_FORMAT;

const uint8  FormatDiskFlagTab[4]={0x5A,0xA5,0x9F,0xAF};

//以下变量定义在mcu的no-volatile ram 区域
/**************start***************/
static uint8  WriteBalanceState = STD_ON;
static uint16 Ea_MCU_NVRAM_Valid = 0;
//static EaBlockAdminType      EaBlockAdmin[EA_BLOCK_NUM_ALL+1] = {0u};
//static uint16                PageNextWriteAdd[PAGENUMALL]     = {0u}; //next write address in pagex
static MemIf_StatusType      MemIf_Status       = MEMIF_UNINIT;
static MemIf_JobResultType   MemIf_JobResult    = MEMIF_JOB_OK;
static EaDeveErrorsType      EaDevelopmentError = EA_E_UNINIT;
/**************end***************/

extern EaBlockAdminType      EaBlockAdmin[];
extern uint16                PageNextWriteAdd[]; //next write address in pagex
extern const uint16 EA_BLOCK_NUM_ALL; 
//0~31  System Markers
#define SysMarkerAddStart    0u
#define SysMarkerAddEnd      31u
#define SysMarkerAddLen      8u
//32~39 Ea Markers
//#define EaMarkerAddStart     32u
//#define EaMarkerAddEnd       39u
//#define EaMarkerAddLen       8u
extern const uint16  EaMarkerAddStart;
extern const uint16  EaMarkerAddEnd;
#define EaMarkerAddLen       8u


//40~159 BlockIndex,a block index need two bytes
//#define BlockIndexAddStart   40u
//#define BlockIndexAddEnd     159u
//#define BlockIndexLen        120u
//#define A_BlockIndexLen      2u
extern const uint16 BlockIndexAddStart;
extern const uint16 BlockIndexAddEnd;
extern const uint8  A_BlockIndexLen;

//160~3159  page1 save diagnostic
//#define PAGEADDSTART_0       160
//#define	PAGEADDEND_0	     3159
extern const uint16   PAGEADDSTART_0;
extern const uint16	  PAGEADDEND_0;

//3160~4095 page2 save others
//#define PAGEADDSTART_1       3160
//#define	PAGEADDEND_1	     4095
extern const uint16 PAGEADDSTART_1;
extern const uint16 PAGEADDEND_1;
extern const uint16 EA_BLOCK_NUM_ALL; 



/*====================[D E F I N E]====================*/

#define  NEWEEPROM  1
#define  NEWVERSION 2
#define  NOCHANGE   3

#define  EaInitReturnCheck(x) if(E_NOT_OK == (x))\
                              {\
                               MemIf_Status = MEMIF_IDLE;\
							   return E_NOT_OK;\
                              }
		
#define  EaReturnCheckBreak(x) if(E_NOT_OK == (x))\
                              {\
							   break;\
                              }

/*====================[F U N C  D E C L A R A T I O N]====================*/
static Std_ReturnType Ea_Inner_Init1();
static Std_ReturnType Ea_Inner_Init2();
static Std_ReturnType Ea_Inner_Init3();
static Std_ReturnType Ea_Inner_Write(uint16 BlockNumber, uint8* DataBufferPtr);
static Std_ReturnType Ea_Inner_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length);
static Std_ReturnType Ea_Inner_InvalidateBlock(uint16 BlockNumber);
static Std_ReturnType Ea_Inner_MemoryUp(uint16 PageNumber);
static Std_ReturnType Ea_Inner_EraseImmediateBlock(uint16 BlockNumber);
static void           Ea_Inner_Memcopy(const uint8* sou, uint8* dst,uint8 Len);
static void           Ea_Inner_MemSet(uint8* sou, uint8 data,uint8 Len);
static Std_ReturnType Ea_Inner_EepromMemoryCopy(uint16 SouAdd, uint16 DstAdd, uint8 Len);
static Std_ReturnType Ea_Inner_EepromMemorySet(uint16 Add, uint8 data, uint8 Len);
static uint16         GetPageStartAdd(EaPageNumType PageNum);
static uint16         GetPageEndAdd(EaPageNumType PageNum);
static EaPageNumType  GetPageNum(uint16 Address);
static uint8          Ea_Inner_EaMarkerCheck(uint8* buff);
static uint8          Ea_Inner_WriteEaMarker();

extern Std_ReturnType Ea_Eep_Write(uint16 address,uint8 *bufferPtr,uint32 length);
extern Std_ReturnType Ea_Eep_Read(uint16 address,uint8 *bufferPtr,uint32 length);

/*====================[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/*************************************************************************/
/*
 * Function Name       Ea_MainFunction()
 * Brief               Ea main funcation
 * Sync/Async          Synchronous
 * Reentrancy          non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
void Ea_MainFunction()
{
   /*this modle is synchronization,do nothing in mainfunction*/
}


/*************************************************************************/
/*
 * Function Name       Initializes the EEPROM abstraction module.
 * Brief               Reads Length bytes of block Blocknumber at offset BlockOffset into the buffer DataBufferPtr.
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Succeed.
 *                     E_NOT_OK: Failed.
 * PreCondition        Driver has initialized
 */
/*************************************************************************/
Std_ReturnType Ea_Init()
{
	uint8  l_Buffer[EaMarkerAddLen]={0u};
	uint16 l_Temp;
	uint8  l_ReturnValue;
	
	if(Ea_MCU_NVRAM_Valid == Ea_BackRamValidValue)
	{
		return E_OK;
	}
	MemIf_Status = MEMIF_BUSY_INTERNAL;
	if(E_NOT_OK == Ea_Eep_Read(EaMarkerAddStart,&l_Buffer[0],EaMarkerAddLen))
	{
		MemIf_Status = MEMIF_UNINIT;
		return E_NOT_OK;
	}
	l_Temp = Ea_Inner_EaMarkerCheck(&l_Buffer[0]);
	switch(l_Temp)
	{
		case NEWEEPROM:
			l_ReturnValue = Ea_Inner_Init1();
			EaInitReturnCheck(l_ReturnValue);
			l_ReturnValue = Ea_Inner_WriteEaMarker();
			EaInitReturnCheck(l_ReturnValue);
			break;
		case NEWVERSION:
			l_ReturnValue = Ea_Inner_Init2();
			EaInitReturnCheck(l_ReturnValue);
			l_ReturnValue = Ea_Inner_WriteEaMarker();
			EaInitReturnCheck(l_ReturnValue);
			break;
		case NOCHANGE:
			l_ReturnValue = Ea_Inner_Init3();
			EaInitReturnCheck(l_ReturnValue);
			break;
	}
	MemIf_Status = MEMIF_IDLE;	
	Ea_MCU_NVRAM_Valid = Ea_BackRamValidValue;
	return E_OK;
}


/*************************************************************************/
/*
 * Function Name       Ea_Read(uint16 BlockNumber,uint16 BlockOffset,uint8* DataBufferPtr,uint16 Length)
 * Brief               Reads Length bytes of block Blocknumber at offset BlockOffset into the buffer DataBufferPtr.
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 *                     BlockOffset: Read address offset inside the block
 *                     Length:      Number of bytes to read
 * Param-Name[out]     DataBufferPtr: Pointer to data buffer
 * Param-Name[in/out]  None
 * Return              E_OK: The requested job has been accepted by the module.
 *                     E_NOT_OK: The requested job has not been accepted by the EA module.
 * PreCondition        Driver has initialized
 */
/*************************************************************************/
Std_ReturnType Ea_Read(uint16 BlockNumber,uint16 BlockOffset,uint8* DataBufferPtr,uint16 Length)
{  
#if (STD_ON == EaDevErrorDetect)	
	if(MemIf_Status==MEMIF_UNINIT)
	{	
		EaDevelopmentError = EA_E_UNINIT;
		return E_NOT_OK;
	}
	if(MemIf_Status==MEMIF_BUSY)
	{	
		EaDevelopmentError = EA_E_BUSY;
		return E_NOT_OK;
	}	
	if((BlockNumber<1u)||(BlockNumber>EA_BLOCK_NUM_ALL))
	{	
		EaDevelopmentError = EA_E_INVALID_BLOCK_NO;
		return E_NOT_OK;
	}
	if(BlockOffset>EaBlockConfiguration[BlockNumber].EaBlockSize)
	{	
		EaDevelopmentError = EA_E_INVALID_BLOCK_OFS;
		return E_NOT_OK;
	}	
	if(Length+BlockOffset>EaBlockConfiguration[BlockNumber].EaBlockSize)
	{
	    EaDevelopmentError = EA_E_INVALID_BLOCK_LEN;
		return E_NOT_OK;
	}	
	if(DataBufferPtr == NULL)
	{
		EaDevelopmentError = EA_E_PARAM_POINTER;
		return E_NOT_OK;
	}	
#endif	
	//��Ԫ�Ƿ� invalid
	if(EaBlockAdmin[BlockNumber].BlockWriteCycles == Blockinvalid)
	{		
		return E_NOT_OK;
	}
	MemIf_Status    = MEMIF_BUSY;
	MemIf_JobResult = MEMIF_JOB_PENDING;
	if(E_OK == Ea_Inner_Read(BlockNumber,BlockOffset,DataBufferPtr,Length))
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_OK;	
		NvM_JobEndNotification();
		return E_OK;
	}
	else
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_FAILED;	
		NvM_JobErrorNotification();
		return E_NOT_OK;
	}	
}


/*************************************************************************/
/*
 * Function Name       Ea_Write(uint16 BlockNumber, uint8* DataBufferPtr)
 * Brief               rites the contents of the DataBufferPtr to the block BlockNumber. 
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 *                     DataBufferPtr: Pointer to data buffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The requested job has been accepted by the module.
 *                     E_NOT_OK: The requested job has not been accepted by the EA module.
 * PreCondition        Driver has initialized
 */
/*************************************************************************/
Std_ReturnType Ea_Write(uint16 BlockNumber, uint8* DataBufferPtr)
{
#if (STD_ON == EaDevErrorDetect)	
	if(MemIf_Status==MEMIF_UNINIT)
	{	
		EaDevelopmentError = EA_E_UNINIT;
		return E_NOT_OK;
	}
	if(MemIf_Status==MEMIF_BUSY)
	{	
		EaDevelopmentError = EA_E_BUSY;
		return E_NOT_OK;
	}
	if((BlockNumber<1u)||(BlockNumber>EA_BLOCK_NUM_ALL))
	{	
		EaDevelopmentError = EA_E_INVALID_BLOCK_NO;
		return E_NOT_OK;
	}
	if(DataBufferPtr == NULL)
	{
		EaDevelopmentError = EA_E_PARAM_POINTER;
	}	
#endif	
	//��Ԫ�Ƿ���Ч
	if( EaBlockAdmin[BlockNumber].BlockWriteCycles >= EaBlockConfiguration[BlockNumber].EaNumberOfWriteCycles
	  ||EaBlockAdmin[BlockNumber].BlockWriteCycles == Blockinvalid)
	{
		return E_NOT_OK;
	}
	MemIf_Status    = MEMIF_BUSY;
	MemIf_JobResult = MEMIF_JOB_PENDING;
	if(E_OK == Ea_Inner_Write(BlockNumber,DataBufferPtr))	
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_OK;	
		NvM_JobEndNotification();
		return E_OK;
	}
	else
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_FAILED;		
		NvM_JobErrorNotification();
		return E_NOT_OK;
	}		
}

/*************************************************************************/
/*
 * Function Name       Ea_Cancel()
 * Brief               Cancels the ongoing asynchronous operation.  
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
void Ea_Cancel()
{
	return;
}

/*************************************************************************/
/*
 * Function Name       Ea_GetStatus()
 * Brief               Service to return the Status. 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
MemIf_StatusType Ea_GetStatus()
{
	return MemIf_Status;
}

/*************************************************************************/
/*
 * Function Name       Ea_GetJobResult()
 * Brief               Service to return the Status. 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              MEMIF_JOB_OK: The last job has been finished successfully. 
 *                     MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed. 
 *                     MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed). 
 *                     MEMIF_JOB_FAILED: The last job was not finished successfully (it failed). 
 *                     MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data.
 * PreCondition        None
 */
/*************************************************************************/
MemIf_JobResultType Ea_GetJobResult()
{
#if (STD_ON == EaDevErrorDetect)	
	if(MemIf_Status==MEMIF_UNINIT)
	{	
		EaDevelopmentError = EA_E_UNINIT;
		return MEMIF_JOB_FAILED;
	}
#endif	

	return MemIf_JobResult;
}

/*************************************************************************/
/*
 * Function Name       Ea_InvalidateBlock(uint16 BlockNumber) 
 * Brief               Invalidates the block BlockNumber.
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The requested job has been accepted by the module. 
 *                     E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 * PreCondition        None
 */
/*************************************************************************/
Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber) 
{
	//uint16 l_BlockNumber = 0;
	//l_BlockNumber   = ((BlockNumber) & 0x0003) + (BlockNumber>>EaDatasetSelectionBits);

#if (STD_ON == EaDevErrorDetect)	
	if(MemIf_Status==MEMIF_UNINIT)
	{	
		EaDevelopmentError = EA_E_UNINIT;
		return E_NOT_OK;
	}
	if(MemIf_Status==MEMIF_BUSY)
	{	
		EaDevelopmentError = EA_E_BUSY;
		return E_NOT_OK;
	}
#endif	
	if((BlockNumber<1u)||(BlockNumber>EA_BLOCK_NUM_ALL))
	{
		EaDevelopmentError = EA_E_INVALID_BLOCK_NO;	
		return E_NOT_OK;
	}	
	MemIf_Status	= MEMIF_BUSY;
	MemIf_JobResult = MEMIF_JOB_PENDING;
	if(E_NOT_OK == Ea_Inner_InvalidateBlock(BlockNumber))	
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_FAILED; 		
		return E_NOT_OK;
	}
	else
	{
		MemIf_JobResult = MEMIF_JOB_OK; 
		MemIf_Status	= MEMIF_IDLE;
		return E_OK;
	}
}

/*************************************************************************/
/*
 * Function Name       Ea_EraseImmediateBlocks(uint16 BlockNumber) 
 * Brief               Erases the block BlockNumber.
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The requested job has been accepted by the module. 
 *                     E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 * PreCondition        None
 */
/*************************************************************************/
Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{

#if (STD_ON == EaDevErrorDetect)	
	if(MemIf_Status==MEMIF_UNINIT)
	{	
		EaDevelopmentError = EA_E_UNINIT;
		return E_NOT_OK;
	}
	if(MemIf_Status==MEMIF_BUSY)
	{	
		EaDevelopmentError = EA_E_BUSY;
		return E_NOT_OK;
	}
#endif	
	if((BlockNumber<1u)||(BlockNumber>EA_BLOCK_NUM_ALL))
	{	
		EaDevelopmentError = EA_E_INVALID_BLOCK_NO;
		return E_NOT_OK;
	}
	MemIf_Status	= MEMIF_BUSY;
	MemIf_JobResult = MEMIF_JOB_PENDING;
	if(E_NOT_OK == Ea_Inner_EraseImmediateBlock(BlockNumber))	
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_FAILED; 		
		return E_NOT_OK;
	}
	else
	{
		MemIf_Status	= MEMIF_IDLE;
		MemIf_JobResult = MEMIF_JOB_OK;
		return E_OK;
	}	
}

/*******************************************EA API for App****************************************/

/*************************************************************************/
/*
 * Function Name       Ea_ResetDevice()
 * Brief               Reset EEPROM Block1,it will reset Nvm when reboot
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : write data to EEPROM fialed
 * PreCondition        None
 */
/*************************************************************************/
Std_ReturnType Ea_ResetDevice()
{
	uint8 l_buffer[8]={0};
	return Ea_Eep_Write(EaMarkerAddStart,&l_buffer[0],EaMarkerAddLen);
}

/*************************************************************************/
/*
 * Function Name       Ea_SetWriteBalance(uint8 State)
 * Brief               Enabled/Disabled balance write 
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      State:STD_ON-Enabled STD_OFF-Disabled
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
void Ea_SetWriteBalance(uint8 State)
{
	WriteBalanceState = State;
}

/************************************** End of EA API for App****************************************/




/*************************************************************************/
/*
 * Function Name       Ea_Inner_Init1()
 * Brief               Initialize EEPROM when EEPROM is a new one
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_Init1()
{
	uint8  i=0u;
	uint16 l_IndexAdd=0;
	uint16 l_DataAdd=0;
	uint16 l_Lengh=0;
	uint8  l_Temp[EA_BlockHeadLength]={0};
	uint8  l_ReturnValue;
	
	PageNextWriteAdd[0] = PAGEADDSTART_0;
	PageNextWriteAdd[1] = PAGEADDSTART_1;
	for(i=1;i<=EA_BLOCK_NUM_ALL;i++)
	{
		if(EA_BLOCK_DELETE == EaBlockConfiguration[i].EaModfiyFlag)
		{
			EaBlockAdmin[i].BlocksDataAddress = BlockIndexAddStart + (i-1)*A_BlockIndexLen;			 
			EaBlockAdmin[i].AddressWriteCycles= 0;
			EaBlockAdmin[i].BlockWriteCycles  = Blockinvalid;	
			continue;
		}
		l_DataAdd  = PageNextWriteAdd[EaBlockConfiguration[i].EaDataPageNum];
		
		l_IndexAdd = BlockIndexAddStart + (i-1)*A_BlockIndexLen;
		l_Temp[0]  = (uint8)l_DataAdd;
		l_Temp[1]  = (uint8)(l_DataAdd>>8);
		//write data address information in data index address
		l_ReturnValue = Ea_Eep_Write(l_IndexAdd,&l_Temp[0],EA_BlockIndexLength);
		EaReturnCheckBreak(l_ReturnValue);
		//write 0 in data data header address
		Ea_Inner_MemSet(&l_Temp[0],0,EA_BlockHeadLength);
		l_ReturnValue = Ea_Eep_Write(l_DataAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		EaBlockAdmin[i].BlocksDataAddress = l_DataAdd;			 
		EaBlockAdmin[i].AddressWriteCycles= 0;
		EaBlockAdmin[i].BlockWriteCycles  = 0;			
		l_Lengh   = EaBlockConfiguration[i].EaBlockSize;
		l_DataAdd = l_DataAdd + EA_BlockHeadLength;

		//block1 is the flag block for Nvm,write this block with 0,
		if(1 ==  i)
		{
			l_DataAdd = l_DataAdd + l_Lengh - EA_CrcLength;//crc address
			l_ReturnValue = Ea_Eep_Write(l_DataAdd,&l_Temp[0],EA_CrcLength);
			EaReturnCheckBreak(l_ReturnValue);
		}
		l_Lengh = l_Lengh + EA_BlockHeadLength;
		PageNextWriteAdd[EaBlockConfiguration[i].EaDataPageNum] += l_Lengh;
	}
	return l_ReturnValue;
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_Init2()
 * Brief               Initialize EEPROM when updata EEPROM block,it will reserve the block data  whicth marked NO_MODFIED
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_Init2()
{
	uint8  i=0;
	uint8  l_Temp[EA_BlockHeadLength]={0u};
	uint16 l_IndexAdd=0;
	uint16 l_TempAdd=0;
	uint16 l_DataAdd;
	uint16 l_LenAll[PAGENUMALL]={0},l_Length[PAGENUMALL]={0};
	uint16 l_NewAdd[PAGENUMALL]={0},l_OldAdd[PAGENUMALL]={0};
	uint8  l_ReturnValue;
	EaPageNumType l_Page;  

	//step1 读出管理信息
	for(i=1;i<=EA_BLOCK_NUM_ALL;i++)
	{
		if(EA_BLOCK_DELETE == EaBlockConfiguration[i].EaModfiyFlag)
		{
			EaBlockAdmin[i].BlocksDataAddress = BlockIndexAddStart + (i-1)*A_BlockIndexLen;			 
			EaBlockAdmin[i].AddressWriteCycles= 0;
			EaBlockAdmin[i].BlockWriteCycles  = Blockinvalid;	
			continue;
		}
		l_Page = EaBlockConfiguration[i].EaDataPageNum;
		l_LenAll[l_Page] = l_LenAll[l_Page] + EaBlockConfiguration[i].EaBlockSize + EA_BlockHeadLength;
		if(EA_BLOCK_NO_MODFIED != EaBlockConfiguration[i].EaModfiyFlag)
		{
			continue;
		}
		l_IndexAdd = BlockIndexAddStart + (i-1)*A_BlockIndexLen;
		l_ReturnValue = Ea_Eep_Read(l_IndexAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		l_DataAdd = (uint16)(l_Temp[0]|(l_Temp[1]<<8));
		l_ReturnValue = Ea_Eep_Read(l_DataAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		EaBlockAdmin[i].BlocksDataAddress = l_DataAdd;			 
		EaBlockAdmin[i].AddressWriteCycles= l_Temp[1];
		EaBlockAdmin[i].BlockWriteCycles  = l_Temp[0];
	}
	if(E_NOT_OK == l_ReturnValue)
	{
		return E_NOT_OK;
	}
	l_NewAdd[0] = PAGEADDSTART_0;
	l_NewAdd[1] = PAGEADDSTART_1;
	Ea_Inner_MemSet(&l_Length[0],0,PAGENUMALL);	
	//step2 memoryup the block which wihtout deleted,added and modified block
	for(i=1;i<=EA_BLOCK_NUM_ALL;i++)
	{
		if(EA_BLOCK_NO_MODFIED != EaBlockConfiguration[i].EaModfiyFlag)
		{
			continue;
		}
		l_Page = EaBlockConfiguration[i].EaDataPageNum;
		l_NewAdd[l_Page] += l_Length[l_Page];
		l_Length[l_Page]  = EaBlockConfiguration[i].EaBlockSize + EA_BlockHeadLength;
		l_OldAdd[l_Page]  = EaBlockAdmin[i].BlocksDataAddress;
		if(l_OldAdd[l_Page] != l_NewAdd[l_Page])
		{
			l_ReturnValue = Ea_Inner_EepromMemoryCopy(l_OldAdd[l_Page], l_NewAdd[l_Page], l_Length[l_Page]);
			EaReturnCheckBreak(l_ReturnValue);
		}			
		EaBlockAdmin[i].BlocksDataAddress  = l_NewAdd[l_Page];
	}
	if(E_NOT_OK == l_ReturnValue)
	{
		return E_NOT_OK;
	}	
	l_NewAdd[0] = PAGEADDSTART_0 + l_LenAll[0];
	l_NewAdd[1] = PAGEADDSTART_1 + l_LenAll[1];
	//step3 reversed order write all data
	for(i=EA_BLOCK_NUM_ALL;i>=1;i--)
	{
		if(EA_BLOCK_DELETE == EaBlockConfiguration[i].EaModfiyFlag)
		{
			continue;
		}
		l_Page = EaBlockConfiguration[i].EaDataPageNum;
		l_Length[l_Page]  = EaBlockConfiguration[i].EaBlockSize + EA_BlockHeadLength;
		l_NewAdd[l_Page]  = l_NewAdd[l_Page] - l_Length[l_Page];
		if(EA_BLOCK_NO_MODFIED != EaBlockConfiguration[i].EaModfiyFlag)
		{
			EaBlockAdmin[i].BlocksDataAddress = l_NewAdd[l_Page];
			EaBlockAdmin[i].AddressWriteCycles= 0;
			EaBlockAdmin[i].BlockWriteCycles  = 0;
		}//no modified, old data is valid
		else
		{
			l_OldAdd[l_Page] = EaBlockAdmin[i].BlocksDataAddress;
			EaBlockAdmin[i].AddressWriteCycles = 0;
			l_ReturnValue = Ea_Inner_EepromMemoryCopy(l_OldAdd[l_Page],l_NewAdd[l_Page],l_Length[l_Page]);
			EaReturnCheckBreak(l_ReturnValue);
			EaBlockAdmin[i].BlocksDataAddress = l_NewAdd[l_Page];
		}
		l_TempAdd = l_NewAdd[l_Page];
		l_IndexAdd = BlockIndexAddStart + (i-1)*A_BlockIndexLen;
		l_Temp[0] = (uint8)l_TempAdd;
		l_Temp[1] = (uint8)(l_TempAdd>>8);
		l_ReturnValue = Ea_Eep_Write(l_IndexAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		if(EA_BLOCK_NO_MODFIED != EaBlockConfiguration[i].EaModfiyFlag)
		{
			Ea_Inner_MemSet(&l_Temp[0],0,EA_BlockHeadLength);
			l_ReturnValue = Ea_Eep_Write(l_TempAdd,&l_Temp[0],EA_BlockHeadLength); 
			EaReturnCheckBreak(l_ReturnValue);
			l_ReturnValue = Ea_Inner_EepromMemorySet(l_TempAdd,0,l_Length[l_Page]);
			EaReturnCheckBreak(l_ReturnValue);
		}	
	}
	if(E_NOT_OK == l_ReturnValue)
	{
		return E_NOT_OK;
	}
	PageNextWriteAdd[0] = PAGEADDSTART_0 + l_LenAll[0];
	PageNextWriteAdd[1] = PAGEADDSTART_1 + l_LenAll[1];
	return E_OK;
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_Init3()
 * Brief               Initialize EEPROM normaly
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_Init3()
{
	uint8  i=0u;
	uint16 l_IndexAdd=0;
	uint16 l_DataAdd=0;
	uint16 l_MaxAdd=0;
	uint8  l_Temp[5]={0};
	uint8  l_ReturnValue;
	for(i=1;i<=EA_BLOCK_NUM_ALL;i++)
	{
		if(EA_BLOCK_DELETE == EaBlockConfiguration[i].EaModfiyFlag)
		{
			EaBlockAdmin[i].BlocksDataAddress = BlockIndexAddStart + (i-1)*A_BlockIndexLen;			 
			EaBlockAdmin[i].AddressWriteCycles= 0;
			EaBlockAdmin[i].BlockWriteCycles  = Blockinvalid;	
			continue;
		}
		l_IndexAdd = BlockIndexAddStart + (i-1)*A_BlockIndexLen;
		l_ReturnValue = Ea_Eep_Read(l_IndexAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		l_DataAdd = (uint16)(l_Temp[0]|(l_Temp[1]<<8));
		l_ReturnValue = Ea_Eep_Read(l_DataAdd,&l_Temp[0],EA_BlockHeadLength);
		EaReturnCheckBreak(l_ReturnValue);
		EaBlockAdmin[i].BlocksDataAddress = l_DataAdd;			 
		EaBlockAdmin[i].AddressWriteCycles= l_Temp[1];
		EaBlockAdmin[i].BlockWriteCycles  = l_Temp[0];

		if(l_DataAdd > l_MaxAdd)
		{
			l_MaxAdd = l_DataAdd;
			PageNextWriteAdd[EaBlockConfiguration[i].EaDataPageNum] = l_MaxAdd + EaBlockConfiguration[i].EaBlockSize + EA_BlockHeadLength;
		}
	}
	return l_ReturnValue;
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_Write(uint16 BlockNumber, uint8* DataBufferPtr)
 * Brief               invoked by Ea_Write
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Refer to Ea_Write
 * Param-Name[out]     Refer to Ea_Write
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_Write(uint16 BlockNumber, uint8* DataBufferPtr)
{	
	uint16 l_DataAdd =0;
	uint16 l_DataLength=0;
	uint16 l_IndexAdd = 0;
	uint8  l_buffer[2]={0};
	uint8  l_return;
	EaPageNumType l_Page;

	l_DataLength = EaBlockConfiguration[BlockNumber].EaBlockSize;
	l_DataAdd    = EaBlockAdmin[BlockNumber].BlocksDataAddress;
	//need write balance
	if(STD_ON == WriteBalanceState)
	{
		EaBlockAdmin[BlockNumber].AddressWriteCycles++;
	}
	if(BlockWriteCycleForever != EaBlockConfiguration[BlockNumber].EaNumberOfWriteCycles)
	{
		EaBlockAdmin[BlockNumber].BlockWriteCycles++;
	}
	l_buffer[0] = (uint8)(EaBlockAdmin[BlockNumber].BlockWriteCycles);
	l_buffer[1] = (uint8)(EaBlockAdmin[BlockNumber].AddressWriteCycles);	
	//don't need a new address to write
	if(AddressWriteCycleLimit >= EaBlockAdmin[BlockNumber].AddressWriteCycles)
	{  		
		l_return = Ea_Eep_Write(l_DataAdd,&l_buffer[0],EA_BlockHeadLength);
		if(E_NOT_OK ==l_return)
			return E_NOT_OK;
		l_return = Ea_Eep_Write((l_DataAdd+EA_BlockHeadLength),DataBufferPtr,l_DataLength);
		if(E_NOT_OK ==l_return)
			return E_NOT_OK;
	}
	//need find a new address to write
	else
	{	
		l_Page = EaBlockConfiguration[BlockNumber].EaDataPageNum;
		if( (PageNextWriteAdd[l_Page]  + l_DataLength + EA_BlockHeadLength ) >= GetPageEndAdd(l_Page) )
		{
			Ea_Inner_MemoryUp(GetPageStartAdd(l_Page));
			//l_DataAdd = EaBlockAdmin[BlockNumber].BlocksDataAddress;
			l_DataAdd = PageNextWriteAdd[l_Page];
			PageNextWriteAdd[l_Page] = l_DataAdd + l_DataLength + EA_BlockHeadLength;

		}
		else
		{
			l_DataAdd = PageNextWriteAdd[l_Page];
			PageNextWriteAdd[l_Page] = l_DataAdd + l_DataLength + EA_BlockHeadLength;
		}
		l_buffer[1] = 0;
		Ea_Eep_Write(l_DataAdd,&l_buffer[0],EA_BlockHeadLength);
		Ea_Eep_Write((l_DataAdd+EA_BlockHeadLength),DataBufferPtr,l_DataLength);
		l_IndexAdd = BlockIndexAddStart+(BlockNumber-1)*A_BlockIndexLen;
		l_buffer[0] = (uint8)(l_DataAdd);
		l_buffer[1] = (uint8)(l_DataAdd>>8);
		Ea_Eep_Write(l_IndexAdd,&l_buffer[0],EA_BlockHeadLength);	
		EaBlockAdmin[BlockNumber].AddressWriteCycles = 0;
		EaBlockAdmin[BlockNumber].BlocksDataAddress  = l_DataAdd;
	}
	return E_OK;
}

/*************************************************************************/
/*
 * Function Name       Ea_Inner_InvalidateBlock(uint16 BlockNumber)
 * Brief               invoked by Ea_Inner_InvalidateBlock
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Refer to Ea_InvalidateBlock
 * Param-Name[out]     Refer to Ea_InvalidateBlock
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_InvalidateBlock(uint16 BlockNumber)
{
	uint16 l_DataAddress=0u;
	uint32 l_BlockWriteCycle=0u;
	uint8  l_Tmp=0;
	Std_ReturnType l_Returnv=E_NOT_OK;
	
	l_BlockWriteCycle = EaBlockConfiguration[BlockNumber].EaNumberOfWriteCycles;
	if(Blockinvalid == l_BlockWriteCycle)
	{
		return E_OK;
	}
	else
	{
		l_DataAddress = EaBlockAdmin[BlockNumber].BlocksDataAddress;
		EaBlockAdmin[BlockNumber].BlockWriteCycles = Blockinvalid;
		l_Tmp = Blockinvalid;
		l_Returnv = Ea_Eep_Write(l_DataAddress,&l_Tmp,1);	
		return l_Returnv;
	}
}

 
/*************************************************************************/
/*
 * Function Name       Ea_Inner_EraseImmediateBlock(uint16 BlockNumber)
 * Brief               invoked by Ea_EraseImmediateBlock
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Refer to Ea_EraseImmediateBlock
 * Param-Name[out]     Refer to Ea_EraseImmediateBlock
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_EraseImmediateBlock(uint16 BlockNumber)
{
	if(EaBlockAdmin[BlockNumber].BlockWriteCycles>=EaBlockConfiguration[BlockNumber].EaNumberOfWriteCycles)
	{
		return E_NOT_OK;
	}
   // blockaddress = EaBlockAdmin[BlockNumber].BlocksDataAddress;
	return E_OK;//Eep_Erase((blockaddress+5),length);
}

#if(STD_ON == EaSetModeSupported)
/*************************************************************************/
/*
 * Function Name       Ea_Inner_SetMode(MemIf_ModeType Mode)
 * Brief               invoked by Ea_SetMode
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Refer to Ea_SetMode
 * Param-Name[out]     Refer to Ea_SetMode
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_SetMode(MemIf_ModeType Mode)
{

 return E_OK;

}
#endif

/*************************************************************************/
/*
 * Function Name       Ea_Inner_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
 * Brief               invoked by Ea_Read
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Refer to Ea_Read
 * Param-Name[out]     Refer to Ea_Read
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
	uint16 l_DataAddress=E_NOT_OK;	
	Std_ReturnType l_ReturnVar=0;
	l_DataAddress     = EaBlockAdmin[BlockNumber].BlocksDataAddress;
	l_DataAddress = l_DataAddress + BlockOffset;
	l_DataAddress = l_DataAddress + EA_BlockHeadLength;//do not need read header
	l_ReturnVar = (Ea_Eep_Read(l_DataAddress,DataBufferPtr,Length));
	return 	l_ReturnVar;
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_MemoryUp(uint16 PageAddress)
 * Brief               MemoryUp EEPROM block
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      PageAddress:page start address
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_MemoryUp(uint16 PageAddress)
{
	uint16 i=0;
	uint8  l_DataBuffer[EA_DataBufferLength]={0};
	uint16 l_NewAdd =0;
	uint16 l_OldAdd =0;
	uint16 l_Length =0;
	uint16 l_IndexAdd=0;
	uint8  l_PageNum=0;
	l_NewAdd = PageAddress;
	for(i=1;i<=EA_BLOCK_NUM_ALL;i++)
	{		
		if(EA_BLOCK_DELETE == EaBlockConfiguration[i].EaModfiyFlag)
		{
			continue;
		}
		if(PageAddress == GetPageStartAdd(EaBlockConfiguration[i].EaDataPageNum))
		{
			l_OldAdd = EaBlockAdmin[i].BlocksDataAddress;
			if(l_OldAdd == l_NewAdd)
			{
				EaBlockAdmin[i].AddressWriteCycles= 0;
				l_NewAdd = l_NewAdd + EaBlockConfiguration[i].EaBlockSize + EA_BlockHeadLength;
				continue;
			}
			l_Length = EaBlockConfiguration[i].EaBlockSize;
			Ea_Eep_Read(l_OldAdd,&l_DataBuffer[0],EA_BlockHeadLength);
			l_OldAdd = l_OldAdd + EA_BlockHeadLength;
			l_DataBuffer[1] = 0;
			Ea_Eep_Write(l_NewAdd, &l_DataBuffer[0], EA_BlockHeadLength);			
			EaBlockAdmin[i].BlocksDataAddress = l_NewAdd;
			EaBlockAdmin[i].AddressWriteCycles= 0;
			l_DataBuffer[0] = (uint8)l_NewAdd;
			l_DataBuffer[1] = (uint8)(l_NewAdd>>8);
			l_IndexAdd    = BlockIndexAddStart + (i-1)*A_BlockIndexLen;
			Ea_Eep_Write(l_IndexAdd, &l_DataBuffer[0], EA_BlockHeadLength);
			l_NewAdd = l_NewAdd + EA_BlockHeadLength;
			Ea_Inner_EepromMemoryCopy(l_OldAdd,l_NewAdd,l_Length);
			l_NewAdd = l_NewAdd + l_Length;
		}
	}
	l_PageNum = GetPageNum(l_NewAdd);
	PageNextWriteAdd[l_PageNum] = l_NewAdd;
	return E_OK;
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_Memcopy(const uint8* sou, uint8* dst,uint8 Len)
 * Brief               memory copy
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      sou: source address
 *                     Len: copy length
 * Param-Name[out]     dst: destination address
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void Ea_Inner_Memcopy(const uint8* sou, uint8* dst,uint8 Len)
{
	uint8 l_loop=0;
	for(l_loop=0u; l_loop<Len; l_loop++)
	{
		*(dst+l_loop) = *(sou + l_loop);
	}
	return ;
}

/*************************************************************************/
/*
 * Function Name       Ea_Inner_MemSet(const uint8* sou, uint8* dst,uint8 Len)
 * Brief               Memory set
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      sou : source address
 *                     data: set data
 *                     Len : length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void Ea_Inner_MemSet(uint8* sou, uint8 data,uint8 Len)
{
	uint8 l_loop=0;
	for(l_loop=0u; l_loop<Len; l_loop++)
	{
		*(sou+l_loop) = data;
	}
	return ;
}

/*************************************************************************/
/*
 * Function Name       Ea_Inner_EepromMemorySet(uint16 Add, uint8 data, uint8 Len)
 * Brief               Memory set in EEPROM
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Add : EEPROM address
 *                     data: set data
 *                     Len : length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : Fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_EepromMemorySet(uint16 Add, uint8 data, uint8 Len)
{
	uint8  l_Buffer[EA_DataBufferLength]={0};
	uint16 l_NewAdd = 0;
	uint16 l_Length = 0;
	uint16 l_Loop   = 0;
	uint8  l_ReturnValue;
	l_Length = Len;
	l_NewAdd = Add;
	Ea_Inner_MemSet(&l_Buffer[0],data,EA_DataBufferLength);
	while(0 != l_Length)
	{	
		if(l_Length > EA_DataBufferLength)
		{
			l_Loop	 = EA_DataBufferLength;
			l_Length = l_Length -  EA_DataBufferLength;
		}
		else
		{
			l_Loop	 =	l_Length;
			l_Length =	0;
		}
		l_ReturnValue = Ea_Eep_Write(l_NewAdd,&l_Buffer[0],l_Loop);
		EaReturnCheckBreak(l_ReturnValue);
		l_NewAdd = l_NewAdd + EA_DataBufferLength;
	}
	return l_ReturnValue;


}

/*************************************************************************/
/*
 * Function Name       Ea_Inner_EaMarkerCheck(uint8* buff)
 * Brief               Check Ea marker
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      buff: Ea marker
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              NEWEEPROM  : this is a new EEPROM
 *                     NEWVERSION : has updata EEPRON version
 *                     NOCHANGE   : normaly
 * PreCondition        None
 */
/*************************************************************************/
static uint8 Ea_Inner_EaMarkerCheck(uint8* buff)
{
	uint16 l_Version;
	if(   0x5A != buff[0]
		||0xA5 != buff[1]
		||0xA5 != buff[6]
		||0x5A != buff[7])
	{
	     return NEWEEPROM;
	}
	else
	{
		if(   (uint8)buff[2] != (uint8)(~buff[4])
			||(uint8)buff[3] != (uint8)(~buff[5]))
		{
			return NEWEEPROM;
		}
		l_Version = (uint16)((buff[2]<<8)|(buff[3]));
		if(EA_VERSION != l_Version)
		{
			//return NEWEEPROM;
			return NEWVERSION;
		}
		else
		{
			return NOCHANGE;
		}
	}
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_WriteEaMarker()
 * Brief               Write Ea marker in EEPROM
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : Fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_WriteEaMarker()
{
	uint8 l_DataBuffer[8]={0};
	l_DataBuffer[0] = 0x5A;
	l_DataBuffer[1] = 0xA5;
	l_DataBuffer[2] = (uint8)(EA_VERSION>>8);
	l_DataBuffer[3] = (uint8)EA_VERSION;
	l_DataBuffer[4] = ~l_DataBuffer[2];
	l_DataBuffer[5] = ~l_DataBuffer[3];
	l_DataBuffer[6] = 0xA5;
	l_DataBuffer[7] = 0x5A;
	return Ea_Eep_Write(EaMarkerAddStart,&l_DataBuffer[0],EaMarkerAddLen);
}


/*************************************************************************/
/*
 * Function Name       Ea_Inner_EepromMemoryCopy(uint16 SouAdd, uint16 DstAdd, uint8 Len)
 * Brief               Memory copy in EEPROM it copyed by reversed order
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      SouAdd : EEPROM source address
 *                     DstAdd : EEPROM destination address
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : Fialed.
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType Ea_Inner_EepromMemoryCopy(uint16 SouAdd, uint16 DstAdd, uint8 Len)
{
	uint8  l_DataBuffer[EA_DataBufferLength]={0};
	uint16 l_NewAdd = 0;
	uint16 l_OldAdd = 0;
	uint16 l_Length = 0;
	uint16 l_Loop   = 0;
	uint8  l_ReturnValue;

	l_Length = Len;
	l_NewAdd = DstAdd + Len;
	l_OldAdd = SouAdd + Len;
	while(0 != l_Length)
	{	
		if(l_Length > EA_DataBufferLength)
		{
			l_Loop	 = EA_DataBufferLength;
			l_Length = l_Length -  EA_DataBufferLength;
		}
		else
		{
			l_Loop	 =	l_Length;
			l_Length =	0;
		}
		l_OldAdd = l_OldAdd - l_Loop;
		l_NewAdd = l_NewAdd - l_Loop;
		l_ReturnValue = Ea_Eep_Read (l_OldAdd,&l_DataBuffer[0],l_Loop);
		EaReturnCheckBreak(l_ReturnValue);
		l_ReturnValue = Ea_Eep_Write(l_NewAdd,&l_DataBuffer[0],l_Loop);
		EaReturnCheckBreak(l_ReturnValue);
	}
	return l_ReturnValue;
}


/*************************************************************************/
/*
 * Function Name       GetPageEndAdd(EaPageNumType level)
 * Brief               Get page end address by page number
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      PageNum : Page number
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Page end address
 * PreCondition        None
 */
/*************************************************************************/
uint16 GetPageEndAdd(EaPageNumType PageNum)
{
	switch(PageNum)
	{
		case Page0:
			return PAGEADDEND_0;
		case Page1:
			return PAGEADDEND_1;
		default:
			return 1;
	}
}

/*************************************************************************/
/*
 * Function Name       GetPageEndAdd(EaPageNumType level)
 * Brief               Get page start address by page number
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      PageNum : Page number
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Page start address
 * PreCondition        None
 */
/*************************************************************************/
uint16 GetPageStartAdd(EaPageNumType PageNum)
{
	switch(PageNum)
	{
		case Page0:
			return PAGEADDSTART_0;
		case Page1:
			return PAGEADDSTART_1;		
		default:
			return 1;
	}
}

/*************************************************************************/
/*
 * Function Name       GetPageEndAdd(EaPageNumType level)
 * Brief               Get page page number by address
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      Address : EEPROM address
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PAGEADDEND_0 : page0
 *                     PAGEADDEND_1 : page1
 * PreCondition        None
 */
/*************************************************************************/
EaPageNumType GetPageNum(uint16 Address)
{
	if((Address>=PAGEADDSTART_0)&&(Address<PAGEADDEND_0))
	{
		return Page0;
	}
	if((Address>=PAGEADDSTART_1)&&(Address<PAGEADDEND_1))
	{
		return  Page1;
	}
	else
	{
		return  1;/*error*/
	}
}


