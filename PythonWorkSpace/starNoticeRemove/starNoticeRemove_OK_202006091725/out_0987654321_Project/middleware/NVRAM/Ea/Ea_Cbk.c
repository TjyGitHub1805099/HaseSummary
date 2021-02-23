

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













