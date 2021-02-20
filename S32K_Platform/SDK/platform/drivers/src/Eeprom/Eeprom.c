;
#include "Std_Types.h"
#include "Eeprom_Cfg.h"
#include "Eeprom.h"
#include "delay.h"
#include "lpi2c0_cfg.h"
lpi2c_master_state_t i2c_master_state;

void Eep_Inner_MemCpy(const uint8* sou,uint8* des,uint16 len);


Std_ReturnType Eep_Init()
{
	Std_ReturnType l_returnValue = E_NOT_OK;
	l_returnValue = LPI2C_DRV_MasterInit(0,&lpi2c0_MasterConfig,&i2c_master_state);
	return(l_returnValue);
}

Std_ReturnType Eep_Read(uint16 address,uint8 *bufferPtr,uint32 length)
{
	Std_ReturnType l_returnValue = E_NOT_OK;
	uint8 l_add[2]={0};

	l_add[0] = (uint8)(address>>8);
	l_add[1] = (uint8)address;
	
	l_returnValue = LPI2C_DRV_MasterSendDataBlocking(0,l_add,2,true,2000);
	if(E_NOT_OK == l_returnValue)
		return E_NOT_OK;
	l_returnValue = LPI2C_DRV_MasterReceiveDataBlocking(0,bufferPtr,length,true,2000);
	if(E_NOT_OK == l_returnValue)
		return E_NOT_OK;

	return(l_returnValue);
}

Std_ReturnType Eep_Write(uint16 address,uint8 *bufferPtr,uint32 length)
{
	Std_ReturnType l_returnValue = E_OK;
	uint32 l_length=0;
	uint32 l_wroffset=0;
	uint8 wr_len=0;
	uint8 l_WriteBuff[EE_24C32_PAGE_SIZE+2];
	l_length = length;
	wr_len = EE_24C32_PAGE_SIZE - address%EE_24C32_PAGE_SIZE;
	wr_len = (l_length>=wr_len)?(wr_len):(l_length);
	while(l_length>0)
	{
		l_WriteBuff[0] = (uint8)((address+l_wroffset)>>8);
		l_WriteBuff[1] = (uint8)(address+l_wroffset);
		Eep_Inner_MemCpy(bufferPtr,&l_WriteBuff[2],wr_len);
		l_returnValue = LPI2C_DRV_MasterSendDataBlocking(0,l_WriteBuff,wr_len+2,true,2000);
		DelayMs(6);
		l_wroffset = l_wroffset + wr_len;
		l_length = l_length - wr_len;
		wr_len = (l_length>=EE_24C32_PAGE_SIZE)?(EE_24C32_PAGE_SIZE):(l_length);
		if(E_NOT_OK == l_returnValue)
		{
			return E_NOT_OK;
		}
	}
	return(l_returnValue);
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

void Eep_Test()
{
	uint8 readdata[5]={0};
	uint8 writedata[5]={1,2,3,4,5};
	uint8 retrunval=0;
	//EepromTest();
	//retrunval = Eeprom_IIC_master_ByteWrite(0,writedata,3);
	//retrunval = Eeprom_IIC_master_ByteRead(0,readdata,3);


}

