#include"uds_cfg.h"
#include"system_task_cfg.h"

#define DIAG_APP_TASK_ID 1//SYSTEM_TASK_ID_DIAGAPP
#define DTC_CORE_TASK_ID SYSTEM_TASK_ID_DTC

//to fix addr align issue
//#ifdef SW_PHASE_APPLICATION
uint8 gUdsRxTxBuffer[UDS_PHY_BUFF_LEN] = {1};
//#else // bootloader
//uint16 sUDSRxTxBuffer[UDS_PHY_BUFF_LEN/2];
//#endif


const tUdsCfgInfor gUdsCfgInfor ={
	.udsWorkMode= UDS_WORK_MODE_IN_APP,
	.udsRxTxBufferLen = UDS_PHY_BUFF_LEN,
	.enableBroadcastUdsCnnStatus = FALSE,
	.sigId = 0,
	.enableRespNrcIfChBusy = FALSE,
	.enableRespNrcSNSIASandSFNSIASforFuncAddrReq = TRUE,
	.enableRespNrc0x78 = TRUE,
	.p2ServerMaxTimerValue = UDS_TIMER_VALUE_P2_SERVER_MAX,
	.p2ServerMaxEnhancedTimerValue = UDS_TIMER_VALUE_P2_SERVER_MAX_ENHANCED,
	.s3ServerTimerValue = UDS_TIMER_VALUE_S3_SERVER,
	.udsResendCounter = UDS_RESNED_COUNTER,
	.diagAppTaskId = DIAG_APP_TASK_ID,
	.dtcCoreTaskId = DTC_CORE_TASK_ID,
	.udsLogLevel = UDS_LOG_LEVEL_1,
};

