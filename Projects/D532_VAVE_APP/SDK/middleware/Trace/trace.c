#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "trace_cfg.h"
#include "trace_if.h"
#include "Std_Types.h" 
#include "BoardDefines.h"
#include "System_Module_Cfg.h"
#include "System_Portable.h"
#include "System_Mutex.h"
#include "System_Tick.h"
#include "CircularQueue.h"
#if (STD_ON == SYS_MODULE_ADD_UART)
#include "lpuart_hw_access.h"
#include "uart_pal.h"
#include "uart_debug_pal.h"
#endif

#include "TraceStore.h"

extern void Tp_Trace_Init();
extern void Tp_Trace_Push(uint16 len, uint8* dataPtr);

static boolean TraceCheckEndFlag(uint8 *pos);
static boolean TraceParseReq();

#if (STD_ON == SYS_MODULE_ADD_UART)
void trace_rx_cbk(void *driverState, uart_event_t event, void *userData);
#endif

#define TRACE_RX_RAW_DATA_BUFF_LEN 64
#define TRACE_RX_QUEUE_BUFF_LEN 128


extern boolean gSystemInitDoneFlag;
extern boolean gRunInMainFlag;

SystemMutexType trace_mutex;
boolean sUartTraceRxNewDataFlag = FALSE;


boolean gTraceInitFlag = FALSE;

uint8 gTraceRxRawDataBuffer[TRACE_RX_RAW_DATA_BUFF_LEN] = {1};

uint8 gTraceRxQueueBuffer[TRACE_RX_QUEUE_BUFF_LEN] = {1};
uint8 gTraceRxBuffer[TRACE_RX_QUEUE_BUFF_LEN] = {1};


uint8 gTraceTxBuffer[TRACE_TX_BUFFER_MAX] = {1};
uint8 gTraceCmdElement[TRACE_CMD_NUM][TRACE_CMD_ELEMENT_LEN] ;//= {{1}};


static boolean gTraceSwitchOn = TRUE;
static uint8 gTraceLevel = TRACE_INFO;
static boolean gTraceRxNewDataFlag = FALSE;
static boolean gTraceRxNewAdbDataFlag = FALSE;

//####### DON'T CHANGE THE ORDER#######
//####### THESE ORDER MUST BE SAME AS THE ONES IN TYPE "Module_Trace_Name" IN trace.h
tTraceModuleInforList gTraceModuleInfoList[] = {
		{"system",SYSTEM_TRACE,1,TRACE_WARNING,NULL},
		{"ipc",IPC_TRACE,1,TRACE_WARNING,NULL},
		{"com",COM_TRACE,1,TRACE_WARNING,NULL},
		{"canapp",CANAPP_TRACE,1,TRACE_WARNING,NULL},
		{"nm",NM_TRACE,1,TRACE_WARNING,NULL},
		{"pm",PM_TRACE,1,TRACE_WARNING,NULL},
		{"input",INPUT_TRACE,1,TRACE_WARNING,NULL},
		{"tp",TP_TRACE,1,TRACE_WARNING,NULL},
		{"j1939tp",J1939TP_TRACE,1,TRACE_WARNING,NULL},
		{"uds",UDS_TRACE,1,TRACE_WARNING,NULL},
		{"diagapp",DIAGAPP_TRACE,1,TRACE_WARNING,NULL},
		{"dtcCore",DTCCORE_TRACE,1,TRACE_WARNING,NULL},
		{"ame",AME_TRACE,1,TRACE_WARNING,NULL},
		{"alarm",ALARM_TRACE,1,TRACE_WARNING,NULL},
		{"virtual",VIRTUAL_TRACE,1,TRACE_WARNING,NULL},
		{"general",GENERAL_TRACE,1,TRACE_WARNING,NULL},
		{"gyroacc",GYROACC_TRACE,1,TRACE_WARNING,NULL},
		{"m_ipc",M_IPC_TRACE,1,TRACE_WARNING,NULL},
	};

static uint8 gTraceNameCount = 0;

boolean TraceRegisterCbk(Module_Trace_Name traceModuleName,tTraceCbk cbk)
{
	uint8 i = 0;

	if(cbk != NULL && traceModuleName<MODULE_TRACE_NAME_MAX)
	{
		for(i=0;i<MODULE_TRACE_NAME_MAX;i++)
		{
			if(gTraceModuleInfoList[i].traceIndex == traceModuleName)
			{
				gTraceModuleInfoList[i].cbk = cbk;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}
uint8 CQUEUE_CH_TRACE = 0xFF;

void TraceInit(void)
{
	status_t l_status = STATUS_ERROR;
	
	if (FALSE == gTraceInitFlag)
	{
		#if (STD_ON == SYS_MODULE_ADD_UART)
		uart_debug_pal_config.baudRate = 500000;
		uart_debug_pal_config.rxCallback = trace_rx_cbk;
		uart_debug_pal_config.transferType = UART_USING_INTERRUPTS;
		l_status =UART_Init(TRACE_HW_CHANNEL,&uart_debug_pal_config);
		#endif
		if(STATUS_SUCCESS == l_status)
		{
			gTraceNameCount = sizeof(gTraceModuleInfoList)/sizeof(tTraceModuleInforList);
			System_Mutex_Create(&trace_mutex);
			gTraceInitFlag = TRUE;
			CQUEUE_CH_TRACE = CQueue_Init(CQUEUE_BUFTYPE_UINT8,gTraceRxQueueBuffer,TRACE_RX_QUEUE_BUFF_LEN);
			Tp_Trace_Init();
		#if (STD_ON == SYS_MODULE_ADD_UART)
			LPUART_DRV_ReceiveData(TRACE_HW_CHANNEL,(const uint8*)gTraceRxRawDataBuffer,TRACE_RX_RAW_DATA_BUFF_LEN);
		#endif
		}
	}
}


void TracePrintf (uint16 moduleId, uint8 level,const char *fmt, ...)
{
	va_list args;
	
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

		if(moduleId < MODULE_TRACE_NAME_MAX && (gTraceModuleInfoList[moduleId].switchOnOff== 0
			|| level<gTraceModuleInfoList[moduleId].traceLevel))
			return;
	}
	
	if(TRUE == gTraceInitFlag)
	{
		if(gSystemInitDoneFlag == TRUE)
			System_Mutex_Lock(trace_mutex,STD_OFF);

		memset(gTraceTxBuffer,0,TRACE_TX_BUFFER_MAX);
		if(moduleId != TRACE_TRACE)
			sprintf(gTraceTxBuffer,"%d:",System_GetTickValue());
		va_start (args, fmt);
		vsprintf (gTraceTxBuffer+(strlen(gTraceTxBuffer)), fmt, args);	
		va_end (args);

		l_sendLen = strlen(gTraceTxBuffer);
		l_msgAddress = gTraceTxBuffer;

		if(gSystemInitDoneFlag == TRUE)
			Tp_Trace_Push(l_sendLen,gTraceTxBuffer);
		
		#if (STD_ON == TRACE_ANSI_COLOR)
			l_msgAddress = (uint8 *)pvPortMalloc(TRACE_TX_BUFFER_MAX);
			if( NULL != l_msgAddress)
			{
				if(TRACE_NONMASK==level)
				{					
					strcpy (l_msgAddress, l_noMaskPre);
					strcpy (l_msgAddress+6, gTraceTxBuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}

				else if(TRACE_WARNING==level)
				{
					strcpy (l_msgAddress, l_warningPre);
					strcpy (l_msgAddress+6, gTraceTxBuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}
				else if(TRACE_ERRO==level)
				{	
					strcpy (l_msgAddress, l_errPre);
					strcpy (l_msgAddress+6, gTraceTxBuffer);
					strcpy (l_msgAddress+l_sendLen+6, l_colorPost);
					l_sendLen = l_sendLen +10;
				}
				
			}
		#endif

		/***Don't move these Logic ***/
		//avoid mutex used add by tjy @2020-1-16
		if((gSystemInitDoneFlag == TRUE) || (TRUE == gRunInMainFlag))
		{
			//add by tjy @2019-12-17
			#if (STD_ON == TRACE_STORE_USING )
				#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
					TraceStore_Push(l_msgAddress, l_sendLen, 1);
				#else
					TraceStore_Push(l_msgAddress, l_sendLen);
				#endif
			#endif
		}
		
		#if (STD_ON == SYS_MODULE_ADD_UART)
			LPUART_DRV_SendDataPolling(TRACE_HW_CHANNEL,l_msgAddress,l_sendLen);
		#endif

		#if (STD_ON == TRACE_ANSI_COLOR)
		vPortFree(l_msgAddress);
		#endif

		if(gSystemInitDoneFlag == TRUE)
			System_Mutex_Unlock(trace_mutex,STD_OFF);
	}
}



boolean TraceCheckEndFlag(uint8 *pos)
{
	uint8 i = 0;
	boolean endFlag = FALSE;
	
	for(i=0;i<TRACE_RX_QUEUE_BUFF_LEN;i++)
	{
		if(gTraceRxBuffer[i] == '\r')
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

#if (STD_ON == SYS_MODULE_ADD_UART)

uint8 trace_LPUART_DRV_GetFifoRxCount(uint32 uartHwInstance)
{
	LPUART_Type * const s_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;
	uint8 waterRxCount = 0;
	
	waterRxCount = (uint8)((s_lpuartBase[uartHwInstance]->WATER & LPUART_WATER_RXCOUNT_MASK)>>24);
	return waterRxCount;
}

void trace_LPUART_DRV_GetData(uint32 uartHwInstance,uint8*rxDataBuffPtr)
{
	LPUART_Type * const s_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;

	*rxDataBuffPtr = (uint8)s_lpuartBase[uartHwInstance]->DATA;
}


void trace_rx_cbk(void *driverState, uart_event_t event, void *userData)
{
	uint8 rxCount = 0;
	uint16 realDataLen = 0;
	static uint32 invalidFrameCounter = 0;
	lpuart_state_t *lpuartState = (lpuart_state_t*)driverState;
	uint8 i = 0;
	static uint32 invalidIntCounter = 0;
	uint8 *rxBufPtr = lpuartState->rxBuff;
	
	if(event == UART_EVENT_RX_FULL || event == UART_EVENT_END_TRANSFER)
	{
		rxCount = trace_LPUART_DRV_GetFifoRxCount(TRACE_HW_CHANNEL);
		if(rxCount>0)
		{
			for(i=0;i<rxCount;i++)
			{
				trace_LPUART_DRV_GetData(TRACE_HW_CHANNEL,rxBufPtr);
				rxBufPtr++;
			}

			if(CQueue_Push(CQUEUE_CH_TRACE,gTraceRxRawDataBuffer,rxCount,STD_ON) != CQUEUE_ERR_PUSHPOP)
			{
				if(event == UART_EVENT_END_TRANSFER)
				{
					memset(gTraceRxRawDataBuffer,0,TRACE_RX_RAW_DATA_BUFF_LEN);
					//ipcSemPost(gIpcRxSem);
					gTraceRxNewDataFlag = TRUE;
				}
			}
		}
		else if(event == UART_EVENT_END_TRANSFER)
		{
			memset(gTraceRxRawDataBuffer,0,TRACE_RX_RAW_DATA_BUFF_LEN);
			//ipcSemPost(gIpcRxSem);
			gTraceRxNewDataFlag = TRUE;
		}
		else
			invalidIntCounter++;

	}

}
#endif

void trace_adb_cbk(uint8 * adbBuf,uint8 len)
{
	if(CQueue_Push(CQUEUE_CH_TRACE,adbBuf,len,STD_ON) != CQUEUE_ERR_PUSHPOP)
	{
		gTraceRxNewAdbDataFlag = TRUE;
	}
}

void TraceLoopRxByTask()
{
	uint16 dataLen = 0;
	uint16 i = 0;
		
	if(gTraceInitFlag == TRUE)
	{
		if((gTraceRxNewDataFlag == TRUE) || (TRUE == gTraceRxNewAdbDataFlag))
		{
			memset(gTraceRxBuffer,0,TRACE_RX_QUEUE_BUFF_LEN);
			gTraceRxNewDataFlag = FALSE;
			gTraceRxNewAdbDataFlag= FALSE;
			if((dataLen =CQueue_Pop(CQUEUE_CH_TRACE,gTraceRxBuffer,CQUEUE_POP_ALL,STD_OFF))>0)
			{
				//for(i=0;i<dataLen;i++)
				{
					TracePrintf(TRACE_TRACE,TRACE_NONMASK,"### %s ###\r\n",gTraceRxBuffer);
				}
				TraceParseReq();
			}		
		}
	}
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
	
	
	strCounter = seprateStringBySpaceChar(gTraceRxBuffer,gTraceCmdElement,TRACE_CMD_NUM);
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
					CQueue_Clear(CQUEUE_CH_TRACE, STD_OFF);
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

					TracePrintf(TRACE_TRACE,TRACE_INFO,"moduleName traceIndex switchOnOff traceLevel[i=%d/w=%d/e=%d/n=%d]\r\n",TRACE_INFO,TRACE_WARNING,TRACE_ERRO,TRACE_NONMASK);
					for(i=0;i<gTraceNameCount;i++)
					{
						TracePrintf(TRACE_TRACE,TRACE_INFO,"%s %d %d %d\r\n",gTraceModuleInfoList[i].moduleName,gTraceModuleInfoList[i].traceIndex,gTraceModuleInfoList[i].switchOnOff,gTraceModuleInfoList[i].traceLevel);
					}
					break;
				default:
					ret = FALSE;
				}
			}
			else
			{
				for(i=0;i<MODULE_TRACE_NAME_MAX;i++)
				{
					if(strcmp(gTraceCmdElement[1],gTraceModuleInfoList[i].moduleName) == 0)
					{
						if(gTraceModuleInfoList[i].cbk != NULL);
						{
							gTraceModuleInfoList[i].cbk(gTraceCmdElement[2],strCounter-2);
						}
						break;
					}
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
