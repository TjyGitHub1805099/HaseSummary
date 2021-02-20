/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: TempBoard.c
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
#include "TempBoard.h"
#include "TempBoard_Cfg.h"
#include "TempConvert.h"
#include "System_Signal.h"
#if (STD_ON == TEMPBOARD_DEBUG_TRACE)
#include "trace.h"
#endif

typedef struct
{
    boolean Init;
    uint8 Status;
} TempBoard_ManageType;

typedef struct
{
    uint16 Start;
    uint16 End;
} TempBoard_VoltRangeType;

TempBoard_ManageType TempBoard_Manager =
{
    STD_OFF, TEMPBOARD_STATE_NORMAL,
};

TempBoard_VoltRangeType TempBoard_VoltRange[TEMPBOARD_STATE_MAX] =
{
    {TEMPBOARD_VOLTRANGE_ULTRALOW_START, TEMPBOARD_VOLTRANGE_ULTRALOW_END},
    {TEMPBOARD_VOLTRANGE_LOW_START, TEMPBOARD_VOLTRANGE_LOW_END},
    {TEMPBOARD_VOLTRANGE_NORMAL_START, TEMPBOARD_VOLTRANGE_NORMAL_END},
    {TEMPBOARD_VOLTRANGE_HIGH_START, TEMPBOARD_VOLTRANGE_HIGH_END},
    {TEMPBOARD_VOLTRANGE_ULTRAHIGH_START, TEMPBOARD_VOLTRANGE_ULTRAHIGH_END}
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
*@param[out] TempValue
*
*@retval E_OK/E_NOT_OK
*
*Service to get the temperature value;
*/
uint8 TempBoard_GetValue(sint16 *TempValue)
{
    uint8 l_returnValue = E_NOT_OK;
    uint16 l_adcValue = 0u;

    if (STD_ON == TempBoard_Manager.Init)
    {
        // l_returnValue = Adc_Read(AD_CHANNEL_MAINBOARD_TEMP, &l_adcValue);
        if (E_OK == l_returnValue)
        {
            *TempValue = Temp_Convert(l_adcValue);
        }
    }
    return (l_returnValue);
}

static uint8 TempBoard_GetInternalState(uint8 * state)
{
    uint8 l_returnValue = E_NOT_OK;
    sint16 l_mainboardValue = 0;

    if (STD_ON == TempBoard_Manager.Init)
    {
        if (E_OK == TempBoard_GetValue(&l_mainboardValue))
        {
            if ((l_mainboardValue <= TempBoard_VoltRange[TempBoard_Manager.Status].Start)
                    || (l_mainboardValue >= TempBoard_VoltRange[TempBoard_Manager.Status].End))
            {
                if (l_mainboardValue < TEMPBOARD_THRESHOLD_ULTRALOW)
                {
                    *state = TEMPBOARD_STATE_ULTRALOW;
                }
                else if (l_mainboardValue < TEMPBOARD_THRESHOLD_LOW)
                {
                    *state = TEMPBOARD_STATE_LOW;
                }
                else if (l_mainboardValue < TEMPBOARD_THRESHOLD_HIGH)
                {
                    *state = TEMPBOARD_STATE_NORMAL;
                }
                else if (l_mainboardValue < TEMPBOARD_THRESHOLD_ULTRAHIGH)
                {
                    *state = TEMPBOARD_STATE_HIGH;
                }
                else
                {
                    *state = TEMPBOARD_STATE_ULTRAHIGH;
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
void TempBoard_Init(void)
{
    if (STD_OFF == TempBoard_Manager.Init)
    {
        TempBoard_Manager.Init = STD_ON;
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
void TempBoard_Scan(void)
{
    uint8 l_portState;
    static uint8 l_sampleCounter = TEMPBOARD_FILTER_TIME;

    if (STD_ON == TempBoard_Manager.Init)
    {
        if (E_OK == TempBoard_GetInternalState(&l_portState))
        {
            if (0 != l_sampleCounter)
            {
                l_sampleCounter--;
            }
            if ((0 == l_sampleCounter) && (TempBoard_Manager.Status != l_portState))
            {
                TempBoard_Manager.Status = l_portState;
                System_Signal_Send(SYSTEM_SIG_ID_TEMPBOARD_STATUS, TempBoard_Manager.Status, STD_OFF);
#if (STD_ON == TEMPBOARD_DEBUG_TRACE)
                TracePrintf(SYSTEM_TRACE, TRACE_INFO, "[Temp-I]>Mainboard State Changed:%d\n\r", TempBoard_Manager.Status);
#endif
            }
            if (l_portState == TempBoard_Manager.Status)
            {
                l_sampleCounter = TEMPBOARD_FILTER_TIME;
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
*@retval TEMPBOARD_STATE_ULTRALOW: Mainboard voltage is ultra low.
*@retval TEMPBOARD_STATE_LOW: Mainboard voltage is low.
*@retval TEMPBOARD_STATE_NORMAL: Mainboard voltage is normal.
*@retval TEMPBOARD_STATE_HIGH: Mainboard voltage is high.
*@retval TEMPBOARD_STATE_ULTRAHIGH: Mainboard voltage is ultra high.
*
*Call this function to register user callback.
*/
uint8 TempBoard_GetStatus(void)
{
    return (TempBoard_Manager.Status);
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
*Service to deinitialize the module;
*/
void TempBoard_Deinit(void)
{
    if (STD_ON == TempBoard_Manager.Init)
    {
        TempBoard_Manager.Init = STD_OFF;
    }
}