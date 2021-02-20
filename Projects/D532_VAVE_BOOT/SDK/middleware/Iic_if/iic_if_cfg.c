
#include "Std_Types.h"
#include "iic_if_cfg.h"
//#include "BoardDefines.h"
//#include "pin_mux.h"



pin_settings_config_t g_pin_mux_i2c1_busoff_recover_Arr[2] =
{
		{
			.base          = PORTE,
			.pinPortIdx    = 0u,
			.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
			.passiveFilter = false,
			.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
			.mux           = PORT_MUX_AS_GPIO,
			.pinLock       = false,
			.intConfig     = PORT_DMA_INT_DISABLED,
			.clearIntFlag  = false,
			.gpioBase      = PTE,
			.direction     = GPIO_INPUT_DIRECTION,
		},
		{
			.base          = PORTE,
			.pinPortIdx    = 1u,
			.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
			.passiveFilter = false,
			.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
			.mux           = PORT_MUX_AS_GPIO,
			.pinLock       = false,
			.intConfig     = PORT_DMA_INT_DISABLED,
			.clearIntFlag  = false,
			.gpioBase      = PTE,
			.direction     = GPIO_OUTPUT_DIRECTION,
			.initValue     = 1u,
		},
};
pin_settings_config_t g_pin_mux_i2c1_Arr[2] =
{
	{
		.base          = PORTE,
		.pinPortIdx    = 1u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux           = PORT_MUX_ALT4,
		.pinLock       = false,
		.intConfig     = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase      = NULL,
	},
	{
		.base          = PORTE,
		.pinPortIdx    = 0u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux           = PORT_MUX_ALT4,
		.pinLock       = false,
		.intConfig     = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase      = NULL,
	},
};
pin_settings_config_t g_pin_mux_i2c0_busoff_recover_Arr[2] =
{
		{
			.base          = PORTA,
			.pinPortIdx    = 2u,
			.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
			.passiveFilter = false,
			.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
			.mux           = PORT_MUX_AS_GPIO,
			.pinLock       = false,
			.intConfig     = PORT_DMA_INT_DISABLED,
			.clearIntFlag  = false,
			.gpioBase      = PTA,
			.direction     = GPIO_INPUT_DIRECTION,
		},
		{
			.base          = PORTA,
			.pinPortIdx    = 3u,
			.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
			.passiveFilter = false,
			.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
			.mux           = PORT_MUX_AS_GPIO,
			.pinLock       = false,
			.intConfig     = PORT_DMA_INT_DISABLED,
			.clearIntFlag  = false,
			.gpioBase      = PTA,
			.direction     = GPIO_OUTPUT_DIRECTION,
			.initValue     = 1u,
		},
};
pin_settings_config_t g_pin_mux_i2c0_Arr[2] =
{
	{
		.base          = PORTA,
		.pinPortIdx    = 3u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux           = PORT_MUX_ALT3,
		.pinLock       = false,
		.intConfig     = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase      = NULL,
	},
	{
		.base          = PORTA,
		.pinPortIdx    = 2u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux           = PORT_MUX_ALT3,
		.pinLock       = false,
		.intConfig     = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase      = NULL,
	},
};


uint8 i2c_FristInitFlag[LPI2C_INSTANCE_COUNT] = {FALSE,FALSE};

#if 1
//indicate I2C0 master
const lpi2c_master_user_config_t* iic0DevicesCfg[IIC0_DEVICES_NUM]=
{
	&lpi2c0_MasterCfg_Amp,
	&lpi2c0_MasterCfg_Rtc,
	&lpi2c0_MasterCfg_Gyro,
	&lpi2c0_MasterCfg_Acce
};

#endif
#if 1
//indicate I2C1 master
const lpi2c_master_user_config_t* iic1DevicesCfg[IIC1_DEVICES_NUM]=
{
	&lpi2c1_MasterCfg_Eep,
};

#endif

//indicate I2C master state
lpi2c_master_state_t sI2CMasterState[LPI2C_INSTANCE_COUNT];


//indicate current master if not have initialized
uint8 sI2CMaterCurCfgNum[LPI2C_INSTANCE_COUNT] = {0xff,0xff};

#if( STD_ON == I2C_USING_SEMAPHORE_HANDLE )
SemaphoreHandle_t xSemaphore[LPI2C_INSTANCE_COUNT] = {NULL,NULL};
#endif

//pointer to I2C master
const lpi2c_master_user_config_t * const *pI2CMasterUserConfig[LPI2C_INSTANCE_COUNT] = {&iic0DevicesCfg[0],&iic1DevicesCfg[0]};



