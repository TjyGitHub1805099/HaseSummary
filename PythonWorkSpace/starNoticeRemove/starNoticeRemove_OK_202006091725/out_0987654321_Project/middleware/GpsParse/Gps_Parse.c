




 

  



#define GPS_PARSE_C_AR_MAJOR_VERSION   1U
#define GPS_PARSE_C_AR_MINOR_VERSION   0U
#define GPS_PARSE_C_AR_PATCH_VERSION   0U

#define GPS_PARSE_C_SW_MAJOR_VERSION   1U
#define GPS_PARSE_C_SW_MINOR_VERSION   0U
#define GPS_PARSE_C_SW_PATCH_VERSION   0U



#include "Platform_Types.h"
#include "trace.h"
#include "status.h"
#include <stdio.h>
#include "lpuart_driver.h"

#include "Gps_Parse.h"
#include "stdlib.h"

#include "uart_gps_cfg.h"
#include "System_Module_Cfg.h"

#include "gyroacc_service.h"




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



float g_gps_longitude = 2321.54747;
float g_gps_latitude = 11308.52922;
uint8 g_gprmc[GPS_GNRMC_MAX_NUM]="$GNRMC,040232.00,A,2321.54747,N,11308.52922,E,14.016,38.03,060617,,,D*7C";

boolean gGpsUartInitFlag = FALSE;

uint8 l_GpsTrace;









void GpsUartInit(void)
{
	if (FALSE == gGpsUartInitFlag)
	{
		gGpsUartInitFlag = TRUE;
		#if (STD_ON == SYS_MODULE_ADD_GPS_UART)
		
		
		
		
		UART_Init(GPS_HW_CHANNEL,&uart_gps_cfg_Config0);
		#endif
	}
}





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

sint32 nmea_atoi(const sint8 *str, sint8 str_sz, sint32 radix)
{
	sint8 *tmp_ptr;
	sint8 buff[NMEA_CONVSTR_BUF] ;
    sint32 res = 0;
    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0' ;
        res = strtol((const char*)&buff[0],(char**)&tmp_ptr,(int)radix);
    }
    return res;
}

sint64 nmea_atof(const uint8 *str, uint8 str_sz, uint8 radix)
{
    sint8 buff[NMEA_CONVSTR_BUF];
    sint64 res = 0;
    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0';
        
    }
    return res;
}

uint8 HexToChar(uint8 bHex,uint8 c_s)
{
	if(( bHex >= 0x00 ) &&  ( bHex <= 0x09 ) )
		return (bHex+'0');
	else if(( bHex >= 0x0a ) &&  ( bHex <= 0x0f ) )
	{
		if( c_s == 1 )
			return (bHex-0x0a+'A');
		else
			return (bHex-0x0a+'a');
	}
	else
		return 0xff;
}

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



void Gps_InfoAnalysis( const sint8 * buff,uint16 buff_size )
{
	
	uint8 *infoHeader=NULL,*infoTail=NULL;
	uint8 i = 0 , feild_i = 0 ;
	sint8 crc = 0 , crc_buf = 0 ;


	if( ( buff == NULL ) || ( buff_size == 0 ) )
	{
		return;
	}
	
	
	for( i = 0 ; i < buff_size ; i++ )
	{
		if( '$' == buff[i] )
		{
			if(('C'==buff[i+5])&&('M'==buff[i+4])&&('R'==buff[i+3])&&('N'==buff[i+2])&&('G'==buff[i+1]))
			{
				infoHeader = (uint8*)&buff[i];
				break;
			}
		}
	}

	
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
					infoTail=(uint8*)&buff[i];
				}
			}
			else
			{
				crc ^= buff[i];
			}
		}
	}

	
	if( NULL != infoTail )
	{
		
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

		
		gpsTimer.hour = nmea_atoi((const sint8*)gpsInfoFeild[GNRMC_HHMMSS].header,2,10);
		gpsTimer.minute = nmea_atoi((const sint8*)(gpsInfoFeild[GNRMC_HHMMSS].header+2),2,10);
		gpsTimer.second = nmea_atoi((const sint8*)(gpsInfoFeild[GNRMC_HHMMSS].header+4),2,10);
		
		gpsTimer.day =(uint8) (nmea_atoi((const sint8*)gpsInfoFeild[GNRMC_DDMMYY].header,2,10)&0xff);
		gpsTimer.month = nmea_atoi((const sint8*)(gpsInfoFeild[GNRMC_DDMMYY].header+2),2,10);
		gpsTimer.year = nmea_atoi((const sint8*)(gpsInfoFeild[GNRMC_DDMMYY].header+4),2,10);
	}
	
}

void Gps_RMCInfoInit()
{
	sint8 len=0;
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

void Gps_RMCInfoPackage(float latitude,float longitude)
{
	static uint8 count = 0 ;
	sint8 feild_i=0,str_i=0,str_len=0,crc=0;

    int LongtitudeHour = 0;
    float LongtitudeMinute = 0;

    int LatitudeHour = 0;
    float LatitudeMinute = 0;
	
    
	if( latitude < 0.00001f )
	{
		latitude = -latitude;
		gpsInfoFromSoc[9]= 'S';
	}
	else
		gpsInfoFromSoc[9]= 'N';
		
	if( longitude < 0.00001f )
	{
		longitude = -longitude;
		gpsInfoFromSoc[8]= 'W';
	}
	else
		gpsInfoFromSoc[8]= 'E';
		

	
	if( latitude > 90.0f )
	{
		latitude = 39.673370 ;
		gpsInfoFromSoc[9]= 'N';
	}
	if( longitude > 180.0f )
	{
		longitude = 116.455070 ;
		gpsInfoFromSoc[8]= 'E';
	}

	memset(g_gprmc, 0, GPS_GNRMC_MAX_NUM);
	g_gprmc[0]='$';
	
	for(feild_i=0;feild_i<=GNRMC_CRC;feild_i++)
	{
		
		if( (GPS_GNRMC_MAX_NUM - 8 ) < strlen((const char*)g_gprmc) )
		{
			TracePrintf(NM_TRACE, TRACE_WARNING, "[GPS-INFO]:last char min than 8 byte\r\n");
			return;
		}
		
		switch(feild_i)
		{
			case GNRMC_HEADER:strcat((char*)g_gprmc,"GPRMC,");break;
			case GNRMC_HHMMSS:strcat((char*)g_gprmc,"040232.00,");break;

			case GNRMC_LOCATION_A_V:strcat((char*)g_gprmc,"A,");break;
			
			case GNRMC_LATITUDE:
				
				(void)ChangetoSexagesimal(latitude, &LatitudeHour, &LatitudeMinute);
				sprintf((char*)(g_gprmc+strlen((char*)g_gprmc)),"%02d%02d.%04d,",LatitudeHour,(int)LatitudeMinute,(int)((LatitudeMinute-(int)LatitudeMinute)*10000));
				break;
			case GNRMC_LATITUDE_N_S:
				if( 'S' == gpsInfoFromSoc[9] )
					strcat((char*)g_gprmc,"S,");
				else
					strcat((char*)g_gprmc,"N,");
				break;

			case GNRMC_LONGITUDE:
				
				(void)ChangetoSexagesimal(longitude, &LongtitudeHour, &LongtitudeMinute);
				sprintf((char*)(g_gprmc+strlen((char*)g_gprmc)),"%03d%02d.%04d,",LongtitudeHour,(int)LongtitudeMinute,(int)((LongtitudeMinute-(int)LongtitudeMinute)*10000));
				break;
			case GNRMC_LONGITUDE_E_W:
				if( 'W' == gpsInfoFromSoc[8] )
					strcat((char*)g_gprmc,"W,");
				else
					strcat((char*)g_gprmc,"E,");
				break;

			case GNRMC_KNOT:strcat((char*)g_gprmc,"00.000,");break;
			case GNRMC_ANGEL:strcat((char*)g_gprmc,"38.03,");break;
			case GNRMC_DDMMYY:strcat((char*)g_gprmc,"060617,");break;
			case GNRMC_MD:strcat((char*)g_gprmc,",");break;
			case GNRMC_MD_E_W:strcat((char*)g_gprmc,",");break;
			case GNRMC_MODE_A_D_E_N:strcat((char*)g_gprmc,"D");break;
			case GNRMC_CRC:
				str_len=strlen((char*)g_gprmc);
				if(str_len <(GPS_GNRMC_MAX_NUM-8))
				{
					for(crc=0,str_i=1;str_i<str_len;str_i++)
						crc^=g_gprmc[str_i];

					g_gprmc[str_i++]='*';
					sprintf((char*)(g_gprmc+strlen((char*)g_gprmc)),"%.2X",crc);
					str_i+=2;
					
					

					
				}
			break;
			default:break;
		}
	}
	
	if( str_i < GPS_GNRMC_MAX_NUM)
	{
		LPUART_DRV_SendDataPolling(GPS_HW_CHANNEL,(const uint8*)&g_gprmc[0],str_i);
		if(( TRUE == l_GpsTrace ) && ( ++count%10 == 0))
			TracePrintf(NM_TRACE, TRACE_WARNING, "%s,latitude=%f,longitude=%f\r\n",g_gprmc,latitude,longitude);
	}
}
