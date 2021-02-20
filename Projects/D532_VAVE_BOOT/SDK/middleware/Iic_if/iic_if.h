#ifndef IIC_IF_H
#define IIC_IF_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "osif.h"

#include "projdefs.h"
#include "iic_if_cfg.h"
/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
//#define IIC_TIMEOUT 		2000
//#define I2C_BLOCKING_BYTE 	0X100
//#define I2C_MAX_WRITE_DATA	0x100

/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/
extern boolean gSystemInitDoneFlag ;

void I2C_Init();
Std_ReturnType LPI2C_PAL_MasterInit(uint32 instance,uint8 cfgNum);



BaseType_t I2C_MutxLock( uint32 instance , uint32 timeOutMs ,uint16 times);
BaseType_t I2C_MutxUnLock( uint32 instance );

Std_ReturnType LPI2C_PAL_MasterSendDataBlocking(uint32 instance, uint8	cfgNum,  const uint8 * txBuff, uint32  txSize, bool  sendStop);
Std_ReturnType LPI2C_PAL_MasterReceiveDataBlocking(uint32 instance, uint8  cfgNum, uint8 * rxBuff, uint32 rxSize, bool sendStop);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* IIC_IF_H */

