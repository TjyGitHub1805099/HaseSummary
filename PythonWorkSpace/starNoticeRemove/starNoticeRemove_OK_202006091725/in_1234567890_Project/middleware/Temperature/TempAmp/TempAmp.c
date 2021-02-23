/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: TempAmp.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-01-16 15:55:25
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#include "TempAmp.h"
#include "TempAmp_Cfg.h"
#include "TempConvert.h"
#include "System_Signal.h"
#if (STD_ON == TEMPAMP_DEBUG_TRACE)
#include "trace.h"
#endif

typedef struct
{
    boolean Init;
    uint8 Status;
} TempAmp_ManageType;

typedef struct
{
    uint16 Start;
    uint16 End;
} TempAmp_VoltRangeType;

TempAmp_ManageType TempAmp_Manager =
{
    STD_OFF, TEMPAMP_STATE_NORMAL,
};

TempAmp_VoltRangeType TempAmp_VoltRange[TEMPAMP_STATE_MAX] =
{
    {TEMPAMP_VOLTRANGE_ULTRALOW_START, TEMPAMP_VOLTRANGE_ULTRALOW_END},
    {TEMPAMP_VOLTRANGE_LOW_START, TEMPAMP_VOLTRANGE_LOW_END},
    {TEMPAMP_VOLTRANGE_NORMAL_START, TEMPAMP_VOLTRANGE_NORMAL_END},
    {TEMPAMP_VOLTRANGE_HIGH_START, TEMPAMP_VOLTRANGE_HIGH_END},
    {TEMPAMP_VOLTRANGE_ULTRAHIGH_START, TEMPAMP_VOLTRANGE_ULTRAHIGH_END}
};

/*
*@par Service ID[hex]
*0X3
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Service to get the temperature value;
*/
uint8 TempAmp_GetValue(sint16 *TempValue)
{
    uint8 l_returnValue = E_NOT_OK;
    uint16 l_adcValue = 0u;

    if (STD_ON == TempAmp_Manager.Init)
    {
        //l_returnValue = Adc_Read(AD_CHANNEL_AMP_TEMP, &l_adcValue);
        if (E_OK == l_returnValue)
        {
            *TempValue = Temp_Convert(l_adcValue);
        }
    }
    return (l_returnValue);
}

static uint8 TempAmp_GetInternalState(uint8 * state)
{
    uint8 l_returnValue = E_NOT_OK;
    sint16 l_ampValue = 0;

    if (STD_ON == TempAmp_Manager.Init)
    {
        if (E_OK == TempAmp_GetValue(&l_ampValue))
        {
            if ((l_ampValue <= TempAmp_VoltRange[TempAmp_Manager.Status].Start)
                    || (l_ampValue >= TempAmp_VoltRange[TempAmp_Manager.Status].End))
            {
                if (l_ampValue < TEMPAMP_THRESHOLD_ULTRALOW)
                {
                    *state = TEMPAMP_STATE_ULTRALOW;
                }
                else if (l_ampValue < TEMPAMP_THRESHOLD_LOW)
                {
                    *state = TEMPAMP_STATE_LOW;
                }
                else if (l_ampValue < TEMPAMP_THRESHOLD_HIGH)
                {
                    *state = TEMPAMP_STATE_NORMAL;
                }
                else if (l_ampValue < TEMPAMP_THRESHOLD_ULTRAHIGH)
                {
                    *state = TEMPAMP_STATE_HIGH;
                }
                else
                {
                    *state = TEMPAMP_STATE_ULTRAHIGH;
                }
                l_returnValue = E_OK;
            }
        }
    }
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0X0
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Service to initialize the module;
*You must call this function first before you call the others.
*/
void TempAmp_Init(void)
{
    if (STD_OFF == TempAmp_Manager.Init)
    {
        TempAmp_Manager.Init = STD_ON;
    }
}

/*
*@par Service ID[hex]
*0X1
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Scanf the signal periodically and do the debouncer.
*/
void TempAmp_Scan(void)
{
    uint8 l_portState;
    static uint8 l_sampleCounter = TEMPAMP_FILTER_TIME;

    if (STD_ON == TempAmp_Manager.Init)
    {
        if (E_OK == TempAmp_GetInternalState(&l_portState))
        {
            if (0 != l_sampleCounter)
            {
                l_sampleCounter--;
            }
            if ((0 == l_sampleCounter) && (TempAmp_Manager.Status != l_portState))
            {
                TempAmp_Manager.Status = l_portState;
                System_Signal_Send(SYSTEM_SIG_ID_TEMPAMP_STATUS, TempAmp_Manager.Status, STD_OFF);
#if (STD_ON == TEMPAMP_DEBUG_TRACE)
                TracePrintf(SYSTEM_TRACE, TRACE_INFO, "[Temp-I]>Amp State Changed:%d\n\r", TempAmp_Manager.Status);
#endif
            }
            if (l_portState == TempAmp_Manager.Status)
            {
                l_sampleCounter = TEMPAMP_FILTER_TIME;
            }
        }
    }
}

/*
*@par Service ID[hex]
*0X2
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval TEMPAMP_STATE_ULTRALOW: Amp voltage is ultra low.
*@retval TEMPAMP_STATE_LOW: Amp voltage is low.
*@retval TEMPAMP_STATE_NORMAL: Amp voltage is normal.
*@retval TEMPAMP_STATE_HIGH: Amp voltage is high.
*@retval TEMPAMP_STATE_ULTRAHIGH: Amp voltage is ultra high.
*
*Call this function to register user callback.
*/
uint8 TempAmp_GetStatus(void)
{
    return (TempAmp_Manager.Status);
}

/*
*@par Service ID[hex]
*0X4
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Service to initialize the module;
*You must call this function first before you call the others.
*/
void TempAmp_Deinit(void)
{
    if (STD_ON == TempAmp_Manager.Init)
    {
        //CALL ADC API
        TempAmp_Manager.Init = STD_OFF;
    }
}