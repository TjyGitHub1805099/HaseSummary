/********************************************************************
File name: Adc_wrapper.h
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
#ifndef _ADC_WRAPPER_H_
#define _ADC_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

typedef struct{
	uint8 logic_ch;
	uint8 adcInstance;
	uint8 phy_ch_index;//s32k adc,one time only measurement 8 channels
}tAdcInstanceChMap;

extern const tAdcInstanceChMap gAdcInstanceChMap[];
extern const uint8 gAdcUsedChNum;

#ifdef __cplusplus
}
#endif

#endif  

