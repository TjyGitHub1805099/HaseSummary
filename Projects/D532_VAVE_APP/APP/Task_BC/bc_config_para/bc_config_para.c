#include "bc_config_para.h"
#include "Ea.h"
#include "bc_common.h"
#include "NvM.h"
#include "MemMap.h"
#include "NvM_if.h"
#include "dea.h"
#if 1
#define BC_CONFIG_PARA_STRU_INIT \
	.u8ValidFlag = MAGIC_BYTE,							\
	.VelModuleRc = 3377,	/* factor 0.0001	*/		\
	.VelModuleRm1 = 3387,	/* factor 0.0001	*/		\
	.VelModuleRm2 = 3500,	/* factor 0.0001	*/		\
	.DteModuleDR = 50,		/* factor 0.01 unit:L	*/	\
	.DteModuleSM = 460,		/* factor 0.01 unit:L	*/	\
	.BestFuelT1 = 30,		/* factor 1 unit:second	*/	\
	.EchoRankT1 = 10,		/* factor 1 unit:min	*/	\
	.EchoRankE1 = 60,		/* factor 1 unit:%	*/	\
	.EchoRankE2 = 75,		/* factor 1 unit:%	*/	\
	.EchoRankE3 = 85,		/* factor 1 unit:%	*/	\
	.EchoRankE4 = 95,		/* factor 1 unit:%	*/	\
	.TachoA = 1000,	 		/* factor 1	*/	\
	.TachoB = 500,	 		/* factor 1	*/	\
	.TachoC = 0,	 		/* factor 1	*/	\
	.TachoD = 100,	 		/* factor 1	*/	\
	.TachoE = 350,	 		/* factor 1	*/	\
	/* end init */

const BC_CONFIG_PARA_STRU g_bc_config_para_init = {
		BC_CONFIG_PARA_STRU_INIT
};


NVM_SEC_VAR_UNSPECIFIED BC_CONFIG_PARA_STRU g_bc_config_para = {
		BC_CONFIG_PARA_STRU_INIT
};

void bc_config_para_init(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		if(g_bc_config_para.u8ValidFlag != MAGIC_BYTE)
		{
			g_bc_config_para = g_bc_config_para_init;
		}
	}
}

#else
void bc_config_para_init(void)
{
}
#endif
