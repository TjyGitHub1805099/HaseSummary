#ifndef _I2C_MEMS_H_
#define _I2C_MEMS_H_

#define IIC_DEBUG

//#include <iodefine.h> changed by tjy
//#include <kernel.h>changed by tjy

//#include "errno.h"changed by tjy
//#include "Itron.h"changed by tjy

#include "iic_if.h"
#include "ais328dq_driver.h"
typedef enum {
	ESUCCESS = 0x01,
  EERROR   = 0x00,
} ER;

boolean mems_i2c_getErrStatus();
int mems_i2c_open(const char *dev);
ER  mems_i2c_read(uint8_t reg_addr, uint8_t *data,uint8_t len);
ER  mems_i2c_write(uint8_t * value,uint8_t len);
ER  mems_i2c_ctrl(T_SERIAL_IIC_CHAN_SETTING *setting);

int acce_i2c_open(const char *dev);
ER  acce_i2c_read(uint8_t reg_addr, uint8_t *data,uint8_t len);
ER  acce_i2c_write(uint8_t * value,uint8_t len);
ER  acce_i2c_ctrl(T_SERIAL_IIC_CHAN_SETTING *setting);

#endif ///_I2C_MEMS_H_
