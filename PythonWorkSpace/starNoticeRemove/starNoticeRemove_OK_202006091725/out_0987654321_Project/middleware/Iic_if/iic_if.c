

#include "Std_Types.h"
#include "iic_if.h"
#include "Delay.h"
#include "trace.h"
#include "pins_port_hw_access.h"






static uint32 i2c_err_Num = 0;



Std_ReturnType LPI2C_PAL_MasterInit(uint32 instance,uint8 cfgNum)
{
	LPI2C_DRV_MasterDeinit(instance);
	LPI2C_DRV_MasterInit(instance,pI2CMasterUserConfig[instance][cfgNum],&sI2CMasterState[instance]);

   return E_OK;
}




BaseType_t I2C_MutxLock( uint32 instance , uint32 timeOutMs ,uint16 times)
{
	BaseType_t xEntryTimeSet = pdFALSE;

	if( TRUE == gSystemInitDoneFlag )
	{
		#if( STD_ON == I2C_USING_SEMAPHORE_HANDLE )
			if( ( times > 0 ) && ( timeOutMs > 0 ))
			{
				if( xSemaphore[instance] != NULL )
				{
					while( ( xEntryTimeSet == pdFALSE ) && ( times-- ) )
					{
						xEntryTimeSet = xSemaphoreTake( xSemaphore[instance], pdMS_TO_TICKS(timeOutMs) );
					}
				}
			}
		#endif
	}
	else
		xEntryTimeSet = pdTRUE;
	return xEntryTimeSet;
}
BaseType_t I2C_MutxUnLock( uint32 instance )
{
	BaseType_t xEntryTimeSet = pdTRUE;

	if( TRUE == gSystemInitDoneFlag )
	{
		#if( STD_ON == I2C_USING_SEMAPHORE_HANDLE )
			if( xSemaphore[instance] != NULL )
			{
				xEntryTimeSet = xSemaphoreGive( xSemaphore[instance] );
			}
		#endif
	}
	return xEntryTimeSet;
}


void I2C_Init(uint32 instance)
{
	if(instance<LPI2C_INSTANCE_COUNT)
		xSemaphore[instance] = xSemaphoreCreateMutex();
}


void LPI2C_PALI2C_BUS_OFF_RECOVER(uint32 instance)
{
	uint8 i = 0 ;
	TracePrintf(TRACE_TRACE, TRACE_INFO, "==I2C%d BUS RECOVER:err:0x%x==\r\n",instance,i2c_err_Num);
	if( instance == INST_LPI2C0_CFG)
	{
		PINS_Init(&g_pin_mux_i2c0_busoff_recover_Arr[0]);
		PINS_Init(&g_pin_mux_i2c0_busoff_recover_Arr[1]);
		
		for( i = 0 ; i < 9 ; i++ )
		{
			Port_SetOutLevel(I2C0_RECOVER_SCL , PORT_HIGH);
			DelayUs(3);
			Port_SetOutLevel(I2C0_RECOVER_SCL , PORT_LOW);
			DelayUs(3);
				if(Port_GetPinLevel(I2C0_RECOVER_SDA) == 1 )
					break;
		}

		PINS_Init(&g_pin_mux_i2c0_Arr[0]);
		PINS_Init(&g_pin_mux_i2c0_Arr[1]);
	}
	else if( instance == INST_LPI2C1_CFG)
	{
		PINS_Init(&g_pin_mux_i2c1_busoff_recover_Arr[0]);
		PINS_Init(&g_pin_mux_i2c1_busoff_recover_Arr[1]);
		
		for( i = 0 ; i < 9 ; i++ )
		{
			Port_SetOutLevel(I2C1_RECOVER_SCL , PORT_HIGH);
			DelayUs(3);
			Port_SetOutLevel(I2C1_RECOVER_SCL , PORT_LOW);
			DelayUs(3);
				if(Port_GetPinLevel(I2C1_RECOVER_SDA) == 1 )
					break;
		}

		PINS_Init(&g_pin_mux_i2c1_Arr[0]);
		PINS_Init(&g_pin_mux_i2c1_Arr[1]);
	}
}

Std_ReturnType LPI2C_PAL_MasterReceiveDataBlocking(uint32 instance, uint8  cfgNum, uint8 * rxBuff, uint32 rxSize, bool sendStop)
{
	 status_t retVal = STATUS_SUCCESS;
#if 1
		 if( cfgNum != sI2CMaterCurCfgNum[instance] )
		 {
			 sI2CMaterCurCfgNum[instance] = cfgNum;
			 LPI2C_PAL_MasterInit(instance,cfgNum);
		 
		 }
		 else if((i2c_FristInitFlag[instance] == FALSE)&&(gSystemInitDoneFlag == TRUE ))
		 {
			 LPI2C_PAL_MasterInit(instance,cfgNum);
			 i2c_FristInitFlag[instance] = TRUE;
		 }
#endif

	retVal = LPI2C_DRV_MasterReceiveDataBlocking(instance,rxBuff,rxSize,sendStop,IIC_TIMEOUT);
	if(STATUS_SUCCESS != retVal )
	{
		i2c_err_Num = retVal;
		
		{
			LPI2C_PALI2C_BUS_OFF_RECOVER(instance);
		}	
		LPI2C_PAL_MasterInit(instance,cfgNum);

		return E_NOT_OK;
	}
	return E_OK;
}
Std_ReturnType LPI2C_PAL_MasterSendDataBlocking(uint32 instance, uint8  cfgNum,  const uint8 * txBuff, uint32  txSize, bool  sendStop)
{
	status_t retVal = STATUS_SUCCESS;
	if( cfgNum != sI2CMaterCurCfgNum[instance] )
	{
		sI2CMaterCurCfgNum[instance] = cfgNum;
		LPI2C_PAL_MasterInit(instance,cfgNum);
	
	}
	else if((i2c_FristInitFlag[instance] == FALSE)&&(gSystemInitDoneFlag == TRUE ))
	{
		LPI2C_PAL_MasterInit(instance,cfgNum);
		i2c_FristInitFlag[instance] = TRUE;
	}
	
	retVal = LPI2C_DRV_MasterSendDataBlocking(instance,txBuff,txSize,sendStop,IIC_TIMEOUT);
	if(STATUS_SUCCESS != retVal )
	{
		i2c_err_Num = retVal;
		
		{
			LPI2C_PALI2C_BUS_OFF_RECOVER(instance);
		}	
		LPI2C_PAL_MasterInit(instance,cfgNum);
		return E_NOT_OK;
	}
	return E_OK;
}

