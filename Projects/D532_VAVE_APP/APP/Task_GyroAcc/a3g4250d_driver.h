/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : a3g4250d_driver.h
* Author             : MSH Application Team
* Author             : Abhishek Anand						
* Version            : $Revision:$
* Date               : $Date:$
* Description        : Descriptor Header for a3g4250d_driver.c driver file
*
* HISTORY:
* Date        | Modification                                | Author
* 16/05/2012  | Initial Revision                            | Abhishek Anand
* 17/05/2012  |  Modified to support multiple drivers in the same program                |	Abhishek Anand

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __A3G4250D_DRIVER__H
#define __A3G4250D_DRIVER__H

/* Includes ------------------------------------------------------------------*/
#include "ais328dq_driver.h"
/* Exported types ------------------------------------------------------------*/

//these could change accordingly with the architecture

#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES
typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;

typedef signed char    i8_t;
typedef signed short   i16_t;
typedef signed int     i32_t;

#endif /*__ARCHDEP__TYPES*/

typedef u8_t A3G4250D_Axis_t;
typedef u8_t A3G4250D_IntConf_t;

//define structure
#ifndef __SHARED__TYPES
#define __SHARED__TYPES

typedef enum {
  MEMS_SUCCESS                            =		0x01,
  MEMS_ERROR			          =		0x00	
} status_t1;

typedef enum {
  MEMS_ENABLE			          =		0x01,
  MEMS_DISABLE			          =		0x00	
} State_t;

#endif /*__SHARED__TYPES*/

typedef struct {
    i16_t x;
    i16_t y;
    i16_t z;
    i32_t sumX;
    i32_t sumY;
    i32_t sumZ;
    u8_t  count;
    u8_t  init;
} GyroRaw_t;

typedef enum {  
  A3G4250D_ODR_100Hz                  =		0x00,
  A3G4250D_ODR_200Hz		          =		0x01,	
  A3G4250D_ODR_400Hz		          =		0x02,
  A3G4250D_ODR_800Hz		          =		0x03
} A3G4250D_ODR_t;

typedef enum {
  A3G4250D_POWER_DOWN                 =		0x00,
  A3G4250D_NORMAL                     =     0x01,
} A3G4250D_Mode_t;

typedef enum {
  A3G4250D_BLE_LSB                        =		0x00,
  A3G4250D_BLE_MSB                        =		0x01
} A3G4250D_Endianess_t;

typedef enum {
  A3G4250D_SPI_4_WIRE                     =             0x00,
  A3G4250D_SPI_3_WIRE                     =             0x01
} A3G4250D_SPIMode_t;

typedef enum {
  A3G4250D_X_ENABLE                       =             0x01,
  A3G4250D_X_DISABLE                      =             0x00,
  A3G4250D_Y_ENABLE                       =             0x02,
  A3G4250D_Y_DISABLE                      =             0x00,
  A3G4250D_Z_ENABLE                       =             0x04,
  A3G4250D_Z_DISABLE                      =             0x00
} A3G4250D_AXISenable_t;

typedef enum {
  A3G4250D_FIFO_DISABLE = 0x00,
  A3G4250D_FIFO_ENABLE  = 0x01,
} A3G4250D_Fifo_t;

typedef enum {
  A3G4250D_FIFO_MODE_BYPASS = 0x00,
  A3G4250D_FIFO_MODE_FIFO = 0x01,
  A3G4250D_FIFO_MODE_STREAM = 0x02,
} A3G4250D_Fifo_Mode_t;
/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                                        0x01
#define MEMS_RESET                                      0x00

#endif /*__SHARED__CONSTANTS*/

#define A3G4250D_MEMS_I2C_ADDRESS                       0xD2

//Register and define
#define A3G4250D_WHO_AM_I				0x0F  // device identification register

// CONTROL REGISTER 1 
#define A3G4250D_CTRL_REG1       		0x20
#define A3G4250D_DR				        BIT(6)
#define A3G4250D_BW				        BIT(4)
#define A3G4250D_PD				        BIT(3)
#define A3G4250D_ZEN					BIT(2)
#define A3G4250D_YEN					BIT(1)
#define A3G4250D_XEN					BIT(0)

//CONTROL REGISTER 2 
#define A3G4250DCTRL_REG2				0x21
#define A3G4250D_HPM     				BIT(4)
#define A3G4250D_HPCF					BIT(0)

//CONTROL REGISTER 3 
#define A3G4250D_CTRL_REG3				0x22
#define A3G4250D_I1_INT1                BIT(7)
#define A3G4250D_I1_BOOT				BIT(6)
#define A3G4250D_H_LACTIVE				BIT(5)
#define A3G4250D_PP_OD  				BIT(4)
#define A3G4250D_I2_DRDY  				BIT(3)
#define A3G4250D_I2_WTM  				BIT(2)
#define A3G4250D_I2_ORUN    			BIT(1)
#define A3G4250D_I2_EMPTY 				BIT(0)

//CONTROL REGISTER 4
#define A3G4250D_CTRL_REG4				0x23
#define A3G4250D_BLE					BIT(6)
#define A3G4250D_ST       				BIT(1)
#define A3G4250D_SIM					BIT(0)

//CONTROL REGISTER 5
#define A3G4250D_CTRL_REG5       		0x24
#define A3G4250D_BOOT                   BIT(7)
#define A3G4250D_FIFO_EN                BIT(6)
#define A3G4250D_HPEN                   BIT(4)
#define A3G4250D_INT_SEL0               BIT(2)
#define A3G4250D_OUT_SEL0               BIT(0)


//REFERENCE/DATA_CAPTURE
#define A3G4250D_REFERENCE_REG		    0x25
#define A3G4250D_REF		            BIT(0)

//OUT_TEMP
#define A3G4250D_OUT_TEMP   		    0x26

//STATUS_REG
#define A3G4250D_STATUS_REG				0x27

//OUTPUT REGISTER
#define A3G4250D_OUT_X_L                0x28
#define A3G4250D_OUT_X_H                0x29
#define A3G4250D_OUT_Y_L			    0x2A
#define A3G4250D_OUT_Y_H		        0x2B
#define A3G4250D_OUT_Z_L			    0x2C
#define A3G4250D_OUT_Z_H		        0x2D

//FIFO_CTRL_REG 
#define A3G4250D_FIFO_CTRL_REG			0x2E
#define A3G4250D_FM0                    BIT(5)

//FIFO_SRC_REG 
#define A3G4250D_FIFO_SRC_REG			0x2F

//INT1_CFG_REG 
#define A3G4250D_INT1_CFG_REG		    0x30

//INT1_SRC_REG 
#define A3G4250D_INT1_SRC_REG 			0x31

//INT1_THS_XH_REG 
#define A3G4250D_INT1_THS_XH_REG		0x32

//INT1_THS_XL_REG  
#define A3G4250D_INT1_THS_XL_REG		0x33

//INT1_THS_YH_REG 
#define A3G4250D_INT1_THS_YH_REG		0x34

//INT1_THS_YL_REG 
#define A3G4250D_INT1_THS_YL_REG		0x35

//INT1_THS_ZH_REG 
#define A3G4250D_INT1_THS_ZH_REG		0x36

//INT1_THS_ZL_REG 
#define A3G4250D_INT1_THS_ZL_REG		0x37

//INT1_DURATION 
#define A3G4250D_INT1_DURATION			0x38

//STATUS REGISTER bit mask
#define A3G4250D_STATUS_REG_ZYXOR                       0x80    // 1	:	new data set has over written the previous one
						                // 0	:	no overrun has occurred (default)	
#define A3G4250D_STATUS_REG_ZOR                         0x40    // 0	:	no overrun has occurred (default)
							        // 1	:	new Z-axis data has over written the previous one
#define A3G4250D_STATUS_REG_YOR                         0x20    // 0	:	no overrun has occurred (default)
						        	// 1	:	new Y-axis data has over written the previous one
#define A3G4250D_STATUS_REG_XOR                         0x10    // 0	:	no overrun has occurred (default)
							        // 1	:	new X-axis data has over written the previous one
#define A3G4250D_STATUS_REG_ZYXDA                       0x08    // 0	:	a new set of data is not yet avvious one
                                                                // 1	:	a new set of data is available 
#define A3G4250D_STATUS_REG_ZDA                         0x04    // 0	:	a new data for the Z-Axis is not availvious one
                                                                // 1	:	a new data for the Z-Axis is available
#define A3G4250D_STATUS_REG_YDA                         0x02    // 0	:	a new data for the Y-Axis is not available
                                                                // 1	:	a new data for the Y-Axis is available
#define A3G4250D_STATUS_REG_XDA                         0x01    // 0	:	a new data for the X-Axis is not available
                                                                // 1	:	a new data for the X-Axis is available
#define A3G4250D_DATAREADY_BIT                          A3G4250D_STATUS_REG_ZYXDA

/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported functions --------------------------------------------------------*/

//Sensor Configuration Functions
status_t1 A3G4250D_GetWHO_AM_I(u8_t* val);
status_t1 A3G4250D_SetODR(A3G4250D_ODR_t dr);
status_t1 A3G4250D_SetMode(A3G4250D_Mode_t pm);
status_t1 A3G4250D_SetAxis(A3G4250D_Axis_t axis);
status_t1 A3G4250D_SetBLE(A3G4250D_Endianess_t ble);
status_t1 A3G4250D_SetFifo(A3G4250D_Fifo_t fifo);
status_t1 A3G4250D_SetFifo_Mode(A3G4250D_Fifo_Mode_t fifo_mode,
        u8_t water_mark_level);

//Interrupt Functions

//Other Reading Functions
status_t1 A3G4250D_GetStatusReg(u8_t* val);
status_t1 A3G4250D_GetStatusBit(u8_t statusBIT, u8_t* val);
status_t1 A3G4250D_GetGyroRaw(GyroRaw_t* buff);
status_t1 A3G4250D_GetTemperature(u8_t* val);

//Generic
// i.e. u8_t A3G4250D_ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data);
// i.e. u8_t A3G4250D_WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data); 
u8_t A3G4250D_ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data);
u8_t A3G4250D_WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data);

extern u8_t gyro_data[6];

#endif /*__A3G4250D_DRIVER__H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/



