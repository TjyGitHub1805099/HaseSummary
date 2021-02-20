//*****************************************************************************
// file			: avs.h
// Description	: calculate average speed .h file
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 

#ifndef __AVS_H__
#define __AVS_H__

#include "avs_cfg.h"


uint8_t  AVS_VcuReadyJudge(void);/**����Ŀ-�ж�VCU Ready�ź�**/


void AVS_vMain( void );
void AVS_vInit(void);




void AVS_ResetTripAAVS(void); //
void AVS_ResetTripBAVS(void); //
                        
#endif

