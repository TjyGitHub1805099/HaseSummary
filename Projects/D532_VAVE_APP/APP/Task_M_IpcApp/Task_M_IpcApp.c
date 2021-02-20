/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_M_IpcApp.c
Author: Stephen Du
Version: V1.0
Timestamp: 2018-12-04 11:08:55
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#include "System_Task.h"
#include "System_Signal.h"
#include "Task_M_IpcApp_Cfg.h" 
#if (STD_ON == TASK_M_IPCAPP_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/
#include "Spi_Ipc_gw_api.h"
#include "System_Signal.h"
#include "Swdl.h"
#include "download_if.h"
#include "diagCommon.h"
#include "PanelKey.h"
#include "AC_CTL.h"
#include"CanApp.h"
#include "ExternRTC.h"
#include "pmStateMachine_If.h"
#include "Task_PowerModing_Cfg.h"
#include "gyroacc_service.h"
#include "diagAppIpc_cbk.h"
#include "SWVersion.h"
#include "HardwareVersion.h"
#include "pmStateMachine_If.h"

#include "NvM.h"
#include "NvM_if.h"
#include "ipc_hb_with_ivi_if.h"
#include "SecondPanel.h"
#include "backram_var.h"
#include "BoardDefines.h"
#include "dea.h"
#include "TraceStore.h"

//#include "HardwareVersion.h"

extern gCircuitVersion;
extern gVariantCtrlVersion;

extern uint8 gCurrentDuty;
extern boolean gIgn_Off_UpdateFlag;


/************ END: INCLUDE FILES *************/

typedef enum{
	SYS_APP_STATUS = 0x00,
	SYS_USB_SWDL_STATUS=0x01,
	SYS_MFG_SWDL_STATUS=0x02
}SysStatus;

//========================================logic of soc serial num control : start========================================
extern uint8 g_AVM_ProgramingDate[];
extern uint8 g_AVM_SysSupplierEcuSwVersionNum[];
extern boolean RVM_CammeraOnOffFlag;
#define SOC_SERIAL_NUM_DATA_LEN	20 // example:VHT09A18A0010001
uint8 g_SerialNumInfoReadWrite = FALSE;
NVM_SEC_VAR_UNSPECIFIED uint8 socSerialNum[SOC_SERIAL_NUM_DATA_LEN]="VHT09A18A0010001";
ExternSTDType gRtcData;

 void SerialNum_MainFunction(void)
 {
	 if( g_SerialNumInfoReadWrite == IPC_SW_SYSTEM_FUCTION_GET_SERIAL_NUMBER )
	 {
		 //read socSerialNum[]
		 ipc_send_notification(GYROCT_IPC_HW_CHANNEL,GYROCT_IPC_SW_CHANNEL,IPC_SW_SYSTEM_FUCTION_GET_SERIAL_NUMBER,&socSerialNum[0],SOC_SERIAL_NUM_DATA_LEN);
		 TracePrintf(NM_TRACE, TRACE_WARNING, "[GPS-I]:SOC-R-SerialNum\r\n");
	 }else if( g_SerialNumInfoReadWrite == IPC_SW_SYSTEM_FUCTION_SET_SERIAL_NUMBER )
	 {
		 //write socSerialNum[]
		 NvM_WriteBlock(socSerialNumNvMBlockID,&socSerialNum[0]);
		 //NvM_SetRamBlockStatus(socSerialNumNvMBlockID,STD_ON);//set block data changed,trriger power off write in E2
		 //NvM_ReadBlock(socSerialNumNvMBlockID,&socSerialNum[0]);
		 TracePrintf(NM_TRACE, TRACE_WARNING, "[GPS-I]:SOC-W-SerialNum\r\n");
	 }
	 g_SerialNumInfoReadWrite = FALSE;
 }
 //========================================logic of soc serial num control : end========================================

 void SWDl_IPCCallback(uint16 function, uint8 *rx_data, uint32 len)
 {
 	uint8  RespondData[4]={0};
	  switch(function) 
	 {		
	 		case 0x05:
				if(rx_data[0]==SID_ECU_RESET)
				{
					TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[M_IPC-I]:SwdlEcuReset\r\n");
					BACKRAM_IPC_FLAG = 0;
					sid0x31IpcUpdater();
				}
			break;
			
			case 0x06:
				TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[M_IPC-I]:SwdlGetCurStatus\r\n");
				RespondData[0] = rx_data[0] + 0x40;
				RespondData[1] = rx_data[1];
				RespondData[2] = rx_data[2];
				RespondData[3] = SYS_APP_STATUS;
				ipc_send_notification(1,IPC_GW_CHANNEL_SWDL,function,&RespondData,4);
			break;
			
			case 0x07:
				RespondData[0] = rx_data[0] + 0x40;
				RespondData[1] = rx_data[1];
				RespondData[2] = rx_data[2];
				ipc_send_notification(1,IPC_GW_CHANNEL_SWDL,function,&RespondData,3);
				if(rx_data[2] == 0x00)
				{
					TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[M_IPC-I]:SwdlClearFlag\r\n");
					SetUsbUpdataFlag(STD_OFF);
				}
				else if(rx_data[2] == 0x01)
				{
					TracePrintf(M_IPC_TRACE, TRACE_WARNING, "[M_IPC-I]:SwdlSetUsbFlag\r\n");
					SetUsbUpdataFlag(STD_ON);
				}
			break;

			case 0x08: //OTA update notification
				if(len >= 2)
				{
					if(rx_data[0] == 0x01)
						gIgn_Off_UpdateFlag = TRUE;
					else if(rx_data[0] == 0x00)
						gIgn_Off_UpdateFlag = FALSE;

					if(rx_data[1] == 0x01)
						SetUsbUpdataFlag(STD_ON);
					else if(rx_data[1] == 0x00)
						SetUsbUpdataFlag(STD_OFF);
				}
				break;


			case 0x20:
				DEA_SetIpcRecoverFlag(1);
				ipc_send_notification(1,IPC_GW_CHANNEL_SWDL, function, RespondData, 2);
				break;
			default: 
			break;
			
	}	
 }


#if 1
void KEY_IPCCallback(uint16 function, uint8 *rx_data, uint32 len)
{
	switch(function) 
	{

	#if 0
		case ACCT_IPC_SW_FUNCTION_ID:
			if( len == ACCT_IPC_SW_FUNCTION_DATA_LEN )
			{
				daFrame.newCmd = TRUE;
				memcpy(&daFrame.rxHMICmd[0],rx_data,len);
			}
		break;

	#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)
			case IPC_SW_KEYSERVICE_FUCTION_MONITOR:/* ioc monitor with soc function */
			if( len == IOC_MONITOR_SOC_DATA_LEN )
			{
					IocMonitorSocFrame.newCmd = TRUE;
					memcpy(&IocMonitorSocFrame.rxHMICmd[0],rx_data,len);
			}
		break;
	#endif
	#endif

		#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)
			case IPC_SW_KEYSERVICE_FUCTION_MONITOR:/* ipc monitor with ivi */
				if( len == IOC_MONITOR_SOC_DATA_LEN )
				{
					ipc_hb_with_ivi_update(rx_data,len);
				}
			break;
	   #endif


		#if (STD_ON == TRACE_STORE_USING )
			case TRACESTORE_IPC_SW_FUNCTION_ID_RECV:
				//TraceStore_traceStartSet(STD_ON);
			break;
		#endif


		default:
			break;
	}
}
#endif

#if 1
void IPC_PM_callback(const void *ctx, uint8 *rx_data, uint32 len)
{
#if 1
	uint8 buff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
   uint8 gCurrentDataBuf = 0;
	
   // SYSTIM  l_curTime;
   // uint16 curTime[2];

    request_context_t *l_ctx = (request_context_t*)ctx;

    switch(l_ctx->function_id)
	{

        case IPC_RX_SYS_UPDATE:
            if(l_ctx != NULL)
            {
               // ipc_gw_send_reply(l_ctx, buff, 8);
            }
            //System_Signal_Send(SYSTEM_SIG_ID_UPDATE,STD_ACTIVE,STD_OFF);
            break;

	    case IPC_RX_SYS_GETSYSINFO:
			 TraceStore_traceStartSet(STD_ON);
			 PM_setSocStatus();
             buff[0] = PM_GetCurrentMode();
			 buff[1] = PM_GetAccStatus();
			 buff[2] = PM_GetIgnStatus();
			 buff[3] = PM_GetTcuAccStatus();
			 buff[4] = DA_PM_GetLcdStatus();
			 buff[5] = PM_GetAmpStatus();
			 buff[6] = PM_GetReverseStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,8);
             break;

        case IPC_RX_SYS_GETMODE:
             buff[0] = PM_GetCurrentMode();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

		case IPC_RX_GET_ACCSTATUS:
             buff[0] = PM_GetAccStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

		case IPC_RX_GET_IGNSTATUS:
             buff[0] = PM_GetIgnStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

		case IPC_RX_GET_TCUACCSTATUS:
             buff[0] = PM_GetTcuAccStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

	    case IPC_RX_GET_LCDSTATUS:
			 buff[0] = DA_PM_GetLcdStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

		case IPC_RX_GET_AMPSTATUS:
			 buff[0] = PM_GetAmpStatus();
             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;

		case IPC_RX_SET_LCDONOFF:
			//modified by tjy @2020-1-6
			//1.when SOC recv ALL_OFF,it should send this order to power off TFT
			//2.this order must execute 3000ms in SOC, then SOC notify IOC can to sleep
			if( PM_MODE_ALL_OFF == PM_GetCurrentMode() )
			{
				DA_TftBackLightOnOff((*rx_data));
				buff[0] = DA_PM_GetLcdStatus();
				ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);			
			}
            break;

		case IPC_RX_SET_AMPONOFF:
			 PM_AMP_OnOff((*rx_data));
	             buff[0] = PM_GetAmpStatus();
	             ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
             break;
		case IPC_RX_GET_REVERSE_STATUS:
			buff[0] = PM_GetReverseStatus();
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
			break;

		case IPC_RX_GET_VOLTAGE_STATUS:
			buff[0] = PM_GetVoltStatus();
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,1);
			break;
			
		case IPC_RX_SET_FAN_SPEED:
			//buff[0] = *rx_data;
			gCurrentDuty = *rx_data;
				ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, &gCurrentDuty,1);
			break;
		case IPC_RX_GET_FAN_SPEED:
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, &gCurrentDuty,1);
			break;
		case IPC_RX_ON_TOUCH_EVENT:
			gCurrentDataBuf = *rx_data;
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, &gCurrentDataBuf,1);
			PM_SetTouchScreenStateSignal();
			break;
		case IPC_RX_SET_RVM_ONOFF:
			if(NULL != rx_data)
			{
				RVM_CammeraOnOff(rx_data[0]);
				ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, &RVM_CammeraOnOffFlag,1);
			}
			break;
		case IPC_RX_GET_RVM_STATUS:
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, &RVM_CammeraOnOffFlag,1);
			break;
			
		case IPC_RX_RESTART:  //when DA TouchSreen Power button long set, Reset IOC add by hyl,@2019-9-28
			if((len <= 0) || (NULL == rx_data))
			{
				break;
			}
			gCurrentDataBuf = *rx_data;
			if(0xff == gCurrentDataBuf)
			{
				g_BootLoaderJumpCmd = JUMP_TO_APP_AT_ONCE;
				SysResetWrapper(IPC_RESTART);
			}
			break;
			
		case IPC_RX_GET_RTC_TIME:
			ExtrRtc_StdTimeGet(&gRtcData);
			buff[0] = gRtcData.year;
			buff[1] = gRtcData.month;
			buff[2] = gRtcData.week;
			buff[3] = gRtcData.day;
			buff[4] = gRtcData.hour;
			buff[5] = gRtcData.minutes;
			buff[6] = gRtcData.second;
			ipc_gw_send_reply(DA_IPC_HW_CH,l_ctx, buff,7);
			break;
			
        default:
                break;
   }
#endif
}
#endif



extern uint8 GyroAcceData_TxEn;
void SYSTEM_IpcRxIndication(const void *ctx, uint8 *data, uint32 len)
{
	//uint8 hardWareVersion[2] = {0xFF,0xFF}; 
	uint8 app_boot_sw_version[IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2] = {0};
	#if debugger
		TracePrintf(CANAPP_TRACE, TRACE_WARNING, "Call Back ==> CanApp_IpcRxIndication\r\n");
	#endif
	//request_context_t *context = (request_context_t*)ctx;
	//if ((data == NULL) || (ctx == NULL)) {
	if ( (ctx == NULL)) {
		#if debugger
			TracePrintf(CANAPP_TRACE, TRACE_WARNING, "recv data is Error!\r\n");
		#endif
		return;
	}

	request_context_t *context = (request_context_t*)ctx;
	
	if(context->channel == IPC_GW_CHANNEL_SYSTEM )
	{
		switch(context->function_id)
		{
			//soc read, write RTC std time
			case RTCCT_IPC_SW_FUNCTION_ID_READ:
					gSetStdData.hmiReadFlag = TRUE;
			break;
			case RTCCT_IPC_SW_FUNCTION_ID_WRITE:
				if( len == RTCCT_RECV_IPC_SW_HMICMD_LEN && NULL != data)
				{
					//gSetStdData.hmiWriteFlag = TRUE;
					memcpy(&gSetStdData.second,data,len);
					ExtrRtc_StdTimeSet(&gSetStdData);
				}
			break;
				
			//soc start, stop GYRO sample
			case IPC_SW_SYSTEM_FUCTION_GYROCT_START:
				GyroAcceData_TxEn = TRUE;
			break;
			case IPC_SW_SYSTEM_FUCTION_GYROCT_STOP:
				GyroAcceData_TxEn = FALSE;
			break;

			//soc read, write own serial num
			case IPC_SW_SYSTEM_FUCTION_GET_SERIAL_NUMBER:
				g_SerialNumInfoReadWrite = IPC_SW_SYSTEM_FUCTION_GET_SERIAL_NUMBER;
			break;
			case IPC_SW_SYSTEM_FUCTION_SET_SERIAL_NUMBER:
				if( len <= SOC_SERIAL_NUM_DATA_LEN )
				{
					g_SerialNumInfoReadWrite = IPC_SW_SYSTEM_FUCTION_SET_SERIAL_NUMBER;
					memset(&socSerialNum[0],' ',SOC_SERIAL_NUM_DATA_LEN);
					memcpy(&socSerialNum[0],data,len);
				}
			break;
				
			case IPC_SW_SYSTEM_FUCTION_GET_HARDWARE_VERSION:
				ipc_gw_send_reply(DA_IPC_HW_CH,context, &gCircuitVersion,1);
				break;
				
			case IPC_SW_SYSTEM_FUCTION_GET_VARIANT_CTRL_VERSION:
				ipc_gw_send_reply(DA_IPC_HW_CH,context, &gVariantCtrlVersion,1);
				break;

			case IPC_SW_SYSTEM_FUCTION_GET_IT_MASTER_VERSION:
				gITMasterCtrlVersion = getITMasterCtrlVerion();
				ipc_gw_send_reply(DA_IPC_HW_CH,context, &gITMasterCtrlVersion,1);
				break;

			case IPC_SW_SYSTEM_FUCTION_GET_IOC_SW_VERSION:
				
				getIocSWVersionRomHex(app_boot_sw_version,IOC_SOFTWARE_VERSION_ROM_HEX_LEN);
				getBootloaderSWVersion(&app_boot_sw_version[IOC_SOFTWARE_VERSION_ROM_HEX_LEN],IOC_SOFTWARE_VERSION_ROM_HEX_LEN);
				ipc_gw_send_reply(DA_IPC_HW_CH,context, app_boot_sw_version,IOC_SOFTWARE_VERSION_ROM_HEX_LEN*2);
				break;


			case IPC_SW_SYSTEM_FUCTION_GET_GPS_INFO://71,soc get EEPROM stord gps infomation
				gpsInfoHandleFlag = IPC_SW_SYSTEM_FUCTION_GET_GPS_INFO;
			break;

			case IPC_SW_SYSTEM_FUCTION_SET_GPS_INFO://70,soc set EEPROM stord gps infomation
				gpsInfoHandleFlag = IPC_SW_SYSTEM_FUCTION_SET_GPS_INFO;
				memcpy(&gpsInfoFromSoc[0],data,SOC_GPS_INFO_DATA_LEN);
			break;

			case IPC_SW_SYSTEM_FUCTION_GET_AVM_PROG_DATE:
				ipc_gw_send_reply(DA_IPC_HW_CH,context, g_AVM_ProgramingDate,AVM_DID_PROGRAMING_DATE_LEN);
				break;
			case IPC_SW_SYSTEM_FUCTION_GET_AVM_SW_VERSION:
				ipc_gw_send_reply(DA_IPC_HW_CH,context, g_AVM_SysSupplierEcuSwVersionNum,AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN);
				break;
					
			default:
			break;
		}

	}
}

void SYSTEM_IpcRx_NotificationIndication(uint16 function, uint8 *rx_data, uint32 len)
{
	int nTempData = 0;

	if((NULL == rx_data) || (0 == len))
	{
		return;
	}
	
	switch(function) 
	{
	case SECONDPANEL_IPC_SW_OUTSIDEPM25_FUNCTION_ID:
		memcpy(&nTempData, rx_data, sizeof(int));
		byte_swap(&nTempData, sizeof(int));
		GetSecondPanelPM25Value(SecondPanel_Outside, &nTempData);
		break;

	case SECONDPANEL_IPC_SW_LIGHTCTL_FUNCTION_ID:
		GetSecondPanelLightnessValue(rx_data[0]);
		break;

	case SECONDPANEL_IPC_SW_OUTSIDETEMP_FUNCTION_ID:
		memcpy(&nTempData, rx_data, sizeof(int));
		byte_swap(&nTempData, sizeof(int));
		GetSecondPanelTemp(SecondPanel_Outside, &nTempData);
		break;
		
	case IC_IPC_AUTOTEST_CTRL:
		DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:AutoTest_2F_service_NOTIFIED_ResultReceived\r\n");
		if(len >0)
			diagApp0x2fIpcCallback(1, rx_data, len);
		break;


	default:
		break;
	}	
}

void Task_M_IpcApp(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
#if (STD_ON == TASK_M_IPCAPP_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};
	
	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	System_Signal_Subscribe(SYSTEM_SIG_ID_100MS, SYSTEM_TASK_ID_M_IPCAPP);
	/*
	System_Signal_Subscribe(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_M_IPCAPP);
	System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_M_IPCAPP, X);
	*/
	ipc_gw_register(IPC_GW_CHANNEL_SWDL, SWDl_IPCCallback, NULL);
	ipc_gw_register(IPC_GW_CHANNEL_KEYSERVICE, KEY_IPCCallback, NULL);
	ipc_gw_register(IPC_GW_CHANNEL_CANAPP, NULL,CanApp_IpcRxIndication);
	ipc_gw_register(IPC_GW_CHANNEL_SYSTEM, SYSTEM_IpcRx_NotificationIndication, SYSTEM_IpcRxIndication);
	ipc_gw_register(IPC_GW_CHANNEL_POWERMODING,NULL,IPC_PM_callback);
	ipc_gw_register(IPC_GW_CHANNEL_AVM, diagApp_AVM_IpcNotifyInd, diagApp_AVM_IpcRequestInd);  //AVM notify, channel 9
	ipc_gw_register(IPC_GW_CHANNEL_DIAG_IVI, diagAppIpcCbk_SocNotify_IVI, diagAppIpcCbk_SocRequest_IVI);
	//ipc_gw_register(IPC_GW_CHANNEL_HUD, diagApp_HUD_IpcNotifyInd, NULL);

	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_M_IPCAPP, &l_msgInfo, FALSE, OS_WAITFOREVER);
		if (E_OK == l_returnValue)
		{
			/*
			You can use: "l_msgInfo.msgId" to get the message id.
			You can use: "l_msgInfo.size" to get the message size.
			You can use: "l_msgInfo.parBufPtr[]" or "l_msgDataBuf[]"
			             to get the message content if the message size not equal 0.
			 */
			/*===============================================================================*/
			/*=============================== START: USER CODE ==============================*/
			/*===============================================================================*/
			if (SYSTEM_SIG_ID_100MS == l_msgInfo.msgId)
			{
#if (STD_ON == TASK_M_IPCAPP_DEBUG_TRACE)
				//TracePrintf(M_IPCAPP_TRACE, TRACE_INFO, "[Task_M_IpcApp-I]:Some infomation\r\n");
#endif
				//soc read or write Serial Num information,stored in NVM
				if( FALSE != g_SerialNumInfoReadWrite )
					SerialNum_MainFunction();

			}

			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_M_IPCAPP_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#endif
		}
	}
}
/*=========================== END OF FILE: Task_M_IpcApp.c ===========================*/
