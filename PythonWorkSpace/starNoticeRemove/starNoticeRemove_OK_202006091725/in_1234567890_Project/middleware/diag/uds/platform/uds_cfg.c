#include"uds_cfg.h"
#include"System_Task_Cfg.h"
#include"diagCommon_cfg.h"

#define DIAG_APP_TASK_ID SYSTEM_TASK_ID_DIAGAPP
#define DTC_CORE_TASK_ID 2 //SYSTEM_TASK_ID_DTC

//to fix addr align issue
//#ifdef SW_PHASE_APPLICATION
uint8 gUdsRxTxBuffer_IVI[UDS_PHY_BUFF_LEN] = {1};
uint8 gUdsRxTxBuffer_IC[UDS_PHY_BUFF_LEN] = {1};
uint8 gUdsRxTxBuffer_AVM[UDS_PHY_BUFF_LEN] = {1};
uint8 gUdsRxTxBuffer_HUD[UDS_PHY_BUFF_LEN] = {1};

uint8 *gUDSBufferPtr[UDS_CH_MAX_NUM] = {gUdsRxTxBuffer_IVI,gUdsRxTxBuffer_IC,gUdsRxTxBuffer_AVM,gUdsRxTxBuffer_HUD};


tUdsChInfor gUdsChInfor[UDS_CH_MAX_NUM] = {
		{UDS_ADDR_TYPE_PHY,FALSE,UDS_CH_STATUS_IDLE,FALSE,2,0,UDS_PHY_BUFF_LEN,(uint8*)gUdsRxTxBuffer_IVI},// IVI PHY
		{UDS_ADDR_TYPE_PHY,FALSE,UDS_CH_STATUS_IDLE,FALSE,2,0,UDS_PHY_BUFF_LEN,(uint8*)gUdsRxTxBuffer_IC},// IC PHY
		{UDS_ADDR_TYPE_PHY,FALSE,UDS_CH_STATUS_IDLE,FALSE,2,0,UDS_PHY_BUFF_LEN,(uint8*)gUdsRxTxBuffer_AVM},//AVM PHY
		{UDS_ADDR_TYPE_PHY,FALSE,UDS_CH_STATUS_IDLE,FALSE,2,0,UDS_PHY_BUFF_LEN,(uint8*)gUdsRxTxBuffer_HUD},//HUD PHY
};

tUdsTimer gUdsSessionTimer[UDS_CH_MAX_NUM];
uint8 sUDSNRCBuffer[UDS_CH_MAX_NUM][3];
uint16 gRespDataLen[UDS_CH_MAX_NUM];
uint32 gUdsRxNewDataTick[UDS_CH_MAX_NUM];
uint8 gSid[UDS_CH_MAX_NUM];
boolean gSuppressPosRespBit[UDS_CH_MAX_NUM];
boolean gRespNRC0x78Flag[UDS_CH_MAX_NUM];
tDiagMutex gUdsSidMutex[UDS_CH_MAX_NUM];
boolean gEnableInnerSidFlag = FALSE;

const tUdsCfgInfor gUdsCfgInfor ={
	.maxChNum = UDS_CH_MAX_NUM,
	.udsWorkMode= UDS_WORK_MODE_IN_APP,
	.udsRxTxBufferLen = UDS_PHY_BUFF_LEN,
	.enableBroadcastUdsCnnStatus = FALSE,
	.sigId = 0,
	.enableRespNrcIfChBusy = FALSE,
	.enableRespNrcSNSIASandSFNSIASforFuncAddrReq = TRUE,
	.enableRespNrc0x78 = FALSE,
	.p2ServerMaxTimerValue = UDS_TIMER_VALUE_P2_SERVER_MAX,
	.p2ServerMaxEnhancedTimerValue = UDS_TIMER_VALUE_P2_SERVER_MAX_ENHANCED,
	.s3ServerTimerValue = UDS_TIMER_VALUE_S3_SERVER,
	.udsResendCounter = UDS_RESNED_COUNTER,
	.diagAppTaskId = DIAG_APP_TASK_ID,
	.dtcCoreTaskId = DTC_CORE_TASK_ID,
	.udsLogLevel = UDS_LOG_LEVEL_1,
	.udsChInfor = gUdsChInfor,
};

