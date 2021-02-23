
#include "diagCommon_if.h"
#include "diagAppWllr_cfg.h"
#include "NvM_Cfg.h" 
#include "odo.h"


extern tWlStatusRecord gWlStatusRecords[];

static boolean gIgnitionOn = FALSE;
static boolean gIgnitionStatusSwitchFlag = FALSE;



boolean diagSetWarningLampLightingStatus(tInnerWlId innerID, tWlStatus wlStatus)
{
	if(innerID >= INNER_WL_ID_MAX)
		return FALSE;
	
	if(wlStatus == WL_ON)
	{
		gWlStatusRecords[innerID].wlStatus = WL_ON;
		return TRUE;
	}
	else if(wlStatus == WL_OFF)
	{
		gWlStatusRecords[innerID].wlStatus = WL_OFF;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void diagAppWllrClear()
{
	uint8 innerID = 0;
	
	for(innerID=0; innerID<INNER_WL_ID_MAX; innerID++)
	{
		gWarningLampLightingRecords[innerID].mileage = MILEAGE_INIT_VALUE;
		gWarningLampLightingRecords[innerID].ignCounter = WLLR_INIT_VALUE;
	}
	
	diagNvmWriteSync(gWarningLampLightingRecords_0_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[0]));
	diagNvmWriteSync(gWarningLampLightingRecords_5_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[5]));
	diagNvmWriteSync(gWarningLampLightingRecords_10_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[10]));
	diagNvmWriteSync(gWarningLampLightingRecords_15_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[15]));
	diagNvmWriteSync(gWarningLampLightingRecords_20_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[20]));
	diagNvmWriteSync(gWarningLampLightingRecords_25_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[25]));
	diagNvmWriteSync(gWarningLampLightingRecords_30_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[30]));
	diagNvmWriteSync(gWarningLampLightingRecords_35_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[35]));
	diagNvmWriteSync(gWarningLampLightingRecords_40_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[40]));
	diagNvmWriteSync(gWarningLampLightingRecords_45_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[45]));
	diagNvmWriteSync(gWarningLampLightingRecords_50_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[50]));
	diagNvmWriteSync(gWarningLampLightingRecords_55_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[55]));
}



void wllrMainLoop()
{
	uint8 innerID = 0;
	uint8 index = 0;
	uint8 accStatus = DIAG_ACC_OFF;
	uint8 KL15Status = DIAG_KL15_OFF;
	
	diagGetAccStatus(&accStatus);
	diagGetKL15Status(&KL15Status);

	if(accStatus == DIAG_ACC_OFF)
		return;

	if(KL15Status == DIAG_KL15_ON)
	{
		if(gIgnitionOn == FALSE)
		{
			gIgnitionOn = TRUE;
			gIgnitionStatusSwitchFlag = TRUE;
		}
	}
	else
	{
		if(gIgnitionOn == TRUE)
		{
			gIgnitionOn = FALSE;
			gIgnitionStatusSwitchFlag = TRUE;
		}
	}

	if(gIgnitionOn == TRUE  && gIgnitionStatusSwitchFlag == TRUE)
	{
		for(innerID=0; innerID<INNER_WL_ID_MAX; innerID++)
		{
			if(gWlStatusRecords[innerID].wlStatus == WL_OFF)
			{
				if(gWarningLampLightingRecords[innerID].ignCounter == WLLR_INIT_VALUE)
				{
					continue;
				}
				else if(gWarningLampLightingRecords[innerID].ignCounter < 39)
				{
					if(gWlStatusRecords[innerID].recordedFlag == FALSE)
					{
						gWarningLampLightingRecords[innerID].ignCounter += 1;
						gWlStatusRecords[innerID].recordedFlag = TRUE;
						index = innerID/5;
						switch(index)
						{
							case 0:
								diagNvmWriteSync(gWarningLampLightingRecords_0_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[0]));
								break;
							case 1:
								diagNvmWriteSync(gWarningLampLightingRecords_5_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[5]));
								break;
							case 2:
								diagNvmWriteSync(gWarningLampLightingRecords_10_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[10]));
								break;
							case 3:
								diagNvmWriteSync(gWarningLampLightingRecords_15_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[15]));
								break;
							case 4:
								diagNvmWriteSync(gWarningLampLightingRecords_20_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[20]));
								break;
							case 5:
								diagNvmWriteSync(gWarningLampLightingRecords_25_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[25]));
								break;
							case 6:
								diagNvmWriteSync(gWarningLampLightingRecords_30_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[30]));
								break;
							case 7:
								diagNvmWriteSync(gWarningLampLightingRecords_35_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[35]));
								break;
							case 8:
								diagNvmWriteSync(gWarningLampLightingRecords_40_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[40]));
								break;
							case 9:
								diagNvmWriteSync(gWarningLampLightingRecords_45_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[45]));
								break;
							case 10:
								diagNvmWriteSync(gWarningLampLightingRecords_50_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[50]));
								break;
							case 11:
								diagNvmWriteSync(gWarningLampLightingRecords_55_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[55]));
								break;

							default:
								return;
						}
						continue;
					}
					else
					{
						continue;
					}
				}
				else if(gWarningLampLightingRecords[innerID].ignCounter == 39)
				{
					continue;
				}
				else
				{
					return;
				}
			}
			else
			{
				if(gWarningLampLightingRecords[innerID].ignCounter == 0)
				{
					if(gWarningLampLightingRecords[innerID].mileage == MILEAGE_INIT_VALUE)
					{
						gWarningLampLightingRecords[innerID].mileage = ODO_GetOdo();
						index = innerID/10;
						switch(index)
						{
							case 0:
								diagNvmWriteSync(gWarningLampLightingRecords_0_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[0]));
								break;
							case 1:
								diagNvmWriteSync(gWarningLampLightingRecords_5_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[5]));
								break;
							case 2:
								diagNvmWriteSync(gWarningLampLightingRecords_10_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[10]));
								break;
							case 3:
								diagNvmWriteSync(gWarningLampLightingRecords_15_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[15]));
								break;
							case 4:
								diagNvmWriteSync(gWarningLampLightingRecords_20_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[20]));
								break;
							case 5:
								diagNvmWriteSync(gWarningLampLightingRecords_25_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[25]));
								break;
							case 6:
								diagNvmWriteSync(gWarningLampLightingRecords_30_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[30]));
								break;
							case 7:
								diagNvmWriteSync(gWarningLampLightingRecords_35_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[35]));
								break;
							case 8:
								diagNvmWriteSync(gWarningLampLightingRecords_40_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[40]));
								break;
							case 9:
								diagNvmWriteSync(gWarningLampLightingRecords_45_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[45]));
								break;
							case 10:
								diagNvmWriteSync(gWarningLampLightingRecords_50_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[50]));
								break;
							case 11:
								diagNvmWriteSync(gWarningLampLightingRecords_55_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[55]));
								break;

							default:
								return;
						}
					}
				}
				else
				{
					gWarningLampLightingRecords[innerID].ignCounter = 0;
					gWarningLampLightingRecords[innerID].mileage = ODO_GetOdo();
					index = innerID/10;
					switch(index)
					{
						case 0:
							diagNvmWriteSync(gWarningLampLightingRecords_0_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[0]));
							break;
						case 1:
							diagNvmWriteSync(gWarningLampLightingRecords_5_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[5]));
							break;
						case 2:
							diagNvmWriteSync(gWarningLampLightingRecords_10_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[10]));
							break;
						case 3:
							diagNvmWriteSync(gWarningLampLightingRecords_15_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[15]));
							break;
						case 4:
							diagNvmWriteSync(gWarningLampLightingRecords_20_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[20]));
							break;
						case 5:
							diagNvmWriteSync(gWarningLampLightingRecords_25_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[25]));
							break;
						case 6:
							diagNvmWriteSync(gWarningLampLightingRecords_30_NvMBlockID,  (uint8*)(&gWarningLampLightingRecords[30]));
							break;
						case 7:
							diagNvmWriteSync(gWarningLampLightingRecords_35_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[35]));
							break;
						case 8:
							diagNvmWriteSync(gWarningLampLightingRecords_40_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[40]));
							break;
						case 9:
							diagNvmWriteSync(gWarningLampLightingRecords_45_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[45]));
							break;
						case 10:
							diagNvmWriteSync(gWarningLampLightingRecords_50_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[50]));
							break;
						case 11:
							diagNvmWriteSync(gWarningLampLightingRecords_55_NvMBlockID, (uint8*)(&gWarningLampLightingRecords[55]));
							break;

						default:
							return;
					}
				}
			}
		}
	}

	gIgnitionOn = FALSE;
	gIgnitionStatusSwitchFlag = FALSE;
}



