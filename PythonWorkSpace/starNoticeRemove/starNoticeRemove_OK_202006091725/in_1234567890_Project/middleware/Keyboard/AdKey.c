
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdKey.h"
#include "trace.h"
#include "adc_wrapper_if.h"
#include "System_Signal.h"
#include "Spi_Ipc_gw_api.h"
#include "Key.h"
//#include "ipc_gw_api.h"


AdKeyFilter AdkeyFil[AD_KEY_CH_NUM];
		
uint8 AdDataConver(uint8 ch,uint16 Val)
{
	uint8 key_data;
	volatile uint16 key_ad;

	key_ad = Val;
    key_data = KEY_NONE;
	#if 1
    if(ch == AD_STEERING_A_KEY)
	{
		 if(key_ad < KEY_NONE_LEVEL)
	 	{
			 if((key_ad <= (SWC_SRC_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= KEY_LOW_LEVEL))
			{
				key_data = KEY_STRG_SRC;
			}
			else if((key_ad <= (SWC_MENU_UP_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_MENU_UP_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_MENU_UP;
			}
			else if((key_ad <= (SWC_MENU_DOWN_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_MENU_DOWN_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_MENU_DOWN;
			}
			else if((key_ad <= (SWC_TEL_ON_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_TEL_ON_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_VOICE_REC;
			}
			else if((key_ad <= (SWC_OK_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_OK_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_ENTER;
			}
		
	 	}
	}
	else if(ch == AD_STEERING_B_KEY)
	{
		if(key_ad < KEY_NONE_LEVEL)
	 	{
			 if((key_ad <= (SWC_VOL_DOWN_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= KEY_LOW_LEVEL))
			{
				key_data = KEY_STRG_VOL_DOWN;
			}
			else if((key_ad <= (SWC_VOL_UP_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_VOL_UP_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_VOL_UP;
			}
			else if((key_ad <= (SWC_CAMERA_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_CAMERA_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_TEL;
			}
			else if((key_ad <= (SWC_MODE_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_MODE_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_BACK;
			}
			else if((key_ad <= (SWC_DISP_LEVEL + SWC_STEP_LEVEL))&&(key_ad >= (SWC_DISP_LEVEL - SWC_STEP_LEVEL)))
			{
				key_data = KEY_STRG_DISP;
			}
		}

	}
	
#endif
	return key_data;
}

void AdKeyFilInit(void)
{
	uint8 i;
	for(i=0;i< AD_KEY_CH_NUM;i++)
	{
		AdkeyFil[i].count = 0;
		AdkeyFil[i].new_value = 0xffff;
		AdkeyFil[i].temp = 0xffff;
		AdkeyFil[i].value = 0xffff;
	}
}

void AdKeyFilterRun(uint8 ch)
{
    uint8 adcChannelMap[AD_KEY_CH_NUM]={ADC_LOGIC_CH_STEERING_A_IN,ADC_LOGIC_CH_STEERING_B_IN};

    #define N    1  //  5
    //Adc_Read(adcChannelMap[ch],&AdkeyFil[ch].new_value);
	//AdcWrapper_Read(ADC_LOGIC_CH_STEERING_AD,&AdkeyFil[ch].new_value);
	AdcWrapper_Read(adcChannelMap[ch],&AdkeyFil[ch].new_value);
	 if((AdkeyFil[ch].value>(AdkeyFil[ch].new_value-5)) &&(AdkeyFil[ch].value<(AdkeyFil[ch].new_value + 5)))
	 {
		 if(AdkeyFil[ch].count >= N)
		{
			 AdkeyFil[ch].count=0;
			 if(AdkeyFil[ch].value != AdkeyFil[ch].temp)
			 {
		 		AdkeyFil[ch].temp=AdkeyFil[ch].value;
			 }
		 } 
	 	AdkeyFil[ch].count++;
	 }
	 else
	 {
		AdkeyFil[ch].count=0;
		AdkeyFil[ch].value =AdkeyFil[ch].new_value;
	 }
}
#endif


