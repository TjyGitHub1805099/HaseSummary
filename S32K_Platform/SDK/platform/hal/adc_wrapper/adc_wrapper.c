#include "adc_wrapper_if.h"
#include "trace.h"
#ifdef PLATFORM_S32K_FOR_ADC
#include "adc_pal.h"
#include "adc0_pal_cfg.h"
#include "string.h"
#endif

static boolean sAdcInitDoneFlag = FALSE;
static uint16 sAdcChResult[ADC_CH_MAX] = {0};

boolean AdcWrapper_Init(uint8 adcInstanceIndex,void* adcCfg)
{
	if(adcCfg==NULL || adcInstanceIndex>=ADC_NUM_MAX)
		return FALSE;
#ifdef PLATFORM_S32K_FOR_ADC
	memset(sAdcChResult,0,sizeof(sAdcChResult));
	if(ADC_Init(adcInstanceIndex,(const adc_config_t * const)adcCfg) != STATUS_SUCCESS)
		return FALSE;
	else
	{
		sAdcInitDoneFlag = TRUE;
		return TRUE;
	}
#else
		return FALSE;
#endif
}

boolean AdcWrapper_StartConv(uint8 adcInstanceIndex)
{
	if(adcInstanceIndex>=ADC_NUM_MAX || sAdcInitDoneFlag == FALSE)
		return FALSE;

#ifdef PLATFORM_S32K_FOR_ADC
	if(ADC_StartGroupConversion(adcInstanceIndex,0) != STATUS_SUCCESS)
		return FALSE;
	else
		return TRUE;
#else
	return FALSE;
#endif
}

boolean AdcWrapper_ConvLoop(uint8 adcInstanceIndex)
{
	uint8 reg_ch_index = 0;
	boolean adcConvDoneFlag = FALSE;
	
	if(sAdcInitDoneFlag == FALSE ||adcInstanceIndex>=ADC_NUM_MAX)
		return FALSE;

#ifdef PLATFORM_S32K_FOR_ADC
		if(ADC_StartGroupConversion(adcInstanceIndex,0) != STATUS_SUCCESS)
			return FALSE;

		// CAUTION: for S32K, if use pdb0 ch1 as pretrigger,adc ch must be started from ch 0,ended at ch 7
		//if use pdb0 ch1 as pretrigger,adc ch must be started from ch 8,ended at ch 15
		//if use pdb0 ch2 as pretrigger,adc ch must be started from ch 16,ended at ch 23.
		//and we can only read adc ch one by one, example: must read ch0->ch1->ch2->ch3, we can't skip any one.
		// more details, please refer to S32K datasheet
		
		for(reg_ch_index=0;reg_ch_index<ADC_CH_MAX;reg_ch_index++)
		{
			sAdcChResult[reg_ch_index] = 0;
			ADC_DRV_WaitConvDone(adcInstanceIndex);
			if(ADC_DRV_GetConvCompleteFlag(adcInstanceIndex,reg_ch_index) == true)
			{
				ADC_DRV_GetChanResult(adcInstanceIndex, reg_ch_index, &sAdcChResult[reg_ch_index]);
				//TracePrintf(SYSTEM_TRACE,TRACE_INFO,"---ADC:ch:%d,result:%d!!!---\r\n",ch,*result);
				adcConvDoneFlag = TRUE;
			}
		}

		AdcWrapper_Stop(adcInstanceIndex);
		return adcConvDoneFlag;
#else
	return FALSE;
#endif
}


boolean AdcWrapper_Read(uint8 adcInstanceIndex,tAdcCh ch,uint16* result)
{
	uint8 reg_ch_index = 0;
	
	if(sAdcInitDoneFlag == FALSE ||adcInstanceIndex>=ADC_NUM_MAX || ch>=ADC_CH_MAX||result == NULL)
		return FALSE;
	*result = sAdcChResult[ch];

	return TRUE;
}

boolean AdcWrapper_Stop(uint8 adcInstanceIndex)
{
	ADC_StopGroupConversion(adcInstanceIndex,0,30);

	return TRUE;
}

boolean AdcWrapper_DisableChannel(uint8 chIndex)
{
	//2TODO:

	return FALSE;
}


