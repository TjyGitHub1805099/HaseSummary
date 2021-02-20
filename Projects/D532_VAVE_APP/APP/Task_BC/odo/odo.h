 /******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : odo.h
**          Author: xiaobo
**
**          Date  : 2017-05-26
**          
**          
******************************************************************************/
#ifndef _ODO_H_
#define _ODO_H_


/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "dea.h"
#include "Ea.h"



/******************************************************************************
**    MACROS
******************************************************************************/
#define ODO_DEBUG

/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/

#define ODO_NVRAM_INDEX				(3U)
#define ODO_BACK_NVRAM_INDEX		(4U)
#define ODO_PARA_NVRAM_INDEX		(5U)
#define ODO_BACK1_NVRAM_INDEX		(15U)
#define ODO_CYCLE					(100)	//100ms
#define ODO_COMP_TIME				(8000/ODO_CYCLE)	//8S
#define TRIP_MAX_VALUE				(99999U)//TripA/TripB meter (591 project range)
#define ODO_MAX_VALUE				(999999UL)//ODO meter
#define ODO_OFFSET_MAX_VALUE		(510)
#define ODO_MAX_RESET_VALUE			(255)
#define ODO_MAX_RESET_TIMES			(100)
#define ODO_MAX_WRITE_TIMES			(10)


#define WRITE_SUCESS	0
#define WRITE_FAILED	1



typedef enum
{
	ODO_DISP_UNIT_KM,	/*! unit:km */
	ODO_DISP_UNIT_MPH,	/*! unit:mph */
}ODO_DISP_UNIT_ENUM;


typedef enum
{
	ODO_DISP_MODE_NORMAL,	/*! normal mode display */
	ODO_DISP_MODE_INVALID,	/*! NVRAM falut cause display invalid, Show "Error KM", CAN output should Invalid */
	ODO_DISP_MODE_ERR,		/*! error mode displasy, show "---", CAN output should Invalid */
}ODO_DISP_MODE;

typedef enum
{
	ODO_OK = 0,	/*! no error */
	ODO_WRITE_NVRAM_ERR,	/*! write data to NVRAM error */
	ODO_READ_NVRAM_ERR,		/*! read data from NVRAM error */
	ODO_WRITE_LE_ERR,	/*! odo write value less than or equal to current odo value */ 
	ODO_WRITE_TIMES_OVERFLOW_ERR,	/*! odo try many times still failed */
	ODO_DISPLAY_OVER_ERR, /*! odo display value >= 255Km */
	ODO_RESET_TIMES_OVER_ERR, /*! odo reset times > 100 */
	ODO_ALREADY_RESET_ERR, /*! odo display value is zero */
	ODO_RESET_ERR,	/*! odo reset faied */
}ODO_ERR_ENUM;

typedef struct
{
	uint16_t Millage;
	uint8_t TripOneMeterCnt;
	uint8_t OdoCnt;
	//uint32_t Odo;	/*! odometer */
	uint32_t OdoDisplay;	
	uint32_t OdoOneMeterCnt;	/*! 1 meter counter*/
	uint16_t OdoOffset;	/*! odometer offset */
	uint32_t TripA;
	uint32_t TripB;
	uint8_t OdoDisplayMode;
	uint8_t TripA_DisplayValid;
	uint8_t TripB_DisplayValid;
}ODO_STRU;

typedef struct
{
	uint16_t OdoOffsetValue;
	uint8_t OdoClearTimes;
	uint8_t OdoWriteTimes;
}ODO_PARA_STRU;




/******************************************************************************
**    FUNCTION DEFINITIONS
******************************************************************************/

void ODO_Init(void);	/*! Init ODO Internal variables and enviromnet */
void ODO_Main(void);	/*! ODO process main loop */
void ODO_MillageProc(void);
uint8_t ODO_WriteOdo(uint32_t NewOdoValue);
uint8_t ODO_ResetOdo(void);
uint8_t ODO_ResetTripA(void);
uint8_t ODO_ResetTripB(void);
void ODO_FactoryReset(void);


uint32_t ODO_GetOdo(void);

uint8_t ODO_GetTripA_Status(void);
uint8_t ODO_GetTripB_Status(void);
uint32_t ODO_GetTripA(void);
uint32_t ODO_GetTripB(void);
uint32_t ODO_GetOdoOneMeterCnt(void);
uint32_t ODO_GetOdo10Tick(void);
uint8_t ODO_GetOdoValid(void);
uint8_t ODO_IsMillageSingalValid(void);
uint8_t OdoWriteCallback(uint8 ServiceId, uint8_t JobResult);




#endif
