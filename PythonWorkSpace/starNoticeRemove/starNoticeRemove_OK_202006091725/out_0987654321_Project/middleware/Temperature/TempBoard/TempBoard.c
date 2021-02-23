

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


uint8 TempBoard_GetValue(sint16 *TempValue)
{
    uint8 l_returnValue = E_NOT_OK;
    uint16 l_adcValue = 0u;

    if (STD_ON == TempBoard_Manager.Init)
    {
        
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


void TempBoard_Init(void)
{
    if (STD_OFF == TempBoard_Manager.Init)
    {
        TempBoard_Manager.Init = STD_ON;
    }
}


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


uint8 TempBoard_GetStatus(void)
{
    return (TempBoard_Manager.Status);
}



void TempBoard_Deinit(void)
{
    if (STD_ON == TempBoard_Manager.Init)
    {
        TempBoard_Manager.Init = STD_OFF;
    }
}
