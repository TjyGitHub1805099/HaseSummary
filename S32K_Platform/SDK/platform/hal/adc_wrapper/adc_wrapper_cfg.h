/********************************************************************
File name: Adc_If.h
Author: Stephen Du
Version: V1.0
Date: 11/09/2016
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
#ifndef _ADC_WRAPPER_CFG_H_
#define _ADC_WRAPPER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

#define PLATFORM_S32K_FOR_ADC
#define ADC_NUM_MAX 2

typedef enum{
	ADC_CH_BOARD_TEMPERATURE,
	ADC_CH_AMP_TEMPERATURE,
	ADC_CH_SYSTEM_VOLTAGE,
	ADC_CH_MAX
}tAdcCh;

#ifdef __cplusplus
}
#endif

#endif  

