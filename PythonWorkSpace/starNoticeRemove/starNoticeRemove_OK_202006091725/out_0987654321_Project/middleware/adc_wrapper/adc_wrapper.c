#include "adc_wrapper_if.h"

#ifdef PLATFORM_S32K_FOR_ADC
#include "adc_pal.h"
#include "string.h"
#endif

boolean AdcWrapper_Stop(uint8 adcInstanceIndex);

static boolean sAdcInitDoneFlag[ADC_INSTANCE_MAX] = {FALSE,FALSE};
static uint16 sAdcChResult[ADC_INSTANCE_MAX][ADC_CH_MAX];
static boolean sAdcConvIsActive[ADC_INSTANCE_MAX] ={FALSE,FALSE};
static const adc_config_t *sAdcCfgPtrArray[ADC_INSTANCE_MAX];

boolean AdcWrapper_Init(uint8 adcInstanceIndex,const void* adcCfg)
{	
	if(adcCfg!=NULL && adcInstanceIndex < ADC_INSTANCE_MAX)
	{
	#ifdef PLATFORM_S32K_FOR_ADC
		if(ADC_Init(adcInstanceIndex,(const adc_config_t * const)adcCfg) == STATUS_SUCCESS)
		{
			sAdcCfgPtrArray[adcInstanceIndex] = (const adc_config_t *)adcCfg;
			memset(sAdcChResult[adcInstanceIndex],0,sizeof(sAdcChResult[adcInstanceIndex]));
			sAdcInitDoneFlag[adcInstanceIndex] = TRUE;
			AdcWrapper_ConvLoop(adcInstanceIndex);
			return TRUE;
		}
	#endif
	}

	return FALSE;
}

boolean AdcWrapper_Deinit(uint8 adcInstanceIndex)
{
	if(AdcWrapper_Stop(adcInstanceIndex) == TRUE)
	{
#ifdef PLATFORM_S32K_FOR_ADC
		if(ADC_Deinit(adcInstanceIndex) == STATUS_SUCCESS)
		{
			sAdcCfgPtrArray[adcInstanceIndex] = NULL;
			sAdcInitDoneFlag[adcInstanceIndex] = FALSE;
			return TRUE;
		}
#endif
	}
	
	return FALSE;
}

boolean AdcWrapper_ConvLoop(uint8 adcInstanceIndex)
{
	
	if(adcInstanceIndex<ADC_INSTANCE_MAX && sAdcInitDoneFlag[adcInstanceIndex] == TRUE)
	{
		if(sAdcConvIsActive[adcInstanceIndex] == FALSE)
		{

#ifdef PLATFORM_S32K_FOR_ADC
			if(ADC_StartGroupConversion(adcInstanceIndex,0) == STATUS_SUCCESS)
			{
				sAdcConvIsActive[adcInstanceIndex] = TRUE;
				return TRUE;
			}
#endif
		}
	}

	return FALSE;
}


boolean AdcWrapper_Read(uint32 ch,uint16* result)
{
	uint8 i = 0;
	boolean ret = FALSE;
	
	if(result != NULL)
	{
		for(i=0;i<gAdcUsedChNum;i++)
		{
			if(ch==gAdcInstanceChMap[i].logic_ch)
			{
				ret = TRUE;
				*result = sAdcChResult[gAdcInstanceChMap[i].adcInstance][gAdcInstanceChMap[i].phy_ch_index];
				break;
			}
		}
	}
	return ret;
}

boolean AdcWrapper_Stop(uint8 adcInstanceIndex)
{
#ifdef PLATFORM_S32K_FOR_ADC
	if(ADC_StopGroupConversion(adcInstanceIndex,0,50) == STATUS_SUCCESS)
	{
		sAdcConvIsActive[adcInstanceIndex] = FALSE;
		return TRUE;
	}
#endif
	return FALSE;
}

boolean AdcWrapper_StopGroup(uint8 adcInstanceIndex,uint32 groupIndex)
{
#ifdef PLATFORM_S32K_FOR_ADC
	ADC_StopGroupConversion(adcInstanceIndex,groupIndex,50);
#endif
	return TRUE;
}


void AdcWrapperIntNotifyCbk(const adc_callback_info_t * const callbackInfo, void * userData)
{
#ifdef PLATFORM_S32K_FOR_ADC
	uint8 offSetNum = callbackInfo->resultBufferTail;
	uint32 adcInstance = 0;
	uint8 i = 0;
	
	if(userData!=0)
		adcInstance = (uint32)userData;
	if(adcInstance>=ADC_INSTANCE_MAX)
		return;
	for(i=0;i<(offSetNum+1);i++)
	{
		sAdcChResult[adcInstance][i] = sAdcCfgPtrArray[adcInstance]->groupConfigArray[callbackInfo->groupIndex].resultBuffer[i];
	}
#endif
}

