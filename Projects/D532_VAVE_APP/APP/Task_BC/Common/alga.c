//******************************************************************************
//file		  		: alga.c 
//Description	  	: algorithm used 
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/5/8         eason             First draft version
//2017/6/1         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup alga
//! @{
//
//*****************************************************************************

#include "alga.h"

/**
  * \brief Nonlinear damp algorithm depend on CRS_Instrument_Cluster_V2.0.20.1.pdf
  *
  *\param u16TargetPos 	target position 
  *\param u16CurrentPos current position  
  *\param u16RiseStep rise damping coefficient
  *\param u16FallStep fall damping coefficient	
  *   
  * \return Show position
  */
unsigned int ALGA_vDelayPT1W(unsigned short u16TargetPos,unsigned short u16CurrentPos,unsigned short u16RiseStep,unsigned short u16FallStep)
{
	unsigned short u16ShowPosition;
	unsigned short u16TargetPos_Varible;
	unsigned short u16StepValue;

	u16TargetPos_Varible = u16TargetPos;
	
	if(u16TargetPos_Varible < u16CurrentPos)			//decrease 
	{
		u16StepValue = (u16CurrentPos - u16TargetPos_Varible)/u16FallStep;
		
		if(u16StepValue < 1)
		{
			u16StepValue = 1;
		}

		u16ShowPosition = u16CurrentPos - u16StepValue;
	}
	else if(u16TargetPos_Varible> u16CurrentPos)		//increase
	{		
		u16StepValue = (u16TargetPos_Varible - u16CurrentPos)/u16RiseStep;
		
		if(u16StepValue < 1)
		{
			u16StepValue = 1;
		}
		u16ShowPosition = u16CurrentPos + u16StepValue;
	}	
	else
	{
		u16ShowPosition = u16CurrentPos;
	}
	
	return u16ShowPosition; 
}

/**
  * \brief Interpolation algorithm
  *
  *\param u16XnData currnet X axis data 
  *\param u8X1Data	X axis data1  
  *\param u8X2Data X axis data2
  *\param u8Y1Data Y axis data1 	
  *\param u8Y2Data Y axis data2 
  *   
  * \return currnet Y axis data 
  */
static unsigned char u8Liner_Seg(unsigned short u16XnData,unsigned char u8X1Data,unsigned char u8X2Data,unsigned char u8Y1Data,unsigned char u8Y2Data) 
{
	unsigned char u8YnData;

	if(u16XnData < u8X1Data)
	{
		u8YnData = u8Y1Data;
	}
	else if(u16XnData > u8X2Data)
	{
		u8YnData = u8Y2Data;
	}
	else
	{
		if(u8Y1Data < u8Y2Data)
		{
			u8YnData = u8Y1Data + (u8Y2Data - u8Y1Data) * (u16XnData - u8X1Data)/(u8X2Data - u8X1Data);
			
		}
		else 
		{
			u8YnData = u8Y1Data - (u8Y1Data - u8Y2Data) * (u16XnData - u8X1Data)/(u8X2Data - u8X1Data);
		}
	}
	return u8YnData;
}

/**
  * \brief Interpolation algorithm
  *
  *\param u16XnData currnet X axis data 
  *\param u8Queuex	X axis array  
  *\param u8Queuey Y axis array
  *\param u8Num number of data	
  *   
  * \return currnet Y axis data 
  */
unsigned char ALGA_u8CurveLimit16_16(unsigned char u16XnData,unsigned char *u8Queuex,unsigned char *u8Queuey,unsigned char u8Num) 

{
	unsigned char u8Count;
	unsigned char u8YnData;
	if(u16XnData < u8Queuex[0])
	{
        u8YnData=55;
	}
	else if(u16XnData > u8Queuex[u8Num-1])
	{
        u8YnData = 0;
	}
	else
	{
    	for(u8Count = 1; u8Count < u8Num-1; u8Count++)
    	{
    		if(u16XnData < u8Queuex[u8Count])
    			break;
    	}
		
    	u8YnData = u8Liner_Seg(u16XnData,u8Queuex[u8Count-1],u8Queuex[u8Count],u8Queuey[u8Count-1],u8Queuey[u8Count]);
	}
	return u8YnData;
}


/**************************************************************************** 
Function:       SampleDataReset
Description:    reset the sample data
Input:          SAMPLE_DATA_STRU *pSampleData ---> the pointer of sample data
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void SampleDataReset(SAMPLE_DATA_STRU *pSampleData)
{
	pSampleData->AddData = 0;
	pSampleData->Cnt = 0;
	pSampleData->MaxData = 0;
	pSampleData->MinData = 0xFFFF;
	pSampleData->AvgData = 0;
}

/**************************************************************************** 
Function:       SampleDataAdd
Description:    put current sample value to sample struct
Input:          SAMPLE_DATA_STRU *pSampleData --> the pointer of sample data
                uint16_t data current sample value
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void SampleDataAdd(SAMPLE_DATA_STRU *pSampleData, uint16_t data)
{
	if(data > pSampleData->MaxData)
	{
		pSampleData->MaxData = data;
	}
	if(data < pSampleData->MinData)
	{
		pSampleData->MinData = data;
	}	
	pSampleData->AddData += data;
	pSampleData->Cnt++;
}

/**************************************************************************** 
Function:       CalAvgSampleData
Description:    calculate of average value of sample data 
Input:          SAMPLE_DATA_STRU *pSampleData --> the pointer of sample data
Output:         none
Return:         average value
Others:         none
*****************************************************************************/
uint16_t CalAvgSampleData(SAMPLE_DATA_STRU *pSampleData)
{
	pSampleData->AddData = pSampleData->AddData-(pSampleData->MinData+pSampleData->MaxData);
	pSampleData->AvgData = pSampleData->AddData/(pSampleData->Cnt-2);
	return pSampleData->AvgData;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


