/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : ais328dq_driver.c
* Author             : MSH Application Team
* Author             : Abhishek Anand	
* Version            : $Revision:$
* Date               : $Date:$
* Description        : AIS328DQ driver file
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
#include "ais328dq_driver.h"
#include "i2c_mems.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8_t acc_data[6] = {0};
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: AIS328DQ_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data Read
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t AIS328DQ_ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data) {
  
  //To be completed with either I2c or SPI reading function
  // i.e. *Data = SPI_Mems_Read_Reg( Reg );
  // i.e. if(!I2C_BufferRead(Data, deviceAddr, Reg, 1)) 
  // return MEMS_ERROR;
  // else  
  // return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    ret = acce_i2c_read(Reg,Data,1);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}


u8_t AIS328DQ_ReadReg_Multi(u8_t deviceAddr, u8_t Reg, u8_t* Data,u8_t len) {
  
  //To be completed with either I2c or SPI reading function
  // i.e. *Data = SPI_Mems_Read_Reg( Reg );
  // i.e. if(!I2C_BufferRead(Data, deviceAddr, Reg, 1)) 
  // return MEMS_ERROR;
  // else  
  // return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    ret = acce_i2c_read(Reg,Data,len);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}

/*******************************************************************************
* Function Name		: AIS328DQ_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t AIS328DQ_WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data) {
  
  //To be completed with either I2c or SPI writing function
  // i.e. SPI_Mems_Write_Reg(Reg, Data);
  // i.e. I2C_ByteWrite(&Data,  deviceAddress,  WriteAddr);
  //  return MEMS_SUCCESS;
    ER ret = ESUCCESS;
    u8_t txMsg[2];

    txMsg[0] = WriteAddr;
    txMsg[1] = Data;
    
    ret = acce_i2c_write(txMsg,2);
    return (ret == ESUCCESS ? MEMS_SUCCESS : MEMS_ERROR);
}

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : AIS328DQ_GetWHO_AM_I
* Description    : Read identification code from AIS328DQ_WHO_AM_I register
* Input          : char to be filled with the Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t1 AIS328DQ_GetWHO_AM_I(u8_t* val){
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_WHO_AM_I, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetODR
* Description    : Sets AIS328DQ Accelerometer Output Data Rate 
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetODR(AIS328DQ_ODR_t dr){
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0xE7;
  value |= dr<<AIS328DQ_DR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetMode
* Description    : Sets AIS328DQ Accelerometer Operating Mode
* Input          : Modality (AIS328DQ_LOW_POWER, AIS328DQ_NORMAL, AIS328DQ_POWER_DOWN...)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetMode(AIS328DQ_Mode_t pm) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0x1F;
  value |= (pm<<AIS328DQ_PM);   
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AIS328DQ_SetAxis
* Description    : Enable/Disable AIS328DQ Axis
* Input          : AIS328DQ_X_ENABLE/AIS328DQ_X_DISABLE | AIS328DQ_Y_ENABLE/AIS328DQ_Y_DISABLE
                   | AIS328DQ_Z_ENABLE/AIS328DQ_Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetAxis(AIS328DQ_Axis_t axis) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, &value) )
    return MEMS_ERROR;
  
  value &= 0xF8;
  value |= (0x07 & axis);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG1, value) )
    return MEMS_ERROR;   
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetFullScale
* Description    : Sets the AIS328DQ FullScale
* Input          : AIS328DQ_FULLSCALE_2/AIS328DQ_FULLSCALE_4/AIS328DQ_FULLSCALE_8
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetFullScale(AIS328DQ_Fullscale_t fs) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xCF;	
  value |= (fs<<AIS328DQ_FS);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetBDU(State_t bdu) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  value |= (bdu<<AIS328DQ_BDU);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : AIS328DQ_BLE_LSB / AIS328DQ_BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetBLE(AIS328DQ_Endianess_t ble) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xBF;	
  value |= (ble<<AIS328DQ_BLE);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetFDS
* Description    : Set Filter Data Selection
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetFDS(State_t fds) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0xEF;	
  value |= (fds<<AIS328DQ_FDS);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBOOT
* Description    : Rebot memory content
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetBOOT(State_t boot) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0x7F;	
  value |= (boot<<AIS328DQ_BOOT);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSelfTest
* Description    : Set Self Test Modality
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetSelfTest(State_t st) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xFD;
  value |= (st<<AIS328DQ_ST);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSelfTestSign
* Description    : Set Self Test Sign (Disable = st_plus, Enable = st_minus)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetSelfTestSign(State_t st_sign) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xF7;
  value |= (st_sign<<AIS328DQ_ST_SIGN);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetIntHighLow
* Description    : Set Interrupt active state (Disable = active high, Enable = active low)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetIntHighLow(State_t ihl) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  value |= (ihl<<AIS328DQ_IHL);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetIntPPOD
* Description    : Set Interrupt Push-Pull/OpenDrain Pad (Disable = Push-Pull, Enable = OpenDrain)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetIntPPOD(State_t pp_od) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0xBF;
  value |= (pp_od<<AIS328DQ_PP_OD);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1DataSign
* Description    : Set Data signal Interrupt 1 pad
* Input          : Modality by AIS328DQ_INT_Conf_t Typedef 
                  (AIS328DQ_INT_SOURCE, AIS328DQ_INT_1OR2_SOURCE, AIS328DQ_DATA_READY, AIS328DQ_BOOT_RUNNING)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1DataSign(AIS328DQ_INT_Conf_t i_cfg) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0xFC;
  value |= (i_cfg<<AIS328DQ_I1_CFG);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2DataSign
* Description    : Set Data signal Interrupt 2 pad
* Input          : Modality by AIS328DQ_INT_Conf_t Typedef 
                  (AIS328DQ_INT_SOURCE, AIS328DQ_INT_1OR2_SOURCE, AIS328DQ_DATA_READY, AIS328DQ_BOOT_RUNNING)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2DataSign(AIS328DQ_INT_Conf_t i_cfg) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0xE7;
  value |= (i_cfg<<AIS328DQ_I2_CFG);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSPI34Wire
* Description    : Set SPI mode 
* Input          : Modality by AIS328DQ_SPIMode_t Typedef (AIS328DQ_SPI_4_WIRE, AIS328DQ_SPI_3_WIRE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetSPI34Wire(AIS328DQ_SPIMode_t sim) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return MEMS_ERROR;
  
  value &= 0xFE;
  value |= (sim<<AIS328DQ_SIM);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_TurnONEnable
* Description    : TurnON Mode selection for sleep to wake function
* Input          : AIS328DQ_SLEEP_TO_WAKE_DIS/AIS328DQ_SLEEP_TO_WAKE_ENA
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_TurnONEnable(AIS328DQ_Sleep_To_Wake_Conf_t stw) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG5, &value) )
    return MEMS_ERROR;
  
  value &= 0x00;
  value |= (stw<<AIS328DQ_TURN_ON);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG5, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_HPFilterReset
* Description    : Reading register for reset the content of internal HP filter
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_HPFilterReset(void) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_HP_FILTER_RESET, &value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetReference
* Description    : Sets Reference register acceleration value as a reference for HP filter
* Input          : Value of reference acceleration value (0-255)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetReference(i8_t ref) {
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_REFERENCE_REG, ref) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : AIS328DQ_HPM_NORMAL_MODE_RES/AIS328DQ_HPM_REF_SIGNAL/AIS328DQ_HPM_NORMAL_MODE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetHPFMode(AIS328DQ_HPFMode_t hpm) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0x9F;
  value |= (hpm<<AIS328DQ_HPM);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : AIS328DQ_HPFCF [0,3]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetHPFCutOFF(AIS328DQ_HPFCutOffFreq_t hpf) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0xFC;
  value |= (hpf<<AIS328DQ_HPCF);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2HPEnable
* Description    : Set Interrupt2 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : AIS328DQ_SetInt2HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2HPEnable(State_t stat) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0xF7;
  value |= stat<<AIS328DQ_HPEN2 ;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}     


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1HPEnable
* Description    : Set Interrupt1 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : AIS328DQ_SetInt1HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1HPEnable(State_t stat) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return MEMS_ERROR;
  
  value &= 0xFB;
  value |= stat<<AIS328DQ_HPEN1 ;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}  


/*******************************************************************************
* Function Name  : AIS328DQ_Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_Int1LatchEnable(State_t latch) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0xFB;
  value |= latch<<AIS328DQ_LIR1;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_Int2LatchEnable
* Description    : Enable Interrupt 2 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_Int2LatchEnable(State_t latch) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return MEMS_ERROR;
  
  value &= 0xDF;
  value |= latch<<AIS328DQ_LIR2;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_ResetInt1Latch(void) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_ResetInt2Latch
* Description    : Reset Interrupt 2 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_ResetInt2Latch(void) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Configuration
* Description    : Interrupt 1 Configuration (without 6D_INT)
* Input          : AIS328DQ_INT_AND/OR | AIS328DQ_INT_ZHIE_ENABLE/DISABLE | AIS328DQ_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use ALL input variable in the argument, as in example above
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1Configuration(AIS328DQ_IntConf_t ic) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, &value) )
    return MEMS_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Configuration
* Description    : Interrupt 2 Configuration (without 6D_INT)
* Input          : AIS328DQ_INT_AND/OR | AIS328DQ_INT_ZHIE_ENABLE/DISABLE | AIS328DQ_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2Configuration(AIS328DQ_IntConf_t ic) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, &value) )
    return MEMS_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Mode
* Description    : Interrupt 1 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : AIS328DQ_INT_MODE_OR, AIS328DQ_INT_MODE_6D_MOVEMENT, AIS328DQ_INT_MODE_AND, AIS328DQ_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1Mode(AIS328DQ_IntMode_t int_mode) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, &value) )
    return MEMS_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<AIS328DQ_INT_6D);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Mode
* Description    : Interrupt 2 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : AIS328DQ_INT_MODE_OR, AIS328DQ_INT_MODE_6D_MOVEMENT, AIS328DQ_INT_MODE_AND, AIS328DQ_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2Mode(AIS328DQ_IntMode_t int_mode) {
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, &value) )
    return MEMS_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<AIS328DQ_INT_6D);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_Get6DPositionInt1
* Description    : 6D Interrupt 1 Position Detect
* Input          : Byte to be filled with AIS328DQ_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_Get6DPositionInt1(u8_t* val){
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case AIS328DQ_UP_SX:   
    *val = AIS328DQ_UP_SX;    
    break;
  case AIS328DQ_UP_DX:   
    *val = AIS328DQ_UP_DX;    
    break;
  case AIS328DQ_DW_SX:   
    *val = AIS328DQ_DW_SX;    
    break;
  case AIS328DQ_DW_DX:   
    *val = AIS328DQ_DW_DX;    
    break;
  case AIS328DQ_TOP:     
    *val = AIS328DQ_TOP;      
    break;
  case AIS328DQ_BOTTOM:  
    *val = AIS328DQ_BOTTOM;  
    break;
  }
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_Get6DPositionInt2
* Description    : 6D Interrupt 2 Position Detect
* Input          : Byte to be filled with AIS328DQ_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_Get6DPositionInt2(u8_t* val){
  u8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case AIS328DQ_UP_SX:   
    *val = AIS328DQ_UP_SX;    
    break;
  case AIS328DQ_UP_DX:   
    *val = AIS328DQ_UP_DX;    
    break;
  case AIS328DQ_DW_SX:   
    *val = AIS328DQ_DW_SX;    
    break;
  case AIS328DQ_DW_DX:   
    *val = AIS328DQ_DW_DX;    
    break;
  case AIS328DQ_TOP:     
    *val = AIS328DQ_TOP;      
    break;
  case AIS328DQ_BOTTOM:  
    *val = AIS328DQ_BOTTOM;   
    break;
  }
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1Threshold(u8_t ths) {
  if (ths > 127)
    return MEMS_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_THS, ths) )
    return MEMS_ERROR;    
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt1Duration(u8_t id) {
  if (id > 127)
    return MEMS_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_DURATION, id) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Threshold
* Description    : Sets Interrupt 2 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2Threshold(u8_t ths) {
  if (ths > 127)
    return MEMS_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_THS, ths) )
    return MEMS_ERROR;    
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Duration
* Description    : Sets Interrupt 2 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_SetInt2Duration(u8_t id) {
  if (id > 127)
    return MEMS_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_DURATION, id) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetStatusReg
* Description    : Read the status register
* Input          : char to empty by Status Reg Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetStatusReg(u8_t* val) {
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_STATUS_REG, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : AIS328DQ_GetStatusBIT
* Description    : Read the status register BIT
* Input          : AIS328DQ_STATUS_REG_ZYXOR, AIS328DQ_STATUS_REG_ZOR, AIS328DQ_STATUS_REG_YOR, AIS328DQ_STATUS_REG_XOR,
                   AIS328DQ_STATUS_REG_ZYXDA, AIS328DQ_STATUS_REG_ZDA, AIS328DQ_STATUS_REG_YDA, AIS328DQ_STATUS_REG_XDA, 
                   AIS328DQ_DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetStatusBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_STATUS_REG, &value) )
    return MEMS_ERROR;
  
  switch (statusBIT){
  case AIS328DQ_STATUS_REG_ZYXOR:     
    if(value &= AIS328DQ_STATUS_REG_ZYXOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_ZOR:       
    if(value &= AIS328DQ_STATUS_REG_ZOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_YOR:       
    if(value &= AIS328DQ_STATUS_REG_YOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }                                 
  case AIS328DQ_STATUS_REG_XOR:       
    if(value &= AIS328DQ_STATUS_REG_XOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_ZYXDA:     
    if(value &= AIS328DQ_STATUS_REG_ZYXDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_ZDA:       
    if(value &= AIS328DQ_STATUS_REG_ZDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_YDA:       
    if(value &= AIS328DQ_STATUS_REG_YDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_XDA:       
    if(value &= AIS328DQ_STATUS_REG_XDA){     
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
* Function Name  : AIS328DQ_GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AccAxesRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetAccAxesRaw(AccRaw_t* buff) {

    uint8 response = MEMS_SUCCESS;

    response = AIS328DQ_ReadReg_Multi(AIS328DQ_MEMS_I2C_ADDRESS,AIS328DQ_OUT_X_L,acc_data,6);

    buff->sumX += (i16_t)(acc_data[0] + (acc_data[1] << 8));

    //value = (i16_t)(gyro_out[2] + (gyro_out[3] << 8));
    //TracePrintf(GYROACC_TRACE,TRACE_INFO,"Gyro:Z=%d\r\n",value);
    buff->sumY += (i16_t)(acc_data[2] + (acc_data[3] << 8));

    //value = (i16_t)(gyro_out[4] + (gyro_out[5] << 8));
    //TracePrintf(GYROACC_TRACE,TRACE_INFO,"Gyro:Y=%d\r\n",value);
    buff->sumZ += (i16_t)(acc_data[4] + (acc_data[5] << 8));
    ++buff->count;
/*
    buff->AXIS_X_L = gyro_out[0];
    buff->AXIS_X_H = gyro_out[1];
    buff->AXIS_Y_L = gyro_out[2];
    buff->AXIS_Y_H = gyro_out[3];
    buff->AXIS_Z_L = gyro_out[4];
    buff->AXIS_Z_H = gyro_out[5];
*/
/*
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_X_L, &(buff->AXIS_X_L)) )
    return MEMS_ERROR;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_X_H, &(buff->AXIS_X_H)) )
    return MEMS_ERROR;

  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_Y_L, &(buff->AXIS_Y_L)) )
    return MEMS_ERROR;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_Y_H, &(buff->AXIS_Y_H)) )
    return MEMS_ERROR;

  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_Z_L, &(buff->AXIS_Z_L)) )
    return MEMS_ERROR;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_OUT_Z_H, &(buff->AXIS_Z_H)) )
    return MEMS_ERROR;
*/
  return response;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : buffer to empty by Int1 Source Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetInt1Src(u8_t* val) {
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt2Src
* Description    : Reset Interrupt 2 Latching function
* Input          : buffer to empty by Int2 Source Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetInt2Src(u8_t* val) {
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt1SrcBit
* Description    : Reset Interrupt 1 Latching function
* Input          : AIS328DQ_INT1_SRC_IA, AIS328DQ_INT1_SRC_ZH, AIS328DQ_INT1_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetInt1SrcBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return MEMS_ERROR;
  
  if(statusBIT == AIS328DQ_INT_SRC_IA){
    if(value &= AIS328DQ_INT_SRC_IA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZH){
    if(value &= AIS328DQ_INT_SRC_ZH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZL){
    if(value &= AIS328DQ_INT_SRC_ZL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YH){
    if(value &= AIS328DQ_INT_SRC_YH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YL){
    if(value &= AIS328DQ_INT_SRC_YL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XH){
    if(value &= AIS328DQ_INT_SRC_XH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XL){
    if(value &= AIS328DQ_INT_SRC_XL){     
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
* Function Name  : AIS328DQ_GetInt2SrcBit
* Description    : Reset Interrupt 2 Latching function
* Input          : AIS328DQ_INT_SRC_IA, AIS328DQ_INT_SRC_ZH, AIS328DQ_INT_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t1 AIS328DQ_GetInt2SrcBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return MEMS_ERROR;
  
  if(statusBIT == AIS328DQ_INT_SRC_IA){
    if(value &= AIS328DQ_INT_SRC_IA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZH){
    if(value &= AIS328DQ_INT_SRC_ZH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZL){
    if(value &= AIS328DQ_INT_SRC_ZL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YH){
    if(value &= AIS328DQ_INT_SRC_YH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YL){
    if(value &= AIS328DQ_INT_SRC_YL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XH){
    if(value &= AIS328DQ_INT_SRC_XH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XL){
    if(value &= AIS328DQ_INT_SRC_XL){     
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
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
