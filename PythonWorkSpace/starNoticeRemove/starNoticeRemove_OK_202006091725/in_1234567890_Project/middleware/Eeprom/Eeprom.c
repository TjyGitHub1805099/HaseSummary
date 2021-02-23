
#include "Std_Types.h"
#include "Eeprom_Cfg.h"
#include "Eeprom.h"
#include "Delay.h"
#include "iic_if.h"


void Eep_Inner_MemCpy(const uint8* sou,uint8* des,uint16 len);

Std_ReturnType Eep_Init()
{
	Std_ReturnType l_returnValue = E_OK;
	//I2C_Init(INST_LPI2C_EEP);
	return(l_returnValue);
}

Std_ReturnType Eep_Read(uint16 address,uint8 *bufferPtr,uint32 length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_add[2]={0};
	uint8 i = 0;

	LockGet = I2C_MutxLock(INST_LPI2C_EEP,200,3);//3 times give lock , interval 200 ms
	if( pdTRUE == LockGet)
	{
		l_add[0] = (uint8)(address>>8);
		l_add[1] = (uint8)address;
		
		for(i=0;i<3;i++)
		{
			l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_EEP,IIC_CFGNUM_EEP,l_add,2,true);
			if(E_OK == l_returnValue)
			{
				l_returnValue = LPI2C_PAL_MasterReceiveDataBlocking(INST_LPI2C_EEP,IIC_CFGNUM_EEP,bufferPtr,length,true);
				if(l_returnValue == E_OK)
					break;
			}
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_EEP);
	}

	return l_returnValue;

}

Std_ReturnType Eep_Write(uint16 address,uint8 *bufferPtr,uint32 length)
{
	BaseType_t LockGet = pdFALSE ;
	Std_ReturnType l_returnValue = E_OK;
	uint32 l_length=0;
	uint32 l_wroffset=0;
	uint8 wr_len=0;
	uint8 l_WriteBuff[EE_24C32_PAGE_SIZE+2];
	uint8* l_bufferPtr=NULL;
	uint8 i = 0;

	if(bufferPtr == NULL)
		return E_NOT_OK;

	LockGet = I2C_MutxLock(INST_LPI2C_EEP,200,3);//3 times give lock , interval 200 ms
	if( LockGet == pdTRUE)
	{

		l_bufferPtr = bufferPtr;
		l_length = length;
		wr_len = EE_24C32_PAGE_SIZE - address%EE_24C32_PAGE_SIZE;
		wr_len = (l_length>=wr_len)?(wr_len):(l_length);
		while(l_length>0)
		{
			l_WriteBuff[0] = (uint8)((address+l_wroffset)>>8);
			l_WriteBuff[1] = (uint8)(address+l_wroffset);
			Eep_Inner_MemCpy(l_bufferPtr,&l_WriteBuff[2],wr_len);
			for(i=0;i<3;i++)
			{
				l_returnValue = LPI2C_PAL_MasterSendDataBlocking(INST_LPI2C_EEP,IIC_CFGNUM_EEP,l_WriteBuff,wr_len+2,true);
				DelayMs(7);
				if(E_OK == l_returnValue)
					break;
			}

			if(E_NOT_OK == l_returnValue)
			{
				break;
			}

			l_wroffset = l_wroffset + wr_len;
			l_length = l_length - wr_len;
			l_bufferPtr = l_bufferPtr + wr_len;
			wr_len = (l_length>=EE_24C32_PAGE_SIZE)?(EE_24C32_PAGE_SIZE):(l_length);
			
		}
		//if take lock success ,in this must unlock !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		I2C_MutxUnLock(INST_LPI2C_EEP);
	}

	return	l_returnValue;
}


void Eep_Inner_MemCpy(const uint8* sou,uint8* des,uint16 len)
{	
	uint16 l_loop=0;
	while(l_loop<len)
	{	
		*(des + l_loop) = *(sou + l_loop);
		l_loop++;
	}
}



