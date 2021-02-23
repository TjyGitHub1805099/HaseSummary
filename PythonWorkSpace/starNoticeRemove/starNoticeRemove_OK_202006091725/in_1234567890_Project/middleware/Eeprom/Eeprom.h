#include "Std_Types.h"

#ifndef  _EEPROM_H_
#define  _EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

Std_ReturnType Eep_Init();
Std_ReturnType Eep_Read(uint16 address,uint8 *bufferPtr,uint32 length);
Std_ReturnType Eep_Write(uint16 address,uint8 *bufferPtr,uint32 length);


#ifdef __cplusplus
}
#endif
#endif

