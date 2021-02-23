/**
*@file  FlagGroup.h
*@brief Describe here.
*@date    2018-03-23 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-23  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _FLAGGROUP_H_
#define _FLAGGROUP_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

#define FLAGGROUP_DEBUG_TRACE STD_OFF

/**
 * Use this macro to define buffer quickly.
 */
#define FlagGroup_BufDef(name,size) uint32 name[(((size)-1)>>5)+5]

/**
 * Use this macro to get buffer name.
 */
//#define FlagGruop_GetBufName(name) FlagGroup_Buf_##name

/**
 * I strongly recommend you use this macro to init this module(Don't use FlagGroup_Init() directly).
 */
#define FlagGroup_Initial(name,size) FlagGroup_Init(name,size)

typedef enum _FlagGroupModeEType
{
    FLAGGROUP_MODE_LOWEST,
    FLAGGROUP_MODE_HIGHEST
} FlagGroupModeEType;

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief I don't suggest you call this func to init unless you familiar with module.
*       You can use the macro:FlagGroup_Initial(size,name);
*/
extern uint32* FlagGroup_Init(uint32* bufPtr, uint16 size);
extern boolean FlagGroup_ClearAll(uint32* handlePtr);
extern uint16 FlagGroup_GetSize(const uint32* handlePtr);
extern boolean FlagGroup_SetFlagState(uint32* handlePtr, uint16 index, boolean trueFalse);
extern boolean FlagGroup_GetFlagState(const uint32* handlePtr, uint16 index, boolean* trueFalse);
extern boolean FlagGroup_CountClearedFlag(const uint32* handlePtr, uint16* num);
extern boolean FlagGroup_CountSettedFlag(const uint32* handlePtr, uint16* num);
extern boolean FlagGroup_GetSettedFlag(const uint32* handlePtr, FlagGroupModeEType mode, uint16* index);
extern boolean FlagGroup_GetClearedFlag(const uint32* handlePtr, FlagGroupModeEType mode, uint16* index);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_FLAGGROUP_H_
/*=========================== END OF FILE: FlagGroup.h ===========================*/
