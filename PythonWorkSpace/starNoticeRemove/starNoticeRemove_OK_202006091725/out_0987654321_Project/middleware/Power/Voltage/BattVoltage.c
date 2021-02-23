

#include "BattVoltage.h"
#include "BattVoltage_Cfg.h"
#include "System_Signal.h"
#include "adc_wrapper_if.h"
#if(STD_ON == BATTVOLT_DEBUG_TRACE)
#include "trace.h"
#endif
#include "alga.h"

typedef struct
{
    uint16 Start;
    uint16 End;
} BattVolt_VoltRangeType;

typedef struct
{
    uint16 Value;
    uint16 Step;
} BattVolt_DevValue;

typedef struct
{
    boolean Init;
    uint8 Status;
	uint16 	BattVoltageVlu;
} BattVolt_ManageType;

static BattVolt_ManageType BattVolt_Manager =
{
    STD_OFF, BATTVOLT_STATE_NORMAL,0
};

static SAMPLE_DATA_STRU BattVoltValue = {0,0,0,0,0};

static const BattVolt_VoltRangeType BattVolt_VoltRange[BATTVOLT_STATE_MAX] =
{
    {BATTVOLT_VOLTRANGE_ULTRALOW_START, BATTVOLT_VOLTRANGE_ULTRALOW_END},
    {BATTVOLT_VOLTRANGE_LOW_START, BATTVOLT_VOLTRANGE_LOW_END},
    {BATTVOLT_VOLTRANGE_NORMAL_START, BATTVOLT_VOLTRANGE_NORMAL_END},
    {BATTVOLT_VOLTRANGE_HIGH_START, BATTVOLT_VOLTRANGE_HIGH_END},
    {BATTVOLT_VOLTRANGE_ULTRAHIGH_START, BATTVOLT_VOLTRANGE_ULTRAHIGH_END}
};

const BattVolt_DevValue Up12V_Step[7]=
{
	{1900,19},
	{1800,17},
	{1700,15},
	{1600,10},
	{1500,9},
	{1400,6},
	{1300,4},
};

const BattVolt_DevValue Down12V_Step[6]=
{
	{1100,1},
	{1000,2},
	{900,5},
	{800,8},
	{700,12},
	{663,13},
};

uint16 TestVolValue;
Std_ReturnType BattVolt_GetVoltage(uint16 *VoltageValue)
{
    uint16 l_adcValue = 0u;
    Std_ReturnType l_returnValue = E_NOT_OK;
    uint16 l_tempValue=0u;
    
    
	
    if (STD_ON == BattVolt_Manager.Init)
    {
		if(AdcWrapper_Read(ADC_LOGIC_CH_UDBATT_AD,&l_adcValue) == TRUE)
		{
			l_tempValue = BATTVOLT_CONVERT_ALGO(l_adcValue);
			#if 1
			*VoltageValue = l_tempValue;
			BattVolt_Manager.BattVoltageVlu = *VoltageValue;
			l_returnValue = E_OK;
			#else
			SampleDataAdd(&BattVoltValue, l_tempValue);
			if(BattVoltValue.Cnt > 4)
			{
				l_tempValue1  = CalAvgSampleData(&BattVoltValue);
				SampleDataReset(&BattVoltValue);
				#if   1
				if(l_tempValue1 > 1250)
				{
					for(i=0;i<7;i++)
					{
						if(l_tempValue>=Up12V_Step[i].Value)
						{
							*VoltageValue = l_tempValue-Up12V_Step[i].Step;
							break;
						}
						*VoltageValue = l_tempValue-Up12V_Step[i].Step;
					}
				}
				else if(l_tempValue1 < 1190)
				{
					for(i=0;i<6;i++)
					{
						if(l_tempValue>=Down12V_Step[i].Value)
						{
							*VoltageValue = l_tempValue+Down12V_Step[i].Step;
							break;
						}
						*VoltageValue = l_tempValue+Down12V_Step[i].Step;
					}
				}
				else
				{
					*VoltageValue = l_tempValue;
				}
				#endif
								BattVolt_Manager.BattVoltageVlu = *VoltageValue;
				l_returnValue = E_OK;
			}
			#endif
		}
    }
    return (l_returnValue);
}

Std_ReturnType BattVolt_GetRawValue(uint16 *batVol)
{
	Std_ReturnType ret = E_NOT_OK;
	uint16 batVolRawValue = 0;

	if(batVol)
	{
		if(AdcWrapper_Read(ADC_LOGIC_CH_UDBATT_AD,&batVolRawValue) == TRUE)
		{
			*batVol = BATTVOLT_CONVERT_ALGO(batVolRawValue);
			ret = E_OK;
		}
	}
	return ret;
}


Std_ReturnType BattVolt_GetInternalState(uint8 * state)
{
    Std_ReturnType l_returnValue = E_NOT_OK;
    static uint16 l_battValue = 0;
	uint8 batState = BATTVOLT_STATE_NORMAL;

    if (STD_ON == BattVolt_Manager.Init && state)
    {
        if (E_OK == BattVolt_GetVoltage(&l_battValue))
        {
            if ((l_battValue <= BattVolt_VoltRange[BattVolt_Manager.Status].Start)
                    || (l_battValue >= BattVolt_VoltRange[BattVolt_Manager.Status].End))
            {
                if (l_battValue < BATTVOLT_THRESHOLD_ULTRALOW)
                {
                    batState = BATTVOLT_STATE_ULTRALOW;
                }
                else if (l_battValue < BATTVOLT_THRESHOLD_LOW)
                {
                    batState = BATTVOLT_STATE_LOW;
                }
                else if (l_battValue < BATTVOLT_THRESHOLD_HIGH)
                {
                    batState = BATTVOLT_STATE_NORMAL;
                }
                else if (l_battValue < BATTVOLT_THRESHOLD_ULTRAHIGH)
                {
                    batState = BATTVOLT_STATE_HIGH;
                }
                else
                {
                    batState = BATTVOLT_STATE_ULTRAHIGH;
                }
				
				*state = batState;
                l_returnValue = E_OK;
            }
        }
    }
    return (l_returnValue);
}


void BattVolt_Init(void)
{
    if (STD_OFF == BattVolt_Manager.Init)
    {
        BattVolt_Manager.Init = STD_ON;
	 SampleDataReset(&BattVoltValue);
    }
}


uint16 BattVolt_Scan(void)
{
    uint8 l_portState;
    boolean l_TxState;
    static uint8 l_sampleCounter = BATTVOLT_FILTER_TIME;

    if (STD_ON == BattVolt_Manager.Init)
    {
        if (E_OK == BattVolt_GetInternalState(&l_portState))
        {
            if (0 != l_sampleCounter)
            {
                l_sampleCounter--;
            }
            if ((0 == l_sampleCounter) && (BattVolt_Manager.Status != l_portState))
            {
                BattVolt_Manager.Status = l_portState;
                l_TxState = System_Signal_Send(SYSTEM_SIG_ID_BATTVOLT_STATUS, BattVolt_Manager.Status, STD_OFF);
#if (STD_ON == BATTVOLT_DEBUG_TRACE)
                TracePrintf(SYSTEM_TRACE, TRACE_INFO, "[Power-I]>Batt Voltage Changed:%d,Tx:%d\r\n", BattVolt_Manager.Status, l_TxState);
#endif
            }
            if (l_portState == BattVolt_Manager.Status)
            {
                l_sampleCounter = BATTVOLT_FILTER_TIME;
            }
        }
    }
	return BattVolt_Manager.BattVoltageVlu ;
}


uint8 BattVolt_GetStatus(void)
{
    return (BattVolt_Manager.Status);
}


void BattVolt_Deinit(void)
{
    if (STD_ON == BattVolt_Manager.Init)
    {
        
        BattVolt_Manager.Init = STD_OFF;
    }
}



