#ifndef _DIAG_APP_IPC_PROCESS_H
#define _DIAG_APP_IPC_PROCESS_H


#include"Platform_Types.h"
#include"Task_M_IpcApp_Cfg.h"
#include"Task_IpcApp_Cfg.h"
#include"sid0x22_cfg.h"


#define IPC_CHANNEL_DIAG IPC_CH_DIAG


//==============================================================================
#define IVI_IPC_UDS_SETUP_CERTIFICATE_STATE     (101)
#define IVI_IPC_UDS_GET_VIN                     (102)
#define IVI_IPC_AME_READ     (104)
#define IVI_IPC_AME_WRITE    (105)
#define IVI_IPC_AME_CTRL     (106)
#define IVI_IPC_UDS_READ     (107)
#define IVI_IPC_UDS_WRITE    (108)
#define IVI_IPC_UDS_CTRL     (109)

//==============================================================================
#define IC_IPC_UDS_READ      (7)
#define IC_IPC_UDS_WRITE     (8)
#define IC_IPC_UDS_CTRL      (9)
#define IC_IPC_AME_READ      (10)
#define IC_IPC_AME_WRITE     (11)
#define IC_IPC_AME_CTRL      (12)
//==============================================================================
#define IC_IPC_AUTOTEST_READ (13)
#define IC_IPC_AUTOTEST_CTRL (14)

#define AVM_IPC_CALIBRATION_REQ  (1)
#define AVM_IPC_CALIBRATION_OR_ERR_RESP  (1)

#define AVM_IPC_QUERY_CAN_MSG_STATUS_REQ (0)
#define AVM_IPC_QUERY_CAN_MSG_STATUS_RESP (0)

#define AVM_IPC_POWER_CTRL_REQ  (2)
#define AVM_IPC_POWER_CTRL_RESP  (2)

#define AVM_IPC_SWVERSION_REQ     (3)
#define AVM_IPC_SWVERSION_RESP    (3)
#define AVM_IPC_PROG_DATE_REQ        (4)
#define AVM_IPC_PROG_DATE_RESP       (4)
#define AVM_IPC_CAN_MSG_STATE_REQ	(5)
#define AVM_IPC_CAN_MSG_STATE_RESP	(5)

#define AVM_IPC_ORIGINAL_STATUS_REQ_1   (6)
#define AVM_IPC_ORIGINAL_STATUS_RESP_1  (6)


#define AVM_IPC_DIAGNOSTIC_CTRL_REQ     (7)
#define AVM_IPC_DIAGNOSTIC_CTRL_RESP    (7)

#define AVM_IPC_ORIGINAL_STATUS_REQ_2   (8)
#define AVM_IPC_ORIGINAL_STATUS_RESP_2  (8)


#define AVM_IPC_GET_PHOTO_STATE_REQ  	(0x09)//add by tjy @2019-9-10
#define AVM_IPC_CAPTURE_STATE_NOTIFY    (0x0A)//add by tjy @2019-9-10
#define AVM_IPC_PHOTO_RESULT_NOTIFY     (0x0B)//add by tjy @2019-9-10



#define AVM_IPC_UDS_READ     (13)
#define AVM_IPC_UDS_WRITE    (14)


#define HUD_IPC_CALIBRATION_OR_RESET_REQ  (0x31)
#define HUD_IPC_CALIBRATION_OR_RESET_RESP (0x31)


#define HUD_IPC_MAIN_SW_ON_OFF_REQ  	   (0x2E)
#define HUD_IPC_MAIN_SW_ON_OFF_RESP        (0x2E)

#define HUD_IPC_WRITE_CAL_DATA_REQ  	   (0x2E)
#define HUD_IPC_WRITE_CAL_DATA_RESP        (0x2E)

#define HUD_IPC_READ_CAL_DATA_REQ  	   	   (0x22)
#define HUD_IPC_READ_CAL_DATA_RESP         (0x22)

#define HUD_IPC_REPORT_CALIBRATION_STATUS  (0x19)

//==============================================================================






void diagAppIpcCbk_SocNotify_IVI(uint16 function, uint8* rx_data, uint32 len);
void diagAppIpcCbk_SocRequest_IVI(const void *ctx, uint8 *rx_data, uint32 len);
//void diagAppIpcCbk_McuRequest(const void* ctx, uint16 function, uint8* rx_data, uint32 len);
void IpcCbk_SocReadMcuReply(uint16 function, uint8* rx_data, uint16 len);
void IpcCbk_SocWriteMcuReply(uint16 function, uint8* rx_data, uint16 len);




//Communicate with AVM via IPC
void diagApp_AVM_IpcNotifyInd(uint16 function, uint8* rx_data, uint32 len);
void diagApp_AVM_IpcRequestInd(const void *ctx, uint8 *rx_data, uint32 len);
void diagApp_AVM_IpcResp(const void* ctx, uint16 function, uint8* rx_data, uint32 len);

//Auto test communicate with SOC
void diagAppIpcCbk_AutoTestMcuRequest(const void* ctx, uint16 function, uint8* rx_data, uint32 len);

//Communicate with HUD via IPC
void diagApp_HUD_IpcNotifyInd(uint16 function, uint8* rx_data, uint32 len);
void diagAppIpcCbk_HUDCalibrationMcuRequest(const void* ctx, uint16 function, uint8* rx_data, uint32 len);


extern uint8 g_AVM_ProgramingDate[AVM_DID_PROGRAMING_DATE_LEN];                               //define in diagAppIpc_cbk.c line 29
extern uint8 g_AVM_SysSupplierEcuSwVersionNum[AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN];   //define in diagAppIpc_cbk.c line 30
#endif
