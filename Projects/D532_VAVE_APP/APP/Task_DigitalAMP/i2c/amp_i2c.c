/*
 * amp_i2c.c
 *
 *  Created on: 2019Äê1ÔÂ30ÈÕ
 *      Author: gaojing
 */

#include "Task_DigitalAMP_Cfg.h"

#include "tdf853x_instance.h"
#include "iic_if_cfg.h"
#include "iic_if.h"

#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
#include "trace.h"
#endif

tdf853x_instance amp_i2c;

static int tdf853x_i2c_write(tdf853x_instance* instance, uint8_t* buffer, uint8_t length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_ret;

	LockGet = I2C_MutxLock(INST_LPI2C_AMP, 200, 3); //3 times give lock , interval 200 ms
	if(pdTRUE == LockGet)
	{
		l_ret = LPI2C_PAL_MasterSendDataBlocking(instance->instance, instance->cfgNum, buffer, length, 1);
		if(l_ret != E_OK)
		{
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
			TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_i2c_write failed\r\n");
#endif
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_AMP);
	}

	return l_ret;
}

static int tdf853x_i2c_read(tdf853x_instance* instance, uint8_t* buffer, uint8_t length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_ret = E_NOT_OK;

	LockGet = I2C_MutxLock(INST_LPI2C_AMP,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet )
	{
		l_ret = LPI2C_PAL_MasterReceiveDataBlocking(instance->instance, instance->cfgNum, buffer, length, 1);
		if(l_ret != E_OK)
		{
#if (STD_ON == TASK_DIGITALAMP_DEBUG_TRACE)
			TracePrintf(SYSTEM_TRACE, TRACE_WARNING, "tdf853x_i2c_read failed\r\n");
#endif
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_AMP);
	}
	return l_ret;
}

void amp_i2c_init()
{
	amp_i2c.instance = INST_LPI2C_AMP;
	amp_i2c.cfgNum = IIC_CFGNUM_AMP;
	amp_i2c.i2c_read = tdf853x_i2c_read;
	amp_i2c.i2c_write = tdf853x_i2c_write;
}

