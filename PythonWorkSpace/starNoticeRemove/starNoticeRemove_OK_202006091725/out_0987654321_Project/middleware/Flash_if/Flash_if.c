

#include "Std_Types.h"
#include "Delay.h"
#include "flash_cfg.h"
#include "interrupt_manager.h"
#include "Flash_if.h"



static flash_ssd_config_t pSSDConfig;

static flash_callback_t pCallBack;

#if 1

void CCIF_Handler(void);


START_FUNCTION_DECLARATION_RAMSECTION
void CCIF_Callback(void)
END_FUNCTION_DECLARATION_RAMSECTION

#endif

void CCIF_Handler(void)
{
    
    FTFx_FCNFG &= (~FTFx_FCNFG_CCIE_MASK);

    return;
}


START_FUNCTION_DEFINITION_RAMSECTION
void CCIF_Callback(void)
{
    
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
	
	
	INT_SYS_InstallHandler(FTFC_IRQn, CCIF_Handler, (isr_t*) 0);
	INT_SYS_EnableIRQ(FTFC_IRQn);

	
	INT_SYS_EnableIRQGlobal();

	
	FLASH_DRV_Init(&flash_cfg_InitConfig0, &pSSDConfig);
	
	pCallBack = (flash_callback_t)CCIF_Callback;




}

boolean Flash_Sector_Programing(uint32 addr, uint8 *data, uint16 wr_size )
{
	static status_t ret=STATUS_ERROR;
	uint32 address,size,failAddr;
    
	  pSSDConfig.CallBack = pCallBack;
	
	  
	  
	  address = addr;
	  size = FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;
	  ret = FLASH_DRV_EraseSector(&pSSDConfig, address, size);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }

	
	  
	  pSSDConfig.CallBack = NULL_CALLBACK;

	  
	  ret = FLASH_DRV_VerifySection(&pSSDConfig, address, size / FTFx_DPHRASE_SIZE, 1u);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }

	
	  
	  size = wr_size;
	  ret = FLASH_DRV_Program(&pSSDConfig, address, size, data);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }


	  
	  ret = FLASH_DRV_ProgramCheck(&pSSDConfig, address, size, data, (uint32_t*)&failAddr, 1u);
	  if(ret != STATUS_SUCCESS)
	  {
		  return ret;
	  }


	  return (ret);
}


void EnterPowerMode(uint8 cType)
{
	switch(cType)
	{
	case ePowerMode_HSRUN:
		POWER_SYS_SetMode(ePowerMode_HSRUN, POWER_MANAGER_POLICY_AGREEMENT);  
		break;

	case ePowerMode_VLPS:
		POWER_SYS_SetMode(ePowerMode_VLPS, POWER_MANAGER_POLICY_AGREEMENT);  
		break;
		
	case ePowerMode_RUN:
		POWER_SYS_SetMode(ePowerMode_RUN, POWER_MANAGER_POLICY_AGREEMENT);  
		break;

	default:
		break;
	}
}





