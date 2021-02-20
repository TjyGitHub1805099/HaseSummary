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
#ifndef _ADC_WRAPPER_IF_H_
#define _ADC_WRAPPER_IF_H_

#include "adc_wrapper_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

boolean AdcWrapper_Init(uint8 adcInstanceIndex,void* adcCfg);
boolean AdcWrapper_StartConv(uint8 adcInstanceIndex);
boolean AdcWrapper_ConvLoop(uint8 adcInstanceIndex);
boolean AdcWrapper_Read(uint8 adcInstanceIndex,tAdcCh ch,uint16* result);
boolean AdcWrapper_Stop(uint8 adcInstanceIndex);

#ifdef __cplusplus
}
#endif

#endif  

