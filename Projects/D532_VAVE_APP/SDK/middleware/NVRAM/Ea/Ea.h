/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Ea.h
**          Modifly:brown
**			author: weltry wang
**          Date  : 2017-06-20
**          
**          History:
**          1. Date:2018-02-26
**             Author:weltry wang
**             Modification:
**
******************************************************************************/

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#ifndef Ea_H
#define Ea_H
/*====================[I N C L U D E S]====================*/
//#include "Ea_Cfg.h"
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef enum  
{	
	EA_E_UNINIT = 0x01,
	EA_E_INVALID_BLOCK_NO,
	EA_E_INVALID_BLOCK_OFS,
	EA_E_PARAM_POINTER,
	EA_E_INVALID_BLOCK_LEN,
	EA_E_BUSY,
	EA_E_INVALID_CANCEL,
	EA_E_INIT_FAILED
}EaDeveErrorsType;

typedef enum  
{	
	Page0 = 0,
	Page1 = 1
}EaPageNumType;

typedef struct  
{	
	uint16  EaBlockSize;   
	EaPageNumType EaDataPageNum;
	uint8   EaNumberOfWriteCycles;
	uint8   EaModfiyFlag;

}EaBlockConfigType;

typedef struct  
{	
	uint8   AddressWriteCycles;  
	uint8   BlockWriteCycles;   
	uint16  BlocksDataAddress;
}EaBlockAdminType;


#define   EaDevErrorDetect    STD_ON
#define   EA_DEBUG_TRACE	  STD_OFF 
#define   EaSetModeSupported  STD_OFF
#define   EaPollingMode       STD_OFF

#define   EaDatasetSelectionBits     2u     //it depend on NvM_Cfg.h EaMDatasetSelectionBits = 2^NvMDatasetSelectionBits
//#define   IndexAddressBase           40u
//#define   IndexAddressOffset         2u

#define   EaDeviceIndex              0u

#define   AddressWriteCycleLimit 0xff     //1byte 
#define   BlockWriteCycleForever 0xff     //no cycle limit 
#define   Blockinvalid           0xfe     

#define   EA_BlockHeadLength  2
#define   EA_Block1Length     4
#define   EA_DataBufferLength 30 //
//#define   EA_CrcLength        2  //nv CRC Ϊ16δ
extern const uint8 EA_CrcLength;

#define   EA_BlockIndexLength 2

#define   EA_BLOCK_NO_MODFIED 0
#define   EA_BLOCK_DELETE     1
#define   EA_BLOCK_ADD        2
#define   EA_BLOCK_MODFIED    3

#define   Ea_BackRamValidValue 0xA55A

//all pages number
#define	PAGENUMALL           2



                                                                                                                                                                                              
#ifdef __cplusplus
}
#endif
#endif

