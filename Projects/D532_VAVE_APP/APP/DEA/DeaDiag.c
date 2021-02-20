#include "DeaDiag.h"
#include "Task_IpcSem.h"
#include "Task_IpcApp_Cfg.h"
#include <string.h>
#include "pin_wrapper_if.h"

#include"diagCommon_cfg.h"

#include <stdio.h>
#include <trace.h>
#include "dea.h" /* yangxl */
#include "FuelProc.h"/* yangxl */
#include "CanKey.h"/* yangxl */



typedef union
{
	tDid_0E07_Info *pDid_0E07_Info;
	tDid_0E0A_Info *pDid_0E0A_Info;
}DID_INFO_TYPE_PTR;

extern uint16_t DEA_GetFilterVehicleSpeed(void); /* ÏÔÊ¾³µËÙÖµ µ¥Î»£º0.1km/h */
extern uint16_t DEA_GetVehicleSpeedOutPut(void);  /* CAN³µËÙÊä³ö µ¥Î»£º0.01km/h */
extern uint32_t ODO_GetOdo(void); /* ÀÛ¼ÆÀï³Ì µ¥Î»£º1km */
extern uint16_t DEA_GetFilterEngineRevolution(void); /* ×ªËÙÏÔÊ¾ µ¥Î»£º1rpm */
extern uint16_t FUEL_GetFuelVol(void);/* ÓÍÏäÓÍÁ¿ µ¥Î»£º0.01L */
extern int32_t DEA_GetCoolantTemp(void); /* Ë®ÎÂ µ¥Î»£º1ÉãÊÏ¶È */
extern uint16_t DEA_GetDTE(void);
extern int16_t DEA_GetOAT_FilterValue(void);

static uint8_t s_SendDiagrequestFlag = 0;
static uint8_t s_SendDiagrequestDataLen = 0;
static uint8_t s_SendDiagrequestTimerCnt = 0;
static uint16_t s_DiagrequestId = 0;

static uint8_t s_BuzzStatusOnOffFlg = 0u;

void DEA_DiagClearData(void)
{
	s_SendDiagrequestFlag = 0;
	s_DiagrequestId = 0;
	s_SendDiagrequestTimerCnt = 0;
	s_SendDiagrequestDataLen = 0;
}

void DEA_DiagPrepareData(IPC_APP_DIAG_BUF_STRU * l_DiagDataPtr)
{
	DID_INFO_TYPE_PTR pDid_XXXX_Info;
	uint8 cycle = 0;
	
	switch(l_DiagDataPtr->did)
	{
		case 0x0E07:
			{
				pDid_XXXX_Info.pDid_0E07_Info = (tDid_0E07_Info *)l_DiagDataPtr->data;
				//pDid_XXXX_Info.pDid_0E07_Info->I_DRIVE = 1; //for test
			
				//F-1-1-C
				pDid_XXXX_Info.pDid_0E07_Info->Inhibitor_P_SW = !GetPinValue_IOC_MT_R();

				//F-2-1
				pDid_XXXX_Info.pDid_0E07_Info->Inhibitor_N_SW = !GetPinValue_IOC_MT_N();
				
				//F-3-1
				pDid_XXXX_Info.pDid_0E07_Info->O_D_OFF_SW = GetPinValue_IOC_D_MODE_SW();
				
				//F-7-1
				pDid_XXXX_Info.pDid_0E07_Info->M_range_SW = !GetPinValue_IOC_M_RANGE();

				//F-8-1
				pDid_XXXX_Info.pDid_0E07_Info->Out_of_M_range_SW = !GetPinValue_IOC_NOT_M_RANGE();
				
				//F-9-1
				pDid_XXXX_Info.pDid_0E07_Info->AT_SFT_UP_SW = !GetPinValue_IOC_AT_SHIFT_UP();
				
				//F-10-1
				pDid_XXXX_Info.pDid_0E07_Info->AT_SFT_DWN_SW = !GetPinValue_IOC_AT_SHIFT_DOWN();
				
				//F-11-1
				//F-12-1
				//F-13-1
				//F-14-1
				//F-15-1
				//F-16-1
				//F-17-1
				//F-18-1
				//F-19-1
				//F-20-1
				//F-21-1
				//F-22-1

				//F-23-1
				pDid_XXXX_Info.pDid_0E07_Info->PKB_SW = GetPinValue_IOC_PKB();

				//F-24-1
				pDid_XXXX_Info.pDid_0E07_Info->BELT_SW = GetPinValue_IOC_DR_SAFETY_BELT_WARNING()|GetPinValue_IOC_AS_SAFETY_BELT_WARNING();

				//F-25-1
				//F-26-1


				//F-27-1
				pDid_XXXX_Info.pDid_0E07_Info->BRAKE_OIL_SW = GetPinValue_IOC_BRAKE_OIL();

				//F-28-1
				//F-29-1
				//pDid_XXXX_Info.pDid_0E07_Info->LED_Lmp_L_cut = DEA_GetHardwareStatus(HARD_LED_HEAD_LAMP_L);//2019-5-6 16:32:55
				//pDid_XXXX_Info.pDid_0E07_Info->LED_Lmp_R_cut = DEA_GetHardwareStatus(HARD_LED_HEAD_LAMP_R);//2019-5-6 16:32:55
				//pDid_XXXX_Info.pDid_0E07_Info->ECO_MODE_SW = DEA_GetHardwareStatus(HARD_ECO_MODE_SW);//2019-5-6 16:32:55
			}
			break;
		case 0x0E0A:
			pDid_XXXX_Info.pDid_0E0A_Info = (tDid_0E0A_Info *)l_DiagDataPtr->data;
			pDid_XXXX_Info.pDid_0E0A_Info->Buzzer = pDid_XXXX_Info.pDid_0E0A_Info->Buzzer; //for test
			break;
		default:
			break;
	}

	for (cycle = 0; cycle < 30; cycle++)
	{
		TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,l_DiagDataPtr->data[cycle]);
	}
}

//void DEA_Diag_GetDid_0E07_Infotest(uint8* databuf,uint8_t len);

void DEA_Diag_RequestProc(uint32_t stat)
{
	IPC_APP_DIAG_BUF_STRU * l_DiagDataPtr = NULL;
	uint8 tempBuf[40] = {0};
	tUdsRespInfor respInfor = {0, FALSE, FALSE, 0, 0, tempBuf, 0};

	if(s_SendDiagrequestFlag)
	{
		if(stat)	//received soc response
		{
			s_SendDiagrequestFlag = 0;
			l_DiagDataPtr = IPC_GetDiagDataPtr();
			if(s_DiagrequestId != l_DiagDataPtr->did)
			{
				DEA_DiagClearData();
				return;
			}
			//prepare data
			DEA_DiagPrepareData(l_DiagDataPtr);
			//respInfor.ch = 0xFF;	//TBD
			respInfor.ch = UDS_IC;	//TBD
			respInfor.data[0] = (uint8)((l_DiagDataPtr->did)>>8);
			respInfor.data[1] = (uint8)(l_DiagDataPtr->did);
			diagCpyData(&(respInfor.data[2]), l_DiagDataPtr->data, s_SendDiagrequestDataLen);
			respInfor.len = s_SendDiagrequestDataLen+2;	//DID(2)+data length
			respInfor.dataFromSocFlag = TRUE;
			respInfor.finishFlag = TRUE;
			respInfor.respCode = NRC_NONE;
			respInfor.recordTick = diagApp_getRxTick(UDS_IC);
			//DEA_Diag_GetDid_0E07_Infotest(respInfor.data,respInfor.len); /* yangxl */
			diagSidResp(&respInfor);
			//after respone to diag task,clear data
			DEA_DiagClearData();
		}
		else
		{
			if(s_SendDiagrequestTimerCnt < 20)	//5*20=100ms
			{
				s_SendDiagrequestTimerCnt++;
			}
			else
			{
				//timeout, clear data
				DEA_DiagClearData();
			}
		}
	}
}

uint8_t Dea_SendDiagRequest(uint16_t id, uint16_t len)
{
	if(IPC_SendDiagRequest(id) == 0)
	{
		s_SendDiagrequestFlag = 1;
		s_DiagrequestId = id;
		s_SendDiagrequestTimerCnt = 0;
		s_SendDiagrequestDataLen = len;
		return 0;
	}
	else
	{
		return 1;
	}
}

DIAG_DEA_RET_ENUM DEA_Diag_RequestSocData(uint16_t did, uint8_t *data, uint16_t len)
{
	boolean l_returnValue = E_NOT_OK;
	IPC_APP_DIAG_BUF_STRU * l_DiagDataPtr = NULL;
	if(IPC_SendDiagRequest(did) == 0)
	{
		l_returnValue = Task_IpcSemWait(100);
		if (E_OK == l_returnValue)
		{
			l_DiagDataPtr = IPC_GetDiagDataPtr();
			if(did != l_DiagDataPtr->did)
			{
				return DIAG_DEA_ERROR;
			}
			memcpy(data, l_DiagDataPtr->data, len);
			return DIAG_DEA_OK;
		}
		else
		{
			return DIAG_DEA_ERROR;
		}
	}
	else
	{
		return DIAG_DEA_ERROR;
	}
}

#if 1
DIAG_DEA_RET_ENUM g_Diagtestflg = 0;
uint8 lenth=0;
uint8 s_flg=0;
uint8 rx_databuf[50] = {0};

void Diagtest(void)
{
	if(s_flg == 1)
	{
		g_Diagtestflg =  DEA_Diag_RequestSocData(0x0E07,rx_databuf,sizeof(tDid_0E07_Info));
		s_flg =0u;
	}
	
}
#endif

/******************************* L351 *************************************/
void DEA_Diag_DidL351_Handler(void)
{
		IPC_APP_DIAG_L351_BUF_STRU * l_DiagL351DataPtr = NULL;
		//static tDid_L351_Info l_Did_L351_Data = {0};
		//tDid_L351_Info s_Did_L351_Data = {0};
		static uint8 l_Did_L351_Databuf[Did_L351_Data_Len] = {0};
		uint8 s_Did_L351_Databuf[Did_L351_Data_Len] = {0};
		uint8 cnt1 =0u;
		uint8 cnt2 =0u;
		uint8 normal_id_num = 0u;
		uint8 cycle =0u;
		uint8 l_tmpstat = 0u;
		uint8 s_tmpstat = 0u;
		
		l_DiagL351DataPtr = IPC_GetDiagL351DataPtr();

		if(DEA_GetIgnStat() == IGN_OFF) 
		{
			memset((uint8*)&l_Did_L351_Databuf,0u,sizeof(tDid_L351_Info));
		}else
		{
			;
		}
		
		if(l_DiagL351DataPtr->did == 0x0351)
		{
			l_DiagL351DataPtr->did = 0x00;
			memcpy((uint8*)&s_Did_L351_Databuf, (uint8*)&(l_DiagL351DataPtr->data), sizeof(tDid_L351_Info));
			
			for(cnt1 = 0u; cnt1 < 8u; cnt1++)
			{
				for (cnt2 = 0u; cnt2 < 8u; cnt2++)
				{
					s_tmpstat = ((s_Did_L351_Databuf[cnt1]>>cnt2)&0x01);
					l_tmpstat = ((l_Did_L351_Databuf[cnt1]>>cnt2)&0x01);
					//if(((s_Did_L351_Databuf[cnt1]>>cnt2)&0x01) == ((l_Did_L351_Databuf[cnt1]>>cnt2)&0x01))
					if(l_tmpstat != s_tmpstat)
					{
						normal_id_num = cnt1*8u+cnt2; /* yangxl */
						(void)diagSetWarningLampLightingStatus(normal_id_num,s_tmpstat);
						TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0351]:chagedata \r\n");
						TracePrintf(COM_TRACE, TRACE_NONMASK, "data_bit%d = %x \r\n",normal_id_num ,s_tmpstat);
					}else
					{
						
					}
				}
				
			}
			memcpy((uint8*)&l_Did_L351_Databuf, (uint8*)&s_Did_L351_Databuf, sizeof(tDid_L351_Info));
		#if 0	
			//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0351]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0351]:data \r\n");
			for (cycle = 0; cycle < 8; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,l_Did_L351_Databuf[cycle]);
			}
		#endif
		}
}



/******************************* F40D *************************************/
uint16 DEA_Diag_GetDid_F40D_Speed_ometer(uint8* respData)
{
	uint16 receive = 0;     //True value needs to be filled, such as 0xFFEE
	uint16 s_res = 0u;
	
	receive = DEA_GetRawVehicleSpeedCanOutput();

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;
}


/******************************* 0E00 *************************************/
uint16 DEA_Diag_GetDid_0E00_Speed_output(uint8* respData)
{
	uint16 receive = 0;     //True value needs to be filled, such as 0xFFEE
	uint16 s_res = 0u;
	/*
	receive = ((uint32)DEA_GetRawVehicleSpeedCanOutput()*3387)/3350;
	if(receive > 35000)
	{
		receive = 35000;
	}
	*/
	receive = DEA_GetVehicleSpeedOutPut();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;	
}


/******************************* 0E01 *************************************/
uint16 DEA_Diag_GetDid_0E01_ODO_output(uint8* respData)
{
	tDid_0E01_ODO_output Did_0E01_ODO_output = {0};
	uint16 s_res = 0u;
	
	Did_0E01_ODO_output.ODO_output = ODO_GetOdo();
	/*
	Did_0E01_ODO_output.ODO_output = 0;   //True value needs to be filled, such as 0xFFEEDD
	*/

	//memcpy(&Did_0E01_ODO_output, &Ipc_Did_0E01_ODO_output, sizeof(Did_0E01_ODO_output));

	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E01_ODO_output, sizeof(Did_0E01_ODO_output)-1);
	
	return 0;
	*/
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E01_ODO_output, sizeof(Did_0E01_ODO_output)-1);
	}else
	{
		s_res = 1u;
	}
		
	return s_res;	
}


/******************************* 0E02 *************************************/
uint16 DEA_Diag_GetDid_0E02_Tachometer(uint8* respData)
{
	uint16 receive = 0;     //True value needs to be filled, such as 0xFFEE
	uint32 s_receive =0ul;
	uint16 s_res = 0u;

	s_receive = DEA_GetFilterEngineRevolution();
	s_receive = s_receive*1000ul;
	s_receive = (uint32)s_receive/125ul;
	receive = (uint16)s_receive;

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;	
	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	
	return 0;
	*/
}


/******************************* 0E03 *************************************/
uint16 DEA_Diag_GetDid_0E03_Fuel_meter(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	uint16 s_receive =0ul;
	uint16 s_res = 0u;
	
	s_receive = FUEL_GetFuelVol();
	s_receive =(uint16)(s_receive/10U);
	receive = (uint16)s_receive/8u;

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;
	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	return 0;
	*/
}


/******************************* 0E04 *************************************/
uint16 DEA_Diag_GetDid_0E04_Water_temperature(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	sint32 s_receive=0;
	uint16 s_res = 0u;
	
	s_receive = DEA_GetCoolantTemp()/10;
	s_receive = s_receive+40;
	receive = (uint8)s_receive;

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;

	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
	*/
}

#if 0
uint8_t Dea_Diag_ReuestData(uint16_t did, uit8_t *addr, uint16_t len)
{
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};

	//ipc request (did);
	System_Task_ReceiveMessage(SYSTEM_TASK_ID_DIAG, &l_msgInfo, FALSE, 100);
	if
		{
			
		}
	else
	{
	}
	
}
#endif

/******************************* 0E07 *************************************/
uint16 DEA_Diag_GetDid_0E07_Info(uint8* respData)
{
	#if 0
	tDid_0E07_Info Did_0E07_Info = {0};
	DIAG_DEA_RET_ENUM s_Diagrxflg = DIAG_DEA_ERROR;
	uint16 s_res = 0u;
		/*
	Did_0E07_Info.High_beam_IND = 0;           //True value needs to be filled
	Did_0E07_Info.Trunk_hatchback = 0;
	Did_0E07_Info.Door_WL = 0;
	Did_0E07_Info.E_PKB_WL = 0;
	Did_0E07_Info.Brake_WL = 0;
	Did_0E07_Info.SLIP_IND = 0;
	Did_0E07_Info.VDC_OFF_WL = 0;
	Did_0E07_Info.ABS_WL = 0;
	Did_0E07_Info.C_ENG2_WL = 0;
	Did_0E07_Info.GLOW_IND = 0;
	Did_0E07_Info.C_ENG_WL = 0;
	Did_0E07_Info.OIL_WL = 0;
	Did_0E07_Info.Light_IND = 0;
	Did_0E07_Info.Rr_fog_IND = 0;
	Did_0E07_Info.Fr_fog_IND = 0;
	Did_0E07_Info.Turn_IND = 0;
	Did_0E07_Info.CVT_IND = 0;
	Did_0E07_Info.AT_fluid_TEMP_WL = 0;
	Did_0E07_Info.ATC_T_AMT_WL = 0;
	Did_0E07_Info.O_D_OFF_IND = 0;
	Did_0E07_Info.BA_WL = 0;
	Did_0E07_Info.CRUISE_WL = 0;
	Did_0E07_Info.SET_IND = 0;
	Did_0E07_Info.CRUISE_IND = 0;
	Did_0E07_Info.KEY_red_WL = 0;
	Did_0E07_Info.KEY_green_yellow_IND = 0;
	Did_0E07_Info.Tire_pressure_WL = 0;
	Did_0E07_Info.WASHER_WL = 0;
	Did_0E07_Info.Fuel_WL = 0;
	Did_0E07_Info._4WD_LOCK_IND = 0;
	Did_0E07_Info._4WD_WL = 0;
	Did_0E07_Info.SPORT_IND = 0;
	Did_0E07_Info.LDP_G_Y_IND = 0;
	Did_0E07_Info.LDP_R_IND = 0;
	Did_0E07_Info.HDC_WL = 0;
	Did_0E07_Info._4WAS_RAS_WL = 0;
	Did_0E07_Info.AFS_OFF_IND = 0;
	Did_0E07_Info.e_4WD_WL = 0;
	Did_0E07_Info.EPS_WL = 0;
	Did_0E07_Info.KEY_knob_WL = 0;
	Did_0E07_Info.SFT_operation_WL = 0;
	Did_0E07_Info.HEV_brake_WL = 0;
	Did_0E07_Info.HV_BAT_WL = 0;
	Did_0E07_Info.SFT_POSI_WL = 0;
	Did_0E07_Info.SYS_FAIL_WL = 0;
	Did_0E07_Info.INVT_TEMP_IND = 0;
	Did_0E07_Info.READY_IND = 0;
	Did_0E07_Info.SLOW_IND = 0;
	Did_0E07_Info.ECO_MODE_IND = 0;
	Did_0E07_Info.Trailer_IND = 0;
	Did_0E07_Info.DPF_WL = 0;
	Did_0E07_Info._4WD_AUTO_IND = 0;
	Did_0E07_Info.Charge_WL = 0;
	Did_0E07_Info.Idle_stop_IND = 0;
	Did_0E07_Info.L_K_MAS_WL = 0;
	Did_0E07_Info.SCR_WL = 0;
	Did_0E07_Info.FILTER_WL = 0;
	Did_0E07_Info.MTSyncRev_IND = 0;
	Did_0E07_Info.DCA_IND = 0;
	Did_0E07_Info.E_SUS_IND = 0;
	Did_0E07_Info.ATP_WL = 0;
	Did_0E07_Info.ACC_own_vehicle = 0;
	Did_0E07_Info.ACC_target_distance = 0;
	Did_0E07_Info.ACC_target = 0;
	Did_0E07_Info.PUSH_ENG_IND = 0;
	Did_0E07_Info.ACC_set_speed = 0;
	Did_0E07_Info.GEAR_SFT_IND = 0;
	Did_0E07_Info.AT_CVT_RNG = 0;
	Did_0E07_Info.ACC_unit = 0;
	Did_0E07_Info.SOW_G_IND = 0;
	Did_0E07_Info.ECO_PDL_O_IND = 0;
	Did_0E07_Info.ECO_PDL_G_IND = 0;
	Did_0E07_Info.OFFLOAD_IND = 0;
	Did_0E07_Info._4WD_IND = 0;
	Did_0E07_Info.I_DRIVE = 0;
	Did_0E07_Info.SOW_O_IND = 0;
	Did_0E07_Info.E_PKB_IND = 0;
	Did_0E07_Info.FUEL_CAP_WL = 0;
	Did_0E07_Info.CHECK_SUS_IND = 0;
	Did_0E07_Info.TOW_mode_IND = 0;
	Did_0E07_Info.SCR_OIL_DISP = 0;
	Did_0E07_Info.Out_of_M_range_SW = 0;
	Did_0E07_Info.M_range_SW = 0;
	Did_0E07_Info.AT_power_mode_SW = 0;
	Did_0E07_Info.AT_snow_mode_SW = 0;
	Did_0E07_Info.RANGE_1_SW = 0;
	Did_0E07_Info.O_D_OFF_SW = 0;
	Did_0E07_Info.Inhibitor_N_SW = 0;
	Did_0E07_Info.Inhibitor_P_SW = 0;
	Did_0E07_Info.A_C_LOW_TEMP = 0;
	Did_0E07_Info.R_DEF_ON = 0;
	Did_0E07_Info.H_FAN_ON = 0;
	Did_0E07_Info.A_C_ON_SIG = 0;
	Did_0E07_Info.AT_STR_DWN_SW = 0;
	Did_0E07_Info.AT_STR_UP_SW = 0;
	Did_0E07_Info.AT_SFT_DWN_SW = 0;
	Did_0E07_Info.AT_SFT_UP_SW = 0;
	Did_0E07_Info.AIR_BAG_SIG = 0;
	Did_0E07_Info.BRAKE_OIL_SW = 0;
	Did_0E07_Info.Brake_SW = 0;
	Did_0E07_Info.BELT_SW = 0;
	Did_0E07_Info.PKB_SW = 0;
	Did_0E07_Info.SYNC_MODE = 0;
	Did_0E07_Info._4WD_LOCK_SW = 0;
	Did_0E07_Info.A_C_PD_CUT = 0;
	Did_0E07_Info.MTSyncRev_SW = 0;
	Did_0E07_Info.MODE_B_SW = 0;
	Did_0E07_Info.MODE_A_SW = 0;
	Did_0E07_Info.AMB_POWER = 0;
	Did_0E07_Info.VDC_TCS_GND = 0;
	Did_0E07_Info.ABS_GND = 0;
	Did_0E07_Info.RR_COMB_status = 0;
	Did_0E07_Info.CHG_SIG = 0;
	Did_0E07_Info.LED_Lmp_L_cut = 0;
	Did_0E07_Info.LED_Lmp_R_cut = 0;
	Did_0E07_Info.CHG_conct_det = 0;
	Did_0E07_Info.PBW_WL = 0;
	Did_0E07_Info.SPORT_MODE_SW = 0;
	Did_0E07_Info.ECO_MODE_SW = 0;
	Did_0E07_Info.TOW_mode_SW = 0;
	Did_0E07_Info.AS_BELT_SW = 0;
	*/


	//memcpy(&Did_0E07_Info, &Ipc_Did_0E07_Info, sizeof(Did_0E07_Info));
	s_Diagrxflg = DEA_Diag_RequestSocData(0x0E07,(uint8*)&Did_0E07_Info,sizeof(Did_0E07_Info));

	//F-1-1-C
	Did_0E07_Info.Inhibitor_P_SW = GetPinValue_IOC_MT_R();

	//F-2-1
	Did_0E07_Info.Inhibitor_N_SW = GetPinValue_IOC_MT_N();
	
	//F-3-1
	Did_0E07_Info.O_D_OFF_SW = GetPinValue_IOC_D_MODE_SW();
	
	//F-7-1
	Did_0E07_Info.M_range_SW = GetPinValue_IOC_M_RANGE();

	//F-8-1
	Did_0E07_Info.Out_of_M_range_SW = GetPinValue_IOC_NOT_M_RANGE();
	
	//F-9-1
	Did_0E07_Info.AT_SFT_UP_SW = GetPinValue_IOC_AT_SHIFT_UP();
	
	//F-10-1
	Did_0E07_Info.AT_SFT_DWN_SW = GetPinValue_IOC_AT_SHIFT_DOWN();
	
	//F-11-1
	
	//F-12-1
	//F-13-1
	//F-14-1
	//F-15-1
	//F-16-1
	//F-17-1
	//F-18-1
	//F-19-1
	//F-20-1
	//F-21-1
	//F-22-1

	//F-23-1
	Did_0E07_Info.PKB_SW = GetPinValue_IOC_PKB();

	//F-24-1
	Did_0E07_Info.BELT_SW = GetPinValue_IOC_DR_SAFETY_BELT_WARNING();

	//F-25-1
	//F-26-1

	//F-27-1
	Did_0E07_Info.BRAKE_OIL_SW = GetPinValue_IOC_BRAKE_OIL();

	//F-28-1
	//F-29-1

		
	if(s_Diagrxflg == DIAG_DEA_OK)
	{
		if(respData != NULL_PTR)
		{
			diagCpyData(respData, (uint8*)&Did_0E07_Info, sizeof(Did_0E07_Info));
		}else
		{
			s_res = 1u;
		}
		
	}else
	{
		s_res = 1u;
	}
	#else
	uint16 s_res = 0u;
	
	s_res = Dea_SendDiagRequest(0x0E07,sizeof(tDid_0E07_Info));
	
	return s_res;
	
	#endif
}


/******************************* 0E08 *************************************/
uint16 DEA_Diag_GetDid_0E08_Distance_To_Empty(uint8* respData)
{
	uint16 receive = 0;     //True value needs to be filled, such as 0xFFEE
	uint16 s_res = 0u;
	
	receive = DEA_GetDTE();
	receive = receive*10u;

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;
	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	
	return 0;
	*/
}


/******************************* 0E09 *************************************/
uint16 DEA_Diag_GetDid_0E09_Ambient_temperature(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	//sint16 s_receive=0;
	uint16 s_res = 0u;
	/*
	s_receive = DEA_GetOAT_FilterValue();
	s_receive = s_receive +40;
	receive = (uint8)s_receive;
	*/
	receive = DEA_GetOAT_CanRawValue(); //2019-8-5 09:56:57
	if(receive >= 1u)
	{
		receive = receive - 1u;
		receive = receive/2;
	}

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;
	/*
	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
	*/
}


/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void SetBuzzerStatus(uint16_t buzzID,uint8_t buzzstatus)
{
	//static uint8_t s_BuzzIdTable[][2];
	uint16_t s_buzzID = 0u;
	
	s_buzzID = buzzID;
	
	if((s_buzzID >= NORMAL_MSG_ID_DoorOpenBuzzer) && (s_buzzID <= NORMAL_MSG_ID_TrFaultContBuzzer)&&(s_buzzID != NORMAL_MSG_ID_IlluminationControlMaxMin))
	{
		s_BuzzStatusOnOffFlg = buzzstatus;
	}
	
}
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint8_t GetBuzzerStatus(void)
{
	return s_BuzzStatusOnOffFlg;
}

/******************************* 0E0A *************************************/
uint16 DEA_Diag_GetDid_0E0A_Info(uint8* respData)
{
	uint16 s_res = 0u;
	tDid_0E0A_Info Did_0E0A_Info = {0};
	uint8_t s_semiSonar = 0u;

	/*
	Did_0E0A_Info.DR_DOOR_SIG = 0;           //True value needs to be filled
	Did_0E0A_Info.DOOR_LOCK_SIG = 0;
	Did_0E0A_Info.ST_CNT_SIG = 0;
	Did_0E0A_Info.CRANK_SIG = 0;
	//Did_0E0A_Info.FLU_SIG = 0;
	Did_0E0A_Info.Parking_Aids_Volume = 0;
	Did_0E0A_Info.Parking_Aids_Sensor = 0;
	Did_0E0A_Info.Parking_Aids_DSP = 0;
	Did_0E0A_Info.BAT_CIR_STA = 0;
	Did_0E0A_Info.Buzzer = 0;
	Did_0E0A_Info.Parking_Aids_Range = 0;
	*/
	
	if(FUEL_GetFuelSigStatus() != FuelSigNormal)
	{
		Did_0E0A_Info.FLU_SIG = 1u;
	}else
	{
		if(FUEL_GetFuelLowStatus() != 0)
		{
			Did_0E0A_Info.FLU_SIG = 1u;
		}else
		{
			Did_0E0A_Info.FLU_SIG = 0u;
		}
	}
	Did_0E0A_Info.Buzzer = GetBuzzerStatus(); //2019-6-12 11:09:38

	s_semiSonar = semiSonarGet();

	if(s_semiSonar == 0u)
	{
		Did_0E0A_Info.Parking_Aids_DSP = 1u;
		Did_0E0A_Info.Parking_Aids_Sensor = 0u;
	}else if(s_semiSonar == 1u)
	{
		Did_0E0A_Info.Parking_Aids_DSP = 0u;
		Did_0E0A_Info.Parking_Aids_Sensor = 3u;
	}else if(s_semiSonar == 2u)
	{
		Did_0E0A_Info.Parking_Aids_DSP = 0u;
		Did_0E0A_Info.Parking_Aids_Sensor = 1u;
	}

	

	//memcpy(&Did_0E0A_Info, &Ipc_Did_0E0A_Info, sizeof(Did_0E0A_Info));

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E0A_Info, sizeof(Did_0E0A_Info));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E0B *************************************/
uint16 DEA_Diag_GetDid_0E0B_General_power_meter(uint8* respData)
{
	tDid_0E0B_General_power_meter Did_0E0B_General_power_meter = {0};

	/*
	Did_0E0B_General_power_meter.General_power_meter = 0;       //True value needs to be filled
	*/


	//memcpy(&Did_0E0B_General_power_meter, &Ipc_Did_0E0B_General_power_meter, sizeof(Did_0E0B_General_power_meter));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E0B_General_power_meter, sizeof(Did_0E0B_General_power_meter));

	return 0;
}


/******************************* 0E0C *************************************/
uint16 DEA_Diag_GetDid_0E0C_SOC_meter(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF

	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E0D *************************************/
uint16 DEA_Diag_GetDid_0E0D_VOLT_meter(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF

	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E0E *************************************/
uint16 DEA_Diag_GetDid_0E0E_Brake_pad(uint8* respData)
{
	tDid_0E0E_General_Brake_pad Did_0E0E_General_Brake_pad = {0};


	/*
	Did_0E0E_General_Brake_pad.Brake_pad = 0;    //True value needs to be filled, such as 0xDDEE
	*/
	
	//memcpy(&Did_0E0E_General_Brake_pad, &Ipc_Did_0E0E_General_Brake_pad, sizeof(Did_0E0E_General_Brake_pad));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E0E_General_Brake_pad, sizeof(Did_0E0E_General_Brake_pad));

	return 0;
}


/******************************* 0E0F *************************************/
uint16 DEA_Diag_GetDid_0E0F_Info(uint8* respData)
{
	uint16 s_res = 0u;
	uint8_t l_u8value =0u;
	tDid_0E0F_Info Did_0E0F_Info = {0};

	/*
	Did_0E0F_Info.Sonar_Set_Ava = 0;           //True value needs to be filled
	Did_0E0F_Info.Tire_status_RL = 0;
	Did_0E0F_Info.Tire_status_RR = 0;
	Did_0E0F_Info.Tire_status_FL = 0;
	Did_0E0F_Info.Tire_status_FR = 0;
	Did_0E0F_Info.TPMS_DISP = 0;
	Did_0E0F_Info.STRG_SW_output = 0;
	Did_0E0F_Info.STRG_SW_input = 0;
	Did_0E0F_Info.Auto_Hi_Lo_beam_IND = 0;
	Did_0E0F_Info.ITS_Sonar_SET_OUTPUT = 0;
	Did_0E0F_Info.Dipped_Beam = 0;
	Did_0E0F_Info.Sonar_det_DSParea = 0;
	Did_0E0F_Info.Sonar_det_DSP_RC = 0;
	Did_0E0F_Info.Sonar_Warn = 0;
	Did_0E0F_Info.Sonar_det_sta = 0;
	Did_0E0F_Info.Sonar_det_DSParea_RR = 0;
	Did_0E0F_Info.Sonar_det_DSP_RR = 0;
	Did_0E0F_Info.Sonar_det_DSParea_RL = 0;
	Did_0E0F_Info.Sonar_det_DSP_RL = 0;
	Did_0E0F_Info.Sonar_det_DSParea_FL = 0;
	Did_0E0F_Info.Sonar_det_DSP_FL = 0;
	Did_0E0F_Info.Sonar_det_DSParea_FC = 0;
	Did_0E0F_Info.Sonar_det_DSP_FC = 0;
	Did_0E0F_Info.Sonar_dist_DSP = 0;
	Did_0E0F_Info.Sonar_det_DSParea_FR = 0;
	Did_0E0F_Info.Sonar_det_DSP_FR = 0;
	*/

	Com_ReceiveSignal(SIG_HND_STAT_CHG_REQ , &l_u8value);
	Did_0E0F_Info.ITS_Sonar_SET_OUTPUT = l_u8value;

	Did_0E0F_Info.Tire_status_FR = DEA_GetTS_FR();
	Did_0E0F_Info.Tire_status_FL = DEA_GetTS_FL();
	Did_0E0F_Info.Tire_status_RR = DEA_GetTS_RR();
	Did_0E0F_Info.Tire_status_RL = DEA_GetTS_RL();
	Did_0E0F_Info.TPMS_DISP = DEA_GetD_STYLE();

	Did_0E0F_Info.Sonar_det_sta = DEA_GetREQ_DISP_MET();
	Did_0E0F_Info.Sonar_Warn = DEA_GetSONAR_S_REQ();
	Did_0E0F_Info.Sonar_det_DSP_RC = DEA_GetRR_CEN();
	Did_0E0F_Info.Sonar_det_DSParea = DEA_GetA_RR_CEN();

	Did_0E0F_Info.Sonar_det_DSP_RL = DEA_GetRR_L();
	Did_0E0F_Info.Sonar_det_DSParea_RL = DEA_GetA_RR_L();

	Did_0E0F_Info.Sonar_det_DSP_RR = DEA_GetRR_R();
	Did_0E0F_Info.Sonar_det_DSParea_RR = DEA_GetA_RR_R();
	
	Did_0E0F_Info.Sonar_det_DSP_FC = DEA_GetrangeStatus_FR_Center();	
	Did_0E0F_Info.Sonar_det_DSParea_FC = DEA_GetA_FR_Center();
	
	Did_0E0F_Info.Sonar_det_DSP_FL = DEA_GetrangeStatus_FR_Left();
	Did_0E0F_Info.Sonar_det_DSParea_FL = DEA_GetA_FR_CorLeft();
	
	Did_0E0F_Info.Sonar_det_DSP_FR = DEA_GetrangeStatus_FR_Right();
	Did_0E0F_Info.Sonar_det_DSParea_FR = DEA_GetA_FR_ConrRight();

	Did_0E0F_Info.Sonar_dist_DSP = DEA_GetSTS_DISTANCE();

	//Did_0E0F_Info.STRG_SW_input = 0;
	if(gCanSwKeyVal.key.Strg_Up == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0001;
		
	}else if(gCanSwKeyVal.key.Strg_Down == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0010;
	}else if(gCanSwKeyVal.key.Strg_Left == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0011;
	}else if(gCanSwKeyVal.key.Strg_Right == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0100;
	}else if(gCanSwKeyVal.key.Strg_Ok == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0101;
	}else if(gCanSwKeyVal.key.Strg_Disp == 1u)
	{
		Did_0E0F_Info.STRG_SW_input = 0b0110;
	}
	//memcpy(&Did_0E0F_Info, &Ipc_Did_0E0F_Info, sizeof(Did_0E0F_Info));


	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E0F_Info, sizeof(Did_0E0F_Info));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E10 *************************************/
uint16 DEA_Diag_GetDid_0E10_ENG_shut_timer(uint8* respData)
{
	tDid_0E10_ENG_shut_timer Did_0E10_ENG_shut_timer = {0};


	/*
	Did_0E10_ENG_shut_timer.ENG_shut_timer = 0;     //True value needs to be filled, such as 0xFFE
	*/

	//memcpy(&Did_0E10_ENG_shut_timer, &Ipc_Did_0E10_ENG_shut_timer, sizeof(Did_0E10_ENG_shut_timer));


	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E10_ENG_shut_timer, sizeof(Did_0E10_ENG_shut_timer));

	return 0;
}


/******************************* 0E11 *************************************/
uint16 DEA_Diag_GetDid_0E11_Torque(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF

	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E12 *************************************/
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint8_t FEB_IND_Get(void)
{
	uint8_t s_Feb_ind = 0u;
	
	uint8_t s_PEBS_INFO = 0u;
	uint8_t s_PRE_WARN_FR = 0u;
	uint8_t s_WARN_VRU = 0u;
	uint8_t s_AEB_ACTIVE = 0u;
	uint8_t s_MSG_VDC = 0u;
	
	if((1U == semiActiveITS_FEBGet())&&(DEA_GetIgnStat() == IGN_ON))
	{
		if((0 == DEA_GetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2_1)) && (0 == DEA_GetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2)))
		{
			s_PEBS_INFO = DEA_GetPEBS_INFO();
			s_PRE_WARN_FR = DEA_GetPRE_WARN_FR();
			s_WARN_VRU = DEA_GetWARN_VRU();
			s_AEB_ACTIVE = DEA_GetAEB_ACTIVE();
			s_MSG_VDC = DEA_GetMSG_VDC();
		
			if(s_PEBS_INFO == 0b000)
			{
				s_Feb_ind = 0b00;
				
			}else if(((s_PEBS_INFO == 0b001) || (s_PEBS_INFO == 0b010)) && (s_WARN_VRU == 0b0) \
				&& (s_PRE_WARN_FR == 0b0) && (s_AEB_ACTIVE == 0b0) && (s_MSG_VDC == 0b0))
			{
				s_Feb_ind = 0b10;
				
			}else if ((s_PEBS_INFO == 0b010) && (s_PRE_WARN_FR == 0b1) && (s_AEB_ACTIVE == 0b0))
			{
				s_Feb_ind = 0b10;
				
			}else if((s_PEBS_INFO == 0b010) && (s_AEB_ACTIVE == 0b1) && (s_MSG_VDC == 0b0))
			{
				s_Feb_ind = 0b10;
				
			}else
			{
				s_Feb_ind = 0b01;
			}
			
		}
	}

	return s_Feb_ind;
}

/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint8_t LDW_IND_Get(void)
{
	uint8_t s_Ldw_ind = 0u;
	uint8_t s_LDW_INFO = 0u;
	uint8_t s_WARN_LANE_L = 0u;
	uint8_t s_WARN_LANE_R = 0u;
	
	if((1U == semiActiveITS_LDWGet())&&(DEA_GetIgnStat() == IGN_ON))
	{
		if((0 == DEA_GetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N1)) && (0 == DEA_GetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N3_7b)))
		{
			s_LDW_INFO = DEA_GetLDW_INFO();
			s_WARN_LANE_L = DEA_GetWARN_LANE_L();
			s_WARN_LANE_R = DEA_GetWARN_LANE_R();
			
			if(s_LDW_INFO == 0b000)
			{
				s_Ldw_ind = 0b00;
				
			}else if((s_LDW_INFO == 0b010) && (s_WARN_LANE_R == 0b11) && (s_WARN_LANE_L <= 0b01))
			{
				s_Ldw_ind = 0b10;
			}else if((s_LDW_INFO == 0b010) && (s_WARN_LANE_L == 0b11) && (s_WARN_LANE_R <= 0b01))
			{
				s_Ldw_ind = 0b10;
			}else
			{
				s_Ldw_ind = 0b01;
			}
		}
	}

	return s_Ldw_ind;
}



uint16 DEA_Diag_GetDid_0E12_Info(uint8* respData)
{
	tDid_0E12_Info Did_0E12_Info = {0};
	uint8_t l_TPMS_low_press = 0u;
	uint8_t l_TPMS_puncture_press = 0u;
	uint16 s_res = 0u;



	/*
	Did_0E12_Info.LDW_IND = 0;           //True value needs to be filled
	Did_0E12_Info.FEB_IND = 0;
	Did_0E12_Info.Lane_IND = 0;
	Did_0E12_Info.TPMS_low_press = 0;
	Did_0E12_Info.TPMS_puncture = 0;
	Did_0E12_Info.ABS_fail = 0;
	Did_0E12_Info.E_SUS_fail = 0;
	Did_0E12_Info._4WD_system_fail = 0;
	Did_0E12_Info._4WD_tire_check = 0;
	Did_0E12_Info._4WD_clutch_hi_tmp = 0;
	Did_0E12_Info.TPMS_fail = 0;
	Did_0E12_Info.engine_system_check = 0;
	Did_0E12_Info.VDC_fail = 0;
	Did_0E12_Info.OVER_BOOST = 0;
	Did_0E12_Info.ASCD_system_fail = 0;
	Did_0E12_Info.CC_SL_condition = 0;
	Did_0E12_Info.CC_SL_speed_blink = 0;
	*/

	//memcpy(&Did_0E12_Info, &Ipc_Did_0E12_Info, sizeof(Did_0E12_Info));
	
	if(DEA_GetLTP_RL() == 1U)
	{
		l_TPMS_low_press |= 0b0001;
	}
	if(DEA_GetLTP_RR() == 1U)
	{
		l_TPMS_low_press |= 0b0010;
	}
	if(DEA_GetLTP_FL() == 1U)
	{
		l_TPMS_low_press |= 0b0100;
	}
	if(DEA_GetLTP_FR() == 1U)
	{
		l_TPMS_low_press |= 0b1000;
	}
	Did_0E12_Info.TPMS_low_press = l_TPMS_low_press;

	if(DEA_GetFT_RL() == 1U)
	{
		l_TPMS_puncture_press |= 0b10001;
		//Did_0E12_Info.TPMS_puncture_byte5 = 0b1;
	}
	if(DEA_GetFT_RR() == 1U)
	{
		l_TPMS_puncture_press |= 0b10010;
		//Did_0E12_Info.TPMS_puncture_byte5 = 0b1;
	}
	if(DEA_GetFT_FL() == 1U)
	{
		l_TPMS_puncture_press |= 0b10100;
		//Did_0E12_Info.TPMS_puncture_byte5 = 0b1;
	}
	if(DEA_GetFT_FR() == 1U)
	{
		l_TPMS_puncture_press |= 0b11000;
		//Did_0E12_Info.TPMS_puncture_byte5 = 0b1;
	}

	//Did_0E12_Info.TPMS_puncture_byte5 = 0;
	if(l_TPMS_puncture_press != 0x00)
	{
		Did_0E12_Info.TPMS_puncture_byte5 = 1;
	}
	Did_0E12_Info.TPMS_puncture_byte6 = l_TPMS_puncture_press;

	Did_0E12_Info.CC_SL_speed_blink = DEA_GetSPDBLREQ();
	Did_0E12_Info.CC_SL_condition = DEA_GetASCDON_DISP();

	Did_0E12_Info.FEB_IND = FEB_IND_Get();
	Did_0E12_Info.LDW_IND = LDW_IND_Get();

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E12_Info, sizeof(Did_0E12_Info));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;

}


/******************************* 0E13 *************************************/
uint16 DEA_Diag_GetDid_0E13_CC_SL_REQ(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	uint16 s_res = 0u;
	
	receive = DEA_GetACCSPDREQ();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, &receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E14 *************************************/
uint16 DEA_Diag_GetDid_0E14_SCR_REMAINING(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF

	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E15 *************************************/
uint16 DEA_Diag_GetDid_0E15_SCR_DISTANCE(uint8* respData)
{
	tDid_0E15_SCR_DISTANCE Did_0E15_SCR_DISTANCE = {0};

	/*
	Did_0E15_SCR_DISTANCE.SCR_DISTANCE = 0;     //True value needs to be filled, such as 0x3FFF
	*/

	
	//memcpy(&Did_0E15_SCR_DISTANCE, &Ipc_Did_0E15_SCR_DISTANCE, sizeof(Did_0E15_SCR_DISTANCE));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E15_SCR_DISTANCE, sizeof(Did_0E15_SCR_DISTANCE));
	
	return 0;
}


/******************************* 0E16 *************************************/
uint16 DEA_Diag_GetDid_0E16_OIL_DISTANCE(uint8* respData)
{
	tDid_0E16_OIL_DISTANCE Did_0E16_OIL_DISTANCE = {0};

	/*
	Did_0E16_OIL_DISTANCE.OIL_DISTANCE = 0;     //True value needs to be filled, such as 0x3FFF
	*/


	//memcpy(&Did_0E16_OIL_DISTANCE, &Ipc_Did_0E16_OIL_DISTANCE, sizeof(Did_0E16_OIL_DISTANCE));


	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E16_OIL_DISTANCE, sizeof(Did_0E16_OIL_DISTANCE));
	
	return 0;
}


/******************************* 0E17 *************************************/
uint16 DEA_Diag_GetDid_0E17_OIL_RESET(uint8* respData)
{
	tDid_0E17_OIL_RESET Did_0E17_OIL_RESET = {0};

	/*
	Did_0E17_OIL_RESET.OIL_RESET = 0; 	 //True value needs to be filled
	*/

	
	//memcpy(&Did_0E17_OIL_RESET, &Ipc_Did_0E17_OIL_RESET, sizeof(Did_0E17_OIL_RESET));
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E17_OIL_RESET, sizeof(Did_0E17_OIL_RESET));
	
	return 0;
}


/******************************* 0E18 *************************************/
uint16 DEA_Diag_GetDid_0E18_ABSOLUTE_TIME(uint8* respData)
{
	tDid_0E18_ABSOLUTE_TIME Did_0E18_ABSOLUTE_TIME = {0};

	/*
	Did_0E18_ABSOLUTE_TIME.ABSOLUTE_TIME = 0;    //True value needs to be filled, such as 0xFFF
	*/


	//memcpy(&Did_0E18_ABSOLUTE_TIME, &Ipc_Did_0E18_ABSOLUTE_TIME, sizeof(Did_0E18_ABSOLUTE_TIME));

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E18_ABSOLUTE_TIME, sizeof(Did_0E18_ABSOLUTE_TIME));
	
	return 0;
}


/******************************* 0E19 *************************************/
uint16 DEA_Diag_GetDid_0E19_Info(uint8* respData)
{
	uint16 s_res = 0u;
	tDid_0E19_Info Did_0E19_Info = {0};

	/*
	Did_0E19_Info.CL2_WARN = 0;          //True value needs to be filled
	Did_0E19_Info.IDLE_SW_IND = 0;
	Did_0E19_Info.IDLE_STOP_JDG = 0;
	Did_0E19_Info.IDLE_STOP_IND = 0;
	*/
	Did_0E19_Info.IDLE_SW_IND = DEA_GetISMSKSW();
	Did_0E19_Info.IDLE_STOP_JDG = DEA_GetRawRevolutionIdleStat();
	Did_0E19_Info.IDLE_STOP_IND = DEA_GetRawRevolutionIdleRequest();

	
	//memcpy(&Did_0E19_Info, &Ipc_Did_0E19_Info, sizeof(Did_0E19_Info));
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E19_Info, sizeof(Did_0E19_Info));
	}else
	{
		s_res = 1u;
	}
		
	return s_res;
}


/******************************* 0E1A *************************************/
uint16 DEA_Diag_GetDid_0E1A_ASST_CHG(uint8* respData)
{
	tDid_0E1A_ASST_CHG Did_0E1A_ASST_CHG = {0};

	/*
	Did_0E1A_ASST_CHG.ASST_CHG = 0;      //True value needs to be filled
	*/

	//memcpy(&Did_0E1A_ASST_CHG, &Ipc_Did_0E1A_ASST_CHG, sizeof(Did_0E1A_ASST_CHG));

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1A_ASST_CHG, sizeof(Did_0E1A_ASST_CHG));
	return 0;
}


/******************************* 0E1B *************************************/
uint16 DEA_Diag_GetDid_0E1B_EV_IND(uint8* respData)
{
	tDid_0E1B_EV_IND Did_0E1B_EV_IND = {0};

	/*
	Did_0E1B_EV_IND.EV_IND = 0;     //True value needs to be filled
	*/

	
	//memcpy(&Did_0E1B_EV_IND, &Ipc_Did_0E1B_EV_IND, sizeof(Did_0E1B_EV_IND));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1B_EV_IND, sizeof(Did_0E1B_EV_IND));
	
	return 0;
}


/******************************* 0E1C *************************************/
uint16 DEA_Diag_GetDid_0E1C_APO_DISPLAY(uint8* respData)
{
	tDid_0E1C_APO_DISPLAY Did_0E1C_APO_DISPLAY = {0};

	/*
	Did_0E1C_APO_DISPLAY.APO_DISPLAY = 0;     //True value needs to be filled
	*/

	//memcpy(&Did_0E1C_APO_DISPLAY, &Ipc_Did_0E1C_APO_DISPLAY, sizeof(Did_0E1C_APO_DISPLAY));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1C_APO_DISPLAY, sizeof(Did_0E1C_APO_DISPLAY));
	
	return 0;
}


/******************************* 0E1D *************************************/
uint16 DEA_Diag_GetDid_0E1D_SOC_DISPLAY(uint8* respData)
{
	tDid_0E1D_SOC_DISPLAY Did_0E1D_SOC_DISPLAY = {0};

	/*
	Did_0E1D_SOC_DISPLAY.SOC_DISPLAY = 0;     //True value needs to be filled
	*/

	//memcpy(&Did_0E1D_SOC_DISPLAY, &Ipc_Did_0E1D_SOC_DISPLAY, sizeof(Did_0E1D_SOC_DISPLAY));

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1D_SOC_DISPLAY, sizeof(Did_0E1D_SOC_DISPLAY));
	
	return 0;
}


/******************************* 0E1E *************************************/
uint16 DEA_Diag_GetDid_0E1E_ECO_APO(uint8* respData)
{
	tDid_0E1E_ECO_APO Did_0E1E_ECO_APO = {0};

	/*
	Did_0E1E_ECO_APO.ECO_APO = 0;        //True value needs to be filled
	*/

	//memcpy(&Did_0E1E_ECO_APO, &Ipc_Did_0E1E_ECO_APO, sizeof(Did_0E1E_ECO_APO));


	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1E_ECO_APO, sizeof(Did_0E1E_ECO_APO));
	
	return 0;
}


/******************************* 0E1F *************************************/
uint16 DEA_Diag_GetDid_0E1F_REGEN_BRAKE_TRQ(uint8* respData)
{
	tDid_0E1F_REGEN_BRAKE_TRQ Did_0E1F_REGEN_BRAKE_TRQ = {0};

	/*
	Did_0E1F_REGEN_BRAKE_TRQ.REGEN_BRAKE_TRQ = 0;      //True value needs to be filled
	*/

	//memcpy(&Did_0E1F_REGEN_BRAKE_TRQ, &Ipc_Did_0E1F_REGEN_BRAKE_TRQ, sizeof(Did_0E1F_REGEN_BRAKE_TRQ));
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E1F_REGEN_BRAKE_TRQ, sizeof(Did_0E1F_REGEN_BRAKE_TRQ));
	
	return 0;
}


/******************************* 0E20 *************************************/
uint16 DEA_Diag_GetDid_0E20_MECHA_BRAKE_TRQ(uint8* respData)
{
	tDid_0E20_MECHA_BRAKE_TRQ Did_0E20_MECHA_BRAKE_TRQ = {0};

	/*
	Did_0E20_MECHA_BRAKE_TRQ.MECHA_BRAKE_TRQ = 0;     //True value needs to be filled
	*/

	//memcpy(&Did_0E20_MECHA_BRAKE_TRQ, &Ipc_Did_0E20_MECHA_BRAKE_TRQ, sizeof(Did_0E20_MECHA_BRAKE_TRQ));

	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E20_MECHA_BRAKE_TRQ, sizeof(Did_0E20_MECHA_BRAKE_TRQ));
	
	return 0;
}


/******************************* 0E21 *************************************/
uint16 DEA_Diag_GetDid_0E21_ENERGY_FLOW(uint8* respData)
{
	tDid_0E21_ENERGY_FLOW Did_0E21_ENERGY_FLOW = {0};

	/*
	Did_0E21_ENERGY_FLOW.ENERGY_FLOW = 0;     //True value needs to be filled
	*/

	//memcpy(&Did_0E21_ENERGY_FLOW, &Ipc_Did_0E21_ENERGY_FLOW, sizeof(Did_0E21_ENERGY_FLOW));
	
	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E21_ENERGY_FLOW, sizeof(Did_0E21_ENERGY_FLOW));
	
	return 0;
}


/******************************* 0E22 *************************************/
uint16 DEA_Diag_GetDid_0E22_Info(uint8* respData)
{
	uint16 s_res = 0u;
	tDid_0E22_Info Did_0E22_Info = {0};

	Did_0E22_Info.Target_Front_Vehicle = 0;     //True value needs to be filled
	Did_0E22_Info.ECO_meter_conv = DEA_GetECO_DISP_RQ();

	//memcpy(&Did_0E22_Info, &Ipc_Did_0E22_Info, sizeof(Did_0E22_Info));
	
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E22_Info, sizeof(Did_0E22_Info));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E23 *************************************/
uint16 DEA_Diag_GetDid_0E23_OIL_DIST_K9K(uint8* respData)
{
	uint16 receive = 0;     //True value needs to be filled, such as 0xFFEE

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E24 *************************************/
uint16 DEA_Diag_GetDid_0E24_Info(uint8* respData)
{
	uint16 s_res = 0u;
	tDid_0E24_Info Did_0E24_Info = {0};

	/*
	Did_0E24_Info.Cruise_Ready_IND = 0;           //True value needs to be filled
	Did_0E24_Info.BCI_Right = 0;
	Did_0E24_Info.BCI_Left = 0;
	Did_0E24_Info.BCI = 0;
	Did_0E24_Info.Set_Lane_timing = 0;
	Did_0E24_Info.Set_BSW = 0;
	Did_0E24_Info.Set_LDW = 0;
	Did_0E24_Info.Set_FEB = 0;
	Did_0E24_Info.Set_LDP = 0;
	Did_0E24_Info.Set_DCA = 0;
	Did_0E24_Info.Set_BCI_brake_redume = 0;
	Did_0E24_Info.Set_BSWBSI_Brightness = 0;
	Did_0E24_Info.Set_BCI_Ignition_Status = 0;
	Did_0E24_Info.Set_RMOD = 0;
	Did_0E24_Info.Set_FCA_timing = 0;
	Did_0E24_Info.Set_BSI = 0;
	Did_0E24_Info.IDM_Fail = 0;
	Did_0E24_Info.MSS_Rader_Deactive = 0;
	Did_0E24_Info.MSS_Pedal_High_Temp = 0;
	Did_0E24_Info.Set_IBA = 0;
	Did_0E24_Info.Set_DAS = 0;
	Did_0E24_Info.MSS_Info_Warn_1 = 0;
	Did_0E24_Info.Set_TSR = 0;
	Did_0E24_Info.Set_NoEntry = 0;
	Did_0E24_Info.Set_EAP = 0;
	Did_0E24_Info.MT_R_SW = 0;
	Did_0E24_Info.Sig_Warn_Sta = 0;
	Did_0E24_Info.MSS_Info_Warn_3 = 0;
	Did_0E24_Info.MSS_Info_Warn_2 = 0;
	Did_0E24_Info.VSP_OFF_IND = 0;
	Did_0E24_Info.Low_HEVBatSOC = 0;
	Did_0E24_Info.HEV_sys_state = 0;
	Did_0E24_Info.MSS_Info_Warn_4 = 0;
	*/

	//Did_0E24_Info.Set_BSW = 0;
	//Did_0E24_Info.Set_LDW = 0;
	//Did_0E24_Info.Set_FEB = 0;
	
	if(SysSettingBSWSwitchGet() == 1u)
	{
		Did_0E24_Info.Set_BSW = 0b10;
	}else
	{
		Did_0E24_Info.Set_BSW = 0b01;
	}	
	if(SysSettingLDWSwitchGet() == 1u)
	{
		Did_0E24_Info.Set_LDW = 0b10;
	}else
	{
		Did_0E24_Info.Set_LDW = 0b01;
	}	
	if(SysSettingFEBSwitchGet() == 1u)
	{
		Did_0E24_Info.Set_FEB = 0b10;
	}else
	{
		Did_0E24_Info.Set_FEB = 0b01;
	}

	if(SysSettingEAPMSwitchGet() == 1u)
	{
		Did_0E24_Info.Set_EAP = 0b10;
	}else
	{
		Did_0E24_Info.Set_EAP = 0b01;
	}

	Did_0E24_Info.MT_R_SW = !GetPinValue_IOC_MT_R();
	
	//memcpy(&Did_0E24_Info, &Ipc_Did_0E24_Info, sizeof(Did_0E24_Info));

	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E24_Info, sizeof(Did_0E24_Info));
	}else
	{
		s_res = 1u;
	}
	
	return s_res;
}


/******************************* 0E25 *************************************/
uint16 DEA_Diag_GetDid_0E25_Tire_Press_FR(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	uint16 s_res = 0u;
	
	receive = DEA_GetTPD_FR();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, &receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E26 *************************************/
uint16 DEA_Diag_GetDid_0E26_Tire_Press_FL(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	uint16 s_res = 0u;

	receive = DEA_GetTPD_FL();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, &receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E27 *************************************/
uint16 DEA_Diag_GetDid_0E27_Tire_Press_RR(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xFF
	uint16 s_res = 0u;
	
	receive = DEA_GetTPD_RR();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, &receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
	return s_res;

}


/******************************* 0E28 *************************************/
uint16 DEA_Diag_GetDid_0E28_Tire_Press_RL(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled, such as 0xF
	uint16 s_res = 0u;
	
	receive = DEA_GetTPD_RL();
	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, &receive, sizeof(receive));
	}else
	{
		s_res = 1u;
	}
	return s_res;
}


/******************************* 0E29 *************************************/
/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint8_t BSW_IND_Get(void)
{
	uint8_t s_Bsw_Ind =0u;

	//uint8_t s_SET_W_BS =0u;
	//uint8_t s_STS_W_BS =0u;
	//uint8_t s_STS_D_BS =0u;

	//if((1U == semiActiveITS_BSWGet())&&(DEA_GetIgnStat() == IGN_ON))
	if((1U == semiActiveITS_BSWGet())&&(DEA_GetIgnStat() == IGN_ON)&&(getITMasterCtrlVerion() == 0b1111))
	{
		if(((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Left))&&(DEA_GetSRRL_SET() == 0b10)&&(DEA_GetFAILD_DT() == 0b1)) || \
			((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Right))&&(DEA_GetSRRR_SET_R() == 0b10)&&(DEA_GetFAILD_DT_R() == 0b1)))
		{
			//l_BSW_WhiteInd = TELLTALE_OFF;
			//l_BSW_YellowInd = TELLTALE_ON;	
			s_Bsw_Ind = 0b11;
			
		}else if(((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Left))&&(DEA_GetSRRL_SET() == 0b10)&&(DEA_GetALT_ST() == 0b100)) || \
			((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Right))&&(DEA_GetSRRR_SET_R() == 0b10)&&(DEA_GetALT_ST_R() == 0b100)))
		{
			//l_BSW_WhiteInd = TELLTALE_0p5Hz;
			//l_BSW_YellowInd = TELLTALE_OFF; 
			s_Bsw_Ind = 0b10;
			
		}else if(((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Left))&&(DEA_GetSRRL_SET() == 0b10)&&(DEA_GetBLOCK_DT() == 0b1)) || \
			((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Right))&&(DEA_GetSRRR_SET_R() == 0b10)&&(DEA_GetBLOCK_DT_R() == 0b1)))
		{
			//l_BSW_WhiteInd = TELLTALE_0p5Hz;
			//l_BSW_YellowInd = TELLTALE_OFF; 
			s_Bsw_Ind = 0b10;
			
		}else if(((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Left))&&(DEA_GetSRRL_SET() == 0b10)&&(DEA_GetALT_ST() == 0b001)&&(DEA_GetALT_IND() == 0b01)) || \
			((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Right))&&(DEA_GetSRRR_SET_R() == 0b10)&&(DEA_GetALT_ST_R() == 0b001)&&(DEA_GetALT_IND_R() == 0b01)))
		{
			//l_BSW_WhiteInd = TELLTALE_OFF;
			//l_BSW_YellowInd = TELLTALE_2p5Hz;
			s_Bsw_Ind = 0b11;
			
		}else if((0 == DEA_GetCanTimeOutStat(IPDU_HND_SRR_Left)) && (DEA_GetSRRL_SET() == 0b10) && (DEA_GetALT_ST() <= 0b011))
		{
			//l_BSW_WhiteInd = TELLTALE_ON;
			//l_BSW_YellowInd = TELLTALE_OFF; 
			s_Bsw_Ind = 0b01;
		}else
		{
			//l_BSW_WhiteInd = TELLTALE_OFF;
			//l_BSW_YellowInd = TELLTALE_OFF; 

			s_Bsw_Ind = 0b00;
		}
	}

	return s_Bsw_Ind;
	
}

uint16 DEA_Diag_GetDid_0E29_Info(uint8* respData)
{
	uint8_t s_res =0u;
	tDid_0E29_Info Did_0E29_Info = {0};

	/*
	Did_0E29_Info.LDP_IND = 0;           //True value needs to be filled
	Did_0E29_Info.DCA_IND = 0;
	Did_0E29_Info.IBA_IND = 0;
	Did_0E29_Info.BSW_IND = 0;
	Did_0E29_Info.MSS_No_Dynamic_Set = 0;
	Did_0E29_Info.MSS_No_Warn_Set = 0;
	Did_0E29_Info.MSS_Status_Warning_Fail = 0;
	Did_0E29_Info.Set_Speed_Flahing = 0;
	Did_0E29_Info.Status_Check_DSP = 0;
	Did_0E29_Info.BSI_IND = 0;
	*/
	Did_0E29_Info.BSW_IND = BSW_IND_Get();
	//memcpy(&Did_0E29_Info, &Ipc_Did_0E29_Info, sizeof(Did_0E29_Info));

	
	if(respData != NULL_PTR)
	{
		diagCpyData(respData, (uint8*)&Did_0E29_Info, sizeof(Did_0E29_Info));
	}else
	{
	 	s_res = 1u;
	}
	return s_res;
}



/******************************* 0E2A *************************************/
uint16 DEA_Diag_GetDid_0E2A_RAM_error_data(uint8* respData)
{
	uint8 receive = 0;     //True value needs to be filled

	if(respData != NULL_PTR)
		diagCpyData(respData, &receive, sizeof(receive));
	
	return 0;
}


/******************************* 0E2B *************************************/
uint16 DEA_Diag_GetDid_0E2B_Info(uint8* respData)
{
	tDid_0E2B_Info Did_0E2B_Info = {0};

	/*
	Did_0E2B_Info.PA_display_request = 0;           //True value needs to be filled
	Did_0E2B_Info.Ack_for_PA_display_request = 0;
	Did_0E2B_Info.Camera_disp_request = 0;
	Did_0E2B_Info.PA_fail_request = 0;
	Did_0E2B_Info.Steering_switch_out = 0;
	*/

	//memcpy(&Did_0E2B_Info, &Ipc_Did_0E2B_Info, sizeof(Did_0E2B_Info));

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E2B_Info, sizeof(Did_0E2B_Info));
	
	return 0;
}


/******************************* 0E2C *************************************/
uint16 DEA_Diag_GetDid_0E2C_Info(uint8* respData)
{
	tDid_0E2C_Info Did_0E2C_Info = {0};

	/*
	Did_0E2C_Info.CALL_ITS_STATUS = 0;           //True value needs to be filled
	Did_0E2C_Info.TJP_LANE_detect = 0;
	Did_0E2C_Info.TJP_operate_guide = 0;
	Did_0E2C_Info.STRG_ASSIST_STATUS = 0;
	Did_0E2C_Info.TJP_HAND_FREE = 0;
	Did_0E2C_Info.ICC_TJP_CANCEL_INFO = 0;
	Did_0E2C_Info.Emergency_Warning = 0;
	Did_0E2C_Info.SET_Steering_Assist = 0;
	Did_0E2C_Info.INFO_DISTANCE_SCALE = 0;
	Did_0E2C_Info.SET_WARN_REAR = 0;
	Did_0E2C_Info.SET_CALL_DISP = 0;
	Did_0E2C_Info.SET_PEDAL_MIS = 0;
	Did_0E2C_Info.MES_TRAFFIC_SIGN = 0;
	*/

	//memcpy(&Did_0E2C_Info, &Ipc_Did_0E2C_Info, sizeof(Did_0E2C_Info));

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E2C_Info, sizeof(Did_0E2C_Info));
	
	return 0;
}


uint16 DEA_Diag_GetDid_0E80_Info(uint8* respData)
{
	tDid_0E80_Info Did_0E80_Info = {0};
	//Byte#4
	Did_0E80_Info.MD65_FUEL_CUP_W_L = 1;
	Did_0E80_Info.MM6_Water_temperature_gauge = 1;
	Did_0E80_Info.MM5_Fuel_meter = 1;
	Did_0E80_Info.MM4_Tachometer = 1;
	Did_0E80_Info.MM3_ODO_output = 1;
	Did_0E80_Info.MM2_SPEED_OUTPUT = 1;
	Did_0E80_Info.MM1_SPEED_INPUT = 1;
	//Byte#5
	Did_0E80_Info.MD8_Tum_IND = 1;
	Did_0E80_Info.MD7_High_beam_IND = 1;
	Did_0E80_Info.MD6_Trunk_hatchback = 1;
	Did_0E80_Info.MD5_Door_W_L = 1;
	Did_0E80_Info.MD4_Brate_W_L = 1;
	Did_0E80_Info.MD3_SLIP_IND = 1;
	Did_0E80_Info.MD2_VDC_TCS_W_L = 1;
	Did_0E80_Info.MD1_ABS_W_L = 1;
	//Byte#6
	Did_0E80_Info.MD16_CRUISE_IND = 1;
	Did_0E80_Info.MD15_C_ENG2_W_L = 0;
	Did_0E80_Info.MD14_GLOW_IND = 0;
	Did_0E80_Info.MD13_C_ENG_W_L = 1;
	Did_0E80_Info.MD12_OIL_W_L = 1;
	Did_0E80_Info.MD11_Light_IND = 1;
	Did_0E80_Info.MD10_Rr_fog_IND = 1;
	Did_0E80_Info.MD9_Fr_fog_IND = 1;
	//Byte#7
	Did_0E80_Info.MD24_SPORT_IND = 1;
	Did_0E80_Info.MD23_CVT_IND = 1;
	Did_0E80_Info.MD22_AT_oil_tmp_W_L = 0;
	Did_0E80_Info.MD21_ATC_T_AMT_W_L = 0;
	Did_0E80_Info.MD20_O_D_OFF_IND = 0;
	Did_0E80_Info.MD19_BA_W_L = 0;
	Did_0E80_Info.MD18_CRUISE_W_L = 0;
	Did_0E80_Info.MD17_SET_IND = 0;
	//Byte#8
	Did_0E80_Info.MD32_KEY_knob_W_L = 0;
	Did_0E80_Info.MD31_KEY_red_W_L = 1;
	Did_0E80_Info.MD30_KEY_green_yellow_IND = 0;
	Did_0E80_Info.MD29_Tire_pressure_W_L = 1;
	Did_0E80_Info.MD28_WASHER_W_L = 0;
	Did_0E80_Info.MD27_Fuel_W_L = 1;
	Did_0E80_Info.MD26_4WD_LOCK_IND = 0;
	Did_0E80_Info.MD25_4WD_W_L = 0;
	//Byte#9
	Did_0E80_Info.MD40_SLOW_IND = 1;
	Did_0E80_Info.MD39_LDP_G_IND = 0;
	Did_0E80_Info.MD38_LDP_Y_IND = 0;
	Did_0E80_Info.MD37_HDC_W_L = 1;
	Did_0E80_Info.MD36_4WAS_RAS_W_L = 0;
	Did_0E80_Info.MD35_AFS_OFF_IND = 0;
	Did_0E80_Info.MD34_e_4WD_W_L = 0;
	Did_0E80_Info.MD33_EPS_W_L = 1;
	//Byte#10
	Did_0E80_Info.MD48_L_K_MAS_W_L = 0;
	Did_0E80_Info.MD47_SFT_operation_W_L = 0;
	Did_0E80_Info.MD46_HEV_brake_W_L = 0;
	Did_0E80_Info.MD45_HV_BAT_W_L = 0;
	Did_0E80_Info.MD44_SFT_POSI_W_L = 0;
	Did_0E80_Info.MD43_SYS_FAIL_W_L = 0;
	Did_0E80_Info.MD42_INVT_TEMP_IND = 0;
	Did_0E80_Info.MD41_READY_IND = 1;
	//Byte#11
	Did_0E80_Info.MD56_E_SUS_IND = 0;
	Did_0E80_Info.MD55_ATP_W_L = 0;
	Did_0E80_Info.MD54_ECO_MODE_IND = 1;
	Did_0E80_Info.MD53_Trailer_IND = 0;
	Did_0E80_Info.MD52_DPF_W_L = 0;
	Did_0E80_Info.MD51_RESERVE = 0;
	Did_0E80_Info.MD50_Charge_W_L = 1;
	Did_0E80_Info.MD49_Idle_stop_IND = 1;
	//Byte#12
	Did_0E80_Info.MD64_GEAR_SFT_IND = 0;
	Did_0E80_Info.MD63_TOW_mode_IND = 0;
	Did_0E80_Info.MD62_4WD_AUTO_IND = 0;
	Did_0E80_Info.MD61_CHECK_SUS_IND = 0;
	Did_0E80_Info.MD60_SCR_W_L = 0;
	Did_0E80_Info.MD59_FILTER_W_L = 0;
	Did_0E80_Info.MD58_MTSyncRev_IND = 0;
	Did_0E80_Info.MD57_DCA_IND = 0;
	//Byte#13
	Did_0E80_Info.ML8_4WD_IND = 0;
	Did_0E80_Info.ML7_AT_CVT_WARN = 0;
	Did_0E80_Info.ML6_ACC_unit = 1;
	Did_0E80_Info.ML5_ACC_set_speed = 1;
	Did_0E80_Info.ML4_ACC_own_vehicle = 0;
	Did_0E80_Info.ML3_ACC_target_distance = 0;
	Did_0E80_Info.ML2_ACC_target = 0;
	Did_0E80_Info.ML1_PUSH_ENG_IND = 0;
	//Byte#14
	Did_0E80_Info.MG33_AS_BELT_SW = 1;
	Did_0E80_Info.MG32_MTSyncRev_SW = 0;
	Did_0E80_Info.ML14_I_DRIVE = 0;
	Did_0E80_Info.ML13_SOW_O_IND = 0;
	Did_0E80_Info.ML12_SOW_G_IND = 0;
	Did_0E80_Info.ML11_ECO_PDL_O_IND = 0;
	Did_0E80_Info.ML10_ECO_PDL_G_IND = 1;
	Did_0E80_Info.ML9_OFFLOAD_IND = 1;
	//Byte#15
	Did_0E80_Info.MG8_Not_M_range_SW = 1;
	Did_0E80_Info.MG7_M_range_SW = 1;
	Did_0E80_Info.MG6_AT_power_mode_SW = 0;
	Did_0E80_Info.MG5_AT_Snow_mode_SW = 0;
	Did_0E80_Info.MG4_RANGE_1_SW = 1;
	Did_0E80_Info.MG3_O_D_OFF_SW = 1;
	Did_0E80_Info.MG2_Inhibitor_N_SW = 1;
	Did_0E80_Info.MG1_Inhibitor_P_SW = 1;
	//Byte#16
	Did_0E80_Info.MG16_A_C_LOW_TEMP = 0;
	Did_0E80_Info.MG15_R_DEF_ON = 0;
	Did_0E80_Info.MG14_H_FAN_ON = 0;
	Did_0E80_Info.MG13_A_C_ON_SIG = 0;
	Did_0E80_Info.MG12_AT_STR_DWN_SW = 0;
	Did_0E80_Info.MG11_AT_STR_UP_SW = 0;
	Did_0E80_Info.MG10_AT_SFT_DWN_SW = 0;
	Did_0E80_Info.MG9_AT_SFT_UP_SW = 0;
	//Byte#17
	Did_0E80_Info.MG24_AIR_BAG_SIG = 1;
	Did_0E80_Info.MG23_BRAKE_OIL_SW = 1;
	Did_0E80_Info.MG22_BRAKE_SW = 1;
	Did_0E80_Info.MG21_BELT_SW = 1;
	Did_0E80_Info.MG20_PKB_SW = 1;
	Did_0E80_Info.MG19_SYNC_MODE = 0;
	Did_0E80_Info.MG18_4WD_LOCK_SW = 0;
	Did_0E80_Info.MG17_A_C_PD_CUT = 0;
	//Byte#18
	Did_0E80_Info.MG34_TOW_mode_SW = 0;
	Did_0E80_Info.MG31_MODE_B_SW = 0;
	Did_0E80_Info.MG30_MODE_A_SW = 0;
	Did_0E80_Info.MG29_AMB_POWER = 0;
	Did_0E80_Info.MG28_VDC_TCS_GND = 0;
	Did_0E80_Info.MG27_ABS_GND = 0;
	Did_0E80_Info.MG26_RR_COMB_status = 0;
	Did_0E80_Info.MG25_CHG_SIG = 0;
	//Byte#19
	Did_0E80_Info.MG38_SPORT_MODE_SW = 0;
	Did_0E80_Info.MC7_DR_DOOR_SIG = 0;
	Did_0E80_Info.MC6_DOOR_LOCK_SIG = 0;
	Did_0E80_Info.MC5_ST_CNT_SIG = 0;
	Did_0E80_Info.MC4_SIG = 0;
	Did_0E80_Info.MC3_FLU_SIG = 0;
	Did_0E80_Info.MC2_Ambient_temperature = 1;
	Did_0E80_Info.MC1_DTE = 1;
	//Byte#20
	Did_0E80_Info.BAT1_BAT_CIR_STA = 0;
	Did_0E80_Info.MG37_LED_Lmp_L_cut = 1;
	Did_0E80_Info.MG36_LED_Lmp_R_cut = 1;
	Did_0E80_Info.MG35_ECO_MODE_SW = 0;
	Did_0E80_Info.MV1_VOLT_meter = 0;
	Did_0E80_Info.MH2_SOC_meter = 0;
	Did_0E80_Info.MH1_General_power_meter = 0;
	Did_0E80_Info.MB1_Buzzer = 1;
	//Byte#21
	Did_0E80_Info.TMMG8_Torque_Distribution = 0;
	Did_0E80_Info.TMGI29_Tire_Status_RL = 1;
	Did_0E80_Info.TMGI28_Tire_Status_RR = 1;
	Did_0E80_Info.TMGI27_Tire_Status_FL = 1;
	Did_0E80_Info.TMGI26_Tire_Status_FR = 1;
	Did_0E80_Info.TMGI25_TPMS_DISP = 1;
	Did_0E80_Info.STRG2_STRG_SW_output = 1;
	Did_0E80_Info.STRG1_STRG_SW_input = 1;
	//Byte#22
	Did_0E80_Info.SNR8_Sonar_det_DSParea_RR = 1;
	Did_0E80_Info.SNR7_Sonar_det_DSP_RR = 1;
	Did_0E80_Info.SNR6_Sonar_det_DSParea_RL = 1;
	Did_0E80_Info.SNR5_Sonar_det_DSP_RL = 1;
	Did_0E80_Info.SNR4_Sonar_det_DSParea_RC = 1;
	Did_0E80_Info.SNR3_Sonar_det_DSP_RC = 1;
	Did_0E80_Info.SNR2_Sonar_Wam = 1;
	Did_0E80_Info.SNR1_Sonar_det_sta = 1;
	//Byte#23
	Did_0E80_Info.TMGI8_TPMS_puncture = 1;
	Did_0E80_Info.SNR15_Sonar_dist_DSP = 1;
	Did_0E80_Info.SNR14_Sonar_det_DSParea_FR = 1;
	Did_0E80_Info.SNR13_Sonar_det_DSP_FR = 1;
	Did_0E80_Info.SNR12_Sonar_det_DSParea_FL = 1;
	Did_0E80_Info.SNR11_Sonar_det_DSP_FL = 1;
	Did_0E80_Info.SNR10_Sonar_det_DSParea_FC = 1;
	Did_0E80_Info.SNR9_Sonar_det_DSP_FC = 1;
	//Byte#24
	Did_0E80_Info.TMGI16_VDC_fail = 0;
	Did_0E80_Info.TMGI15_ABS_fail = 0;
	Did_0E80_Info.TMGI14_E_SUS_fail = 0;
	Did_0E80_Info.TMGI13_4WD_system_fail = 0;
	Did_0E80_Info.TMGI12_4WD_tire_check = 0;
	Did_0E80_Info.TMGI11_4WD_clutch_hi_tmp = 0;
	Did_0E80_Info.TMGI10_TPMS_fail = 1;
	Did_0E80_Info.TMGI9_TPMS_low_press = 1;
	//Byte#25
	Did_0E80_Info.TMGI24_Tire_press_RL = 1;
	Did_0E80_Info.TMGI23_Tire_press_RR = 1;
	Did_0E80_Info.TMGI22_Tire_press_FL = 1;
	Did_0E80_Info.TMGI21_Tire_press_FR = 1;
	Did_0E80_Info.TMGI20_Brake_pad = 0;
	Did_0E80_Info.TMGI19_OVER_BOOST = 0;
	Did_0E80_Info.TMGI18_ASCD_system_fail = 0;
	Did_0E80_Info.TMGI17_engine_system_check = 0;
	//Byte#26
	Did_0E80_Info.ENG5_ABSOLUTE_TIME = 0;
	Did_0E80_Info.ENG4_OIL_RESET = 0;
	Did_0E80_Info.ENG3_OIL_DISTANCE = 0;
	Did_0E80_Info.ENG2_SCR_DISTANCE = 0;
	Did_0E80_Info.ENG1_SCR_REMAINING = 0;
	Did_0E80_Info.TMAS3_CC_SL_REQ_speed = 0;
	Did_0E80_Info.TMAS2_CC_SL_condition = 0;
	Did_0E80_Info.TMAS1_CC_SL_speed_blink = 0;
	//Byte#27
	Did_0E80_Info.ENG9_IDLE_SW = 1;
	Did_0E80_Info.ENG8_IDLE_STOP_JDG = 0;
	Did_0E80_Info.ENG7_IDLE_STOP_IND = 1;
	Did_0E80_Info.ENG6_SCR_OIL_DISP = 0;
	//Byte#28
	Did_0E80_Info.HEV8_MECHA_BRAKE_TRQ = 0;
	Did_0E80_Info.HEV7_REGEN_BRAKE_TRQ = 0;
	Did_0E80_Info.HEV6_ECO_APO = 0;
	Did_0E80_Info.HEV5_APO_DISPLAY = 0;
	Did_0E80_Info.HEV4_SOC_DISPLAY = 0;
	Did_0E80_Info.HEV3_EV_IND = 0;
	Did_0E80_Info.HEV2_ASST_CHG = 0;
	Did_0E80_Info.HEV1_CL2_WARN = 0;
	//Byte#29
	Did_0E80_Info.SNR20_Parking_Aids_Range = 0;
	Did_0E80_Info.SNR19_Parking_Aids_Volume = 0;
	Did_0E80_Info.SNR18_Parking_Aids_Sensor = 0;
	Did_0E80_Info.SNR17_Parking_Aids_DSP = 0;
	Did_0E80_Info.SNR16_Sonar_Set_Ava = 0;
	Did_0E80_Info.ENG10_ECO_METER_CONV = 0;
	Did_0E80_Info.HEV9_ENERGY_FLOW = 0;
	//Byte#30
	Did_0E80_Info.ADAS7_BSW_IND = 1;
	Did_0E80_Info.ADAS6_LDW_IND = 1;
	Did_0E80_Info.ADAS5_FCW_IND = 1;
	Did_0E80_Info.ADAS4_Lane_IND = 0;
	Did_0E80_Info.ADAS3_Target_Front_Vehicle = 0;
	Did_0E80_Info.ADAS2_Cruise_Ready_IND = 0;
	Did_0E80_Info.ADAS1_ITS_SONAR_SET_output = 1;
	//Byte#31
	Did_0E80_Info.ADAS15_MSS_No_Wam_Set = 0;
	Did_0E80_Info.ADAS14_MSS_Status_Waming_Fail = 0;
	Did_0E80_Info.ADAS13_Set_Speed_Flashing = 0;
	Did_0E80_Info.ADAS12_Status_Check_DSP = 0;
	Did_0E80_Info.ADAS11_BSI_IND = 0;
	Did_0E80_Info.ADAS10_LDP_IND = 0;
	Did_0E80_Info.ADAS9_DCA_IND = 0;
	Did_0E80_Info.ADAS8_IBA_IND = 0;
	//Byte#32
	Did_0E80_Info.ADAS23_Set_Lane_timing = 0;
	Did_0E80_Info.ADAS22_Set_BSW = 1;
	Did_0E80_Info.ADAS21_Set_LDW = 1;
	Did_0E80_Info.ADAS20_Set_FEB = 1;
	Did_0E80_Info.ADAS19_BCI_Right = 0;
	Did_0E80_Info.ADAS18_BCI_Left = 0;
	Did_0E80_Info.ADAS17_BCI = 0;
	Did_0E80_Info.ADAS16_MSS_No_Dynamic_Set = 0;
	//Byte#33
	Did_0E80_Info.ADAS31_Set_BCI_ignition_Status = 0;
	Did_0E80_Info.ADAS30_Set_RMOD = 0;
	Did_0E80_Info.ADAS29_Set_FCA_timing = 0;
	Did_0E80_Info.ADAS28_Set_BSI = 0;
	Did_0E80_Info.ADAS27_Set_LDP = 0;
	Did_0E80_Info.ADAS26_Set_DCA = 0;
	Did_0E80_Info.ADAS25_Set_BCI_brake_redume = 0;
	Did_0E80_Info.ADAS24_Set_BSW_Brightness = 0;
	//Byte#34
	Did_0E80_Info.ADAS39_Sig_Wam_Sta = 0;
	Did_0E80_Info.ADAS38_MSS_Info_Wam_4 = 0;
	Did_0E80_Info.ADAS37_MSS_Info_Wam_3 = 0;
	Did_0E80_Info.ADAS36_MSS_Info_Wam_2 = 0;
	Did_0E80_Info.ADAS35_MSS_Info_Wam_1 = 0;
	Did_0E80_Info.ADAS34_MSS_Rader_Deactive = 0;
	Did_0E80_Info.ADAS33_MSS_Pedal_High_Temp = 0;
	Did_0E80_Info.ADAS32_Set_IBA = 0;
	//Byte#35
	Did_0E80_Info.ADAS47_DAS_Wam_Lv = 0;
	Did_0E80_Info.ADAS46_IDM_Fail = 0;
	Did_0E80_Info.ADAS45_ITS_Pedal_Fail = 0;
	Did_0E80_Info.ADAS44_ITS_Wam_Fail = 1;
	Did_0E80_Info.ADAS43_ALL_ITS_Wam_Fail = 0;
	Did_0E80_Info.ADAS42_Affordable_ITS_Disable = 0;
	Did_0E80_Info.ADAS41_ALL_ITS_Desable = 0;
	Did_0E80_Info.ADAS40_Side_Rader_Wam = 0;
	//Byte#36
	Did_0E80_Info.MG39_MT_R_SW = 0;
	Did_0E80_Info.ADAS74_Set_TSR = 0;
	Did_0E80_Info.ADAS73_Set_NoEntry = 0;
	Did_0E80_Info.ADAS72_Set_EAP = 0;
	Did_0E80_Info.ADAS71_Set_DAS = 0;
	Did_0E80_Info.ADAS49_ITS_Camera_Wam = 0;
	Did_0E80_Info.ADAS48_DAS_Wam_Msg = 0;
	//Byte#37
	Did_0E80_Info.MD73_Low_Bat_SOC = 0;
	Did_0E80_Info.MD72_SCM_Waming = 0;
	Did_0E80_Info.MD71_Auto_Hi_Lo_beam_IND = 0;
	Did_0E80_Info.MD70_Dipped_beam_IND = 0;
	Did_0E80_Info.MD69_VSP_OFF_IND = 0;
	Did_0E80_Info.MD68_Low_Water_temp_IND = 0;
	Did_0E80_Info.MD67_AirBag_W_L = 0;
	//Byte#38
	Did_0E80_Info.MD80_E_PKB_IND = 1;
	Did_0E80_Info.MD79_EAP_Waming = 0;
	Did_0E80_Info.MD78_Low_HEVBatSOC = 0;
	Did_0E80_Info.MD77_HEV_sys_state = 0;
	Did_0E80_Info.MD76_E_PKB_W_L = 0;
	Did_0E80_Info.MD75_Battery_Saver = 0;
	Did_0E80_Info.MD74_Light_reminder_Waming = 0;
	//Byte#39
	Did_0E80_Info.EV8_regene_W_L = 0;
	Did_0E80_Info.EV7_ECO_METER = 0;
	Did_0E80_Info.EV6_ECO_gauge = 0;
	Did_0E80_Info.EV5_regene_MAX = 0;
	Did_0E80_Info.EV4_POWER_MAX = 0;
	Did_0E80_Info.EV3_BAT_tmp = 0;
	Did_0E80_Info.EV2_BAT_rate = 0;
	Did_0E80_Info.EV1_BAT_remain = 0;
	//Byte#40
	Did_0E80_Info.EV16_SLOW_Disp = 0;
	Did_0E80_Info.EV15_SOON_CHG_Disp = 0;
	Did_0E80_Info.EV14_Push_Wrong = 0;
	Did_0E80_Info.EV13_PBW_Disp = 0;
	Did_0E80_Info.EV12_SFT_P_W_DSP = 0;
	Did_0E80_Info.MD81_PBW_W_L = 0;
	//Byte#41
	Did_0E80_Info.EV24_ePKB_sig = 0;
	Did_0E80_Info.EV23_PBW_FAIL_SIG = 0;
	Did_0E80_Info.EV21_DCDC_W_disp = 0;
	Did_0E80_Info.EV20_Chrge_state = 0;
	Did_0E80_Info.EV19_200V_chg_time = 0;
	Did_0E80_Info.EV18_100V_chg_time = 0;
	Did_0E80_Info.EV17_ePKB_Disp = 0;
	//Byte#42
	Did_0E80_Info.EV32_DTEinput = 0;
	Did_0E80_Info.EV31_DTE_def = 0;
	Did_0E80_Info.EV30_ePKB_Disp4 = 0;
	Did_0E80_Info.EV29_ePKB_Disp3 = 0;
	Did_0E80_Info.EV28_ePKB_Disp2 = 0;
	Did_0E80_Info.EV27_ePKB_Disp1 = 0;
	Did_0E80_Info.EV26_EV_BAT_SOH_seg = 0;
	Did_0E80_Info.EV25_EV_BAT_SOC_seg = 0;
	//Byte#43
	Did_0E80_Info.KEY1_Key_Fob_Infomation = 0;
	Did_0E80_Info.EV37_DTE_flash = 1;
	Did_0E80_Info.EV36_ELE_CONP_ON = 0;
	Did_0E80_Info.EV35_ELE_CONP_OFF = 0;
	Did_0E80_Info.EV34_BAT_LOW_W_L = 0;
	Did_0E80_Info.EV33_DTE_2nd_wam = 1;
	//Byte#44
	Did_0E80_Info.ADAS57_TSR_sub_sign2 = 0;
	Did_0E80_Info.ADAS56_TSR_sub_sign1 = 0;
	Did_0E80_Info.ADAS55_TSR_status2 = 0;
	Did_0E80_Info.ADAS54_TSR_value2 = 0;
	Did_0E80_Info.ADAS53_TSR_status1 = 0;
	Did_0E80_Info.ADAS52_TSR_value1 = 0;
	Did_0E80_Info.ADAS51_TSR_High_temp = 0;
	Did_0E80_Info.ADAS50_TSR_Fail = 0;
	//Byte#45
	Did_0E80_Info.MM7_Distance_Unit = 1;
	Did_0E80_Info.ADAS59_TSR_overtake = 0;
	Did_0E80_Info.ADAS58_TSR_sub_sign3 = 0;
	//Byte#46
	Did_0E80_Info.ADAS67_SET_Steering_Assist = 0;
	Did_0E80_Info.ADAS66_Emergency_Waming = 0;
	Did_0E80_Info.ADAS65_ICC_TJP_CANCEL_INFO = 0;
	Did_0E80_Info.ADAS64_TJP_HAND_FREE = 0;
	Did_0E80_Info.ADAS63_STRG_ASSIST_STATUS = 0;
	Did_0E80_Info.ADAS62_TJP_operate_guide = 0;
	Did_0E80_Info.ADAS61_TJP_LANE_detect = 0;
	Did_0E80_Info.ADAS60_CALL_ITS_STATUS = 0;
	//Byte#47
	Did_0E80_Info.CAM05_Steering_switch_out = 0;
	Did_0E80_Info.CAM04_PA_fail_request = 0;
	Did_0E80_Info.CAM03_Camera_disp_request = 0;
	Did_0E80_Info.CAM02_Ack_for_PA_display_request = 0;
	Did_0E80_Info.CAM01_PA_display_request = 0;
	Did_0E80_Info.ADAS70_MES_TRAFFIC_SIGN = 0;
	Did_0E80_Info.ADAS69_SET_CALL_DISP = 0;
	Did_0E80_Info.ADAS68_SET_WARN_REAR = 0;

	if(respData != NULL_PTR)
		diagCpyData(respData, (uint8*)&Did_0E80_Info, sizeof(Did_0E80_Info));
	
	return 0;
}


void Diag_DidDataTest(uint16_t pdid)
{
	uint8 databuf[43]={0};
	uint8 res =0;
	uint8 cycle =0;
	//static uint16 did = 0xF40D;
	uint16 did = 0x0E07;
	
	did = pdid;
	switch (did)
	{
		case 0xF40D:
		{
			res = DEA_Diag_GetDid_F40D_Speed_ometer(databuf);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0xF40D]:reqstatus %d \r\n", res);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0xF40D]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E00;
		}
		break;

		case 0x0E00:
		{
			res = DEA_Diag_GetDid_0E00_Speed_output(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E00]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E00]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}

			//did = 0x0E01;
		}
		break;

		case 0x0E01:
		{
			res = DEA_Diag_GetDid_0E01_ODO_output(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E01]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E01]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E02;
		}
		break;

		case 0x0E02:
		{
			res = DEA_Diag_GetDid_0E02_Tachometer(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E02]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E02]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E03;
		}
		break;

		case 0x0E03:
		{
			res = DEA_Diag_GetDid_0E03_Fuel_meter(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E03]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E03]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E04;
		}
		break;

		case 0x0E04:
		{
			res = DEA_Diag_GetDid_0E04_Water_temperature(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E04]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E04]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E08;
		}
		break;

		case 0x0E07:
		{
			res = DEA_Diag_GetDid_0E07_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E07]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E07]:data \r\n");
			//did = 0x0E13;
		}
		break;

		case 0x0E08:
		{
			res = DEA_Diag_GetDid_0E08_Distance_To_Empty(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E08]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E08]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E09;
		}
		break;

		case 0x0E09:
		{
			res = DEA_Diag_GetDid_0E09_Ambient_temperature(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E09]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E09]:data \r\n");
			
			for (cycle = 0; cycle < 20; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E07;
		}
		break;


		case 0x0E0A:
		{
			res = DEA_Diag_GetDid_0E0A_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E0A]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E0A]:data \r\n");
			for (cycle = 0; cycle < 3; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
		}
		break;

		case 0x0E0F:
		{
			res = DEA_Diag_GetDid_0E0F_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E0F]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E0F]:data \r\n");
			for (cycle = 0; cycle < 10; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E19;
		}
		break;

		case 0x0E12:
		{
			res = DEA_Diag_GetDid_0E12_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E12]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E12]:data \r\n");
			for (cycle = 0; cycle < 10; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E19;
		}
		break;
		
		case 0x0E13:
		{
			res = DEA_Diag_GetDid_0E13_CC_SL_REQ(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E13]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E13]:data \r\n");
			for (cycle = 0; cycle < 1; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E25;
		}
		break;

		case 0x0E19:
		{
			res = DEA_Diag_GetDid_0E19_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E19]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E19]:data \r\n");
			for (cycle = 0; cycle < 10; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E80;
		}
		break;

		case 0x0E22:
		{
			res = DEA_Diag_GetDid_0E22_Info(databuf);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E22]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E22]:data \r\n");
			for (cycle = 0; cycle < 3; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0xF40D;
		}
		break;

		case 0x0E24:
		{
			res = DEA_Diag_GetDid_0E24_Info(databuf);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E24]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E24]:data \r\n");
			for (cycle = 0; cycle < 5; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0xF40D;
		}
		break;
		
		case 0x0E25:
		{
			res = DEA_Diag_GetDid_0E25_Tire_Press_FR(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E25]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E25]:data \r\n");
			for (cycle = 0; cycle < 1; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E26;
		}
		break;
		
		case 0x0E26:
		{
			res = DEA_Diag_GetDid_0E26_Tire_Press_FL(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E26]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E26]:data \r\n");
			for (cycle = 0; cycle < 1; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E27;
		}
		break;
		
		case 0x0E27:
		{
			res = DEA_Diag_GetDid_0E27_Tire_Press_RR(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E27]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E27]:data \r\n");
			for (cycle = 0; cycle < 1; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E28;
		}
		break;
		
		case 0x0E28:
		{
			res = DEA_Diag_GetDid_0E28_Tire_Press_RL(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E28]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E28]:data \r\n");
			for (cycle = 0; cycle < 1; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E0F;
		}
		break;

		case 0x0E29:
		{
			res = DEA_Diag_GetDid_0E29_Info(databuf);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E29]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E29]:data \r\n");
			for (cycle = 0; cycle < 5; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0xF40D;
		}
		break;
		
		case 0x0E80:
		{
			res = DEA_Diag_GetDid_0E80_Info(databuf);
			
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E80]:reqstatus %d \r\n", res);
			TracePrintf(COM_TRACE, TRACE_NONMASK, "[Diag_DID_0x0E80]:data \r\n");
			for (cycle = 0; cycle < 43; cycle++)
			{
				TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
			}
			//did = 0x0E22;
		}
		break;
		

		default:
		break;
	}
	
}


void DEA_Diag_GetDid_0E07_Infotest(uint8* databuf,uint8_t len)
{
	uint8_t cycle = 0;
	for (cycle = 0; cycle < len; cycle++)
	{
		TracePrintf(COM_TRACE, TRACE_NONMASK, "data%d = %x \r\n",cycle ,databuf[cycle]);
	}
}


uint16_t str_to_hex(uint8_t *buf)
{
	uint8_t i;
	uint16_t ret = 0;
	for(i = 0; i < 4; i++)
	{
		ret <<= 4;
		if(buf[i] >= '0' && buf[i] <= '9')
		  ret += buf[i]-'0';
		else if(buf[i] >= 'a' && buf[i] <= 'f')
		  ret += buf[i]-'a' + 0x0A;
		else if(buf[i] >= 'A' && buf[i] <= 'F')
		  ret += buf[i]-'A' + 0x0A;
		else
	  	return 0xFFFF;
	}
	return ret;
}



