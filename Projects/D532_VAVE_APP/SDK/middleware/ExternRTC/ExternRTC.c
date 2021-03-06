/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "ExternRTC_Cfg.h"
#include "ExternRTC.h"
#include "Delay.h"
#include "iic_if.h"
#include "System_Signal.h"
#include "PanelKey.h"
#include "dea.h"

#include "MemMap.h"
#include "NvM.h"
#include "NvM_if.h"

#if ((STD_ON == EXTERN_DEBUG_TRACE) || (STD_ON == EXTERN_ERROR_DET_TRACE) )
#include "trace.h"
#endif

/*========================[E X T E R N A L  V A R I A B L E]====================================*/
/************************************************************************************************/
extern const uint8 gIocSWVersion[];

/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
ExternRTCType sRtcDefaultData,sRtcData;
ExternSTDType sStdDefaultData,sStdData;
ExternSTDType gSetStdData;

NVM_SEC_VAR_UNSPECIFIED uint8 rtcStoredData[RTCCT_RECV_IPC_SW_HMICMD_LEN];

#define EXTR_RTC_VLF_READ_CNT	5
#define EX_RTC_TIME_LEN			7

const uint8 rtaDataMap[EX_RTC_TIME_LEN][8]=
{
	{0,	40,	20,	10,8,4,2,1},//sec
	{0,	40,	20,	10,8,4,2,1},//min
	{0,	0,	20,	10,8,4,2,1},//hour
	{0,	6,	5,	4, 3,2,1,0},//week
	{0,	0,	20,	10,8,4,2,1},//day
	{0,	0,	0,	10,8,4,2,1},//month
	{80,40,	20,	10,8,4,2,1}//year
};

uint32 sTickValue = 0 ;

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/
/* pointer sou  copy to point des */
static void ExtrRtc_MemCpy(const uint8* sou,uint8* des,uint16 len)
{	
	uint16 l_loop=0;
	while(l_loop<len)
	{	
		*(des + l_loop) = *(sou + l_loop);
		l_loop++;
	}
}

/* external RTC read */
static Std_ReturnType ExtrRtc_Read(uint16 address,uint8 *bufferPtr,uint32 length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_databuf[EXTERN_RTC_DATA_MAX_LEN+1],i=0;
	
	LockGet = I2C_MutxLock(INST_LPI2C_RTC,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet)
	{	
		/* RTC write atart address*/
		l_databuf[0] = address ;

		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_RTC,IIC_CFGNUM_RTC,l_databuf,1,true);
			if(E_OK == l_returnValue)
			{
				l_returnValue = LPI2C_PAL_MasterReceiveDataBlocking(INST_LPI2C_RTC,IIC_CFGNUM_RTC,bufferPtr,length,true);
				if(E_OK == l_returnValue)
					break;
			}
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_RTC);
	}
	
	return l_returnValue;

}

/* external RTC write */
static Std_ReturnType ExtrRtc_Write(uint16 address,uint8 *bufferPtr,uint32 length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_databuf[EXTERN_RTC_DATA_MAX_LEN+1],i=0;

	LockGet = I2C_MutxLock(INST_LPI2C_RTC,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet )
	{			
		/* RTC write atart address*/
		l_databuf[0] = address ;
		/* RTC write data copy to l_databuf */
		ExtrRtc_MemCpy(bufferPtr,&l_databuf[1],length);

		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_RTC,IIC_CFGNUM_RTC,l_databuf,length+1,true);
			if(E_OK == l_returnValue)
				break;
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_RTC);
	}
	return l_returnValue;
}

/* external RTC data 2 std data */
static void ExtrRtc_RtcTime2StdTime(ExternRTCType *rtcData,ExternSTDType *stdData)
{
	uint8 l_pos = 0 ,l_i = 0;
	ExternSTDType stdDataBuf={0} ;
	ExternRTCType rtcDataBuf = *rtcData;
	
	for(l_pos=0;l_pos<EX_RTC_TIME_LEN;l_pos++)
	{
		*(&stdDataBuf.second+l_pos) = 0 ;
		if(l_pos!=3)//sec min hour  day month year
		{
			for(l_i=0;l_i<8;l_i++)
			{
				if( ( *(&rtcDataBuf.second + l_pos) & (1<<(7-l_i)) ) != 0 )
				{
					*(&stdDataBuf.second+l_pos) += rtaDataMap[l_pos][l_i] ;
				}
			}
		}
		else//week
		{
			for(l_i=0;l_i<8;l_i++)
			{
				if( ( *(&rtcDataBuf.second + l_pos) & (1<<(7-l_i)) ) != 0 )
				{
					*(&stdDataBuf.second+l_pos) += rtaDataMap[l_pos][l_i];
					break;
				}
			}
		}
	}
	*stdData = stdDataBuf;
}

/* external std data 2 RTC data */
static void ExtrRtc_StdTime2RtcTime(ExternSTDType *stdData,ExternRTCType *rtcData)
{
	uint8 l_pos = 0 ,l_i = 0;
	ExternSTDType stdDataBuf=*stdData ;
	ExternRTCType rtcDataBuf = {0};

	for(l_pos=0;l_pos<EX_RTC_TIME_LEN;l_pos++)
	{
		*(&rtcDataBuf.second+l_pos) = 0 ;
		if(l_pos!=3)//sec min hour  day month year
		{
			for(l_i=0;l_i<8;l_i++)
			{
				if(( rtaDataMap[l_pos][l_i] != 0 ) &&
					( *(&stdDataBuf.second + l_pos) >= rtaDataMap[l_pos][l_i] ) )
				{
					*(&rtcDataBuf.second+l_pos) |= (1<<(7-l_i));
					*(&stdDataBuf.second + l_pos) -= rtaDataMap[l_pos][l_i];
				}
			}
		}
		else//week
		{
			if( *(&stdDataBuf.second + l_pos) < 7 )// 0 1 2 3 4 5 6
			{
				*(&rtcDataBuf.second+l_pos) |= (1<<(*(&stdDataBuf.second + l_pos)));
			}
		}
	}
	*rtcData = rtcDataBuf;
}

/* standard time set to extern rtc*/
Std_ReturnType ExtrRtc_StdTimeSet(ExternSTDType *stdData)
{
	Std_ReturnType l_returnValue = E_NOT_OK;
	if( NULL != stdData)
	{
		/*if continuous 10 time overflow ,set rtc data to default data */
		if( stdData->month > 12 || stdData->month == 0 ||stdData->day > 31 || stdData->day == 0  || stdData->week >= 7	||	stdData->hour >= 24 || stdData->minutes >= 60 || stdData->second >= 60 )
		{
			gSetStdData.hmiWriteFlag = FALSE;
		}
		else
		{
			gSetStdData = *stdData;
			
			gSetStdData.hmiWriteFlag = TRUE;
			l_returnValue = E_OK;
		}
	}
	return l_returnValue;
}

/* standard time set to extern rtc*/
static Std_ReturnType inner_StdTimeSet(ExternSTDType *stdData)
{
	Std_ReturnType l_returnValue = E_NOT_OK;
	if( NULL != stdData)
	{
		DelayMs(1);
		ExtrRtc_StdTime2RtcTime(stdData,&sRtcDefaultData);
		l_returnValue = ExtrRtc_Write(0,(uint8 *)&sRtcDefaultData,7);

		//if set new data to extern rtc,set inner valiable to sStdData
		if( E_OK == l_returnValue )
		{
			sStdData = *stdData;
			sTickValue = System_GetTickValue();
		}
	}
	return l_returnValue;
}

/* standard time get from extern rtc*/
Std_ReturnType ExtrRtc_StdTimeGet(ExternSTDType *stdData)
{
	ExternSTDType l_stdData = sStdData;
	uint32 sTickValue_Buf = System_GetTickValue();//ms
	uint8 diffDay = 0 ;
	
	if( sTickValue_Buf >= sTickValue )
		sTickValue_Buf -= sTickValue;
	else
		sTickValue_Buf += 0xffffffff - sTickValue;

	//
	l_stdData.second += sTickValue_Buf/1000;
	if( l_stdData.second >= 60 )//s
	{
		l_stdData.minutes += l_stdData.second/60;
		l_stdData.second = l_stdData.second%60;

		if( l_stdData.minutes >= 60 )//min
		{
			l_stdData.hour += l_stdData.minutes/60;
			l_stdData.minutes = l_stdData.minutes%60;

			if( l_stdData.hour >= 24 )//hour
			{
				l_stdData.day += l_stdData.hour/24;
				l_stdData.week+= l_stdData.hour/24;
				l_stdData.week = l_stdData.week%7;
				l_stdData.hour = l_stdData.hour%24;

				if(( l_stdData.month == 1 ) || ( l_stdData.month == 3 ) || ( l_stdData.month == 5 )
					|| ( l_stdData.month == 7 ) || ( l_stdData.month == 8 ) || ( l_stdData.month == 10 ) || ( l_stdData.month == 12 ))
				{
					diffDay = 31;
				}else if( l_stdData.month == 2 )
				{
					if(( (2000+l_stdData.year)%4 == 0 ) && ( (2000+l_stdData.year)%100 != 0 ) )
						diffDay = 29;
					else
						diffDay = 28;
				}
				else
				{
					diffDay = 30;
				}

				if( l_stdData.day > diffDay )//day
				{
					l_stdData.month += l_stdData.day/diffDay;
					l_stdData.day = l_stdData.day%diffDay;

					if( l_stdData.month > 12 )//month
					{
						l_stdData.year += l_stdData.month/12;
						l_stdData.month = l_stdData.month%12;
					}
				}
			}
		}
	}

	*stdData = l_stdData;
	//sStdData = l_stdData;
}

/* standard time set to extern rtc*/
static Std_ReturnType inner_StdTimeGet(ExternSTDType *stdData)
{
	Std_ReturnType l_returnValue = E_NOT_OK;
	if( NULL != stdData)
	{
		DelayMs(1);
		/* read rtc data */
		l_returnValue = ExtrRtc_Read(0,(uint8 *)&sRtcData,0x10);
		if( E_OK == l_returnValue )
		{
			/* rtc data 2 std data */
			ExtrRtc_RtcTime2StdTime(&sRtcData,stdData);
			sTickValue = System_GetTickValue();
		}
	}
	return l_returnValue;
}


/* external rtc voltage low flag check */
static Std_ReturnType ExtrRtc_VLFCheck()
{
	Std_ReturnType l_returnValue = E_OK;
	/* if( vlf != 0 ),write 0 */
	if( ( ( sRtcData.flagRegister ) & ( 0X01 << EXTERN_RTC_VLF_SHIFT ) ) != 0 )
	{
		sRtcData.flagRegister = sRtcData.flagRegister & (~(uint8)( 0X01 << EXTERN_RTC_VLF_SHIFT ) );
		l_returnValue = ExtrRtc_Write(EXTERN_RTC_VLF_ADDRESS,(uint8 *)&sRtcData.flagRegister,1);
		//if(E_NOT_OK == l_returnValue)
			//return E_NOT_OK;
		l_returnValue = E_NOT_OK ;
	}
	return l_returnValue;
}

int inner_weekday(int year,int month,int day) {
	if(month == 1 || month == 2){
		month += 12;
		year--;
	}
	int c = year / 100;
	int y = year % 100;
	int m = month;
	int d = day;
	int w = c / 4 - 2 *c + y + y / 4 + 26*(m+1) / 10 + d - 1;
	if(w < 0)
		return (w + (-w / 7 + 1) * 7) % 7;
	return w % 7;
}

/* extern RTC i2c initial */
Std_ReturnType ExtrRtc_Init()
{
#if 0
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 needSetRtcUseEedata = FALSE;
	uint16 eeYearMonth,rtcYearMonth;

	inner_StdTimeGet(&sStdData);
	if( E_NOT_OK == ExtrRtc_VLFCheck() )
	{
		needSetRtcUseEedata = TRUE;
		#if ( STD_ON == EXTERN_DEBUG_TRACE)
			TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-INFO]:rtc mcu voltage drop !!!\r\n");
		#endif
	}
	else
	{
		eeYearMonth = rtcStoredData[6]*12+rtcStoredData[5];//eeprom year and month
		rtcYearMonth = sStdData.year*12+sStdData.month;//rtc year and month
		if(( eeYearMonth > rtcYearMonth ) || (eeYearMonth+12 < rtcYearMonth) )
		{
			needSetRtcUseEedata = TRUE;
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-INFO]:rtc not match eeprom data !!!\r\n");
			#endif
		}
	}
	if( TRUE == needSetRtcUseEedata )
	{
		memcpy(&sStdData.second,rtcStoredData,RTCCT_RECV_IPC_SW_HMICMD_LEN);
		inner_StdTimeSet(&sStdData);
		#if ( STD_ON == EXTERN_DEBUG_TRACE)
			TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-INFO]:init rtc data use ee = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
		#endif
	}

	sRtcData.initFlag = TRUE;
	return(E_OK);
#else
	char dataChange[3];
	uint8 flagRegReadMaxTime = EXTR_RTC_VLF_READ_CNT ;
	Std_ReturnType l_returnValue = E_NOT_OK;

	//sStdDefaultData use build time
	//gIocSWVersion[IOC_SOFTWARE_VERSION_LEN] = "190522102144";
	memcpy(&dataChange[0], &gIocSWVersion[0], 2);
	sStdDefaultData.year = strtol(&dataChange[0],NULL,10);
	
	memcpy(&dataChange[0], &gIocSWVersion[2], 2);
	sStdDefaultData.month = strtol(&dataChange[0],NULL,10);

	memcpy(&dataChange[0], &gIocSWVersion[4], 2);
	sStdDefaultData.day = strtol(&dataChange[0],NULL,10);
	
	sStdDefaultData.week = inner_weekday((2000+sStdDefaultData.year),sStdDefaultData.month,sStdDefaultData.day);

	memcpy(&dataChange[0], &gIocSWVersion[6], 2);
	sStdDefaultData.hour = strtol(&dataChange[0],NULL,10);

	memcpy(&dataChange[0], &gIocSWVersion[8], 2);
	sStdDefaultData.minutes = strtol(&dataChange[0],NULL,10);

	memcpy(&dataChange[0], &gIocSWVersion[10], 2);
	sStdDefaultData.second = strtol(&dataChange[0],NULL,10);
	
	//sStdData use eeprom
	memcpy(&sStdData.second,rtcStoredData,RTCCT_RECV_IPC_SW_HMICMD_LEN);
	//if sStdData is't normal data, use default data
	if( sStdData.month > 12 || sStdData.month == 0 || sStdData.day > 31 || sStdData.day == 0 || sStdData.week >= 7 || sStdData.hour >= 24 || sStdData.minutes >= 60 || sStdData.second >= 60 )
	{
		sStdData = sStdDefaultData;
		#if ( STD_ON == EXTERN_DEBUG_TRACE)
			TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:e2 data not standard , used default = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
		#endif
	}
	else
	{
		#if ( STD_ON == EXTERN_DEBUG_TRACE)
			TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:e2 data is standard ,%d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
		#endif
	}
	
	return(E_OK);

#endif
}

//extern boolean gSoftWareResetFlag;
extern boolean gPowerOnResetFlag;
void ExtrRtc_MainFunction()
{
	static uint32 rtcCounter = 0;
	Std_ReturnType l_returnValue = E_NOT_OK;

	if( TRUE == sRtcData.initFlag )
	{
		rtcCounter++;
		
		//SOC write RTC
		if( gSetStdData.hmiWriteFlag == TRUE )
		{
			gSetStdData.hmiWriteFlag = FALSE;
			inner_StdTimeSet(&gSetStdData);
			System_Signal_Send(SYSTEM_SIG_ID_SOCUPDATARTC,STD_ON,STD_OFF);//STD_ON
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[socSetRTC]:%d-%d-%d %d %d:%d:%d\r\n",gSetStdData.year,gSetStdData.month,gSetStdData.day,gSetStdData.week,gSetStdData.hour,gSetStdData.minutes,gSetStdData.second);
			#endif

			//write rtcStoredData[]
			//memcpy(rtcStoredData,&gSetStdData.second,RTCCT_RECV_IPC_SW_HMICMD_LEN);
			//NvM_WriteBlock(rtcStoredDataNvMBlockID,rtcStoredData);
		}

		//SOC read RTC
		if( gSetStdData.hmiReadFlag == TRUE )
		{
			gSetStdData.hmiReadFlag = FALSE;
			inner_StdTimeGet(&sStdData);
			ipc_send_notification(RTCCT_IPC_HW_CHANNEL,RTCCT_IPC_SW_CHANNEL,RTCCT_IPC_SW_FUNCTION_ID_READ,&sStdData.second,RTCCT_RECV_IPC_SW_HMICMD_LEN);
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[socGetRTC]:%d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
			#endif
		}

		//extern rtc read and stored in eeprom, cycle = 60s
		if( rtcCounter%6000 == 0 )
		{
			if( E_NOT_OK == inner_StdTimeGet(&sStdData) )
			{
				#if ( STD_ON == EXTERN_ERROR_DET_TRACE)
					TracePrintf(NM_TRACE, TRACE_WARNING, "[extern rtc read err]:cycle = 60s \r\n");
				#endif
			}
			
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I-MIN]:%d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
			#endif

			if( E_NOT_OK == ExtrRtc_VLFCheck() )
			{
				#if ( STD_ON == EXTERN_DEBUG_TRACE)
					TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-INFO]:external RTC supply voltage drop !!!\r\n");
				#endif
			}

			//write rtcStoredData[]
			memcpy(rtcStoredData,&sStdData.second,RTCCT_RECV_IPC_SW_HMICMD_LEN);
			NvM_WriteBlock(rtcStoredDataNvMBlockID,rtcStoredData);
		}
	}
	else if( System_GetTickValue() > 1000 )
	{
		sRtcData.initFlag = TRUE;

		if( TRUE == gPowerOnResetFlag )
		{
			inner_StdTimeSet(&sStdData);
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:mcu power on,used e2 data = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
			#endif
		}
		else//sleep -> wake read RTC stored time
		{
			inner_StdTimeGet(&sStdData);
			#if ( STD_ON == EXTERN_DEBUG_TRACE)
				TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:normal up reads rtc = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
			#endif

			/*if vlf checked ,set rtc data to e2 data */
			if( E_NOT_OK == ExtrRtc_VLFCheck() )
			{
				memcpy(&sStdData.second,rtcStoredData,RTCCT_RECV_IPC_SW_HMICMD_LEN);
				inner_StdTimeSet(&sStdData);
				#if ( STD_ON == EXTERN_DEBUG_TRACE)
					TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:vlf checked, used e2 data = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
				#endif
			}/*if overflow ,set rtc data to default data */
			else if( sStdData.month > 12 || sStdData.month == 0 ||sStdData.day > 31 || sStdData.day == 0  || sStdData.week >= 7	||	sStdData.hour >= 24 || sStdData.minutes >= 60 || sStdData.second >= 60 )
			{
				sStdData = sStdDefaultData;
				#if ( STD_ON == EXTERN_DEBUG_TRACE)
					TracePrintf(NM_TRACE, TRACE_NONMASK, "[RTC-I]:normal up reads not standard , used default = %d-%d-%d %d %d:%d:%d\r\n",sStdData.year,sStdData.month,sStdData.day,sStdData.week,sStdData.hour,sStdData.minutes,sStdData.second);
				#endif
				inner_StdTimeSet(&sStdData);
			}
		}
	}
}

void lpi2c0_cfg_MasterCallbackADAU(uint32_t instance,lpi2c_master_event_t masterEvent,void *userData)
{

}

uint8 GetRtcTime(ExternSTDType *stData)
{
	if(E_OK != inner_StdTimeGet(stData))
	{
		return E_NOT_OK;
	}	

	return E_OK;	
}

