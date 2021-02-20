//*****************************************************************************
// file			: alga.h
// Description	: alga
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/6/1           eason          Modify for HC Cluster
//****************************************************************************

#ifndef ALGA_H
#define ALGA_H
#include "Std_Types.h"

typedef struct
{
	uint16_t MaxData;			/* The maximum of sampled values */
	uint16_t MinData;			/* The minimum of sampled values */
	uint16_t AvgData;			/* The average of sampled values */
	uint16_t Cnt;				/* sample count */
	uint32_t AddData;			/* sample sum */
}SAMPLE_DATA_STRU;	/* sample data struct */

//*****************************************************************************
//
//! \addtogroup alga
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
unsigned int ALGA_vDelayPT1W(unsigned short u16TargetPos,unsigned short u16CurrentPos,unsigned short u16RiseStep,unsigned short u16FallStep);
unsigned char ALGA_u8CurveLimit16_16(unsigned char u16XnData,unsigned char *u8Queuex,unsigned char *u8Queuey,unsigned char u8Num); 

/**************************************************************************** 
Function:       SampleDataReset
Description:    reset the sample data
Input:          SAMPLE_DATA_STRU *pSampleData ---> the pointer of sample data
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void SampleDataReset(SAMPLE_DATA_STRU *pSampleData);
/**************************************************************************** 
Function:       SampleDataAdd
Description:    put current sample value to sample struct
Input:          SAMPLE_DATA_STRU *pSampleData --> the pointer of sample data
                uint16_t data current sample value
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void SampleDataAdd(SAMPLE_DATA_STRU *pSampleData, uint16_t data);

/**************************************************************************** 
Function:       CalAvgSampleData
Description:    calculate of average value of sample data 
Input:          SAMPLE_DATA_STRU *pSampleData --> the pointer of sample data
Output:         none
Return:         average value
Others:         none
*****************************************************************************/
uint16_t CalAvgSampleData(SAMPLE_DATA_STRU *pSampleData);

                          
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  ALGA_H


