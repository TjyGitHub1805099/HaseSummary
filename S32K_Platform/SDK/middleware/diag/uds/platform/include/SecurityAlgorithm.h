/**************************
description: 0x27 SecurityAlgorithm
author: wangyunchuan@hangsheng.com.cn
version: 
date:2017/11/06
**************************/

#ifndef SecurityAlgorithm_H
#define SecurityAlgorithm_H

#include "Platform_types.h"

void CreateKey(uint8* ECUSerNum,uint8 ECUSerNumLen,const uint8* seed,uint8 SecurityLevel,uint8* RetKey);



#endif



