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

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "System_Tick.h"

#include "gyroacc_service.h"

//#include <kernel_api.h> changed by tjy
#include "trace.h"
#include "errno.h"
//#include "ipc_gw_api.h" changed by tjy

#include "ais328dq_driver.h"
#include "a3g4250d_driver.h"

#include "i2c_mems.h"

#include "Spi_Ipc_gw_api.h"

#include "MemMap.h"
#include "NvM.h"
#include "NvM_if.h"
//#include "Task_IPC_Cfg.h" changed by tjy

/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/
#define MEMS_IIC_CHANNEL           "dev\\iic\\0"

#define ACC3D_IIC_ADDRESS                AIS328DQ_MEMS_I2C_ADDRESS
#define ACC3D_IIC_SPEED                 (100)

#define GYRO_IIC_ADDRESS                 A3G4250D_MEMS_I2C_ADDRESS
#define GYRO_IIC_SPEED                  (100)

#define SENSOR_EVENT_BUFFER_SIZE 27
#define SPEED_COUNT   4
#define TEMP_COUNT    20
#define SEND_COUNT    2
#define CLEAR_COUNT   20
#define INIT_COUNT    1

/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/
typedef enum
{
	GyroAcc_Idle = 0x0u,
	GyroAcc_InitGyro = 0x1u,
	GyroAcc_InitAcc = 0x2u,
	GyroAcc_Cycle = 0x3u,
} GyroAcc_StateType;

typedef enum
{
	GyroAcc_Data_Gyro_X_L = 0x0u,
	GyroAcc_Data_Gyro_X_H = 0x1u,

	GyroAcc_Data_Gyro_Y_L = 0x2u,
	GyroAcc_Data_Gyro_Y_H = 0x3u,

	GyroAcc_Data_Gyro_Z_L = 0x4u,
	GyroAcc_Data_Gyro_Z_H = 0x5u,

	GyroAcc_Data_Acce_X_L = 0x6u,
	GyroAcc_Data_Acce_X_H = 0x7u,

	GyroAcc_Data_Acce_Y_L = 0x8u,
	GyroAcc_Data_Acce_Y_H = 0x9u,

	GyroAcc_Data_Acce_Z_L = 0xAu,
	GyroAcc_Data_Acce_Z_H = 0xBu,

	GyroAcc_Data_Temper = 0xCu,
	GyroAcc_Data_End
} GyroAcc_SensorDataType;
/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
static uint8 SensorDataBuffer_M[GyroAcc_Data_End] = { 0x00 };

static GyroRaw_t gyroRawData;
static AccRaw_t  accRawData;

GyroAcc_StateType sGyroAccState = GyroAcc_Idle ;
GyroAcc_StateType sGyroAccState_Pre = GyroAcc_Idle ;
uint8 GyroAcceData_TxEn = FALSE;

uint8 gpsInfoHandleFlag = FALSE;
NVM_SEC_VAR_UNSPECIFIED uint8 gpsInfoFromSoc[SOC_GPS_INFO_DATA_LEN];

/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/

/************************************************************************************************/
/*
 * Brief               initial gyro.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
static uint8 GyroInit(void)
{
    uint8 response = 0;
    //Inizialize MEMS Sensor
    //set ODR (turn ON device)
    response = A3G4250D_SetODR( A3G4250D_ODR_100Hz);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO, "[Gyro] A3G4250D_SetODR failed\r\n");
    } 

    //set axis Enable
    response = A3G4250D_SetAxis( A3G4250D_X_ENABLE | A3G4250D_Y_ENABLE | A3G4250D_Z_ENABLE);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] A3G4250D_SetAxis failed\r\n");
    }

    // response = A3G4250D_SetBLE(A3G4250D_BLE_LSB);
    // if(response != MEMS_SUCCESS){
    //     TracePrintf(GENERAL_TRACE, TRACE_INFO,"[Gyro] A3G4250D_SetBLE failed\r\n");
    // }

    response = A3G4250D_SetFifo_Mode(A3G4250D_FIFO_MODE_STREAM,0x02);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] A3G4250D_SetFifo_Mode failed\r\n");
    }

    response = A3G4250D_SetFifo(A3G4250D_FIFO_ENABLE);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] A3G4250D_SetFifo failed\r\n");
    }

    //set PowerMode 
    response = A3G4250D_SetMode( A3G4250D_NORMAL);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] A3G4250D_SetMode failed\r\n");
    }

    gyroRawData.init = 0;
    gyroRawData.count = 0;
    gyroRawData.sumX = 0;
    gyroRawData.sumY = 0;
    gyroRawData.sumZ = 0;
    gyroRawData.x = 0;
    gyroRawData.y = 0;
    gyroRawData.z = 0;

    return response;
}
/************************************************************************************************/
/*
 * Brief               initial acc.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
static uint8 AccInit(void)
{
	status_t1 response = 0;

    //Inizialize MEMS Sensor
    //set ODR (turn ON device)
    response = AIS328DQ_SetODR(AIS328DQ_ODR_100Hz);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE,TRACE_INFO, "[Gyro] AIS328DQ_SetODR failed\r\n");
    } 

    //set Fullscale
    response = AIS328DQ_SetFullScale(AIS328DQ_FULLSCALE_2);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE,TRACE_INFO,"[Gyro] AIS328DQ_SetFullScale failed\r\n");
    } 

    //set axis Enable
    response = AIS328DQ_SetAxis(AIS328DQ_X_ENABLE | AIS328DQ_Y_ENABLE | AIS328DQ_Z_ENABLE);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE,TRACE_INFO,"[Gyro] AIS328DQ_SetAxis failed\r\n");
    }

    response = AIS328DQ_SetBLE(AIS328DQ_BLE_LSB);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] AIS328DQ_SetBLE failed\r\n");
    }
    
    //set PowerMode 
    response = AIS328DQ_SetMode(AIS328DQ_NORMAL);
    if(response != MEMS_SUCCESS)
    {
        TracePrintf(GYROACC_TRACE, TRACE_INFO,"[Gyro] AIS328DQ_SetMode failed\r\n");
    }
    
    accRawData.count = 0;
    accRawData.sumX = 0;
    accRawData.sumY = 0;
    accRawData.sumZ = 0;

    return response;
}

void GyroAccServiceInit(void)
{}
void GyroAccDataCapture(void)
{}
/************************************************************************************************/
/*
 * Brief               transmit cycle data from gyro and acc to soc.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
void GyroAccTimerTransmit(void)
{}
void GyroAcc_StateChangePrintf( GyroAcc_StateType *cur,GyroAcc_StateType *pre )
{
	if( *cur == *pre )
	{
		return;
	}

	char stringP[0x20]="[GyroAcc-I]:";
	switch(*cur)
	{
		case 	GyroAcc_Idle:strcat(stringP,"Idle");break;
		case 	GyroAcc_InitGyro:strcat(stringP,"InitGyro");break;
		case 	GyroAcc_InitAcc:strcat(stringP,"InitAcc");break;
		case 	GyroAcc_Cycle:strcat(stringP,"Cycle");break;
		default:break;
	}
	strcat(stringP,"2");
	switch(*pre)
	{
		case 	GyroAcc_Idle:strcat(stringP,"Idle");break;
		case 	GyroAcc_InitGyro:strcat(stringP,"InitGyro");break;
		case 	GyroAcc_InitAcc:strcat(stringP,"InitAcc");break;
		case 	GyroAcc_Cycle:strcat(stringP,"Cycle");break;
		default:break;
	}
	strcat(stringP,"\r\n");
	TracePrintf(NM_TRACE, TRACE_NONMASK, stringP);
	*pre = *cur;
}
/************************************************************************************************/
/*
 * Brief               stord gps info from SOC
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
void GyroAccGpsStoreHandle(void)
{
	if( gpsInfoHandleFlag != FALSE )
	{
		if( gpsInfoHandleFlag == IPC_SW_SYSTEM_FUCTION_GET_GPS_INFO )//read 71,soc get EEPROM stord gps infomation
		{
			//read eeprom
			NvM_ReadBlock(gpsInfoFromSocNvMBlockID,&gpsInfoFromSoc[0]);
			ipc_send_notification(GYROCT_IPC_HW_CHANNEL,GYROCT_IPC_SW_CHANNEL,IPC_SW_SYSTEM_FUCTION_GET_GPS_INFO,&gpsInfoFromSoc[0],SOC_GPS_INFO_DATA_LEN);
		}if( gpsInfoHandleFlag == IPC_SW_SYSTEM_FUCTION_SET_GPS_INFO )//write 70,soc set EEPROM stord gps infomation
		{
			//write eeprom
			NvM_WriteBlock(gpsInfoFromSocNvMBlockID,&gpsInfoFromSoc[0]);
			//NvM_ReadBlock(socSerialNumNvMBlockID,&socSerialNum[0]);
		}
		TracePrintf(NM_TRACE, TRACE_WARNING, "[GpsStore-Handle]:gpsInfoHandleFlag = %d\r\n",gpsInfoHandleFlag);
		gpsInfoHandleFlag = FALSE;
	}
}
/************************************************************************************************/
/*
 * Brief               Main function of the GyroAcc which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/************************************************************************************************/
void GyroAcc_MainFunction(void)
{
	status_t1 response = MEMS_SUCCESS;
	
	static uint32 startTickValu = 0 ;
	static uint8 tempCount = 0 ;

	#if (STD_ON == GYROCT_DEBUG_TRACE)
		static uint32 monitorTickValu=0;
		static uint16 monitorCount[2]={0,0};
	#endif
	
	GyroAccGpsStoreHandle();

	switch( sGyroAccState )
	{
		case GyroAcc_Idle:
		{
			startTickValu = System_GetTickValue();
			//state change
			sGyroAccState = GyroAcc_InitGyro ;
		}
		break;
		case GyroAcc_InitGyro:
		{
			if( ( System_GetTickValue() - startTickValu ) >= MEMS_INITIAL_DELAY_COUNT )
			{
				startTickValu = System_GetTickValue();
				//doing
				GyroInit();
				//state change
				sGyroAccState = GyroAcc_InitAcc ;
			}
		}
		break;
		case GyroAcc_InitAcc:
		{
			if( ( System_GetTickValue() - startTickValu ) >= MEMS_INITIAL_DELAY_COUNT )
			{
				startTickValu = System_GetTickValue();
				#if (STD_ON == GYROCT_DEBUG_TRACE)
				monitorTickValu = startTickValu;
				#endif
				//doing
				AccInit();
				//state change
				sGyroAccState = GyroAcc_Cycle ;
				
			}
		}
		break;
		case GyroAcc_Cycle:
		{
			if( ( System_GetTickValue() - startTickValu ) >= MEMS_CYCLE_DELAY_COUNT )
			{
				startTickValu = System_GetTickValue();
				//doing
				response = A3G4250D_GetGyroRaw(&gyroRawData);
				#if (STD_ON == GYROCT_DEBUG_TRACE)
				if(response != MEMS_SUCCESS)
					monitorCount[0]++;
				#endif
				
				response = AIS328DQ_GetAccAxesRaw(&accRawData);
				#if (STD_ON == GYROCT_DEBUG_TRACE)
				if(response != MEMS_SUCCESS)
					monitorCount[1]++;
				#endif

				SensorDataBuffer_M[GyroAcc_Data_Gyro_X_L] = gyro_data[4];
				SensorDataBuffer_M[GyroAcc_Data_Gyro_X_H] = gyro_data[5];
				SensorDataBuffer_M[GyroAcc_Data_Gyro_Y_L] = gyro_data[0];
				SensorDataBuffer_M[GyroAcc_Data_Gyro_Y_H] = gyro_data[1];
				SensorDataBuffer_M[GyroAcc_Data_Gyro_Z_L] = gyro_data[2];
				SensorDataBuffer_M[GyroAcc_Data_Gyro_Z_H] = gyro_data[3];

				SensorDataBuffer_M[GyroAcc_Data_Acce_X_L] = acc_data[0];
				SensorDataBuffer_M[GyroAcc_Data_Acce_X_H] = acc_data[1];
				SensorDataBuffer_M[GyroAcc_Data_Acce_Y_L] = acc_data[2];
				SensorDataBuffer_M[GyroAcc_Data_Acce_Y_H] = acc_data[3];
				SensorDataBuffer_M[GyroAcc_Data_Acce_Z_L] = acc_data[4];
				SensorDataBuffer_M[GyroAcc_Data_Acce_Z_H] = acc_data[5];

				//send data to soc by spi IPC
				if( TRUE == GyroAcceData_TxEn )
					ipc_send_notification(GYROCT_IPC_HW_CHANNEL,GYROCT_IPC_SW_CHANNEL,GYROCT_IPC_SW_FUNCTION_ID,&SensorDataBuffer_M[GyroAcc_Data_Gyro_X_L],GYROCT_RECV_IPC_SW_HMICMD_LEN);

				//state change
				sGyroAccState = GyroAcc_Cycle ;
				
				//err printf
				#if (STD_ON == GYROCT_DEBUG_TRACE)
					if( ( monitorCount[0] != 0 ) || ( monitorCount[1] != 0 ) )
					{
						if( ( System_GetTickValue() - monitorTickValu ) >= 5000 )//5s
						{
							monitorTickValu = System_GetTickValue();
							TracePrintf(NM_TRACE, TRACE_WARNING, "[GetSensorData-ERR]:last 5s, cycle = %d ms, faild read count G= %d ,A =%d\r\n",MEMS_CYCLE_DELAY_COUNT,monitorCount[0],monitorCount[1]);
							monitorCount[0]=0;
							monitorCount[1]=0;
						}
					}
				#endif
			}

			if( ++tempCount%100 == 0 )
			{
				tempCount = 0 ;
				A3G4250D_GetTemperature(&SensorDataBuffer_M[GyroAcc_Data_Temper]);
				//send data to soc by spi IPC
				if( TRUE == GyroAcceData_TxEn )
					ipc_send_notification(GYROCT_IPC_HW_CHANNEL,GYROCT_IPC_SW_CHANNEL,GYROCT_IPC_SW_TEMP_FUNCTION_ID,&SensorDataBuffer_M[GyroAcc_Data_Temper],1);

				#if 0
					TracePrintf(NM_TRACE, TRACE_WARNING, "[TEMP-DATA]:%d \r\n",SensorDataBuffer_M[GyroAcc_Data_Temper]);
				#endif
			}
			
		}
		break;
		default:
		{
			sGyroAccState = GyroAcc_Idle ;
		}
		break;
	}

	#if 1
		GyroAcc_StateChangePrintf(&sGyroAccState,&sGyroAccState_Pre);
	#endif

}
/* END OF FILE */
