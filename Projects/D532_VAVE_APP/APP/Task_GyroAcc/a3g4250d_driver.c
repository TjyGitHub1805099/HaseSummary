/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : a3g4250d_driver.c
* Author             : MSH Application Team
* Author             : Abhishek Anand	
* Version            : $Revision:$
* Date               : $Date:$
* Description        : A3G4250D driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 16/05/2012         |	Initial Revision                |	Abhishek Anand
* 17/05/2012         |  Modified to support multiple drivers in the same program                |	Abhishek Anand

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

/* Includes ------------------------------------------------------------------*/
#include "a3g4250d_driver.h"
#include "i2c_mems.h"
#include "trace.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  MAX_RED_FIFO_SIZE  10
#define  MAX_LIMIT_DIGIT    500000
/* Private macro -------------------------------------------------------------*/
#define ST_ABS(x)     (((x) ^ ((x) < 0 ? -1 : 0)) - ((x) < 0 ? -1 : 0))
/* Private variables ---------------------------------------------------------*/
u8_t gyro_data[6];
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: A3G4250D_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data Read
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t A3G4250D_ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data) {
  
  //To be completed with either I2c or SPI reading function
  // i.e. *Data = SPI_Mems_Read_Reg( Reg );
  // i.e. if(!I2C_BufferRead(Data, deviceAddr, Reg, 1)) 
  // return MEMS_ERROR;
  // else  
  // return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    ret = mems_i2c_read(Reg,Data,1);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}

u8_t A3G4250D_ReadReg_Multi(u8_t deviceAddr, u8_t Reg, u8_t* Data,u8_t len) {
  
  //To be completed with either I2c or SPI reading function
  // i.e. *Data = SPI_Mems_Read_Reg( Reg );
  // i.e. if(!I2C_BufferRead(Data, deviceAddr, Reg, 1)) 
  // return MEMS_ERROR;
  // else  
  // return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    ret = mems_i2c_read(Reg,Data,len);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}


/*******************************************************************************
* Function Name		: A3G4250D_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t A3G4250D_WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data) {
  
  //To be completed with either I2c or SPI writing function
  // i.e. SPI_Mems_Write_Reg(Reg, Data);
  // i.e. I2C_ByteWrite(&Data,  deviceAddress,  WriteAddr);
  //  return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    u8_t txMsg[2];

    txMsg[0] = WriteAddr;
    txMsg[1] = Data;
    
    ret = mems_i2c_write(txMsg,2);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}


/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : A3G4250D_GetWHO_AM_I
* Description    : Read identification code from A3G4250D_WHO_AM_I register
* Input          : char to be filled with the Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t1 A3G4250D_GetWHO_AM_I(u8_t* val){
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_WHO_AM_I, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : A3G4250D_SetODR
* Description    : Sets A3G4250D Gyroscope Output Data Rate 
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_SetODR(A3G4250D_ODR_t dr){
  u8_t value;
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0x3F;
  value |= (dr << A3G4250D_DR);
  
  if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : A3G4250D_SetMode
* Description    : Sets A3G4250D Gyroscope Operating Mode
* Input          : Modality (A3G4250D_LOW_POWER, A3G4250D_NORMAL, A3G4250D_POWER_DOWN...)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_SetMode(A3G4250D_Mode_t pm) {
  u8_t value;
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0xF7;
  value |= (pm << A3G4250D_PD);
  
  if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : A3G4250D_SetAxis
* Description    : Enable/Disable A3G4250D Axis
* Input          : A3G4250D_X_ENABLE/A3G4250D_X_DISABLE | A3G4250D_Y_ENABLE/A3G4250D_Y_DISABLE
                   | A3G4250D_Z_ENABLE/A3G4250D_Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_SetAxis(A3G4250D_Axis_t axis) {
  u8_t value;
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0xF8;
  value |= (0x07 & axis);
  
  if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG1, value) )
    return MEMS_ERROR;   
  
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : A3G4250D_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : A3G4250D_BLE_LSB / A3G4250D_BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_SetBLE(A3G4250D_Endianess_t ble) {
  u8_t value;
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xBF;	
  value |= (ble<<A3G4250D_BLE);
  
  if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

status_t1 A3G4250D_SetFifo(A3G4250D_Fifo_t fifo) {
  u8_t value;
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG5, &value) )
    return MEMS_ERROR;
  
  value &= 0xBF;	
  value |= (fifo << A3G4250D_FIFO_EN);
  value |= (1 << A3G4250D_HPEN);
  value |= (0x3 << A3G4250D_OUT_SEL0);
  
  if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_CTRL_REG5, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

status_t1 A3G4250D_SetFifo_Mode(A3G4250D_Fifo_Mode_t fifo_mode,
                                    u8_t water_mark_level) {
    u8_t value = 0x0;

    if(!A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS,
            A3G4250D_FIFO_CTRL_REG, &value))
    {
        return MEMS_ERROR;
    }
    
    value |= (fifo_mode << A3G4250D_FM0);
    value |= water_mark_level;

    if( !A3G4250D_WriteReg(A3G4250D_MEMS_I2C_ADDRESS,
            A3G4250D_FIFO_CTRL_REG, value))
    {
        return MEMS_ERROR;
    }

    return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : A3G4250D_GetStatusReg
* Description    : Read the status register
* Input          : char to empty by Status Reg Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_GetStatusReg(u8_t* val) {
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS, A3G4250D_STATUS_REG, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : A3G4250D_GetStatusBIT
* Description    : Read the status register BIT
* Input          : A3G4250D_STATUS_REG_ZYXOR, A3G4250D_STATUS_REG_ZOR, A3G4250D_STATUS_REG_YOR, A3G4250D_STATUS_REG_XOR,
                   A3G4250D_STATUS_REG_ZYXDA, A3G4250D_STATUS_REG_ZDA, A3G4250D_STATUS_REG_YDA, A3G4250D_STATUS_REG_XDA, 
                   A3G4250D_DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_GetStatusBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS,A3G4250D_STATUS_REG, &value) )
    return MEMS_ERROR;
  
  switch (statusBIT){
  case A3G4250D_STATUS_REG_ZYXOR:
    if(value &= A3G4250D_STATUS_REG_ZYXOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case A3G4250D_STATUS_REG_ZOR:       
    if(value &= A3G4250D_STATUS_REG_ZOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case A3G4250D_STATUS_REG_YOR:       
    if(value &= A3G4250D_STATUS_REG_YOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }                                 
  case A3G4250D_STATUS_REG_XOR:       
    if(value &= A3G4250D_STATUS_REG_XOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case A3G4250D_STATUS_REG_ZYXDA:     
    if(value &= A3G4250D_STATUS_REG_ZYXDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case A3G4250D_STATUS_REG_ZDA:       
    if(value &= A3G4250D_STATUS_REG_ZDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case A3G4250D_STATUS_REG_YDA:       
    if(value &= A3G4250D_STATUS_REG_YDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case A3G4250D_STATUS_REG_XDA:       
    if(value &= A3G4250D_STATUS_REG_XDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }                                      
  }
  
  return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : A3G4250D_GetGyroRaw
* Description    : Read the Gyroscope Values Output Registers
* Input          : buffer to empity by GyroRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 A3G4250D_GetGyroRaw(GyroRaw_t* buff) {
    status_t1 ret = MEMS_SUCCESS;
    u8_t  fifo_src = 0;
    u8_t over_run = MEMS_RESET;
    if(!A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS,A3G4250D_FIFO_SRC_REG,&fifo_src))
    {
        return MEMS_ERROR;
    }

    fifo_src &= 0x1F;

    if (fifo_src > 0)
    {
        if (fifo_src == 31)
        {   
            A3G4250D_GetStatusBit(A3G4250D_STATUS_REG_ZYXOR,&over_run);
            if (over_run == MEMS_SET)
            {
                fifo_src = 32;
            }
        }

        while( fifo_src >= 1)
        {
            A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS,A3G4250D_OUT_X_L,gyro_data);
            A3G4250D_ReadReg_Multi(A3G4250D_MEMS_I2C_ADDRESS,A3G4250D_OUT_Y_L,gyro_data,6);

            fifo_src--;
        }

        ret = MEMS_SUCCESS;
    }
    else
    {
        ret = MEMS_ERROR;
    }
  return ret;
}

status_t1 A3G4250D_GetTemperature(u8_t* val)
{
    status_t1 ret = MEMS_SUCCESS;
    if( !A3G4250D_ReadReg(A3G4250D_MEMS_I2C_ADDRESS,A3G4250D_OUT_TEMP,val))
    {
        ret = MEMS_ERROR;
    }
    return ret;
}
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
