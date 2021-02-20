#ifndef IIC_IF_CFG_H
#define IIC_IF_CFG_H

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "lpi2c0_cfg.h"
#include "lpi2c1_cfg.h"
#include "BoardDefines.h"
#include "pin_mux.h"


/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
#define I2C_USING_SEMAPHORE_HANDLE	STD_ON

#define IIC0_DEVICES_NUM	4 //i2c0 bus have 4 device :AMP RTC GYRO ACC
#define IIC1_DEVICES_NUM	1 //i2c1 bus have 1 device :EEPROM

#define IIC_TIMEOUT 		2000

/************************************************************************************************/
//LPI2C0 : manage four divice ,AMP RTC GYRO ACCE
#define INST_LPI2C_AMP	INST_LPI2C0_CFG
#define INST_LPI2C_RTC	INST_LPI2C0_CFG
#define INST_LPI2C_GYRO	INST_LPI2C0_CFG
#define INST_LPI2C_ACCE	INST_LPI2C0_CFG

#define IIC_CFGNUM_AMP	0
#define IIC_CFGNUM_RTC	1
#define IIC_CFGNUM_GYRO	2
#define IIC_CFGNUM_ACCE	3

/************************************************************************************************/
//LPI2C1 : manage one divice ,EEPROM
#define INST_LPI2C_EEP	INST_LPI2C1_CFG
#define IIC_CFGNUM_EEP	0

/*============================[V A R I A B L E   D E C L A R A T I O N]=========================*/
/************************************************************************************************/

extern pin_settings_config_t g_pin_mux_i2c1_busoff_recover_Arr[];
extern pin_settings_config_t g_pin_mux_i2c1_Arr[];
extern pin_settings_config_t g_pin_mux_i2c0_busoff_recover_Arr[];
extern pin_settings_config_t g_pin_mux_i2c0_Arr[];


extern uint8 i2c_FristInitFlag[];
extern const lpi2c_master_user_config_t* iic0DevicesCfg[];
extern const lpi2c_master_user_config_t* iic1DevicesCfg[];

extern lpi2c_master_state_t sI2CMasterState[];
extern uint8 sI2CMaterCurCfgNum[];
#if( STD_ON == I2C_USING_SEMAPHORE_HANDLE )
extern SemaphoreHandle_t xSemaphore[];
#endif
extern const lpi2c_master_user_config_t * const *pI2CMasterUserConfig[];




#endif /* IIC_PAL_CFG_H */
