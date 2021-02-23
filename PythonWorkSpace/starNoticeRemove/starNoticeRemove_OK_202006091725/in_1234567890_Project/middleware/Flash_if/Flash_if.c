

#include "Std_Types.h"
#include "Delay.h"
#include "flash_cfg.h"
#include "interrupt_manager.h"
#include "Flash_if.h"



static flash_ssd_config_t pSSDConfig;

static flash_callback_t pCallBack;

#if 1
/* Function declarations */
void CCIF_Handler(void);

/* If target is flash, insert this macro to locate callback function into RAM */
START_FUNCTION_DECLARATION_RAMSECTION
void CCIF_Callback(void)
END_FUNCTION_DECLARATION_RAMSECTION

#endif

void CCIF_Handler(void)
{
    /* Disable Flash Command Complete interrupt */
    FTFx_FCNFG &= (~FTFx_FCNFG_CCIE_MASK);

    return;
}

/*!
  \brief Callback function for Flash operations
*/
START_FUNCTION_DEFINITION_RAMSECTION
void CCIF_Callback(void)
{
    /* Enable interrupt for Flash Command Complete */
    if ((FTFx_FCNFG & FTFx_FCNFG_CCIE_MASK) == 0u)
    {
        FTFx_FCNFG |= FTFx_FCNFG_CCIE_MASK;
    }
}
END_FUNCTION_DEFINITION_RAMSECTION

void DevAssert2(volatile bool x,uint8 checkPoint)
{
    if(x)
    {

    }
    else
    {
    	BKPT_ASM;
    	for(;;)
    	{

    	}
    }
}

void Flash_If_Init()
{
	
	/* Install interrupt for Flash Command Complete event */
	INT_SYS_InstallHandler(FTFC_IRQn, CCIF_Handler, (isr_t*) 0);
	INT_SYS_EnableIRQ(FTFC_IRQn);

	/* Enable global interrupt */
	INT_SYS_EnableIRQGlobal();

	/* Always initialize the driver before calling other functions */
	FLASH_DRV_Init(&flash_cfg_InitConfig0, &pSSDConfig);
	
	pCallBack = (flash_callback_t)CCIF_Callback;




}

boolean Flash_Sector_Programing(uint32 addr, uint8 *data, uint16 wr_size )
{
	static status_t ret=STATUS_ERROR;
	uint32 address,size,failAddr;
    /* Set callback function before a long time consuming flash operation
     * (ex: erasing) to let the application code do other tasks while flash
     * in operation. In this case we use it to enable interrupt for
     * Flash Command Complete event 
     */
	  pSSDConfig.CallBack = pCallBack;
	
	  /* Erase the PFlash sector */
	  //address = 62u * FEATURE_FLS_PF_BLOCK_SECTOR_SIZE; //0x3EC80
	  address = addr;
	  size = FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;
	  ret = FLASH_DRV_EraseSector(&pSSDConfig, address, size);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }
//	  DEV_ASSERT2(STATUS_SUCCESS == ret,1);
	
	  /* Disable Callback */
	  pSSDConfig.CallBack = NULL_CALLBACK;

	  /* Verify the erase operation at margin level value of 1, user read */
	  ret = FLASH_DRV_VerifySection(&pSSDConfig, address, size / FTFx_DPHRASE_SIZE, 1u);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }
//	  DEV_ASSERT2(STATUS_SUCCESS == ret,1);
	
	  /* Write some data to the erased PFlash sector */
	  size = wr_size;
	  ret = FLASH_DRV_Program(&pSSDConfig, address, size, data);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }
//	  DEV_ASSERT2(STATUS_SUCCESS == ret,1);

	  /* Verify the program operation at margin level value of 1, user margin */
	  ret = FLASH_DRV_ProgramCheck(&pSSDConfig, address, size, data, (uint32_t*)&failAddr, 1u);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }
//	  DEV_ASSERT2(STATUS_SUCCESS == ret,1);

	  return (ret);
}

//Enter HSRUN power mode
void EnterPowerMode(uint8 cType)
{
	switch(cType)
	{
	case ePowerMode_HSRUN:
		POWER_SYS_SetMode(ePowerMode_HSRUN, POWER_MANAGER_POLICY_AGREEMENT);  //active HSRUN mode		
		break;

	case ePowerMode_VLPS:
		POWER_SYS_SetMode(ePowerMode_VLPS, POWER_MANAGER_POLICY_AGREEMENT);  //active VLPS mode	
		break;
		
	case ePowerMode_RUN:
		POWER_SYS_SetMode(ePowerMode_RUN, POWER_MANAGER_POLICY_AGREEMENT);  //active RUN mode	
		break;

	default:
		break;
	}
}




/*******************************************************************************
 * EOF
 ******************************************************************************/
