

/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Eeprom_Cfg.h
            Modifly:brown
**
**          Date  : 2017-06-9
**          
**          
******************************************************************************/


#ifndef  _EEPROM_CFG_H_
#define  _EEPROM_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define EE_EARE_VAL				0x00


#define EE_24C01_SIZE			127ul
#define EE_24C02_SIZE			255ul
#define EE_24C04_SIZE			511ul
#define EE_24C08_SIZE			1023ul
#define EE_24C16_SIZE			2047ul
#define EE_24C32_SIZE			4095ul
#define EE_24C64_SIZE	    	8191ul
#define EE_24C128_SIZE			16383ul
#define EE_24C256_SIZE			32767ul 

#define EE_24C01_PAGE_SIZE		8ul
#define EE_24C02_PAGE_SIZE		8ul
#define EE_24C04_PAGE_SIZE		8ul
#define EE_24C08_PAGE_SIZE		8ul
#define EE_24C16_PAGE_SIZE		16ul
#define EE_24C32_PAGE_SIZE		32ul
#define EE_24C64_PAGE_SIZE	    32ul
#define EE_24C128_PAGE_SIZE		64ul
#define EE_24C256_PAGE_SIZE		128ul  
//IC tpye check

enum{
EE_24C01 = 0,
EE_24C02,
EE_24C04,
EE_24C06,
EE_24C16,
EE_24C32,
EE_24C64,
EE_24C128,
EE_24C256,
};

#define EEPROM_TYPE 		EE_24C128
#define I2C_PageSize  		EE_24C128_PAGE_SIZE 
#define EEPROM_MAX_SIZE		EE_24C128_SIZE

#ifdef __cplusplus
}
#endif

#endif

