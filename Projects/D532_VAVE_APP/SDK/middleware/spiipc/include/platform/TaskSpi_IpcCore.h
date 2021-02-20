

#ifndef _SPI_IF_H_
#define _SPI_IF_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"


extern void Task_SpiCoreRead(void * pvParameters);
extern void Task_SpiCoreRx0(void * pvParameters);
extern void Task_SpiCoreRead_M(void * pvParameters);
extern void Task_SpiCoreRx0_M(void * pvParameters);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif

