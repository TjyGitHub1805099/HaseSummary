/*============================[R E V I S I O N   H I S T O R Y]=================================*/
/************************************************************************************************/
/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * The integer value 1 is being added or subtracted from a pointer.
 */
 
/* MISRA-C:2004 Rule 19.1,
 * #include statements in a file should only be preceded by other preprocessor directives or comments.
 */
  
/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

#define GPS_PARSE_C_AR_MAJOR_VERSION   1U
#define GPS_PARSE_C_AR_MINOR_VERSION   0U
#define GPS_PARSE_C_AR_PATCH_VERSION   0U

#define GPS_PARSE_C_SW_MAJOR_VERSION   1U
#define GPS_PARSE_C_SW_MINOR_VERSION   0U
#define GPS_PARSE_C_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Platform_Types.h"
#include "trace.h"
#include "status.h"

#include "Gps_Parse.h"
#include "stdlib.h"

#include "uart_gps_cfg.h"
#include "System_Module_Cfg.h"

#include "gyroacc_service.h"

/*============================[V E R S I O N  C H E C K]========================================*/
/************************************************************************************************/

#if (GPS_PARSE_C_AR_MAJOR_VERSION != GPS_PARSE_H_AR_MAJOR_VERSION)
#error "AcCtl.c:Mismatch in Specification Major Version"
#endif

#if (GPS_PARSE_C_AR_MINOR_VERSION != GPS_PARSE_H_AR_MINOR_VERSION)
#error "AcCtl.c:Mismatch in Specification Minor Version"
#endif

#if (GPS_PARSE_C_AR_PATCH_VERSION != GPS_PARSE_H_AR_PATCH_VERSION)
#error "AcCtl.c:Mismatch in Specification Patch Version"
#endif

#if (GPS_PARSE_C_SW_MAJOR_VERSION != GPS_PARSE_H_SW_MAJOR_VERSION)
#error "CanNm.c:Mismatch in Specification Major Version"
#endif

#if (GPS_PARSE_C_SW_MINOR_VERSION != GPS_PARSE_H_SW_MINOR_VERSION)
#error "CanNm.c:Mismatch in Specification Minor Version"
#endif

/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/
#define NMEA_CONVSTR_BUF	32

typedef enum
{
	GNRMC_HEADER = 0x00u,
	GNRMC_HHMMSS ,
	GNRMC_LOCATION_A_V,
	GNRMC_LATITUDE,
	GNRMC_LATITUDE_N_S,
	GNRMC_LONGITUDE,
	GNRMC_LONGITUDE_E_W,
	GNRMC_KNOT,
	GNRMC_ANGEL,
	GNRMC_DDMMYY,
	GNRMC_MD,
	GNRMC_MD_E_W,
	GNRMC_MODE_A_D_E_N,
	GNRMC_CRC,
	GNRMC_MAX_FEILD,
} Gps_InfoFeildGNRMCEnumType;

/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
typedef struct
{
	uint8 	*header;
	uint8	length;
} Gps_InfoFeildType;
Gps_InfoFeildType gpsInfoFeild[GNRMC_MAX_FEILD];

typedef struct
{
	uint8	second;
	uint8	minute;
	uint8	hour;
	uint8	week;
	uint8	day;
	uint8	month;
	uint8	year;
} Gps_TimeType;
Gps_TimeType gpsTimer;

/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/
float g_gps_longitude = 2321.54747;
float g_gps_latitude = 11308.52922;
uint8 g_gprmc[GPS_GNRMC_MAX_NUM]="$GNRMC,040232.00,A,2321.54747,N,11308.52922,E,14.016,38.03,060617,,,D*7C";

boolean gGpsUartInitFlag = FALSE;

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/

/*============================[D A T A  M A N A G E M E N T] ===================================*/
/************************************************************************************************/

/*============================[L O C A L  F U N C T I O N S  D E F I N I T I O N] ==============*/
/************************************************************************************************/
//gps hardware:uart1 init
void GpsUartInit(void)
{
	status_t l_status = STATUS_ERROR;

	if (FALSE == gGpsUartInitFlag)
	{
		gGpsUartInitFlag = TRUE;
		#if (STD_ON == SYS_MODULE_ADD_GPS_UART)
		//uart_gps_cfg_Config0.txCallback=
		//uart_gps_cfg_Config0.rxCallback=
		//uart_gps_cfg_Config0.transferType = UART_USING_INTERRUPTS;
		//uart_gps_cfg_Config0.baudRate = 9600;
		l_status =UART_Init(GPS_HW_CHANNEL,&uart_gps_cfg_Config0);
		#endif
	}
}




/**
 * \brief Calculate control sum of binary buffer
 */
sint8 nmea_calc_crc(const sint8 *buff, uint8 buff_sz)
{
	sint8 chsum = 0;
	uint8 it;
    for(it = 0; it < buff_sz; ++it)
    {
    	chsum ^= buff[it];
    }
    return chsum;
}
/**
 * \brief Convert string to int number
 */
sint32 nmea_atoi(const sint8 *str, sint8 str_sz, sint32 radix)
{
	sint8 *tmp_ptr;
	sint8 buff[NMEA_CONVSTR_BUF] ;
    sint32 res = 0;
    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0' ;
        res = strtol(&buff[0],&tmp_ptr,radix);
    }
    return res;
}
/**
 * \brief Convert string to fraction number
 */
sint64 nmea_atof(const uint8 *str, uint8 str_sz, uint8 radix)
{
    sint8 *tmp_ptr;
    sint8 buff[NMEA_CONVSTR_BUF];
    sint64 res = 0;
    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0';
        //res = strtod(&buff[0], &tmp_ptr,str_sz);
    }
    return res;
}
//
uint8 HexToChar(uint8 bHex,uint8 c_s)
{
	if(( bHex >= 0x00 ) &&  ( bHex <= 0x09 ) )
		return (bHex+'0');
	else if(( bHex >= 0x0a ) &&  ( bHex <= 0x0f ) )
	{
		if( c_s == 1 )//da xie
			return (bHex-0x0a+'A');
		else
			return (bHex-0x0a+'a');
	}
	else
		return 0xff;
}
//
uint8 CharHexToHex(uint8 bHex)
{
	if(( bHex >= '0' ) &&  ( bHex <= '9' ) )
		return (bHex-'0');
	else if(( bHex >= 'a' ) &&  ( bHex <= 'f' ) )
		return (bHex-'a'+10);
	else if(( bHex >= 'A' ) &&  ( bHex <= 'F' ) )
		return (bHex-'A'+10);
	else
		return 0xff;
}

//==========================================uppack GPRMC=======================================
/*
 * Function Name       Gps_InfoAnalysis
 * Brief               GPS infomation checkout.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      *buff point the GPS data info
 * Param-Name[out]     buff_size GPS info length
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
void Gps_InfoAnalysis( const sint8 * buff,uint16 buff_size )
{
	/* 	
	 	*$GNRMC,030951.00,A,3144.95902,N,11714.15143,E,0.000,273.21,280618,,,A*7B
		*$GNVTG,273.21,T,,M,0.000,N,0.000,K,A*26
		*$GNGGA,030951.00,3144.95902,N,11714.15143,E,1,12,1.16,45.2,M,-2.3,M,,*63
		*$GNGSA,A,3,11,18,01,07,30,08,27,28,16,,,,2.08,1.16,1.73*19
		*$GNGSA,A,3,407,408,403,413,,,,,,,,,2.08,1.16,1.73*1B
		*$GPGSV,5,1,18,01,47,173,38,07,65,283,35,08,56,024,32,09,12,224,*71
		*$GPGSV,5,2,18,10,01,058,,11,70,200,39,16,12,097,20,18,64,136,44*72
		*$GPGSV,5,3,18,22,05,150,,23,01,193,,27,23,048,28,28,18,302,25*7B
		*$GPGSV,5,4,18,30,38,310,38,40,15,254,,41,38,232,,42,42,135,*79
		*$GPGSV,5,5,18,50,42,135,,193,12,149,22*40
		*$GBGSV,3,1,09,402,38,230,,403,54,193,29,404,32,119,,405,17,251,*63
		*$GBGSV,3,2,09,407,74,079,32,408,66,193,26,409,05,209,,410,67,333,*6B
		*$GBGSV,3,3,09,413,43,206,27*62
		*$GNGLL,3144.95902,N,11714.15143,E,030951.00,A,A*7C
	*/
	uint8 *infoHeader=NULL,*infoTail=NULL;
	uint8 i = 0 , feild_i = 0 ;
	sint8 crc = 0 , crc_buf = 0 ;


	if( ( buff == NULL ) || ( buff_size == 0 ) )
	{
		return;
	}
	
	//1st : find the header = "$GNRMC" i
	for( i = 0 ; i < buff_size ; i++ )
	{
		if( '$' == buff[i] )
		{
			if(('C'==buff[i+5])&&('M'==buff[i+4])&&('R'==buff[i+3])&&('N'==buff[i+2])&&('G'==buff[i+1]))
			{
				infoHeader = &buff[i];
				break;
			}
		}
	}

	//2nd : dind tail and caculate the crc data( except '$' and '*' )
	if( NULL != infoHeader )
	{
		crc = 0 ;
		for( i++ ; i < buff_size  ; i++ )
		{
			if( '*' == buff[i] )
			{
				crc_buf = nmea_atoi((&buff[i+1]),2,16);
				if( crc_buf == crc )
				{
					infoTail=&buff[i];
				}
			}
			else
			{
				crc ^= buff[i];
			}
		}
	}

	//3th data vaild
	if( NULL != infoTail )
	{
		//split info
		gpsInfoFeild[0].header = infoHeader + 1 ;

		for( i = 0 ,feild_i = 1; infoHeader + i <= infoTail ; i++ )
		{
			if( ( ',' == infoHeader[i] ) || ( '*' == infoHeader[i] ) )
			{
				gpsInfoFeild[feild_i].header = &infoHeader[i+1] ;
				gpsInfoFeild[feild_i-1].length = gpsInfoFeild[feild_i].header - gpsInfoFeild[feild_i-1].header - 1;

				if( ++feild_i >= GNRMC_MAX_FEILD )
				{
					break;
				}
			}
		}

		//use data : timer hhssmm
		gpsTimer.hour = nmea_atoi(gpsInfoFeild[GNRMC_HHMMSS].header,2,10);
		gpsTimer.minute = nmea_atoi((gpsInfoFeild[GNRMC_HHMMSS].header+2),2,10);
		gpsTimer.second = nmea_atoi((gpsInfoFeild[GNRMC_HHMMSS].header+4),2,10);
		//use data : timer ddmmyy
		gpsTimer.day =(uint8) (nmea_atoi(gpsInfoFeild[GNRMC_DDMMYY].header,2,10)&0xff);
		gpsTimer.month = nmea_atoi((gpsInfoFeild[GNRMC_DDMMYY].header+2),2,10);
		gpsTimer.year = nmea_atoi((gpsInfoFeild[GNRMC_DDMMYY].header+4),2,10);
	}
	
}
//==========================================package GPRMC=======================================
void Gps_RMCInfoInit()
{
	sint8 i=0,len=0;
	memset(g_gprmc, 0, sizeof(g_gprmc));
	g_gprmc[len++]='$';
}

int ChangetoSexagesimal(float Num, int *a, float *b )
{
    int Hour;
    float Minute;
        
    Hour = (int)(Num);	
    Minute = ((Num-Hour)*60.0);

    *a = Hour;
    *b = Minute;

    return 1;
}
/*
 * Function Name       Gps_RMCInfoPackage
 * Brief               GPS infomation Package.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      *buff point the GPS data info
 * Param-Name[out]     buff_size GPS info length
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
void Gps_RMCInfoPackage(float latitude,float longitude)
{
	static uint8 count = 0 ;
	sint8 feild_i=0,str_i=0,str_len=0,crc=0;

    int LongtitudeHour = 0;
    float LongtitudeMinute = 0;

    int LatitudeHour = 0;
    float LatitudeMinute = 0;
	
    //minus data
	if( latitude < 0.00001f )
	{
		latitude = -latitude;
		gpsInfoFromSoc[9]= 'S';//S
	}
	else
		gpsInfoFromSoc[9]= 'N';//N
		
	if( longitude < 0.00001f )
	{
		longitude = -longitude;
		gpsInfoFromSoc[8]= 'W';//W
	}
	else
		gpsInfoFromSoc[8]= "E";//E
		

	//if larger than 180.0 ,set default location = bei jin
	if( latitude > 90.0f )
	{
		latitude = 39.673370 ;//bei jin
		gpsInfoFromSoc[9]= 'N';//N
	}
	if( longitude > 180.0f )
	{
		longitude = 116.455070 ;//bei jin
		gpsInfoFromSoc[8]= "E";//E
	}

	memset(g_gprmc, 0, GPS_GNRMC_MAX_NUM);//clr g_gprmc
	g_gprmc[0]='$';//="$GNRMC,040232.00,A,2321.54747,N,11308.52922,E,14.016,38.03,060617,,,D*7C";
	
	for(feild_i=0;feild_i<=GNRMC_CRC;feild_i++)
	{
		//last char min than 8 byte , avoid 
		if( (GPS_GNRMC_MAX_NUM - 8 ) < strlen(g_gprmc) )
		{
			TracePrintf(NM_TRACE, TRACE_WARNING, "[GPS-INFO]:last char min than 8 byte\r\n");
			return;
		}
		
		switch(feild_i)
		{
			case GNRMC_HEADER:strcat(g_gprmc,"GPRMC,");break;
			case GNRMC_HHMMSS:strcat(g_gprmc,"040232.00,");break;

			case GNRMC_LOCATION_A_V:strcat(g_gprmc,"A,");break;
			
			case GNRMC_LATITUDE:
				//sprintf((g_gprmc+strlen(g_gprmc)),"%.5f,",latitude);
				(void)ChangetoSexagesimal(latitude, &LatitudeHour, &LatitudeMinute);
				sprintf((g_gprmc+strlen(g_gprmc)),"%02d%02d.%04d,",LatitudeHour,(int)LatitudeMinute,(int)((LatitudeMinute-(int)LatitudeMinute)*10000));
				break;
			case GNRMC_LATITUDE_N_S:
				if( 'S' == gpsInfoFromSoc[9] )
					strcat(g_gprmc,"S,");
				else
					strcat(g_gprmc,"N,");//bei jin defalut is N
				break;

			case GNRMC_LONGITUDE:
				//sprintf((g_gprmc+strlen(g_gprmc)),"%.5f,",longitude);
				(void)ChangetoSexagesimal(longitude, &LongtitudeHour, &LongtitudeMinute);
				sprintf((g_gprmc+strlen(g_gprmc)),"%03d%02d.%04d,",LongtitudeHour,(int)LongtitudeMinute,(int)((LongtitudeMinute-(int)LongtitudeMinute)*10000));
				break;
			case GNRMC_LONGITUDE_E_W:
				if( 'W' == gpsInfoFromSoc[8] )
					strcat(g_gprmc,"W,");
				else
					strcat(g_gprmc,"E,");//bei jin defalut is E
				break;

			case GNRMC_KNOT:strcat(g_gprmc,"00.000,");break;
			case GNRMC_ANGEL:strcat(g_gprmc,"38.03,");break;
			case GNRMC_DDMMYY:strcat(g_gprmc,"060617,");break;
			case GNRMC_MD:strcat(g_gprmc,",");break;
			case GNRMC_MD_E_W:strcat(g_gprmc,",");break;
			case GNRMC_MODE_A_D_E_N:strcat(g_gprmc,"D");break;
			case GNRMC_CRC:
				str_len=strlen(g_gprmc);
				if(str_len <(GPS_GNRMC_MAX_NUM-8))
				{
					for(crc=0,str_i=1;str_i<str_len;str_i++)
						crc^=g_gprmc[str_i];

					g_gprmc[str_i++]='*';
					sprintf((g_gprmc+strlen(g_gprmc)),"%.2X",crc);
					str_i+=2;
					//g_gprmc[str_i++]=HexToChar((crc>>4)&0x0f,1);
					//g_gprmc[str_i++]=HexToChar((crc>>0)&0x0f,1);

					//send this sting to TCU
				}
			break;
			default:break;
		}
	}
	//LPUART_DRV_SendDataBlocking(GPS_HW_CHANNEL,&g_gprmc[0],str_i,200);
	if( str_i < GPS_GNRMC_MAX_NUM)
	{
		LPUART_DRV_SendDataPolling(GPS_HW_CHANNEL,&g_gprmc[0],str_i);
		if( ++count%10 == 0)
			TracePrintf(NM_TRACE, TRACE_WARNING, "%s,latitude=%f,longitude=%f\r\n",g_gprmc,latitude,longitude);
	}
}
