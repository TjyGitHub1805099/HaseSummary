#define IOC_PLATFORM
#include "FuelProc.h"
#include "Std_Types.h"
//#include "Adc_If.h"
#include "adc_wrapper_cfg.h" /**AD sample file**/
#include "adc_wrapper_if.h"	 /**AD sample file**/

#include "ErrorTypes.h"
#include "trace.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "Task_IpcApp_Cfg.h"
#include "stdlib.h"
#include "MemMap.h"

static uint8_t s_FuelCircuitVersion = 5;

#define CAL_FUEL_PERCENT(FuelVol) (uint16_t)((((uint32_t)FuelVol-FUEL_E_VOL)*1000)\
                                       /(FUEL_FULL_VOL-FUEL_E_VOL))

#ifdef FUEL_INLINE_SUPPORT
#pragma inline FUEL_FillUpProc, FUEL_DampCalProc
#endif

BACKRAM_SEC_VAR_UNSPECIFIED FUEL_PROC_STRU gFuelData;   //fuel module variable
FuelFillUpFuncType gFuelFillUpFuncList[5] = {NULL};
static uint8_t gFuelFillUpFuncCnt = 0;


static uint8_t s_InitFuelProcFlag = 0; //fuel module init falg


void FUEL_RegisterFillUpFunc(FuelFillUpFuncType pfuc)
{
	if((pfuc != NULL) && (gFuelFillUpFuncCnt < (sizeof(gFuelFillUpFuncList)/sizeof(gFuelFillUpFuncList[0]) )))
	{
		gFuelFillUpFuncList[gFuelFillUpFuncCnt] = pfuc;
		gFuelFillUpFuncCnt++;
	}
}

void FUEL_CalFillUpFunc(void)
{
	uint8_t cnt;
	for(cnt = 0; cnt < gFuelFillUpFuncCnt; cnt++)
	{
		if(gFuelFillUpFuncList[cnt] != NULL)
		{
			gFuelFillUpFuncList[cnt]();
		}
	}
}


uint16_t GetRpmValue(void)
{
	return 0;
}

uint16_t GetFuelPowerAdValue(void)	/** get sampled ad value of fuel sample circuit power voltage */
{
	uint16_t l_adValue;
	//if(GetFuelPwrVal(&l_adValue) != E_OK)
	if(AdcWrapper_Read(ADC_LOGIC_CH_FUEL_PWR,&l_adValue) != TRUE)
	{
		l_adValue = 0xFFFF;
	}

	return l_adValue;
}
uint16_t GetFuelSensorAdValue(void)	/** get sampled ad value of fuel sensor access point voltage */
{
	uint16_t l_adValue;
	//if(GetBatFuelAdVal(&l_adValue) != E_OK)
	if(AdcWrapper_Read(ADC_LOGIC_CH_FUEL_AD,&l_adValue) !=TRUE)
	{
		l_adValue = 0xFFFF;
	}
	return l_adValue;
}


uint16_t FUEL_GetFuelResValue(void)
{
#ifdef FUEL_DEBUG
	uint16_t l_adValue;
	uint16_t l_value = 0;
	if(GetBatFuelAdVal(&l_adValue) == E_OK)
	{
		if(l_adValue < 3980)
		{
			l_value = (((uint32_t)l_adValue*1000)/(3980-l_adValue));
		}
		else
		{
			l_value = 0xFFFF;
		}
	}
	return l_value;
#else
	return gFuelData.ResValue;
#endif
}

/**************************************************************************** 
Function:       GetFuelSigStatus
Description:    get fuel sensor status 
Input:          none
Output:         none
Return:         fuel sensor status
Others:         none
*****************************************************************************/
FUEL_SIG_ENUM FUEL_GetFuelSigStatus(void)
{
    if(gFuelData.Stat.SensorStat == 0)
    {
        return FuelSigNormal; //fuel sensor normal
    }
    else if(gFuelData.Stat.SensorOpen == 1)
    {
        return FuelSigOpen; //fuel sensor circuit open 
    }
    else if(gFuelData.Stat.SensorShort == 1)
    {
        return FuelSigShort; //fuel sensor circuit short
    }
	else
	{
		return FuelSigUnknow; //invalid
	}
}

/**************************************************************************** 
Function:       FUEL_GetFillUpStatus
Description:    get fuel fill up status 
Input:          none
Output:         none
Return:         fuel fill up status
Others:         none
*****************************************************************************/
uint8_t FUEL_GetFillUpStatus(void)
{
    return (gFuelData.Stat.FuelFillUpFlag|gFuelData.Stat.IgnOnFuelFillUpFlag);
}


/**************************************************************************** 
Function:       GetFuelLowStatus
Description:    get fuel low status 
Input:          none
Output:         none
Return:         fuel low status
Others:         none
*****************************************************************************/
uint8_t FUEL_GetFuelLowStatus(void)
{
    return gFuelData.Stat.LowFuel;
}

/**************************************************************************** 
Function:       GetFuelVol
Description:    get fuel volume 
Input:          none
Output:         none
Return:         fuel volume
Others:         none
*****************************************************************************/
uint16_t FUEL_GetFuelVol(void)
{
    return gFuelData.FuelVolDamp;
}

/**************************************************************************** 
Function:       GetFuelVolNow
Description:    get fuel volume 
Input:          none
Output:         none
Return:         fuel volume
Others:         none
*****************************************************************************/
uint16_t FUEL_GetFuelVolNow(void)
{
    return gFuelData.SampleData0p5.FuelVol;
}


/**************************************************************************** 
Function:       GetIndicatePos
Description:    get fuel indicate position
Input:          none
Output:         none
Return:         fuel indicate position
Others:         none
*****************************************************************************/
uint16_t FUEL_GetIndicatePos(void)
{
    return gFuelData.IndicatePosCur;
}

/**************************************************************************** 
Function:       FUEL_GetFuelSensorResNow
Description:    get fuel res
Input:          none
Output:         none
Return:         fuel res now
Others:         none
*****************************************************************************/
uint16_t FUEL_GetFuelSensorResNow(void)
{
	return gFuelData.ResValueNow;
}

/**************************************************************************** 
Function:       FUEL_GetIndicateSeg
Description:    get fuel indicate segment
Input:          none
Output:         none
Return:         fuel indicate position
Others:         none
*****************************************************************************/
uint16_t FUEL_GetIndicateSeg(void)
{
	return gFuelData.IndicateSegCur;
}

/**************************************************************************** 
Function:       GetFuelPercent
Description:    get fuel volume percent
Input:          none
Output:         none
Return:         fuel volume percent
Others:         none
*****************************************************************************/
uint16_t FUEL_GetFuelPercent(void)
{
    return gFuelData.FuelVolPercent;
}

/**************************************************************************** 
Function:       GetFuelSleepEnableStat
Description:    get fule module sleep enable flag
Input:          none
Output:         none
Return:         fuel module sleep enable flag
Others:         none
*****************************************************************************/
uint8_t FUEL_GetFuelSleepEnableStat(void)
{
    return gFuelData.Stat.SleepEnable;
}

/**************************************************************************** 
Function:       GetFuelIndMode
Description:    get fule indicate response mode
Input:          none
Output:         none
Return:         fule module data
Others:         none
*****************************************************************************/
uint8_t FUEL_GetFuelIndMode(void)
{
    return gFuelData.Stat.FuelIndMode;
}

/**************************************************************************** 
Function:       FUEL_GetFuelVolumeValid
Description:    get fule volume valid
Input:          none
Output:         none
Return:         fule module data
Others:         none
*****************************************************************************/
uint8_t FUEL_GetFuelVolumeValid(void)
{
	return gFuelData.Stat.FuelVolumeValid;
}

#ifdef FUEL_DEBUG
/**************************************************************************** 
Function:       GetFuelData
Description:    get fule module data
Input:          none
Output:         none
Return:         fule module data
Others:         none
*****************************************************************************/
FUEL_PROC_STRU GetFuelData(void)
{
	return gFuelData;
}

/**************************************************************************** 
Function:       SetInitFuelProcFlag
Description:    set init fuel module flag
Input:          init fuel module flag
Output:         none
Return:         void
Others:         none
*****************************************************************************/
void SetInitFuelProcFlag(uint8_t flag)
{
	s_InitFuelProcFlag = flag;
}
#endif

/**************************************************************************** 
Function:       lookTableTrans
Description:    Using look-up table method 
                To solve the problem of piecewise linear evaluation
Input:          uint16_t Value --> current value
                uint16_t Table[][2] --> Transition table's Addr
                uint16_t tableSize -->length of Transition table
Output:         none
Return:         Transormed data
Others:         none
*****************************************************************************/
uint16_t lookTableTrans(uint16_t Value, const uint16_t Table[][2], uint16_t tableSize)
{
	uint8_t cycle;
	uint16_t res = 0;
#if FUEL_SIG_FROM_RES		
	if(Value <= Table[0][0])
	{
		res = Table[0][1];
	}
	else if(Value >= Table[tableSize-1][0])
	{
		res = Table[tableSize-1][1];
	}
	else
	{
		for(cycle = 1; cycle < tableSize; cycle++)
		{
			if(Table[0][1] > Table[1][1])	//judge the table data size sort
			{
				if(Value <= Table[cycle][0])
				{
					res = Table[cycle-1][1]-((uint32_t)(Value - Table[cycle-1][0])*\
						(Table[cycle-1][1]-Table[cycle][1]))/(Table[cycle][0]-Table[cycle-1][0]);
					break;
				}
			}
			else
			{
				if(Value <= Table[cycle][0])
				{
					res = Table[cycle-1][1]+((uint32_t)(Value - Table[cycle-1][0])*\
						(Table[cycle][1]-Table[cycle-1][1]))/(Table[cycle][0]-Table[cycle-1][0]);
					break;
				}
			}
		}
	}
#else
	if(Value >= Table[0][0])
	{
		res = Table[0][1];
	}
	else if(Value <= Table[tableSize-1][0])
	{
		res = Table[tableSize-1][1];
	}
	else
	{
		for(cycle = 1; cycle < tableSize; cycle++)
		{
			if(Table[0][1] > Table[1][1])	//judge the table data size sort
			{
				if(Value >= Table[cycle][0])
				{
					res = Table[cycle][1]+((uint32_t)(Value - Table[cycle][0])*\
						(Table[cycle-1][1]-Table[cycle][1]))/(Table[cycle-1][0]-Table[cycle][0]);
					break;
				}
			}
			else
			{
				if(Value >= Table[cycle][0])
				{
					res = Table[cycle][1]+((uint32_t)(Value - Table[cycle][0])*\
						(Table[cycle][1]-Table[cycle-1][1]))/(Table[cycle-1][0]-Table[cycle][0]);
					break;
				}
			}
		}
	}
#endif
	return res;
}

/**************************************************************************** 
Function:       GetDispSegment
Description:    Get target display segment
Input:          uint32_t CurSeg --> current segment
                uint32_t CurRes --> current resistance
Output:         none
Return:         taget display
Others:         none
*****************************************************************************/
uint32_t GetDispSegment(uint32_t CurSeg, uint32_t CurRes)
{
	uint32_t RetSeg = 0;
	uint32_t i;
    const uint16_t *pFuelDisTableUp = NULL;
    const uint16_t *pFuelDisTableDown = NULL;
    uint16_t TableSize = 0;

    pFuelDisTableUp = GetFuelDisTableUp();
    pFuelDisTableDown = GetFuelDisTableDown();
    TableSize = GetFuelDisTableSize();
#if FUEL_SIG_FROM_RES	
	for (i = 0; i < TableSize; i++)	
	{
		if (CurRes < pFuelDisTableUp[i])	//get ride of edgel effect
		{
			break;
		}
	}

	if ((i >= TableSize) && (CurRes > pFuelDisTableDown[i - 1]))	//check if reach E point
	{
		RetSeg = TableSize - i;
	}
	else if (i == 0)	//reach F point
	{
		RetSeg = TableSize;
	}
	else
	{
		if ((TableSize - i) > CurSeg)	//segment up
		{
			RetSeg = TableSize - i;
		}
		else if (CurRes > pFuelDisTableDown[TableSize-CurSeg])	//segment down
		{
			RetSeg = TableSize - i;
		}
		else
		{
			RetSeg = CurSeg;
		}
	}
#else
	for (i = TableSize; i > 0; i--)	
	{
		if (CurRes > pFuelDisTableUp[i-1])	//get ride of edgel effect
		{
			break;
		}
	}

	
	if (i >= TableSize)	//reach F point
	{
		RetSeg = TableSize;
	}
	else if ((i == 0) && (CurRes < pFuelDisTableDown[0]))	//check if reach E point
	{
		RetSeg = 0;
	}
	else
	{
		if (i > CurSeg)	//segment up
		{
			RetSeg = i;
		}
		else if (CurRes < pFuelDisTableDown[CurSeg-1])	//segment down
		{
			if(CurRes < pFuelDisTableDown[i])
			{
				RetSeg = i;
			}
			else
			{
				RetSeg = i+1;
			}
		}
		else
		{
			RetSeg = CurSeg;
		}
	}
#endif
	return RetSeg;
}

/**************************************************************************** 
Function:       CalFuelSensorRes
Description:    calculate current fuel sensor resistance
Input:          none
Output:         none
Return:         currnet fuel sensor resistance
Others:         none
*****************************************************************************/
uint16_t CalFuelSensorRes(void)
{
#ifdef FUEL_DEBUG
	return FUEL_GetFuelResValue();
#else
	uint32_t fuelPowerAdValue, fuelSensorAdValue;
	uint32_t fuelSensorResValue;
	
	if(s_FuelCircuitVersion >= 5)
	{
		fuelPowerAdValue = (GetFuelPowerAdValue()*FUEL_ADAPT_PARA*2)/1000;	//get sampled ad value of fuel sample circuit power voltage
	}
	else
	{
		fuelPowerAdValue = (GetFuelPowerAdValue()*(FUEL_ADAPT_PARA+3)*2)/1000;	//get sampled ad value of fuel sample circuit power voltage
	}
	fuelSensorAdValue = GetFuelSensorAdValue();	//get sampled ad value of fuel sensor access point voltage
#if 0
	if(fuelPowerAdValue>5)
	{
		fuelPowerAdValue -= 5;
	}
#endif
	if(/*(fuelPowerAdValue <= FUEL_AD_MAX) && */(fuelSensorAdValue < FUEL_AD_MAX) && (fuelPowerAdValue > fuelSensorAdValue))
	{	
		//calculate current fuel sensor resistance
		fuelSensorResValue = ((fuelSensorAdValue*(FUEL_SAMPLE_RES_VALUE))/(fuelPowerAdValue - fuelSensorAdValue));
		fuelSensorResValue = (fuelSensorResValue*110000)/(110000-fuelSensorResValue);
		if(fuelSensorResValue > 8)
		{
			fuelSensorResValue= fuelSensorResValue - 8;
		}
		return fuelSensorResValue;
	}
	else
	{
		return 0;	//return a invalid value
	}
#endif
}

#ifndef FILTER_EXT
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
#endif

/**************************************************************************** 
Function:       InitFuelModule
Description:    init fuel module data and status 
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void InitFuelModule(void)
{
	gFuelData.CurCarStat = FUEL_PROC_SLEEP;
	gFuelData.LastCarStat = FUEL_PROC_SLEEP;
	gFuelData.FuelVolDamp = 0;
	gFuelData.FuelVolNow = 0;
	gFuelData.FuelVolPercent = 0;
	gFuelData.FuelVolSave = 0;
	//gFuelData.FuelVolSaveIgnOff = 0;
	gFuelData.IndicatePosTarget = 0;
	gFuelData.IndicatePosCur = 0;
	gFuelData.IndicatePosSave = 0;
	gFuelData.ResValue = 0;
	gFuelData.ResValueDamp = 0;
	gFuelData.FuelVolSaveTimeCnt = 0;
    gFuelData.Stat.BatOnOrSigRecoverlag = 1;
    gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_FAST;
    gFuelData.Stat.FuelFillUpFlag = 0;
    gFuelData.Stat.FuelIndSaved = 0;
    gFuelData.Stat.FuelVolSaved = 0;
    gFuelData.Stat.LowFuel = 0;
    gFuelData.Stat.SensorOpen = 0;
    gFuelData.Stat.SensorShort = 0;
    gFuelData.Stat.SensorStat = FuelSigUnknow;	/* valid */
	gFuelData.Stat.FuelVolumeValid = 0;	/* Invalid */
    gFuelData.Stat.SleepEnable = 1;
	gFuelData.Stat.FirstBatOnFlag = 1;
	gFuelData.Stat.IgnOnFuelFillUpFlag = 0;
	gFuelData.Stat.FuelIgnOffVolSaved = 0;

    //clean the sample data
	SampleDataReset(&gFuelData.SampleData0p5.SampleData);
	SampleDataReset(&gFuelData.SampleDataMid.SampleData);
	gFuelData.SampleData0p5.FinishFlag = 0;
	gFuelData.SampleDataMid.FinishFlag = 0;
	//gFuelData.SampleData5Min.FinishFlag = 0;

    gFuelData.IndicateSegTarget  = 0;
	gFuelData.IndicateSegCur = 0;
	gFuelData.ResValueNow = 0;
}

/**************************************************************************** 
Function:       GetFuelSampleData
Description:    sample fuel sensor signal one time
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
int16_t GetFuelSampleData(void)
{
	static uint16_t SignalNormalCnt = 0, SignalErrorCnt = 0;    //fuel sensor normal or fuel sensor fault status counter
	uint16_t fuelSensorResValue = 0;
#if FUEL_SIG_FROM_RES	
	fuelSensorResValue = CalFuelSensorRes();
	if((fuelSensorResValue > FUEL_SENSOR_SHORT) && (fuelSensorResValue < FUEL_SENSOR_OPEN))	//fuel sensor normal
	{
		SignalErrorCnt = 0;
		if(gFuelData.Stat.FirstBatOnFlag == 1)
		{
			SignalNormalCnt++;
			if(SignalNormalCnt > 2)
			{
				gFuelData.Stat.SensorStat = 0;	//reset fuel sensor fault bit flag
				gFuelData.Stat.SensorOpen = 0;
				gFuelData.Stat.SensorShort = 0;	
				gFuelData.Stat.FirstBatOnFlag = 0;
				DEA_SetFuelSigStat(DEA_FUEL_SIG_NORMAL);
			}
		}
		else if(gFuelData.Stat.SensorStat == 1)	//sensor signal error before  (传感器由故障到正常)
		{
			SignalNormalCnt++;
			if(SignalNormalCnt > (FUEL_SENSOR_OK_JUDGE_TIME/FUEL_SAMPLE_CYCLE))
			{
				gFuelData.Stat.SensorStat = 0;	//reset fuel sensor fault bit flag
				gFuelData.Stat.SensorOpen = 0;
				gFuelData.Stat.SensorShort = 0;	
				DEA_SetFuelSigStat(DEA_FUEL_SIG_NORMAL);
			}
		}
		else
		{

			SampleDataAdd(&gFuelData.SampleData0p5.SampleData, fuelSensorResValue);
            
			if(gFuelData.SampleData0p5.SampleData.Cnt >= 10)	//0.5S sample data got
			{
				gFuelData.SampleData0p5.ResValue = CalAvgSampleData(&gFuelData.SampleData0p5.SampleData);
				gFuelData.ResValueNow = gFuelData.SampleData0p5.ResValue;
				
				//Using the look-up table method, the resistance value of the fuel sensor is converted to the value of the fuel oil.
				gFuelData.SampleData0p5.FuelVol = lookTableTrans(gFuelData.SampleData0p5.ResValue, FuelResToVolTable, 		// look up the value table for fuel vol
													GetFuelResToVolTableSize());
				//calculate indicate position
				gFuelData.SampleData0p5.IndicatePos = lookTableTrans(gFuelData.SampleData0p5.FuelVol, FuelVolToIndPos, 		// look up the value table for fuel indication 
													GetFuelVolToIndPosSize());
				
				gFuelData.SampleData0p5.FinishFlag = 1;
				
				SampleDataAdd(&gFuelData.SampleDataMid.SampleData, gFuelData.SampleData0p5.SampleData.AvgData);
				//SampleDataAdd(&gFuelData.SampleData5Min.SampleData, gFuelData.SampleData0p5.SampleData.AvgData);
				
				SampleDataReset(&gFuelData.SampleData0p5.SampleData);
			}

			if(gFuelData.SampleDataMid.SampleData.Cnt >= 10)		//Mid sample data got
			{
				gFuelData.SampleDataMid.ResValue = CalAvgSampleData(&gFuelData.SampleDataMid.SampleData);
				//Using the look-up table method, the resistance value of the fuel sensor is converted to the value of the fuel oil.
				gFuelData.SampleDataMid.FuelVol = lookTableTrans(gFuelData.SampleDataMid.ResValue, FuelResToVolTable, 
													GetFuelResToVolTableSize());
				//calculate indicate position
				gFuelData.SampleDataMid.IndicatePos = lookTableTrans(gFuelData.SampleDataMid.FuelVol, FuelVolToIndPos, 
													GetFuelVolToIndPosSize());
				
				gFuelData.SampleDataMid.FinishFlag = 1;
				
				SampleDataReset(&gFuelData.SampleDataMid.SampleData);
			}
		}
	}
	else	//fuel sensor fault
	{
		gFuelData.ResValueNow = fuelSensorResValue;
		SignalNormalCnt = 0;
		if(gFuelData.Stat.SensorStat == 0)	//sensor signal anomaly before
		{
			SignalErrorCnt++;
			if(SignalErrorCnt > (FUEL_SENSOR_ERR_JUDGE_TIME/FUEL_SAMPLE_CYCLE))
			{
				gFuelData.Stat.SensorStat = 1;	//set fuel sensor fault bit flag
			}
		}
		else
		{
			if(fuelSensorResValue == 0 || fuelSensorResValue >= FUEL_SENSOR_OPEN)	//fuel sensor fault
			{
				gFuelData.Stat.SensorOpen = 1;
				gFuelData.Stat.SensorShort = 0;	
				DEA_SetFuelSigStat(DEA_FUEL_SIG_OPEN);
			}
			else
			{
				gFuelData.Stat.SensorShort = 1;
				gFuelData.Stat.SensorOpen = 0;
				DEA_SetFuelSigStat(DEA_FUEL_SIG_SHORT);
			}
		}
	}
#else
	if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_Msg1) == 0)
	{
		if(DEA_GetFuelLevelPercentValid())
		{
			SignalErrorCnt = 0;
			if(gFuelData.Stat.SensorStat == 1)	//sensor signal error before
			{
				SignalNormalCnt++;
				if(SignalNormalCnt > (FUEL_SENSOR_OK_JUDGE_TIME/FUEL_SAMPLE_CYCLE))
				{
					gFuelData.Stat.SensorStat = 0;	//reset fuel sensor fault bit flag
					gFuelData.Stat.SensorOpen = 0;
					gFuelData.Stat.SensorShort = 0;	
					DEA_SetFuelSigStat(DEA_FUEL_SIG_NORMAL);
				}
			}
			else
			{
				fuelSensorResValue = DEA_GetFuelLevelPercent();
				SampleDataAdd(&gFuelData.SampleData0p5.SampleData, fuelSensorResValue);
			            
				if(gFuelData.SampleData0p5.SampleData.Cnt >= 10)	//0.5S sample data got
				{
					gFuelData.SampleData0p5.ResValue = CalAvgSampleData(&gFuelData.SampleData0p5.SampleData);
					//Using the look-up table method, the resistance value of the fuel sensor is converted to the value of the fuel oil.
					gFuelData.SampleData0p5.FuelVol = lookTableTrans(gFuelData.SampleData0p5.ResValue, FuelResToVolTable, 
														GetFuelResToVolTableSize());
					//calculate indicate position
					gFuelData.SampleData0p5.IndicatePos = lookTableTrans(gFuelData.SampleData0p5.FuelVol, FuelVolToIndPos, 
														GetFuelVolToIndPosSize());
					
					gFuelData.SampleData0p5.FinishFlag = 1;
					
					SampleDataAdd(&gFuelData.SampleDataMid.SampleData, gFuelData.SampleData0p5.SampleData.AvgData);
					//SampleDataAdd(&gFuelData.SampleData5Min.SampleData, gFuelData.SampleData0p5.SampleData.AvgData);
					
					SampleDataReset(&gFuelData.SampleData0p5.SampleData);
				}

				if(gFuelData.SampleDataMid.SampleData.Cnt >= FUEL_MID_SAMPLE_CYCLE)		//Mid sample data got
				{
					gFuelData.SampleDataMid.ResValue = CalAvgSampleData(&gFuelData.SampleDataMid.SampleData);
					//Using the look-up table method, the resistance value of the fuel sensor is converted to the value of the fuel oil.
					gFuelData.SampleDataMid.FuelVol = lookTableTrans(gFuelData.SampleDataMid.ResValue, FuelResToVolTable, 
														GetFuelResToVolTableSize());
					//calculate indicate position
					gFuelData.SampleDataMid.IndicatePos = lookTableTrans(gFuelData.SampleDataMid.FuelVol, FuelVolToIndPos, 
														GetFuelVolToIndPosSize());
					
					gFuelData.SampleDataMid.FinishFlag = 1;
					
					SampleDataReset(&gFuelData.SampleDataMid.SampleData);
				}
			}
		}
		else
		{
			SignalNormalCnt = 0;
			if(gFuelData.Stat.SensorStat == 0)	//sensor signal anomaly before
			{
				SignalErrorCnt++;
				if(SignalErrorCnt >= 100)	//continue 5 seconds received invalid fuel sig
				{
					gFuelData.Stat.SensorStat = 1;	//set fuel sensor fault bit flag
					gFuelData.Stat.SensorOpen = 1;
					DEA_SetFuelSigStat(DEA_FUEL_SIG_OPEN);
				}
			}
		}
	}
	else
	{
		SignalNormalCnt = 0;
		if(gFuelData.Stat.SensorStat == 0)	//sensor signal anomaly before
		{
			//SignalErrorCnt++;
			//if(SignalErrorCnt >= 5)
			{
				gFuelData.Stat.SensorStat = 1;	//set fuel sensor fault bit flag
				gFuelData.Stat.SensorOpen = 1;
				DEA_SetFuelSigStat(DEA_FUEL_SIG_OPEN);
			}
		}
	}
#endif

	return 0;
}

/**************************************************************************** 
Function:       FuelDispProc
Description:    fuel display process
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void FuelDispProc(void)
{
    static uint16_t cycle = 0;
    uint16_t min_step;
#ifdef FUEL_TRACE_DEBUG
	static uint16_t s_lastSeg = 0;
#endif
    if(GetDispType() == 0U) //the fuel display style is step
    {
    	if(DEA_GetIgnStat() == IGN_ON)
        {
	        switch(gFuelData.Stat.FuelIndMode)
	        {
	            case FUEL_IND_MODE_FAST:
	                min_step = 1;
	                if(gFuelData.IndicatePosCur > gFuelData.IndicatePosTarget)	//possition_now > possition_target
	                {
	                    if(gFuelData.IndicatePosCur > min_step)
	                    {
	                        gFuelData.IndicatePosCur -= min_step;	//limit the indication possition
	                    }
	                    else
	                    {
	                        gFuelData.IndicatePosCur = 0;
	                    }
	                    if(gFuelData.IndicatePosCur < gFuelData.IndicatePosTarget)
	                    {
	                        gFuelData.IndicatePosCur = gFuelData.IndicatePosTarget;
	                    }
	                }
	                else if(gFuelData.IndicatePosCur < gFuelData.IndicatePosTarget)	//possition_now < possition_target
	                {

	                   gFuelData.IndicatePosCur += min_step;
	                   if(gFuelData.IndicatePosCur > gFuelData.IndicatePosTarget)
	                   {
	                        gFuelData.IndicatePosCur = gFuelData.IndicatePosTarget;
	                   }
	                }
	                else
	                {
	                    gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_MID;
	                }
	                break;
	            case FUEL_IND_MODE_MID:
	                min_step = 1;
	                if(gFuelData.IndicatePosCur > gFuelData.IndicatePosTarget)
	                {
	                    if(gFuelData.IndicatePosCur > min_step)
	                    {
	                        gFuelData.IndicatePosCur -= min_step;
	                    }
	                    else
	                    {
	                        gFuelData.IndicatePosCur = 0;
	                    }
	                    if(gFuelData.IndicatePosCur < gFuelData.IndicatePosTarget)
	                    {
	                        gFuelData.IndicatePosCur = gFuelData.IndicatePosTarget;
	                    }
	                }
	                else if(gFuelData.IndicatePosCur < gFuelData.IndicatePosTarget)
	                {

	                   gFuelData.IndicatePosCur += min_step;
	                   if(gFuelData.IndicatePosCur > gFuelData.IndicatePosTarget)
	                   {
	                        gFuelData.IndicatePosCur = gFuelData.IndicatePosTarget;
	                   }
	                }
	                break;
	            case FUEL_IND_MODE_SLOW:
	                gFuelData.IndicatePosCur = gFuelData.IndicatePosTarget;
	                break;
	            default:
	                break;
	        }
    	}
		else
		{
#if 0			 //for 591 ,ign off do nothing
			 gFuelData.IndicatePosCur = 0;	
#endif
		}
    }
    else    ////the fuel display style is segment
    {
    	if(DEA_GetIgnStat() == IGN_ON)
        {
        	if(gFuelData.Stat.FuelVolumeValid == 0)
	        {
	        	gFuelData.IndicateSegTarget = 0;
        	}
	        else
			{
				gFuelData.IndicateSegTarget = GetDispSegment(gFuelData.IndicateSegTarget, gFuelData.ResValueDamp);
	        }
	        switch(gFuelData.Stat.FuelIndMode)
	        {
	            case FUEL_IND_MODE_FAST:
	                if(cycle < 5)
	                {
						cycle++;
	                }
	                else
	                {
	                   	cycle = 0;
	                    if(gFuelData.IndicateSegCur < gFuelData.IndicateSegTarget)
	                    {
	                        gFuelData.IndicateSegCur++;
	                    }
	                    else if(gFuelData.IndicateSegCur > gFuelData.IndicateSegTarget)
	                    {
	                        gFuelData.IndicateSegCur--; 
	                    }
	                    else
	                    {
	                        gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_MID;
	                    }
	                }
	                break;
	            case FUEL_IND_MODE_MID:
	                if(cycle < 10)
	                {
						cycle++;
	                }
	                else
	                {
	                  	cycle = 0;
	                    if(gFuelData.IndicateSegCur < gFuelData.IndicateSegTarget)
	                    {
	                        gFuelData.IndicateSegCur++;
	                    }
	                    else if(gFuelData.IndicateSegCur > gFuelData.IndicateSegTarget)
	                    {
	                        gFuelData.IndicateSegCur--; 
	                    }
	                }
	                break;
	            case FUEL_IND_MODE_SLOW:
#if FUEL_JUST_RUNNING_DOWN
					if(gFuelData.IndicateSegTarget <  gFuelData.IndicateSegCur)
#endif
	                {
	                	gFuelData.IndicateSegCur = gFuelData.IndicateSegTarget;
					}
	                break;
	            default:
	                break;
	        }
    	}
		else
		{
			gFuelData.IndicateSegCur = 0;
		}
#ifdef FUEL_TRACE_DEBUG
		if(s_lastSeg != gFuelData.IndicateSegCur)
		{
			s_lastSeg = gFuelData.IndicateSegCur;
			TracePrintf(0, 0, 0, 0,"[FUEL]: M=%d T=%d Seg=%d\r\n", gFuelData.Stat.FuelIndMode, xTaskGetTickCount(), s_lastSeg);
		}
#endif
    }
}

/**************************************************************************** 
Function:       FUEL_FillUpProc
Description:     
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void FUEL_FillUpProc(FUEL_PROC_STRU *pFuelData, FUEL_SAMPLE_DATA_STRU *pSampleData)
{
	pFuelData->ResValue = pSampleData->ResValue;
	pFuelData->ResValueDamp = pSampleData->ResValue;
	pFuelData->FuelVolNow = pSampleData->FuelVol;
	pFuelData->FuelVolDamp = pSampleData->FuelVol;
	pFuelData->IndicatePosTarget = pSampleData->IndicatePos;
	pFuelData->FuelFillUpCnt = 0;
	pFuelData->FuelVolPercent = CAL_FUEL_PERCENT(pSampleData->FuelVol);
	FUEL_CalFillUpFunc();
}

/**************************************************************************** 
Function:       FUEL_DampCalProc
Description:    
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void FUEL_DampCalProc(FUEL_PROC_STRU *pFuelData, FUEL_SAMPLE_DATA_STRU *pSampleData, float32_t damp_para)
{
	pFuelData->ResValue = pSampleData->ResValue;
	pFuelData->FuelVolNow = pSampleData->FuelVol;
	pFuelData->ResValueDamp = pFuelData->ResValueDamp + ((float32_t)pFuelData->ResValue-pFuelData->ResValueDamp)/damp_para;
	pFuelData->FuelVolDamp = lookTableTrans((uint16_t)pFuelData->ResValueDamp, FuelResToVolTable, GetFuelResToVolTableSize());
	pFuelData->IndicatePosTarget = lookTableTrans(pFuelData->FuelVolDamp, FuelVolToIndPos, GetFuelVolToIndPosSize());
    pFuelData->FuelVolPercent = CAL_FUEL_PERCENT(pFuelData->FuelVolDamp);
}

/**************************************************************************** 
Function:       FUEL_LinerDampCalProc
Description:    
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void FUEL_LinerDampCalProc(FUEL_PROC_STRU *pFuelData, FUEL_SAMPLE_DATA_STRU *pSampleData, float32_t damp_para)
{
	uint16_t l_ResValueDamp;
	pFuelData->ResValue = pSampleData->ResValue;
	pFuelData->FuelVolNow = pSampleData->FuelVol;
	l_ResValueDamp = pFuelData->ResValueDamp;
	if((l_ResValueDamp > pSampleData->ResValue) && ((l_ResValueDamp - pSampleData->ResValue) > damp_para))
	{
		pFuelData->ResValueDamp = pFuelData->ResValueDamp - damp_para;
	}
	else if((l_ResValueDamp < pSampleData->ResValue) && ((pSampleData->ResValue - l_ResValueDamp) > damp_para))
	{
		pFuelData->ResValueDamp = pFuelData->ResValueDamp + damp_para;
	}
	pFuelData->FuelVolDamp = lookTableTrans((uint16_t)pFuelData->ResValueDamp, FuelResToVolTable, GetFuelResToVolTableSize());
	pFuelData->IndicatePosTarget = lookTableTrans(pFuelData->FuelVolDamp, FuelVolToIndPos, GetFuelVolToIndPosSize());
    pFuelData->FuelVolPercent = CAL_FUEL_PERCENT(pFuelData->FuelVolDamp);
}

void FUEL_LowFuelProc(void)
{
	static uint8_t s_lowfuel_flag = 0;
	static uint16_t s_IgnOnCnt = 0;
	boolean l_boolValue = 0;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnOnCnt < 15*100)	//15S
		{
			 if(s_IgnOnCnt == 0)//yangxl 2020-5-9 14:23:00
		      {
		        Com_TxSig_FUEL_FirstW_L(COM_SET_SIG_VALUE, &l_boolValue);
		      }
			s_IgnOnCnt++;
			return;
		}
		
		if(FUEL_GetFuelSigStatus() != FuelSigNormal)	
		{
			if(s_lowfuel_flag != 2)	//for 591 project
			{
				s_lowfuel_flag = 2;

				IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_ON, ICON_OFF, ACOUSTIC_OFF);//TELLTALE_ON, ICON_ON, ACOUSTIC_OFF
				l_boolValue = 1;
				Com_TxSig_FUEL_FirstW_L(COM_SET_SIG_VALUE, &l_boolValue);
			}
		}
		else
		{
			if(FUEL_GetFuelLowStatus() != 0)
			{
				if(s_lowfuel_flag != 1)
				{
					s_lowfuel_flag = 1;

					IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);//TELLTALE_ON, ICON_ON, ACOUSTIC_OFF
					l_boolValue = 1;
					Com_TxSig_FUEL_FirstW_L(COM_SET_SIG_VALUE, &l_boolValue);
				}

			}
			else
			{
				if(s_lowfuel_flag != 0)
				{
					s_lowfuel_flag = 0;

					IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF
					l_boolValue = 0;
					Com_TxSig_FUEL_FirstW_L(COM_SET_SIG_VALUE, &l_boolValue);
				}
			}
		}

	}
	else
	{
		s_lowfuel_flag = 0;	//every time IGN_OFF -> IGN_ON,judge from the initial again
		s_IgnOnCnt = 0;
	}

}

uint8_t FUEL_CarMoveDetect(void)
{
	static uint8_t s_MoveDetectOnCnt = 0;
	static uint8_t s_MoveDetectOffCnt = 0;
	static uint8_t s_MoveDetectState = 0;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetRawVehicleSpeedCanOutput() >= FUEL_CAR_RUN_THRESHOLD)
		{
			s_MoveDetectOffCnt = 0;
			if(s_MoveDetectOnCnt < 5)
			{
				s_MoveDetectOnCnt++;
			}
			else
			{
				s_MoveDetectState = 1;
			}
		}
		else
		{
			s_MoveDetectOnCnt = 0;
			if(s_MoveDetectOffCnt < 5)
			{
				s_MoveDetectOffCnt++;
			}
			else
			{
				s_MoveDetectState = 0;
			}
		}
	}
	else
	{
		s_MoveDetectOnCnt = 0;
		s_MoveDetectState = 0;
	}
	return s_MoveDetectState;
}

/**************************************************************************** 
Function:       FuelProc
Description:    fuel module process func ,call cycle:10ms 
Input:          none
Output:         none
Return:         none
Others:         none
*****************************************************************************/
void FUEL_FuelProc(void)
{
	static uint16_t Cnt = 0;
	static uint16_t Sample0p5_Cnt = 0;  //0.5S sample data counter
	IGN_STAT_ENUM IgnStat = IGN_OFF;
	int l_FuelFillUpThreshold = FUEL_FILL_UP_THRESHOLD;
	
	Cnt++;
	
	if((s_InitFuelProcFlag == 0) && (DEA_GetSysRstType() == RST_IRRST))	//power on reset(BAT ON)
	{
		InitFuelModule();
#ifdef FUEL_DEBUG
		Cnt = 0;
		Sample0p5_Cnt = 0;
#endif
		s_InitFuelProcFlag = 1;
	}

	IgnStat = DEA_GetIgnStat();	//get IGN status
	
	if((gFuelData.CurCarStat != FUEL_PROC_SLEEP) && (Cnt%5 == 0))	//When fuel module was not in Sleep Status  per 50ms sample data once
	{
		(void)GetFuelSampleData();	//sample the fuel data 
	}

	if(gFuelData.Stat.SensorStat == 0)	//fuel sensor status normal
	{
		if(gFuelData.Stat.BatOnOrSigRecoverlag == 1)	//BAT ON or fuel sensor revover form fault
		{
			if((IgnStat == FUEL_IGN_OFF) && (gFuelData.CurCarStat == FUEL_PROC_SLEEP))
			{
				return;
			}
			else
			{
				gFuelData.CurCarStat = FUEL_PROC_IGN_OFF_TO_ON;	//jump to IGN_OFF to IGN ON status
				gFuelData.Stat.SleepEnable = 0;
			}
		}
        
		switch( gFuelData.CurCarStat )
		{
			case FUEL_PROC_IGN_OFF_TO_ON:	/* IGN OFF to IGN ON */
			{
				if(gFuelData.Stat.FuelIndSaved == 1)	//memerise indicate possition successful
				{
					gFuelData.IndicatePosTarget = gFuelData.IndicatePosSave;	//fast action to memerise indicate possition
					gFuelData.Stat.FuelIndSaved = 0;	//clear memerise data
				}
                gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_FAST;
				if(IgnStat == FUEL_IGN_ON)
				{
					if(gFuelData.Stat.BatOnOrSigRecoverlag == 1)	//BAT ON or fuel sensor revover form fault
					{
						if(gFuelData.SampleData0p5.FinishFlag == 1)
						{
							if(Sample0p5_Cnt < FUEL_IGN_ON_DROP_CYCLE)	//throw the first time 0.5S sampled data
							{
								Sample0p5_Cnt++;
							}
							else
							{
								FUEL_FillUpProc(&gFuelData, &gFuelData.SampleData0p5);
#ifdef IPC_FUEL_STAT_REPORT
								IPC_UpdateFuelState(1, gFuelData.SampleData0p5.FuelVol, gFuelData.FuelVolSave);
#endif								
								gFuelData.Stat.FuelFillUpFlag = 1;	/* set fuel fill up flag */
								gFuelData.CurCarStat = FUEL_PROC_ING_ON;							
								gFuelData.FuelVolSave = gFuelData.SampleData0p5.FuelVol;	//memory Fuel level
								gFuelData.Stat.FuelVolSaved = 1;
                                gFuelData.Stat.BatOnOrSigRecoverlag = 0;
								gFuelData.Stat.FuelVolumeValid = 1;
							}
							gFuelData.SampleData0p5.FinishFlag = 0;
						}
					}
					else
					{
						if(gFuelData.Stat.FuelVolSaved == 1)	//memory fuel volume finish
						{
							if(gFuelData.SampleData0p5.FinishFlag == 1)
							{
								if(Sample0p5_Cnt < FUEL_IGN_ON_DROP_CYCLE)	//throw the first time 0.5S sampled data
								{
									Sample0p5_Cnt++;
								}
								else
								{
									if(abs(gFuelData.SampleData0p5.FuelVol - gFuelData.FuelVolSave) >= FUEL_FILL_UP_THRESHOLD)
									{
										FUEL_FillUpProc(&gFuelData, &gFuelData.SampleData0p5);	
#ifdef IPC_FUEL_STAT_REPORT
										IPC_UpdateFuelState(1, gFuelData.SampleData0p5.FuelVol, gFuelData.FuelVolSave);
#endif
										gFuelData.Stat.FuelFillUpFlag = 1;	/* set fuel fill up flag */
										gFuelData.CurCarStat = FUEL_PROC_ING_ON;
									}
									else
									{	
#ifdef IPC_FUEL_STAT_REPORT
										IPC_UpdateFuelState(0, gFuelData.SampleData0p5.FuelVol, gFuelData.FuelVolSave);
#endif
										gFuelData.FuelFillUpCnt = 0;
										gFuelData.CurCarStat = FUEL_PROC_ING_ON;
									}
								}
                                gFuelData.SampleData0p5.FinishFlag = 0;
							}
						}
						else	//Incomplete memory Fuel level
						{
							gFuelData.FuelFillUpCnt = 0;
							gFuelData.CurCarStat = FUEL_PROC_ING_ON;
#ifdef IPC_FUEL_STAT_REPORT
							IPC_UpdateFuelState(2, 0, 0);
#endif
						}
					}
				}
				else
				{
					gFuelData.CurCarStat = FUEL_PROC_IGN_ON_TO_OFF;	//jump to IGN OFF Status
#ifdef IPC_FUEL_STAT_REPORT
					IPC_UpdateFuelState(3, gFuelData.SampleData0p5.FuelVol, gFuelData.FuelVolSave);
#endif
				}
				gFuelData.LastCarStat = FUEL_PROC_IGN_OFF_TO_ON;
				break;
			}
			case FUEL_PROC_ING_ON:	/* IGN ON and park */	
			{
				if(IgnStat == FUEL_IGN_ON)
				{
					if(FUEL_CarMoveDetect() == 0)	//speed Less than FUEL_CAR_RUN_THRESHOLD km/h 
					{
						if(gFuelData.Stat.FuelVolSaved == 1)	//Complete memory Fuel level
						{
							if(gFuelData.Stat.IgnOnFuelFillUpFlag == 1)
							{
								if(gFuelData.SampleDataMid.FinishFlag == 1)
								{
									FUEL_FillUpProc(&gFuelData, &gFuelData.SampleData0p5);
									gFuelData.SampleDataMid.FinishFlag = 0;
								}
							}
							else
							{
								if(gFuelData.SampleDataMid.FinishFlag == 1)
								{
									//for 591, if IGN OFF Volume saved, using IGN OFF threshold.
									if(gFuelData.Stat.FuelIgnOffVolSaved)
									{
										l_FuelFillUpThreshold = FUEL_FILL_UP_THRESHOLD;
									}
									else
									{
										l_FuelFillUpThreshold = FUEL_FILL_UP_THRESHOLD_ON;
									}
									
									if(abs(gFuelData.SampleData0p5.FuelVol - gFuelData.FuelVolSave) > l_FuelFillUpThreshold)
									{
										if(gFuelData.FuelFillUpCnt == 0)
										{
											gFuelData.FuelFillUpCnt++;
										}
										else
										{	
											//5) If meter judged twice continuation that ("Input fuel volume at IGN OFF to ON"(c)) is greater than or equals to ("fuel volume after IGN OFF"(b) + Frf1), fill up occur
											FUEL_FillUpProc(&gFuelData, &gFuelData.SampleData0p5);
#ifdef IPC_FUEL_STAT_REPORT
											IPC_UpdateFuelState(4, gFuelData.SampleData0p5.FuelVol, gFuelData.FuelVolSave);
#endif

											gFuelData.Stat.FuelFillUpFlag = 1;	/* set fuel fillup flag */
											gFuelData.Stat.IgnOnFuelFillUpFlag = 1; /* set fuel fillup at ign on moment flag */
										}
									}
                                    else
                                    {
                                    	gFuelData.FuelFillUpCnt = 0;
            							//if(GetRpmValue() > 100)	//In Idle speed status, need to Calculate the idle speed fuel consumption
            							//{
												FUEL_LinerDampCalProc(&gFuelData, &gFuelData.SampleDataMid, FUEL_IDLE_DAMP_PARA);
            							//}
                                    }
									gFuelData.SampleDataMid.FinishFlag = 0;
								}
							}
						}
						else	//Incomplete memory Fuel level
						{
							if(gFuelData.SampleDataMid.FinishFlag == 1)
							{
								if(gFuelData.FuelVolSaveTimeCnt < 3)	//METER detects input vehicle speed = 0km/h for 20sec continuously
								{
									gFuelData.FuelVolSaveTimeCnt++;
								}
								else
								{
									gFuelData.FuelVolSave = gFuelData.SampleData0p5.FuelVol;	//memory Fuel level
									gFuelData.Stat.FuelVolSaved = 1;
									//gFuelData.Stat.IgnOnFuelSaveReqFlag = 0;
								}
								gFuelData.SampleDataMid.FinishFlag = 0;
							}
						}
					}
					else
					{
						gFuelData.Stat.FuelFillUpFlag = 0;	//quit fuel fillup status
						gFuelData.Stat.IgnOnFuelFillUpFlag = 0;
						gFuelData.CurCarStat = FUEL_PROC_CAR_RUN;	//jump to car driving status
					}
				}
				else
				{
					gFuelData.Stat.FuelFillUpFlag = 0;		//quit fuel fillup status
					gFuelData.CurCarStat = FUEL_PROC_IGN_ON_TO_OFF;	//jump to IGN OFF status
					gFuelData.Stat.IgnOnFuelFillUpFlag = 0;
				}
                gFuelData.LastCarStat = FUEL_PROC_ING_ON;
				break;
			}
			case FUEL_PROC_CAR_RUN:	/* car driving status */	
			{
                gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_SLOW;
				if(FUEL_CarMoveDetect() != 0) //speed More than 1 km/h 
				{
					if(gFuelData.SampleDataMid.FinishFlag == 1)
					{
						FUEL_LinerDampCalProc(&gFuelData, &gFuelData.SampleDataMid, FUEL_CAR_RUN_DAMP_PARA);
						gFuelData.SampleDataMid.FinishFlag = 0;
					}
				}
				else
				{
					gFuelData.CurCarStat = FUEL_PROC_CAR_RUN_TO_STOP;
				}
                gFuelData.LastCarStat = FUEL_PROC_CAR_RUN;
				break;
			}
			case FUEL_PROC_CAR_RUN_TO_STOP:	/* car driving status to stop */
			{
				gFuelData.Stat.FuelVolSaved = 0;	//reset complete memory fuel level flag		
				gFuelData.Stat.FuelIgnOffVolSaved = 0;	//reset complete memory fuel level flag
				gFuelData.FuelVolSaveTimeCnt = 0;
				//gFuelData.Stat.IgnOnFuelSaveReqFlag = 1;
				
				gFuelData.SampleDataMid.FinishFlag = 0;	//reset Mid sample data, Start a new sampling process
				SampleDataReset(&gFuelData.SampleDataMid.SampleData);

                gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_FAST;
				gFuelData.FuelFillUpCnt = 0;
				gFuelData.CurCarStat = FUEL_PROC_ING_ON;
                gFuelData.LastCarStat = FUEL_PROC_CAR_RUN_TO_STOP;
				break;
			}
			case FUEL_PROC_IGN_ON_TO_OFF:	/* IGN ON to IGN OFF */
			{
                if(gFuelData.Stat.BatOnOrSigRecoverlag == 1) //direct jump to SLEEP status
                {
                    Sample0p5_Cnt = 0;
                    gFuelData.CurCarStat = FUEL_PROC_SLEEP;
                    gFuelData.LastCarStat = FUEL_PROC_IGN_ON_TO_OFF;
                }
				else
                {
                    if(gFuelData.Stat.FuelIndSaved == 0)
    				{
						gFuelData.IndicatePosSave = gFuelData.IndicatePosTarget;  //memory current indicate position
						gFuelData.IndicatePosTarget = 0;                          //reset current indicate position
    					gFuelData.Stat.FuelIndSaved = 1;    
    				}
					gFuelData.Stat.FuelIgnOffVolSaved = 0;	//reset complete memory fuel level flag
					gFuelData.FuelVolSaveTimeCnt = 0;
					gFuelData.SampleDataMid.FinishFlag = 0; //reset Mid sample data, Start a new sampling process
    				gFuelData.CurCarStat = FUEL_PROC_PRESLEEP;
                    gFuelData.LastCarStat = FUEL_PROC_IGN_ON_TO_OFF;
                }
				break;
			}
			case FUEL_PROC_PRESLEEP:	/* Presleep mode */
			{
				if(IgnStat == FUEL_IGN_ON)
				{
					Sample0p5_Cnt = 0;
					gFuelData.CurCarStat = FUEL_PROC_IGN_OFF_TO_ON;
				}
				else
				{
					if(gFuelData.Stat.FuelIgnOffVolSaved == 1)
					{
						//jump to sleep mode
						gFuelData.CurCarStat = FUEL_PROC_SLEEP;
					}
					else
					{
						if(gFuelData.SampleDataMid.FinishFlag == 1)
						{
							if(gFuelData.FuelVolSaveTimeCnt < 1)
							{
								gFuelData.FuelVolSaveTimeCnt++;
							}
							else
							{
								gFuelData.FuelVolSave = gFuelData.SampleDataMid.FuelVol;	//memory sampled fuel level
								gFuelData.Stat.FuelVolSaved = 1;
								gFuelData.Stat.FuelIgnOffVolSaved = 1;

								//IgnOnFuelSaveReqFlag is not needed when Fuel voume saved at IGN  OFF
								//gFuelData.Stat.IgnOnFuelSaveReqFlag = 0;
							}
							gFuelData.SampleDataMid.FinishFlag = 0;
						}
					}			
				}
                gFuelData.LastCarStat = FUEL_PROC_PRESLEEP;
				break;
			}
			case FUEL_PROC_SLEEP:	/* Sleep Mode */
			{
				if(IgnStat == FUEL_IGN_ON)
				{
					gFuelData.Stat.SleepEnable = 0;
					SampleDataReset(&gFuelData.SampleData0p5.SampleData);
					gFuelData.SampleData0p5.FinishFlag = 0;
					SampleDataReset(&gFuelData.SampleDataMid.SampleData);
					gFuelData.SampleDataMid.FinishFlag = 0;
					Sample0p5_Cnt = 0;
					gFuelData.CurCarStat = FUEL_PROC_IGN_OFF_TO_ON;	//IGN OFF to IGN ON
				}
				else
				{
					gFuelData.Stat.SleepEnable = 1;
				}
                gFuelData.LastCarStat = FUEL_PROC_SLEEP;
				break;
			}
			default:
				break;
		}

		//low fuel cheak handle
		if(gFuelData.Stat.SensorStat == 0 && gFuelData.Stat.FuelVolumeValid)	//senser stat normal & fuel value valid
        {	
	        //low fuel detect  
	        if(gFuelData.Stat.LowFuel == 0) //low fuel normal state
	        {
	            if(gFuelData.FuelVolDamp < FUEL_LOW_VOL) //10.4L
	            {
	                gFuelData.Stat.LowFuel = 1; //set the low fuel flag
	            }
	        }
	        else 	//low fuel warn state
	        {
	            if(gFuelData.FuelVolDamp > FUEL_LOW_RELIEVE_VOL) //13L,
	            {
	                gFuelData.Stat.LowFuel = 0; //clear the low fuel flag
	            }
				else
				{
					gFuelData.Stat.LowFuel = 1; //set the low fuel flag
				}
	        }
			
		}
		else
		{
			 gFuelData.Stat.LowFuel = 0;
		}
	}
	else    //fuel sensor signal fault
	{  
		if(gFuelData.Stat.BatOnOrSigRecoverlag != 1)
		{
			gFuelData.FuelVolNow = 0;
			gFuelData.FuelVolDamp = 0;
			gFuelData.FuelVolPercent = 0;
			gFuelData.IndicatePosTarget = 0;
			gFuelData.ResValue = 0;
			gFuelData.ResValueDamp = 0;
            gFuelData.Stat.LowFuel = 0;
			gFuelData.Stat.FuelIndMode = FUEL_IND_MODE_FAST;
			SampleDataReset(&gFuelData.SampleData0p5.SampleData);
			gFuelData.SampleData0p5.FinishFlag = 0;
			SampleDataReset(&gFuelData.SampleDataMid.SampleData);
			gFuelData.SampleDataMid.FinishFlag = 0;
			gFuelData.Stat.BatOnOrSigRecoverlag = 1;
			gFuelData.Stat.FuelVolumeValid = 0;
			gFuelData.Stat.IgnOnFuelFillUpFlag = 0;
			//gFuelData.Stat.IgnOnFuelSaveReqFlag = 0;
		}

		if(IgnStat == FUEL_IGN_OFF)
        {
            gFuelData.CurCarStat = FUEL_PROC_SLEEP;
            gFuelData.Stat.SleepEnable = 1;
        }
        else
        {
            Sample0p5_Cnt = 0;
            gFuelData.CurCarStat = FUEL_PROC_IGN_OFF_TO_ON;
            gFuelData.Stat.SleepEnable = 0;
        }
	}
	
	FuelDispProc();	//indication of fuel vol,the fuel display 

	FUEL_LowFuelProc();	//low fuel vol Alarm
}

void FUEL_vInit(void)
{
	s_FuelCircuitVersion = getCircuitVerion();
}
