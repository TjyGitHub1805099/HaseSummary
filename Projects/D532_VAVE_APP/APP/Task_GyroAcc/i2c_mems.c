/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "i2c_mems.h"
#include "iic_if.h"
#include "trace.h"

#define I2C_AUTO_INCREMENT  0x80

static int32_t fd_mems_i2c = -1;
static boolean sGyroI2cWithErrFlag = FALSE;

int mems_i2c_open(const char *dev)
{
   // fd_mems_i2c = open(dev,O_RDWR, 0); changed by tjy
    return fd_mems_i2c;
}

boolean mems_i2c_getErrStatus()
{
	return sGyroI2cWithErrFlag;
}

ER mems_i2c_read(uint8 reg_addr, uint8 *data,uint8 len)
{
	BaseType_t LockGet = pdFALSE ;
	ER result = EERROR;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_add[1]={0},i=0;

	LockGet = I2C_MutxLock(INST_LPI2C_GYRO,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet)
	{		
		l_add[0] = (uint8)(reg_addr);
		l_add[0] |= ((len > 1) ? I2C_AUTO_INCREMENT : 0);
		
		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_GYRO,IIC_CFGNUM_GYRO,l_add,1,true);
			if(E_OK == l_returnValue)
			{
				l_returnValue = LPI2C_PAL_MasterReceiveDataBlocking(INST_LPI2C_GYRO,IIC_CFGNUM_GYRO,data,len,true);
				if(E_OK == l_returnValue)
				{
					result = ESUCCESS;
					break;
				}
			}
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_GYRO);
	}

	if(result == ESUCCESS)
		sGyroI2cWithErrFlag = FALSE;
	else
		sGyroI2cWithErrFlag = TRUE;
	return result;
}

ER mems_i2c_write(uint8 * value,uint8 len)
{
	BaseType_t LockGet = pdFALSE ;
	ER result = EERROR;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 i = 0 ;

	LockGet = I2C_MutxLock(INST_LPI2C_GYRO,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet )
	{
		value[0] |= ((len > 2) ? I2C_AUTO_INCREMENT : 0);

		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_GYRO,IIC_CFGNUM_GYRO,value,len,true);
			if(E_OK == l_returnValue)
			{
				result = ESUCCESS;
				break;
			}
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_GYRO);
	}
	
   return result;
}

ER mems_i2c_ctrl(T_SERIAL_IIC_CHAN_SETTING *setting)
{
    ER result = ESUCCESS;
    //result = ioctl(fd_mems_i2c, SIIC_SET_CHANNEL,setting); changed by tjy

    return result;
}






static int32_t fd_acce_i2c = -1;

int acce_i2c_open(const char *dev)
{
   // fd_acce_i2c = open(dev,O_RDWR, 0); changed by tjy
    return fd_acce_i2c;
}

ER acce_i2c_read(uint8 reg_addr, uint8 *data,uint8 len)
{
	BaseType_t LockGet = pdFALSE ;
	ER result = EERROR;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_add[1]={0},i=0;

	LockGet = I2C_MutxLock(INST_LPI2C_ACCE,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet)
	{
		l_add[0] = (uint8)(reg_addr);
		l_add[0] |= ((len > 1) ? I2C_AUTO_INCREMENT : 0);

		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_ACCE,IIC_CFGNUM_ACCE,l_add,1,true);
			if(E_OK == l_returnValue)
			{
				l_returnValue = LPI2C_PAL_MasterReceiveDataBlocking(INST_LPI2C_ACCE,IIC_CFGNUM_ACCE,data,len,true);
				if(E_OK == l_returnValue)
				{
					result = ESUCCESS;
					break;
				}
			}
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_ACCE);
	}
	
	if(result == ESUCCESS)
		sGyroI2cWithErrFlag = FALSE;
	else
		sGyroI2cWithErrFlag = TRUE;
	return result;
}

ER acce_i2c_write(uint8 * value,uint8 len)
{
	BaseType_t LockGet = pdFALSE ;
	ER result = EERROR;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 i = 0 ;

	LockGet = I2C_MutxLock(INST_LPI2C_ACCE,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet )
	{
		value[0] |= ((len > 2) ? I2C_AUTO_INCREMENT : 0);

		for(i=0;i<3;i++)//max time = 3
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_ACCE,IIC_CFGNUM_ACCE,value,len,true);
			if(E_OK == l_returnValue)
			{
				result = ESUCCESS;
				break;
			}
		}

		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_ACCE);
	}

   return result;
}

ER acce_i2c_ctrl(T_SERIAL_IIC_CHAN_SETTING *setting)
{
    ER result = ESUCCESS;
    //result = ioctl(fd_acce_i2c, SIIC_SET_CHANNEL,setting); changed by tjy

    return result;
}


