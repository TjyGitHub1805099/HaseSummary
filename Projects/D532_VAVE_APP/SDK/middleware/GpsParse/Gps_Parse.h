#ifndef GPS_PARSE_H
#define GPS_PARSE_H

/*=============[I N T E R F A C E  W I T H  A P P L I C A T I O N  S O F T W A R E]=============*/
/************************************************************************************************/

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

#define GPS_PARSE_H_AR_MAJOR_VERSION   1U
#define GPS_PARSE_H_AR_MINOR_VERSION   0U
#define GPS_PARSE_H_AR_PATCH_VERSION   0U

#define GPS_PARSE_H_SW_MAJOR_VERSION   1U
#define GPS_PARSE_H_SW_MINOR_VERSION   0U
#define GPS_PARSE_H_SW_PATCH_VERSION   0U

#define GPS_HW_CHANNEL (INST_UART_GPS_CFG)

/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
#define GPS_GNRMC_MAX_NUM 100

typedef union
{
	char bytes[4];
	float f;
} Gps_FloatType;

/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/
extern float g_gps_longitude,g_gps_latitude;
/************************************************************************************************/
/*
 * Function Name       Gps_InfoAnalysis
 * Brief               GPS infomation checkout.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
void Gps_InfoAnalysis( const sint8 * buff,uint16 buff_size);
void Gps_RMCInfoPackage(float latitude,float longitude);

#endif /* GPS_PARSE_H */
