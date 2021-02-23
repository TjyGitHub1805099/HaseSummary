

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


uint8 TempAmp_GetValue(sint16 *TempValue)
{
    uint8 l_returnValue = E_NOT_OK;
    uint16 l_adcValue = 0u;

    if (STD_ON == TempAmp_Manager.Init)
    {
        
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


void TempAmp_Init(void)
{
    if (STD_OFF == TempAmp_Manager.Init)
    {
        TempAmp_Manager.Init = STD_ON;
    }
}


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


uint8 TempAmp_GetStatus(void)
{
    return (TempAmp_Manager.Status);
}


void TempAmp_Deinit(void)
{
    if (STD_ON == TempAmp_Manager.Init)
    {
        
        TempAmp_Manager.Init = STD_OFF;
    }
}
