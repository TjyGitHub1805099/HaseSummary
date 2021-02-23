/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Ea.c
**          Modifly:brown
**			author: weltry wang
**          Date  : 2017-06-20
**          
**          History:
**          1. Date:2018-02-26
**             Author:weltry wang
**             Modification:
**
******************************************************************************/

#include "Ea_Cbk.h" 
#include "Std_Types.h"
#include "Eeprom.h"
void Ea_JobEndNotification()
{
	
	
	
}
void Ea_JobErrorNotification()
{
	
	
	
	
}


Std_ReturnType Ea_Eep_Write(uint16 address,uint8 *bufferPtr,uint32 length)
{

    return Eep_Write(address,bufferPtr,length);
}

Std_ReturnType Ea_Eep_Read(uint16 address,uint8 *bufferPtr,uint32 length)
{
	return Eep_Read(address,bufferPtr,length);
}













