//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "trace_cfg.h"
#include "trace_if.h"
#include "Std_Types.h" 
#include "BoardDefines.h"
#include "System_Module_Cfg.h"
#include "System_Portable.h"
#include "System_Mutex.h"
#include "uart_pal.h"
#include "string.h"
#include "system_tick.h"
#include "circularQueue.h"
#include "lpuart_hw_access.h"

static boolean TraceCheckEndFlag(uint8 *pos);
static boolean TraceParseReq();

#define TRACE_TX_BIG_BUFF_LEN 2048
#define TRACE_TX_CELL_BUFF_LEN 80
#define TRACE_RX_CELL_BUFF_LEN 50
#define TRACE_RX_BIG_BUFF_LEN 54

SystemMutexType trace_mutex;
static uint8 gTraceTxBigBuffer[TRACE_TX_BIG_BUFF_LEN];
static uint8 gTraceTxBufferElem[TRACE_TX_CELL_BUFF_LEN];

static uint8 gTraceRxBigBuffer[TRACE_RX_BIG_BUFF_LEN] = {0};
static uint8 gTraceRxBufferElem[TRACE_RX_CELL_BUFF_LEN] = {0};


static uint8 gTraceFailPushBytes = 0;
static uint8 gTraceFailTxBytes = 0;

static uint8 gTraceModuleSwitch[MODULE_TRACE_NAME_MAX] = {0};

static boolean gTraceSwitchOn = TRUE;
static uint8 gTraceLevel = TRACE_INFO;

//####### DON'T CHANGE THE ORDER#######
//####### THESE ORDER MUST BE SAME AS THE ONES IN TYPE "Module_Trace_Name" IN trace.h
static tTraceModuleInforList gTraceModuleInfoList[] = {
		{"system",SYSTEM_TRACE,1,TRACE_WARNING},
		{"ipc",IPC_TRACE,1,TRACE_WARNING},
		{"com",COM_TRACE,1,TRACE_WARNING},
		{"canapp",CANAPP_TRACE,1,TRACE_WARNING},
		{"nm",NM_TRACE,1,TRACE_WARNING},
		{"pm",PM_TRACE,1,TRACE_WARNING},
		{"input",INPUT_TRACE,1,TRACE_WARNING},
		{"tp",TP_TRACE,1,TRACE_WARNING},
		{"j1939tp",J1939TP_TRACE,1,TRACE_WARNING},
		{"uds",UDS_TRACE,1,TRACE_WARNING},
		{"diagapp",DIAGAPP_TRACE,1,TRACE_WARNING},
		{"ame",AME_TRACE,1,TRACE_WARNING},
		{"alarm",ALARM_TRACE,1,TRACE_WARNING},
		{"virtual",VIRTUAL_TRACE,1,TRACE_WARNING},
		{"general",GENERAL_TRACE,1,TRACE_WARNING},
		{"test",TEST_TRACE,1,TRACE_WARNING},
	};

static uint8 gTraceNameCount = 0;

uint8 gTraceCmdElement[TRACE_CMD_NUM][TRACE_CMD_ELEMENT_LEN];


#if (defined(UART_OVER_LPUART))
    /*! @brief LPUART state structures */
    lpuart_state_t g_lpuartState[NO_OF_LPUART_INSTS_FOR_UART];
    /*! @brief LPUART state-instance matching */
    uart_instance_t g_lpuartStateInstanceMapping[NO_OF_LPUART_INSTS_FOR_UART];
    /*! @brief LPUART available resources table */
    bool g_lpuartStateIsAllocated[NO_OF_LPUART_INSTS_FOR_UART];
#endif


#ifdef TRACE_DEBUG_NEW
#define io_putc(data,len) 

uint16 vpf(uint8 * l_buf_p,char *fmtstr, va_list va)
{
	char *cptr,*va_char;
	uint16 loop = 0u,totalLen = 5;
	int	va_int = 0;
	boolean fmtStart = FALSE;
	boolean convertFalg = FALSE;

	for (cptr = fmtstr; *cptr != '\0'; cptr++)
	{
		if(TRUE == convertFalg)
		{
			convertFalg = FALSE;
		}
		else if(*cptr == '\\')
		{
			convertFalg = TRUE;
		}
		else if (*cptr == '%')
		{
			fmtStart = TRUE;
		}
		else if (TRUE == fmtStart)
		{
			if (*cptr == 's')
			{
				fmtStart = FALSE;
				va_char = va_arg(va, char*);
				while(*va_char)
				{
					*(l_buf_p+totalLen++) = *va_char++;
				}
				*(l_buf_p+totalLen++) = '\0';
			}
			else if((*cptr == 'd')||(*cptr == 'u')||(*cptr == 'x')||(*cptr == 'X')||(*cptr == 'o'))
			{
				fmtStart = FALSE;
				va_int = va_arg(va, int);
				loop = 0;
				while(loop<4)
				{
					*(l_buf_p+totalLen) = *((uint8*)(&va_int)+loop);
					loop++;
					totalLen++;
				}
			}
		}
	}
	return(totalLen);
}

//Encoder
/*
 * StuffData byte stuffs ¡°length¡± bytes of
 * data at the location pointed to by ¡°ptr¡±,
 * writing the output to the location pointed
 * to by ¡°dst¡±.
 */
#define FinishBlock(X) (*code_ptr = (X), \
						code_ptr = dst++, \
						code = 0x01 )
uint16 StuffData(const uint8 *ptr, uint16 length,uint8 *dst)
{
	const uint8 *end = ptr + length;
	uint8 *code_ptr = dst++;
	uint8 code = 0x01;
	uint16 totalLen = 0u;
	
	if ((length != 0)&&(NULL_PTR != ptr)&&(NULL_PTR != dst))
	{
		while (ptr < end)
		{
			if (*ptr == 0)
			{
				FinishBlock(code);
				totalLen++;
			}
			else
			{
				*dst++ = *ptr;
				totalLen++;
				if (++code == 0xFF)
				{
					FinishBlock(code);
					totalLen++;
				}
			}
			ptr++;
		}
		FinishBlock(code);
		*(--dst) = 0;
		totalLen+=2;
	}
	return(totalLen);
}

void StuffData_addlen(const unsigned char *ptr, unsigned long length,unsigned char *dst)
{
	const unsigned char *end = ptr + length;
	unsigned char *code_ptr = dst++;
	unsigned char code = 0x01;
	if ((length != 0)&&(NULL_PTR != ptr)&&(NULL_PTR != dst))
	{
		*dst++ = length;
		code++;
		while (ptr < end)
		{
			if (*ptr == 0)
				FinishBlock(code);
			else
			{
				*dst++ = *ptr;
				code++;
				if (code == 0xFF)
					FinishBlock(code);
			}
			ptr++;
		}
		FinishBlock(code);
		*(--dst) = 0;
	}
}

//Decoder
/*
 * UnStuffData decodes ¡°length¡± bytes of
 * data at the location pointed to by ¡°ptr¡±,
 * writing the output to the location pointed
 * to by ¡°dst¡±.
 */
void UnStuffData(const uint8 *ptr, uint16 length,uint8 *dst)
{
	const uint8 *end = ptr + length;
	uint8 loop = 1u,code = 0u;
	
	while (ptr < end)
	{
		code = *ptr++;
		for (loop = 1; loop < code; loop++)
			*dst++ = *ptr++;
		if (code < 0xFF)
			*dst++ = 0;
	}
}

/*
 * Mini printf called "pf"
 */
void TraceTest(uint16 moduleId, uint8 level, uint16 fileId, uint16 lineNum, const char *fmt, ...)
{
	va_list	va;
	uint16 totalLength = 0;
	uint16 rawLen = 0;
	uint8 sendBuf[30]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,}; 
	uint8 rawBuf[30]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,};
	
	EnterCriticalCode();
	//*(uint16*)&rawBuf[0] = fileId;
	//*(uint16*)&rawBuf[2] = lineNum;
	rawBuf[1] = (fileId>>8)&0xff;
	rawBuf[2] = fileId&0xff;
	rawBuf[3] = (lineNum>>8)&0xff;
	rawBuf[4] = lineNum&0xff;
	va_start(va, fmt);
	rawLen = vpf(&rawBuf[0],fmt, va);
	rawBuf[0] = rawLen;
	va_end(va);
	totalLength=StuffData(rawBuf,rawLen,sendBuf);
	ExitCriticalCode();
	io_putc(sendBuf,totalLength);
	//io_putc(rawBuf,totalLength);
}

uint16 BKDRHash(char *str)
{
    unsigned int seed = 31; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
 	str += 54;
    while (*str)
    {
        hash = hash * seed + (*str++);
        //hash = (hash<<5)-1 + (*str++);
    }
 //17 37 79 163 331 673 1361 2729 471 10949 21911 32767 43853 87719 175447 350899
 //701819 1403641 2807303 5614657 11229331 22458671 44917381 89834777 
 //179669557 359339171 718678369 1437356741 2147483647
    //return (hash & 0x7FFFFFFF);
    return (hash % 43853);
}
#endif
uint8 TraceInitFlag = STD_OFF;


uint8_t UART_AllocateState2(bool* isAllocated,
                                  uart_instance_t* instanceMapping,
                                  uart_instance_t instance,
                                  uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the UART state structures for this instance */
    for (i = 0; i < numberOfinstances; i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance;
            isAllocated[i] = true;
            break;
        }
    }
    return i;
}


status_t UART_Init_Private(uart_instance_t instance, const uart_user_config_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define UART PAL over LPUART */
    #if (defined (UART_OVER_LPUART))
    if ((uint8_t)instance <= LPUART_HIGH_INDEX)
    {
        lpuart_user_config_t lpuartConfig;

        lpuartConfig.baudRate = config->baudRate;

        /* LPUART supports only 8, 9 or 10 bits per character */
        DEV_ASSERT((config->bitCount >= UART_8_BITS_PER_CHAR) &&
                   (config->bitCount <= UART_10_BITS_PER_CHAR));
        switch (config->bitCount)
        {
            case UART_8_BITS_PER_CHAR:
                lpuartConfig.bitCountPerChar = LPUART_8_BITS_PER_CHAR;
                break;
            case UART_9_BITS_PER_CHAR:
                lpuartConfig.bitCountPerChar = LPUART_9_BITS_PER_CHAR;
                break;
            case UART_10_BITS_PER_CHAR:
                lpuartConfig.bitCountPerChar = LPUART_10_BITS_PER_CHAR;
                break;
            default:
                /* Impossible type - do nothing */
                break;
        }

        lpuartConfig.parityMode = (lpuart_parity_mode_t)(config->parityMode);
        lpuartConfig.stopBitCount = (lpuart_stop_bit_count_t)(config->stopBitCount);
        lpuartConfig.transferType = (lpuart_transfer_type_t)(config->transferType);
        lpuartConfig.rxDMAChannel = config->rxDMAChannel;
        lpuartConfig.txDMAChannel = config->txDMAChannel;

        /* Allocate one of the LPUART state structure for this instance */
        index = UART_AllocateState2(g_lpuartStateIsAllocated,
                                   g_lpuartStateInstanceMapping,
                                   instance,
                                   NO_OF_LPUART_INSTS_FOR_UART);
        /* Initialize LPUART instance */
        status = LPUART_DRV_Init((uint32_t)instance, (lpuart_state_t*)(&g_lpuartState[index]), &lpuartConfig);

        /* Install Rx callback */
        if (config->rxCallback != NULL)
        {
            (void)LPUART_DRV_InstallRxCallback((uint32_t)instance,
                                               config->rxCallback,
                                               config->rxCallbackParam);
        }

        /* Install Tx callback */
        if (config->txCallback != NULL)
        {
            (void)LPUART_DRV_InstallTxCallback((uint32_t)instance,
                                                config->txCallback,
                                                config->txCallbackParam);
        }
    }
    #endif
}


void TraceInit(void)
{
	status_t l_status = STATUS_ERROR;
	
	if (STD_OFF == TraceInitFlag)
	{
		//l_status =UART_Init(UART_CHANNEL_DEBUG,&uart1_pal_config);
		#if (STD_ON == SYS_MODULE_ADD_UART)
		l_status =UART_Init_Private(UART_CHANNEL_DEBUG,&uart1_pal_config);
		#endif
		if(STATUS_SUCCESS == l_status)
		{
			gTraceNameCount = sizeof(gTraceModuleInfoList)/sizeof(tTraceModuleInforList);
			trace_mutex = System_Mutex_Create();
			CQueue_Init(CQUEUE_CH_TRACE,gTraceTxBigBuffer,TRACE_TX_BIG_BUFF_LEN);
			TraceInitFlag = STD_ON;
		}
	}
}

uint16 TraceGetFailPushBytes()
{
	return gTraceFailPushBytes;
}

uint16 TraceGetFailTxBytes()
{
	return gTraceFailTxBytes;
}


void TracePrintf (uint16 moduleId, uint8 level,const char *fmt, ...)
{
	va_list args;
	
	uint8 printbuffer[TRACE_BUFFER_MAX];
	sint8 curTick[10];
	uint16 l_sendLen = 0;
	uint8 * l_msgAddress = NULL;
	
	#if (STD_ON == TRACE_ANSI_COLOR)
	uint8 l_warningPre[] = TRACE_COLOR_YELLO;
	uint8 l_errPre[] = TRACE_COLOR_RED;
	uint8 l_noMaskPre[] = TRACE_COLOR_PURPLE;
	uint8 l_colorPost[] = TRACE_COLOR_NONE;
	#endif

	if(moduleId != TRACE_TRACE)
	{
		if(gTraceSwitchOn == FALSE || level <gTraceLevel)
			return;

		if(moduleId < MODULE_TRACE_NAME_MAX && gTraceModuleInfoList[moduleId].switchOnOff== 0
			|| level<gTraceModuleInfoList[moduleId].traceLevel)
			return;
	}
	
	if(STD_ON == TraceInitFlag)
	{
		System_Mutex_Lock(trace_mutex,STD_OFF);

		memset(printbuffer,0,11);
		sprintf(printbuffer,"%d:",System_GetTickValue());
		va_start (args, fmt);
		vsprintf (printbuffer+(strlen(printbuffer)), fmt, args);	
		va_end (args);

		l_sendLen = strlen(printbuffer);
		l_msgAddress = printbuffer;
		#if (STD_ON == TRACE_ANSI_COLOR)
			l_msgAddress = (uint8 *)pvPortMalloc(TRACE_BUFFER_MAX);
			if( NULL != l_msgAddress)
			{
				if(TRACE_NONMASK==level)
				{					
					strcpy (l_msgAddress, l_noMaskPre);
					strcpy (l_msgAddress+6, printbuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}

				else if(TRACE_WARNING==level)
				{
					strcpy (l_msgAddress, l_warningPre);
					strcpy (l_msgAddress+6, printbuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}
				else if(TRACE_ERRO==level)
				{	
					strcpy (l_msgAddress, l_errPre);
					strcpy (l_msgAddress+6, printbuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}
				
			}
		#endif
		
		#if (STD_ON == SYS_MODULE_ADD_UART)
			if(CQueue_Push(0,printbuffer,l_sendLen,STD_OFF) == E_NOT_OK)
				gTraceFailPushBytes += l_sendLen;
		#endif
		
		#if (STD_ON == TRACE_ANSI_COLOR)
		vPortFree(l_msgAddress);
		#endif
		
		System_Mutex_Unlock(trace_mutex,STD_OFF);
	}
}



void TraceLoopTxByDMA(void *driverState, uart_event_t event, void *userData)
{
	uint16 len = 0;
	boolean driverPendingFlag = FALSE;

	//System_Mutex_Lock(trace_mutex2,STD_OFF);

	if(!LPUART_GetStatusFlag(LPUART1, LPUART_TX_DATA_REG_EMPTY))
    {
		driverPendingFlag = TRUE;
		return;
	}

	g_lpuartState[0].isTxBusy = FALSE;
	CQueue_GetLen(0,&len, STD_OFF);
	if(len > 0)
	{
		if(len >TRACE_TX_CELL_BUFF_LEN)
			len = TRACE_TX_CELL_BUFF_LEN;
		CQueue_Pop(0,gTraceTxBufferElem,len, STD_OFF);
		if(LPUART_DRV_SendData(UART_CHANNEL_DEBUG,(const uint8*)gTraceTxBufferElem,len) == STATUS_BUSY)
			gTraceFailTxBytes++;
	}
	
	//System_Mutex_Unlock(trace_mutex2,STD_OFF);
}

void TraceLoopTxByTask()
{	
	uint8 len = 0;
	const sint8 *data = NULL;
	static uint16 popFailCounter = 0;
	boolean queueEmptyFlag = FALSE;
	uint32 bytesRemaining = 0;
	boolean triggerDmaTxFlag = FALSE;
	//System_Mutex_Lock(trace_mutex2,STD_OFF);

	if(LPUART_DRV_GetTransmitStatus(UART_CHANNEL_DEBUG, &bytesRemaining) == STATUS_BUSY) // Driver is busy
	{
		if(bytesRemaining == 0) // we have miss DMA Interrupt signal due to other somewhere had disable all interrupts
		{
			triggerDmaTxFlag = TRUE;
		}
	}
	else
		triggerDmaTxFlag = TRUE;

	if(triggerDmaTxFlag == TRUE)
	{

		CQueue_GetLen(0,&len, STD_OFF);
		if(len > 0)
		{
			if(len >TRACE_TX_CELL_BUFF_LEN)
				len = TRACE_TX_CELL_BUFF_LEN;
			memset(gTraceTxBufferElem,0,TRACE_TX_CELL_BUFF_LEN);
			CQueue_Pop(0,gTraceTxBufferElem,len, STD_OFF);
			if(LPUART_DRV_SendData(UART_CHANNEL_DEBUG,(const uint8*)gTraceTxBufferElem,len) == STATUS_BUSY)
				popFailCounter++;
		}
		else
			queueEmptyFlag = TRUE;
	}
	
	//System_Mutex_Unlock(trace_mutex2,STD_OFF);
}

void TraceLoopRxByTask()
{
	uint32 bytesRemaining = 0;
	status_t status = STATUS_BUSY;
	uint8 i = 0;


	status = LPUART_DRV_GetReceiveStatus(UART_CHANNEL_DEBUG, &bytesRemaining);
	
	if(status == STATUS_SUCCESS)
	{
		//todo: check content: '\r'
		//TracePrintf(SYSTEM_TRACE,TRACE_INFO,"###############%s#############\r\n",gTraceRxBufferElem);
		memset(gTraceRxBufferElem,0,TRACE_RX_CELL_BUFF_LEN);
		LPUART_DRV_ReceiveData(UART_CHANNEL_DEBUG,(const uint8*)gTraceRxBufferElem,TRACE_RX_CELL_BUFF_LEN);
	}
	else if(status == STATUS_BUSY)
	{
		if(bytesRemaining<=(TRACE_RX_CELL_BUFF_LEN-9))
		{
			//todo: check content: '\r'
			if(TraceCheckEndFlag(&i) == TRUE)
			{
				if(TraceParseReq() == TRUE)
					TracePrintf(TRACE_TRACE,TRACE_INFO,"#####TRACE REQ DONE#####\r\n");
				else
					TracePrintf(TRACE_TRACE,TRACE_INFO,"#####INVALID TRACE REQ FORMAT#####\r\n");
				LPUART_DRV_AbortReceivingData(UART_CHANNEL_DEBUG);
				memset(gTraceRxBufferElem,0,TRACE_RX_CELL_BUFF_LEN);
				LPUART_DRV_ReceiveData(UART_CHANNEL_DEBUG,(const uint8*)gTraceRxBufferElem,TRACE_RX_CELL_BUFF_LEN);
			}
		}
	}
	
}

void TraceLoopRxByDMA(void *driverState, uart_event_t event, void *userData)
{
	
	g_lpuartState[0].isRxBusy = FALSE;
}

boolean TraceCheckEndFlag(uint8 *pos)
{
	uint8 i = 0;
	boolean endFlag = FALSE;
	
	for(i=0;i<TRACE_RX_CELL_BUFF_LEN;i++)
	{
		if(gTraceRxBufferElem[i] == '\r')
		{
			if(pos!=NULL)
				*pos = i;
			endFlag = TRUE;
			break;
		}
	}
	
	return endFlag;
}


boolean SkipSpaceChar(uint8 **str)
{
	boolean moreStr = TRUE;
	
	if(str && *str)
	{
		while(**str == ' ' )
		{	
			*str += 1;
			if(**str == '\0' || **str == '\r')
			{
				moreStr = FALSE;
			}
			
		}
	}

	return moreStr;
}

boolean SearchNextSpaceChar(uint8 **ptr)
{
	boolean findSpaceFlag = FALSE;
	
	if(ptr && *ptr)
	{
		while(**ptr != ' ')
		{
			if(**ptr == '\0' || **ptr == '\r')
				return FALSE;
			*ptr += 1;
		}

		findSpaceFlag = TRUE;
	}

	return findSpaceFlag;
}

uint8 seprateStringBySpaceChar(uint8 *srcStr,uint8 (*newBuff)[TRACE_CMD_ELEMENT_LEN],uint8 buffCounter)
{
	uint8 counter = 0;
	uint8 *ptr = srcStr;
	uint8 *startPtr = NULL;
	boolean finishFlag = FALSE;
	uint8 optionLen = 0;
	
	if(srcStr && newBuff && *newBuff)
	{
		ptr = srcStr;

		//memset(gTraceCmdElement,0,sizeof(gTraceCmdElement));
		memset(newBuff,0,buffCounter*TRACE_CMD_ELEMENT_LEN);

		while(finishFlag == FALSE)
		{
			if(counter>=TRACE_CMD_NUM)
				break;

			if(SkipSpaceChar(&ptr)== FALSE)
				break;
			startPtr = ptr;
			
			if(SearchNextSpaceChar(&ptr) == FALSE)// reach the str end
			{
				finishFlag = TRUE;
			}
			
			*ptr++ = '\0';
			optionLen = strlen(startPtr);
			if(optionLen < TRACE_CMD_ELEMENT_LEN)
			{
				memcpy(newBuff[counter],startPtr,optionLen);
				counter++;
			}
			else // invalid option len,exceed the buffer len
				break;
		
		}

	}

	return counter;
}

boolean TraceParseReq()
{
	uint8 *ptr = NULL;
	boolean validReq = TRUE;
	boolean ret = TRUE;
	uint8 *nameStartPtr = NULL;
	uint8 *nameEndPtr = NULL;
	uint8 strCounter = 0;
	uint8 i = 0;
	uint8 moduleSwitch = 0;
	boolean validReqFormat = FALSE;
	uint8 optionNum = 0;
	uint8 switchOnOff = 0;
	uint8 traceLevel = 0;
	sint32 memValue = 0;
	
	
	strCounter = seprateStringBySpaceChar(gTraceRxBufferElem,gTraceCmdElement,TRACE_CMD_NUM);
	if(strCounter > 0)
	{
		if(memcmp(gTraceCmdElement[0],"trace",6) == 0)
		{
			if(memcmp(gTraceCmdElement[1],"on",3) == 0 || (memcmp(gTraceCmdElement[1],"off",4) == 0))
			{
				if(memcmp(gTraceCmdElement[1],"on",3) == 0)
					switchOnOff = 1;
				else
					switchOnOff = 0;
				
				if(memcmp(gTraceCmdElement[2],"all",4) == 0)
				{
					gTraceSwitchOn = (switchOnOff == 1)? TRUE:FALSE;
					for(i=0;i<gTraceNameCount;i++)
						gTraceModuleInfoList[i].switchOnOff = switchOnOff;
				}
				else
				{
					if(switchOnOff == 1)
						gTraceSwitchOn = TRUE;
					for(optionNum=2;optionNum<strCounter;optionNum++)
					{
						for(i=0;i<gTraceNameCount;i++)
						{
							if(memcmp(gTraceCmdElement[optionNum],gTraceModuleInfoList[i].moduleName,TRACE_CMD_ELEMENT_LEN) == 0)
							{
								gTraceModuleInfoList[i].switchOnOff = switchOnOff;
								break;
							}
						}

					}
					
				}
			}
			else if(gTraceCmdElement[1][0] == '-' && strlen(gTraceCmdElement[1]) == 2)
			{
				switch(gTraceCmdElement[1][1])
				{
				case 'l':
					switch(gTraceCmdElement[2][0])
					{
					case 'i':
						traceLevel = TRACE_INFO;
						break;
					case 'w':
						traceLevel = TRACE_WARNING;
						break;
					case 'e':
						traceLevel = TRACE_ERRO;
						break;
					default:
						ret = FALSE;
					}
					
					if(ret != FALSE)
					{
						if(memcmp(gTraceCmdElement[3],"-m",3) == 0)
						{
							if(memcmp(gTraceCmdElement[4],"all",3) == 0)
								gTraceLevel = traceLevel;
							else
							{
								for(optionNum=4;optionNum<strCounter;optionNum++)
								{
									for(i=0;i<gTraceNameCount;i++)
									{
										if(memcmp(gTraceCmdElement[optionNum],gTraceModuleInfoList[i].moduleName,TRACE_CMD_ELEMENT_LEN) == 0)
										{
											gTraceModuleInfoList[i].traceLevel = traceLevel;
											
											break;
										}
									}
								}
							}
						}
					}
					
					break;
				case 'h':
					//memset(gTraceTxBigBuffer,0,2048);
					CQueue_Clear(UART_CHANNEL_DEBUG, STD_OFF);
					TracePrintf(TRACE_TRACE,TRACE_INFO,"#####TRACE REQ example:#####\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"Enable/Disable all trace:\r\n");
					TracePrintf(TRACE_TRACE,TRACE_INFO,"\"trace on/off all\"\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"Enable/Disable some modules' trace:\r\n");
					TracePrintf(TRACE_TRACE,TRACE_INFO,"\"trace on/off moduleA moduleB\"\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"Change all trace level{i:infor,w:warn,e:err}:\r\n");
					TracePrintf(TRACE_TRACE,TRACE_INFO,"\"trace -l i/w/e -m all\"\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"Change some modules' trace level{i:infor,w:warn,e:err}:\r\n");
					TracePrintf(TRACE_TRACE,TRACE_INFO,"\"trace -l i/w/e -m moduleA moduleB\"\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"Get value of memory addr:\r\n");
					//TracePrintf(TRACE_TRACE,TRACE_INFO,"\"getvalue addr[hex]\"\r\n");
					break;
				default:
					ret = FALSE;
				}
			}
		}
		else if(memcmp(gTraceCmdElement[0],"getvalue",9) == 0)
		{
			if(memcmp(gTraceCmdElement[1],gTraceModuleInfoList[i].moduleName,TRACE_CMD_ELEMENT_LEN) == 0)
			{
				;
			}
		}
		else if(memcmp(gTraceCmdElement[0],"setvalue",9) == 0)
		{
			if(memcmp(gTraceCmdElement[1],gTraceModuleInfoList[i].moduleName,TRACE_CMD_ELEMENT_LEN) == 0)
			{
				;
			}
		}
		else
			ret = FALSE;
	}

	return ret;
}

/* End of File */
