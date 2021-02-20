#ifndef __DEA_DIAG_H_
#define __DEA_DIAG_H_

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "Std_Types.h"
#include "diagCommon_if.h"
#include "diagAppWllr_cfg.h"

typedef enum
{
	DIAG_DEA_OK,
	DIAG_DEA_ERROR,
}DIAG_DEA_RET_ENUM;


/********************************* 0E01 ***********************************/
typedef struct{

	uint32 ODO_output:24;
	uint32           :8;
	
}tDid_0E01_ODO_output;


/********************************* 0E07 ***********************************/
typedef struct{
//Byte#4
	uint8 High_beam_IND:1;
	uint8 Trunk_hatchback:1;
	uint8 Door_WL:1;
	uint8 E_PKB_WL:1;
	uint8 Brake_WL:1;
	uint8 SLIP_IND:1;
	uint8 VDC_OFF_WL:1;
	uint8 ABS_WL:1;
//Byte#5
	uint8 C_ENG2_WL:1;
	uint8 GLOW_IND:1;
	uint8 C_ENG_WL:1;
	uint8 OIL_WL:1;
	uint8 Light_IND:1;
	uint8 Rr_fog_IND:1;
	uint8 Fr_fog_IND:1;
	uint8 Turn_IND:1;
//Byte#6
	uint8 CVT_IND:1;
	uint8 AT_fluid_TEMP_WL:1;
	uint8 ATC_T_AMT_WL:1;
	uint8 O_D_OFF_IND:1;
	uint8 BA_WL:1;
	uint8 CRUISE_WL:1;
	uint8 SET_IND:1;
	uint8 CRUISE_IND:1;
//Byte#7
	uint8 KEY_red_WL:1;
	uint8 KEY_green_yellow_IND:1;
	uint8 Tire_pressure_WL:1;
	uint8 WASHER_WL:1;
	uint8 Fuel_WL:1;
	uint8 _4WD_LOCK_IND:1;
	uint8 _4WD_WL:1;
	uint8 SPORT_IND:1;
//Byte#8
	uint8 LDP_G_Y_IND:1;
	uint8 LDP_R_IND:1;
	uint8 HDC_WL:1;
	uint8 _4WAS_RAS_WL:1;
	uint8 AFS_OFF_IND:1;
	uint8 e_4WD_WL:1;
	uint8 EPS_WL:1;
	uint8 KEY_knob_WL:1;
//Byte#9
	uint8 SFT_operation_WL:1;
	uint8 HEV_brake_WL:1;
	uint8 HV_BAT_WL:1;
	uint8 SFT_POSI_WL:1;
	uint8 SYS_FAIL_WL:1;
	uint8 INVT_TEMP_IND:1;
	uint8 READY_IND:1;
	uint8 SLOW_IND:1;
//Byte#10
	uint8 ECO_MODE_IND:1;
	uint8 Trailer_IND:1;
	uint8 DPF_WL:1;
	uint8 _4WD_AUTO_IND:2;
	uint8 Charge_WL:1;
	uint8 Idle_stop_IND:1;
	uint8 L_K_MAS_WL:1;
//Byte#11
	uint8 SCR_WL:2;
	uint8 FILTER_WL:1;
	uint8 MTSyncRev_IND:1;
	uint8 DCA_IND:1;
	uint8 E_SUS_IND:2;
	uint8 ATP_WL:1;
//Byte#12
	uint8 ACC_own_vehicle:1;
	uint8 ACC_target_distance:2;
	uint8 ACC_target:1;
	uint8 PUSH_ENG_IND:4;
//Byte#13
	uint8 ACC_set_speed;
//Byte#14
	uint8 GEAR_SFT_IND:2;
	uint8 AT_CVT_RNG:5;
	uint8 ACC_unit:1;
//Byte#15
	uint8 SOW_G_IND:1;
	uint8 ECO_PDL_O_IND:1;
	uint8 ECO_PDL_G_IND:1;
	uint8 OFFLOAD_IND:2;
	uint8 _4WD_IND:3;
//Byte#16
	uint8 I_DRIVE:7;
	uint8 SOW_O_IND:1;
//Byte#17
	uint8 E_PKB_IND:1;
	uint8 FUEL_CAP_WL:1;
	uint8 CHECK_SUS_IND:1;
	uint8 TOW_mode_IND:1;
	uint8 SCR_OIL_DISP:3;
	uint8             :1;
//Byte#18
	uint8 Out_of_M_range_SW:1;
	uint8 M_range_SW:1;
	uint8 AT_power_mode_SW:1;
	uint8 AT_snow_mode_SW:1;
	uint8 RANGE_1_SW:1;
	uint8 O_D_OFF_SW:1;
	uint8 Inhibitor_N_SW:1;
	uint8 Inhibitor_P_SW:1;
//Byte#19
	uint8 A_C_LOW_TEMP:1;
	uint8 R_DEF_ON:1;
	uint8 H_FAN_ON:1;
	uint8 A_C_ON_SIG:1;
	uint8 AT_STR_DWN_SW:1;
	uint8 AT_STR_UP_SW:1;
	uint8 AT_SFT_DWN_SW:1;
	uint8 AT_SFT_UP_SW:1;
//Byte#20
	uint8 AIR_BAG_SIG:1;
	uint8 BRAKE_OIL_SW:1;
	uint8 Brake_SW:1;
	uint8 BELT_SW:1;
	uint8 PKB_SW:1;
	uint8 SYNC_MODE:1;
	uint8 _4WD_LOCK_SW:1;
	uint8 A_C_PD_CUT:1;
//Byte#21
	uint8 MTSyncRev_SW:1;
	uint8 MODE_B_SW:1;
	uint8 MODE_A_SW:1;
	uint8 AMB_POWER:1;
	uint8 VDC_TCS_GND:1;
	uint8 ABS_GND:1;
	uint8 RR_COMB_status:1;
	uint8 CHG_SIG:1;
//Byte#22
	uint8 LED_Lmp_L_cut:1;
	uint8 LED_Lmp_R_cut:1;
	uint8 CHG_conct_det:1;
	uint8 PBW_WL:1;
	uint8 SPORT_MODE_SW:1;
	uint8 ECO_MODE_SW:1;
	uint8 TOW_mode_SW:1;
	uint8 AS_BELT_SW:1;

}tDid_0E07_Info;



/********************************* 0E0A ***********************************/
typedef struct{
//Byte#4
	uint8            :2;
	uint8 DR_DOOR_SIG:1;
	uint8 DOOR_LOCK_SIG:1;
	uint8 ST_CNT_SIG:1;
	uint8 CRANK_SIG:2;
	uint8 FLU_SIG:1;
//Byte#5
	uint8 Parking_Aids_Volume:2;
	uint8 Parking_Aids_Sensor:3;
	uint8 Parking_Aids_DSP:1;
	uint8 BAT_CIR_STA:1;
	uint8 Buzzer:1;
//Byte#6
	uint8                   :6;
	uint8 Parking_Aids_Range:2;

}tDid_0E0A_Info;



/********************************* 0E0B ***********************************/
typedef struct{

	uint16 General_power_meter:12;
	uint16                    :4;

}tDid_0E0B_General_power_meter;


/********************************* 0E0E ***********************************/
typedef struct{

	uint16          :1;
	uint16 Brake_pad:15;

}tDid_0E0E_General_Brake_pad;


/********************************* 0E0F ***********************************/
typedef struct{
//Byte#4
	uint8              :1;
	uint8 Sonar_Set_Ava:1;
	uint8 Tire_status_RL:1;
	uint8 Tire_status_RR:1;
	uint8 Tire_status_FL:1;
	uint8 Tire_status_FR:1;
	uint8 TPMS_DISP:2;
//Byte#5
	uint8 STRG_SW_output:4;
	uint8 STRG_SW_input:4;
//Byte#6
	uint8                    :1;
	uint8 Auto_Hi_Lo_beam_IND:1;
	uint8 ITS_Sonar_SET_OUTPUT:6;
//Byte#7
	uint8 Dipped_Beam:1;
	uint8 Sonar_det_DSParea:1;
	uint8 Sonar_det_DSP_RC:3;
	uint8 Sonar_Warn:2;
	uint8 Sonar_det_sta:1;
//Byte#8
	uint8 Sonar_det_DSParea_RR:1;
	uint8 Sonar_det_DSP_RR:3;
	uint8 Sonar_det_DSParea_RL:1;
	uint8 Sonar_det_DSP_RL:3;
//Byte#9
	uint8 Sonar_det_DSParea_FL:1;
	uint8 Sonar_det_DSP_FL:3;
	uint8 Sonar_det_DSParea_FC:1;
	uint8 Sonar_det_DSP_FC:3;
//Byte#10
	uint8 Sonar_dist_DSP:4;
	uint8 Sonar_det_DSParea_FR:1;
	uint8 Sonar_det_DSP_FR:3;

}tDid_0E0F_Info;



/********************************* 0E10 ***********************************/
typedef struct{

	uint16               :4;
	uint16 ENG_shut_timer:12;
	
}tDid_0E10_ENG_shut_timer;



/********************************* 0E12 ***********************************/
typedef struct{
//Byte#4
	uint8        :2;
	uint8 LDW_IND:2;
	uint8 FEB_IND:2;
	uint8 Lane_IND:2;
//Byte#5
	uint8 TPMS_puncture_byte5:1;
	uint8 :7;
//Byte#6
	uint8 TPMS_low_press:4;
	uint8 TPMS_puncture_byte6:4;
//Byte#7
	uint8 ABS_fail:3;
	uint8 E_SUS_fail:1;
	uint8 _4WD_system_fail:1;
	uint8 _4WD_tire_check:1;
	uint8 _4WD_clutch_hi_tmp:1;
	uint8 TPMS_fail:1;
//Byte#8
	uint8 engine_system_check:4;
	uint8 VDC_fail:4;
//Byte#9
	uint8           :4;
	uint8 OVER_BOOST:1;
	uint8 ASCD_system_fail:3;
//Byte#10
	uint8			     :4;
	uint8 CC_SL_condition:3;
	uint8 CC_SL_speed_blink:1;

}tDid_0E12_Info;



/********************************* 0E15 ***********************************/
typedef struct{

	uint16             :2;
	uint16 SCR_DISTANCE:14;
	
}tDid_0E15_SCR_DISTANCE;


/********************************* 0E16 ***********************************/
typedef struct{

	uint16             :4;
	uint16 OIL_DISTANCE:12;
	
}tDid_0E16_OIL_DISTANCE;


/********************************* 0E17 ***********************************/
typedef struct{
	uint8          :1;
	uint8 OIL_RESET:1;
	uint8          :6;

}tDid_0E17_OIL_RESET;


/********************************* 0E18 ***********************************/
typedef struct{

	uint16              :4;
	uint16 ABSOLUTE_TIME:12;
	
}tDid_0E18_ABSOLUTE_TIME;



/********************************* 0E19 ***********************************/
typedef struct{

	uint8 CL2_WARN:1;
	uint8 IDLE_SW_IND:1;
	uint8 IDLE_STOP_JDG:1;
	uint8 IDLE_STOP_IND:5;
	
}tDid_0E19_Info;


/********************************* 0E1A ***********************************/
typedef struct{

	uint8         :1;
	uint8 ASST_CHG:7;
	
}tDid_0E1A_ASST_CHG;


/********************************* 0E1B ***********************************/
typedef struct{

	uint8       :7;
	uint8 EV_IND:1;
	
}tDid_0E1B_EV_IND;


/********************************* 0E1C ***********************************/
typedef struct{

	uint8            :1;
	uint8 APO_DISPLAY:7;
	
}tDid_0E1C_APO_DISPLAY;


/********************************* 0E1D ***********************************/
typedef struct{

	uint8            :1;
	uint8 SOC_DISPLAY:7;
	
}tDid_0E1D_SOC_DISPLAY;


/********************************* 0E1E ***********************************/
typedef struct{

	uint8 ECO_APO:7;
	uint8        :1;
	
}tDid_0E1E_ECO_APO;


/********************************* 0E1F ***********************************/
typedef struct{

	uint8 REGEN_BRAKE_TRQ:7;
	uint8                :1;
	
}tDid_0E1F_REGEN_BRAKE_TRQ;


/********************************* 0E20 ***********************************/
typedef struct{

	uint8 MECHA_BRAKE_TRQ:7;
	uint8                :1;
	
}tDid_0E20_MECHA_BRAKE_TRQ;



/********************************* 0E21 ***********************************/
typedef struct{

	uint16            :4;
	uint16 ENERGY_FLOW:12;
	
}tDid_0E21_ENERGY_FLOW;



/********************************* 0E22 ***********************************/
typedef struct{

	uint8                     :1;
	uint8 Target_Front_Vehicle:2;
	uint8 ECO_meter_conv:5;
	
}tDid_0E22_Info;


/********************************* 0E24 ***********************************/
typedef struct{
//Byte#4
	uint8 Cruise_Ready_IND:1;
	uint8 BCI_Right:2;
	uint8 BCI_Left:2;
	uint8 BCI:3;
//Byte#5
	uint8 Set_Lane_timing:2;
	uint8 Set_BSW:2;
	uint8 Set_LDW:2;
	uint8 Set_FEB:2;
//Byte#6
	uint8 Set_LDP:2;
	uint8 Set_DCA:2;
	uint8 Set_BCI_brake_redume:2;
	uint8 Set_BSWBSI_Brightness:2;
//Byte#7
	uint8 Set_BCI_Ignition_Status:2;
	uint8 Set_RMOD:2;
	uint8 Set_FCA_timing:2;
	uint8 Set_BSI:2;
//Byte#8
	uint8         :2;
	uint8 IDM_Fail:1;
	uint8 MSS_Rader_Deactive:1;
	uint8 MSS_Pedal_High_Temp:2;
	uint8 Set_IBA:2;
//Byte#9
	uint8 Set_DAS:2;
	uint8 MSS_Info_Warn_1:6;
//Byte#10
	uint8        :2;
	uint8 Set_TSR:2;
	uint8 Set_NoEntry:2;
	uint8 Set_EAP:2;
//Byte#11
	uint8 MT_R_SW:1;
	uint8        :7;
//Byte#12
	uint8 Sig_Warn_Sta:1;
	uint8 MSS_Info_Warn_3:3;
	uint8 MSS_Info_Warn_2:4;
//Byte#13
	uint8 VSP_OFF_IND:1;
	uint8 Low_HEVBatSOC:2;
	uint8 HEV_sys_state:2;
	uint8 MSS_Info_Warn_4:3;

}tDid_0E24_Info;



/********************************* 0E29 ***********************************/
typedef struct{
//Byte#4
	uint8 LDP_IND:2;
	uint8 DCA_IND:2;
	uint8 IBA_IND:2;
	uint8 BSW_IND:2;
//Byte#5
	uint8                   :1;
	uint8 MSS_No_Dynamic_Set:1;
	uint8 MSS_No_Warn_Set:1;
	uint8 MSS_Status_Warning_Fail:1;
	uint8 Set_Speed_Flahing:1;
	uint8 Status_Check_DSP:1;
	uint8 BSI_IND:2;
//Byte#6   Reserve
	//uint8 :8;

}tDid_0E29_Info;



/********************************* 0E2B ***********************************/
typedef struct{
//Byte#4
	uint8 PA_display_request:2;
	uint8                   :6;
//Byte#5
	uint8 Ack_for_PA_display_request:2;
	uint8                           :6;
//Byte#6
	uint8 Camera_disp_request:2;
	uint8			  	     :6;
//Byte#7
	uint8 PA_fail_request:1;
	uint8				 :7;
//Byte#8
	uint8 Steering_switch_out:4;
	uint8				     :4;

}tDid_0E2B_Info;



/********************************* 0E2C ***********************************/
typedef struct{
//Byte#4
	uint8 CALL_ITS_STATUS:1;
	uint8				 :7;
//Byte#5
	uint8 TJP_LANE_detect:1;
	uint8				 :7;
//Byte#6
	uint8 TJP_operate_guide:3;
	uint8				   :5;
//Byte#7
	uint8 STRG_ASSIST_STATUS:3;
	uint8				    :5;
//Byte#8
	uint8 TJP_HAND_FREE:1;
	uint8			   :7;
//Byte#9
	uint8 ICC_TJP_CANCEL_INFO:3;
	uint8			         :5;
//Byte#10
	uint8 Emergency_Warning:1;
	uint8				   :7;
//Byte#11
	uint8 SET_Steering_Assist:2;
	uint8				     :6;
//Byte#12
	uint8 INFO_DISTANCE_SCALE;
//Byte#13
	uint8 SET_WARN_REAR:2;
	uint8              :6;
//Byte#14
	uint8 SET_CALL_DISP:2;
	uint8			   :6;
//Byte#15
	uint8 SET_PEDAL_MIS:2;
	uint8			   :6;
//Byte#16
	uint8 MES_TRAFFIC_SIGN:2;
	uint8			      :6;

}tDid_0E2C_Info;


/********************************* L-3-3-1 ***********************************/
typedef struct{
//Byte#4
	uint8 MD65_FUEL_CUP_W_L 		   :1;
	uint8							   :1;
	uint8 MM6_Water_temperature_gauge  :1;
	uint8 MM5_Fuel_meter			   :1;
	uint8 MM4_Tachometer			   :1;
	uint8 MM3_ODO_output			   :1;
	uint8 MM2_SPEED_OUTPUT			   :1;
	uint8 MM1_SPEED_INPUT			   :1;
//Byte#5
	uint8 MD8_Tum_IND          :1;
	uint8 MD7_High_beam_IND    :1;
	uint8 MD6_Trunk_hatchback  :1;
	uint8 MD5_Door_W_L         :1;
	uint8 MD4_Brate_W_L        :1;
	uint8 MD3_SLIP_IND         :1;
	uint8 MD2_VDC_TCS_W_L      :1;
	uint8 MD1_ABS_W_L          :1;
//Byte#6
	uint8 MD16_CRUISE_IND :1;
	uint8 MD15_C_ENG2_W_L :1;
	uint8 MD14_GLOW_IND   :1;
	uint8 MD13_C_ENG_W_L  :1;
	uint8 MD12_OIL_W_L    :1;
	uint8 MD11_Light_IND  :1;
	uint8 MD10_Rr_fog_IND :1;
	uint8 MD9_Fr_fog_IND  :1;
//Byte#7
	uint8 MD24_SPORT_IND      :1;
	uint8 MD23_CVT_IND        :1;
	uint8 MD22_AT_oil_tmp_W_L :1;
	uint8 MD21_ATC_T_AMT_W_L  :1;
	uint8 MD20_O_D_OFF_IND    :1;
	uint8 MD19_BA_W_L         :1;
	uint8 MD18_CRUISE_W_L     :1;
	uint8 MD17_SET_IND        :1;
//Byte#8
	uint8 MD32_KEY_knob_W_L         :1;
	uint8 MD31_KEY_red_W_L          :1;
	uint8 MD30_KEY_green_yellow_IND :1;
	uint8 MD29_Tire_pressure_W_L    :1;
	uint8 MD28_WASHER_W_L           :1;
	uint8 MD27_Fuel_W_L             :1;
	uint8 MD26_4WD_LOCK_IND         :1;
	uint8 MD25_4WD_W_L              :1;
//Byte#9
	uint8 MD40_SLOW_IND     :1;
	uint8 MD39_LDP_G_IND    :1;
	uint8 MD38_LDP_Y_IND    :1;
	uint8 MD37_HDC_W_L      :1;
	uint8 MD36_4WAS_RAS_W_L :1;
	uint8 MD35_AFS_OFF_IND  :1;
	uint8 MD34_e_4WD_W_L    :1;
	uint8 MD33_EPS_W_L      :1;
//Byte#10
	uint8 MD48_L_K_MAS_W_L       :1;
	uint8 MD47_SFT_operation_W_L :1;
	uint8 MD46_HEV_brake_W_L     :1;
	uint8 MD45_HV_BAT_W_L        :1;
	uint8 MD44_SFT_POSI_W_L      :1;
	uint8 MD43_SYS_FAIL_W_L      :1;
	uint8 MD42_INVT_TEMP_IND     :1;
	uint8 MD41_READY_IND         :1;
//Byte#11
	uint8 MD56_E_SUS_IND     :1;
	uint8 MD55_ATP_W_L       :1;
	uint8 MD54_ECO_MODE_IND  :1;
	uint8 MD53_Trailer_IND   :1;
	uint8 MD52_DPF_W_L       :1;
	uint8 MD51_RESERVE       :1;
	uint8 MD50_Charge_W_L    :1;
	uint8 MD49_Idle_stop_IND :1;
//Byte#12
	uint8 MD64_GEAR_SFT_IND  :1;
	uint8 MD63_TOW_mode_IND  :1;
	uint8 MD62_4WD_AUTO_IND  :1;
	uint8 MD61_CHECK_SUS_IND :1;
	uint8 MD60_SCR_W_L       :1;
	uint8 MD59_FILTER_W_L    :1;
	uint8 MD58_MTSyncRev_IND :1;
	uint8 MD57_DCA_IND       :1;
//Byte#13
	uint8 ML8_4WD_IND             :1;
	uint8 ML7_AT_CVT_WARN         :1;
	uint8 ML6_ACC_unit            :1;
	uint8 ML5_ACC_set_speed       :1;
	uint8 ML4_ACC_own_vehicle     :1;
	uint8 ML3_ACC_target_distance :1;
	uint8 ML2_ACC_target          :1;
	uint8 ML1_PUSH_ENG_IND        :1;
//Byte#14
	uint8 MG33_AS_BELT_SW    :1;
	uint8 MG32_MTSyncRev_SW  :1;
	uint8 ML14_I_DRIVE       :1;
	uint8 ML13_SOW_O_IND     :1;
	uint8 ML12_SOW_G_IND     :1;
	uint8 ML11_ECO_PDL_O_IND :1;
	uint8 ML10_ECO_PDL_G_IND :1;
	uint8 ML9_OFFLOAD_IND    :1;
//Byte#15
	uint8 MG8_Not_M_range_SW   :1;
	uint8 MG7_M_range_SW       :1;
	uint8 MG6_AT_power_mode_SW :1;
	uint8 MG5_AT_Snow_mode_SW  :1;
	uint8 MG4_RANGE_1_SW       :1;
	uint8 MG3_O_D_OFF_SW       :1;
	uint8 MG2_Inhibitor_N_SW   :1;
	uint8 MG1_Inhibitor_P_SW   :1;
//Byte#16
	uint8 MG16_A_C_LOW_TEMP  :1;
	uint8 MG15_R_DEF_ON      :1;
	uint8 MG14_H_FAN_ON      :1;
	uint8 MG13_A_C_ON_SIG    :1;
	uint8 MG12_AT_STR_DWN_SW :1;
	uint8 MG11_AT_STR_UP_SW  :1;
	uint8 MG10_AT_SFT_DWN_SW :1;
	uint8 MG9_AT_SFT_UP_SW   :1;
//Byte#17
	uint8 MG24_AIR_BAG_SIG  :1;
	uint8 MG23_BRAKE_OIL_SW :1;
	uint8 MG22_BRAKE_SW     :1;
	uint8 MG21_BELT_SW      :1;
	uint8 MG20_PKB_SW       :1;
	uint8 MG19_SYNC_MODE    :1;
	uint8 MG18_4WD_LOCK_SW  :1;
	uint8 MG17_A_C_PD_CUT   :1;
//Byte#18
	uint8 MG34_TOW_mode_SW    :1;
	uint8 MG31_MODE_B_SW      :1;
	uint8 MG30_MODE_A_SW      :1;
	uint8 MG29_AMB_POWER      :1;
	uint8 MG28_VDC_TCS_GND    :1;
	uint8 MG27_ABS_GND        :1;
	uint8 MG26_RR_COMB_status :1;
	uint8 MG25_CHG_SIG        :1;
//Byte#19
	uint8 MG38_SPORT_MODE_SW      :1;
	uint8 MC7_DR_DOOR_SIG         :1;
	uint8 MC6_DOOR_LOCK_SIG       :1;
	uint8 MC5_ST_CNT_SIG          :1;
	uint8 MC4_SIG                 :1;
	uint8 MC3_FLU_SIG             :1;
	uint8 MC2_Ambient_temperature :1;
	uint8 MC1_DTE                 :1;
//Byte#20
	uint8 BAT1_BAT_CIR_STA        :1;
	uint8 MG37_LED_Lmp_L_cut      :1;
	uint8 MG36_LED_Lmp_R_cut      :1;
	uint8 MG35_ECO_MODE_SW        :1;
	uint8 MV1_VOLT_meter          :1;
	uint8 MH2_SOC_meter           :1;
	uint8 MH1_General_power_meter :1;
	uint8 MB1_Buzzer              :1;
//Byte#21
	uint8 TMMG8_Torque_Distribution :1;
	uint8 TMGI29_Tire_Status_RL     :1;
	uint8 TMGI28_Tire_Status_RR     :1;
	uint8 TMGI27_Tire_Status_FL     :1;
	uint8 TMGI26_Tire_Status_FR     :1;
	uint8 TMGI25_TPMS_DISP          :1;
	uint8 STRG2_STRG_SW_output      :1;
	uint8 STRG1_STRG_SW_input       :1;
//Byte#22
	uint8 SNR8_Sonar_det_DSParea_RR :1;
	uint8 SNR7_Sonar_det_DSP_RR     :1;
	uint8 SNR6_Sonar_det_DSParea_RL :1;
	uint8 SNR5_Sonar_det_DSP_RL     :1;
	uint8 SNR4_Sonar_det_DSParea_RC :1;
	uint8 SNR3_Sonar_det_DSP_RC     :1;
	uint8 SNR2_Sonar_Wam            :1;
	uint8 SNR1_Sonar_det_sta        :1;
//Byte#23
	uint8 TMGI8_TPMS_puncture        :1;
	uint8 SNR15_Sonar_dist_DSP       :1;
	uint8 SNR14_Sonar_det_DSParea_FR :1;
	uint8 SNR13_Sonar_det_DSP_FR     :1;
	uint8 SNR12_Sonar_det_DSParea_FL :1;
	uint8 SNR11_Sonar_det_DSP_FL     :1;
	uint8 SNR10_Sonar_det_DSParea_FC :1;
	uint8 SNR9_Sonar_det_DSP_FC      :1;
//Byte#24
	uint8 TMGI16_VDC_fail          :1;
	uint8 TMGI15_ABS_fail          :1;
	uint8 TMGI14_E_SUS_fail        :1;
	uint8 TMGI13_4WD_system_fail   :1;
	uint8 TMGI12_4WD_tire_check    :1;
	uint8 TMGI11_4WD_clutch_hi_tmp :1;
	uint8 TMGI10_TPMS_fail         :1;
	uint8 TMGI9_TPMS_low_press     :1;
//Byte#25
	uint8 TMGI24_Tire_press_RL       :1;
	uint8 TMGI23_Tire_press_RR       :1;
	uint8 TMGI22_Tire_press_FL       :1;
	uint8 TMGI21_Tire_press_FR       :1;
	uint8 TMGI20_Brake_pad           :1;
	uint8 TMGI19_OVER_BOOST          :1;
	uint8 TMGI18_ASCD_system_fail    :1;
	uint8 TMGI17_engine_system_check :1;
//Byte#26
	uint8 ENG5_ABSOLUTE_TIME      :1;
	uint8 ENG4_OIL_RESET          :1;
	uint8 ENG3_OIL_DISTANCE       :1;
	uint8 ENG2_SCR_DISTANCE       :1;
	uint8 ENG1_SCR_REMAINING      :1;
	uint8 TMAS3_CC_SL_REQ_speed   :1;
	uint8 TMAS2_CC_SL_condition   :1;
	uint8 TMAS1_CC_SL_speed_blink :1;
//Byte#27
	uint8                    :1;
	uint8                    :1;
	uint8                    :1;
	uint8                    :1;
	uint8 ENG9_IDLE_SW       :1;
	uint8 ENG8_IDLE_STOP_JDG :1;
	uint8 ENG7_IDLE_STOP_IND :1;
	uint8 ENG6_SCR_OIL_DISP  :1;
//Byte#28
	uint8 HEV8_MECHA_BRAKE_TRQ :1;
	uint8 HEV7_REGEN_BRAKE_TRQ :1;
	uint8 HEV6_ECO_APO         :1;
	uint8 HEV5_APO_DISPLAY     :1;
	uint8 HEV4_SOC_DISPLAY     :1;
	uint8 HEV3_EV_IND          :1;
	uint8 HEV2_ASST_CHG        :1;
	uint8 HEV1_CL2_WARN        :1;
//Byte#29
	uint8                           :1;
	uint8 SNR20_Parking_Aids_Range  :1;
	uint8 SNR19_Parking_Aids_Volume :1;
	uint8 SNR18_Parking_Aids_Sensor :1;
	uint8 SNR17_Parking_Aids_DSP    :1;
	uint8 SNR16_Sonar_Set_Ava       :1;
	uint8 ENG10_ECO_METER_CONV      :1;
	uint8 HEV9_ENERGY_FLOW          :1;
//Byte#30
	uint8 ADAS7_BSW_IND              :1;
	uint8 ADAS6_LDW_IND              :1;
	uint8 ADAS5_FCW_IND              :1;
	uint8 ADAS4_Lane_IND             :1;
	uint8 ADAS3_Target_Front_Vehicle :1;
	uint8 ADAS2_Cruise_Ready_IND     :1;
	uint8 ADAS1_ITS_SONAR_SET_output :1;
	uint8                            :1;
//Byte#31
	uint8 ADAS15_MSS_No_Wam_Set         :1;
	uint8 ADAS14_MSS_Status_Waming_Fail :1;
	uint8 ADAS13_Set_Speed_Flashing     :1;
	uint8 ADAS12_Status_Check_DSP       :1;
	uint8 ADAS11_BSI_IND                :1;
	uint8 ADAS10_LDP_IND                :1;
	uint8 ADAS9_DCA_IND                 :1;
	uint8 ADAS8_IBA_IND                 :1;
//Byte#32
	uint8 ADAS23_Set_Lane_timing    :1;
	uint8 ADAS22_Set_BSW            :1;
	uint8 ADAS21_Set_LDW            :1;
	uint8 ADAS20_Set_FEB            :1;
	uint8 ADAS19_BCI_Right          :1;
	uint8 ADAS18_BCI_Left           :1;
	uint8 ADAS17_BCI                :1;
	uint8 ADAS16_MSS_No_Dynamic_Set :1;
//Byte#33
	uint8 ADAS31_Set_BCI_ignition_Status :1;
	uint8 ADAS30_Set_RMOD                :1;
	uint8 ADAS29_Set_FCA_timing          :1;
	uint8 ADAS28_Set_BSI                 :1;
	uint8 ADAS27_Set_LDP                 :1;
	uint8 ADAS26_Set_DCA                 :1;
	uint8 ADAS25_Set_BCI_brake_redume    :1;
	uint8 ADAS24_Set_BSW_Brightness      :1;
//Byte#34
	uint8 ADAS39_Sig_Wam_Sta         :1;
	uint8 ADAS38_MSS_Info_Wam_4      :1;
	uint8 ADAS37_MSS_Info_Wam_3      :1;
	uint8 ADAS36_MSS_Info_Wam_2      :1;
	uint8 ADAS35_MSS_Info_Wam_1      :1;
	uint8 ADAS34_MSS_Rader_Deactive  :1;
	uint8 ADAS33_MSS_Pedal_High_Temp :1;
	uint8 ADAS32_Set_IBA             :1;
//Byte#35
	uint8 ADAS47_DAS_Wam_Lv             :1;
	uint8 ADAS46_IDM_Fail               :1;
	uint8 ADAS45_ITS_Pedal_Fail         :1;
	uint8 ADAS44_ITS_Wam_Fail           :1;
	uint8 ADAS43_ALL_ITS_Wam_Fail       :1;
	uint8 ADAS42_Affordable_ITS_Disable :1;
	uint8 ADAS41_ALL_ITS_Desable        :1;
	uint8 ADAS40_Side_Rader_Wam         :1;
//Byte#36
	uint8                       :1;
	uint8 MG39_MT_R_SW          :1;
	uint8 ADAS74_Set_TSR        :1;
	uint8 ADAS73_Set_NoEntry    :1;
	uint8 ADAS72_Set_EAP        :1;
	uint8 ADAS71_Set_DAS        :1;
	uint8 ADAS49_ITS_Camera_Wam :1;
	uint8 ADAS48_DAS_Wam_Msg    :1;
//Byte#37
	uint8 MD73_Low_Bat_SOC         :1;
	uint8 MD72_SCM_Waming          :1;
	uint8 MD71_Auto_Hi_Lo_beam_IND :1;
	uint8 MD70_Dipped_beam_IND     :1;
	uint8 MD69_VSP_OFF_IND         :1;
	uint8 MD68_Low_Water_temp_IND  :1;
	uint8 MD67_AirBag_W_L          :1;
	uint8                          :1;
//Byte#38
	uint8                            :1;
	uint8 MD80_E_PKB_IND             :1;
	uint8 MD79_EAP_Waming            :1;
	uint8 MD78_Low_HEVBatSOC         :1;
	uint8 MD77_HEV_sys_state         :1;
	uint8 MD76_E_PKB_W_L             :1;
	uint8 MD75_Battery_Saver         :1;
	uint8 MD74_Light_reminder_Waming :1;
//Byte#39
	uint8 EV8_regene_W_L :1;
	uint8 EV7_ECO_METER  :1;
	uint8 EV6_ECO_gauge  :1;
	uint8 EV5_regene_MAX :1;
	uint8 EV4_POWER_MAX  :1;
	uint8 EV3_BAT_tmp    :1;
	uint8 EV2_BAT_rate   :1;
	uint8 EV1_BAT_remain :1;
//Byte#40
	uint8 EV16_SLOW_Disp     :1;
	uint8 EV15_SOON_CHG_Disp :1;
	uint8 EV14_Push_Wrong    :1;
	uint8 EV13_PBW_Disp      :1;
	uint8 EV12_SFT_P_W_DSP   :1;
	uint8                    :1;
	uint8 MD81_PBW_W_L       :1;
	uint8                    :1;
//Byte#41
	uint8 EV24_ePKB_sig      :1;
	uint8 EV23_PBW_FAIL_SIG  :1;
	uint8                    :1;
	uint8 EV21_DCDC_W_disp   :1;
	uint8 EV20_Chrge_state   :1;
	uint8 EV19_200V_chg_time :1;
	uint8 EV18_100V_chg_time :1;
	uint8 EV17_ePKB_Disp     :1;
//Byte#42
	uint8 EV32_DTEinput       :1;
	uint8 EV31_DTE_def        :1;
	uint8 EV30_ePKB_Disp4     :1;
	uint8 EV29_ePKB_Disp3     :1;
	uint8 EV28_ePKB_Disp2     :1;
	uint8 EV27_ePKB_Disp1     :1;
	uint8 EV26_EV_BAT_SOH_seg :1;
	uint8 EV25_EV_BAT_SOC_seg :1;
//Byte#43
	uint8                         :1;
	uint8                         :1;
	uint8 KEY1_Key_Fob_Infomation :1;
	uint8 EV37_DTE_flash          :1;
	uint8 EV36_ELE_CONP_ON        :1;
	uint8 EV35_ELE_CONP_OFF       :1;
	uint8 EV34_BAT_LOW_W_L        :1;
	uint8 EV33_DTE_2nd_wam        :1;
//Byte#44
	uint8 ADAS57_TSR_sub_sign2 :1;
	uint8 ADAS56_TSR_sub_sign1 :1;
	uint8 ADAS55_TSR_status2   :1;
	uint8 ADAS54_TSR_value2    :1;
	uint8 ADAS53_TSR_status1   :1;
	uint8 ADAS52_TSR_value1    :1;
	uint8 ADAS51_TSR_High_temp :1;
	uint8 ADAS50_TSR_Fail      :1;
//Byte#45
	uint8                      :1;
	uint8                      :1;
	uint8                      :1;
	uint8                      :1;
	uint8 MM7_Distance_Unit    :1;
	uint8                      :1;
	uint8 ADAS59_TSR_overtake  :1;
	uint8 ADAS58_TSR_sub_sign3 :1;
//Byte#46
	uint8 ADAS67_SET_Steering_Assist :1;
	uint8 ADAS66_Emergency_Waming    :1;
	uint8 ADAS65_ICC_TJP_CANCEL_INFO :1;
	uint8 ADAS64_TJP_HAND_FREE       :1;
	uint8 ADAS63_STRG_ASSIST_STATUS  :1;
	uint8 ADAS62_TJP_operate_guide   :1;
	uint8 ADAS61_TJP_LANE_detect     :1;
	uint8 ADAS60_CALL_ITS_STATUS     :1;
//Byte#47
	uint8 CAM05_Steering_switch_out        :1;
	uint8 CAM04_PA_fail_request            :1;
	uint8 CAM03_Camera_disp_request        :1;
	uint8 CAM02_Ack_for_PA_display_request :1;
	uint8 CAM01_PA_display_request         :1;
	uint8 ADAS70_MES_TRAFFIC_SIGN          :1;
	uint8 ADAS69_SET_CALL_DISP             :1;
	uint8 ADAS68_SET_WARN_REAR             :1;

}tDid_0E80_Info;


/********************************* L-3-5-1 ***********************************/
	typedef struct{
	//Byte#4
		uint8 ABS_WL				:1;
		uint8 VDC_TCSOFF_WL			:1;
		uint8 SLIP_IND				:1;
		uint8 BRAKE_WL				:1;
		uint8 DOOR_WL				:1;
		uint8 Trunk_hatchback		:1;
		uint8 OIL_WL		    	:1;
		uint8 C_ENG_WL          	:1;
	//Byte#5
		uint8 C_ENG2_WL				:1;	
		uint8 CRUISE_IND			:1;
		uint8 SET_IND				:1;
		uint8 CRUISE_WL				:1;
		uint8 BA_WL					:1;
		uint8 O_D_OFF_IND			:1;
		uint8 ATC_T_AMT_WL			:1;
		uint8 AT_oil_tmp_WL			:1;
	//Byte#6
		uint8 CVT_IND				:1;
		uint8 SPORT_IND				:1;
		uint8 _4WD_WL				:1;
		uint8 Fuel_WL				:1;
		uint8 WASHER_WL				:1;
		uint8 Tire_pressure_WL		:1;
		uint8 KEY_green_yellow_IND	:1;
		uint8 KEY_red_WL			:1;
	//Byte#7
		uint8 KEY_knob_WL			:1;
		uint8 EPS_WL				:1;
		uint8 e_4WD_WL				:1;
		uint8 AFS_OFF_IND			:1;
		uint8 _4WAS_RAS_WL			:1;
		uint8 HDC_WL				:1;
		uint8 SYS_FAIL_WL			:1;
		uint8 SFT_POSI_WL			:1;
	//Byte#8
		uint8 HV_BAT_WL				:1;
		uint8 HEV_Brake_WL			:1;
		uint8 SFT_operetion_WL		:1;
		uint8 L_K_MAS_WL			:1;
		uint8 Charge_WL				:1;
		uint8 Charge2_WL			:1;
		uint8 OIL_LEV_WARN			:1;
		uint8 DPF_WL				:1;
	//Byte#9
		uint8 Trailer_IND			:1;
		uint8 Run_flat_WL			:1;
		uint8 E_SUS_WL				:1;
		uint8 Rounch_CNT_WL			:1;
		uint8 SOW_O_IND				:1;
		uint8 FILTER_WL				:1;
		uint8 Brake_pad_WL			:1;
		uint8 SCR_WL				:1;
	//Byte#10
		uint8 E_PKB_IND				:1;
		uint8 E_PKB_WL				:1;
		uint8 LED_Headlamp_WL		:1;
		uint8 ISS_IND				:1;
		uint8 Fuel_CAP_WL			:1;
		uint8 ENG_REMOTE_IND		:1;
		uint8 Battery_remaining_WL	:1;
		uint8 Ready_IND				:1;
	//Byte#11
		uint8 SLOW_IND				:1;
		uint8 caution_WL			:1;
		uint8 Battery_Brakedown_WL	:1;
		uint8 reserve1				:5;

	}tDid_L351_Info;

#define	Did_L351_Data_Len sizeof(tDid_L351_Info)

void DEA_Diag_RequestProc(uint32_t stat);


/**************************************************************************/
uint16 DEA_Diag_GetDid_F40D_Speed_ometer(uint8* respData);
uint16 DEA_Diag_GetDid_0E00_Speed_output(uint8* respData);
uint16 DEA_Diag_GetDid_0E01_ODO_output(uint8* respData);
uint16 DEA_Diag_GetDid_0E02_Tachometer(uint8* respData);
uint16 DEA_Diag_GetDid_0E03_Fuel_meter(uint8* respData);
uint16 DEA_Diag_GetDid_0E04_Water_temperature(uint8* respData);
//0x0E05 0x0E06 Reserve
uint16 DEA_Diag_GetDid_0E07_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E08_Distance_To_Empty(uint8* respData);
uint16 DEA_Diag_GetDid_0E09_Ambient_temperature(uint8* respData);
uint16 DEA_Diag_GetDid_0E0A_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E0B_General_power_meter(uint8* respData);
uint16 DEA_Diag_GetDid_0E0C_SOC_meter(uint8* respData);
uint16 DEA_Diag_GetDid_0E0D_VOLT_meter(uint8* respData);
uint16 DEA_Diag_GetDid_0E0E_Brake_pad(uint8* respData);
uint16 DEA_Diag_GetDid_0E0F_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E10_ENG_shut_timer(uint8* respData);
uint16 DEA_Diag_GetDid_0E11_Torque(uint8* respData);
uint16 DEA_Diag_GetDid_0E12_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E13_CC_SL_REQ(uint8* respData);
uint16 DEA_Diag_GetDid_0E14_SCR_REMAINING(uint8* respData);
uint16 DEA_Diag_GetDid_0E15_SCR_DISTANCE(uint8* respData);
uint16 DEA_Diag_GetDid_0E16_OIL_DISTANCE(uint8* respData);
uint16 DEA_Diag_GetDid_0E17_OIL_RESET(uint8* respData);
uint16 DEA_Diag_GetDid_0E18_ABSOLUTE_TIME(uint8* respData);
uint16 DEA_Diag_GetDid_0E19_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E1A_ASST_CHG(uint8* respData);
uint16 DEA_Diag_GetDid_0E1B_EV_IND(uint8* respData);
uint16 DEA_Diag_GetDid_0E1C_APO_DISPLAY(uint8* respData);
uint16 DEA_Diag_GetDid_0E1D_SOC_DISPLAY(uint8* respData);
uint16 DEA_Diag_GetDid_0E1E_ECO_APO(uint8* respData);
uint16 DEA_Diag_GetDid_0E1F_REGEN_BRAKE_TRQ(uint8* respData);
uint16 DEA_Diag_GetDid_0E20_MECHA_BRAKE_TRQ(uint8* respData);
uint16 DEA_Diag_GetDid_0E21_ENERGY_FLOW(uint8* respData);
uint16 DEA_Diag_GetDid_0E22_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E23_OIL_DIST_K9K(uint8* respData);
uint16 DEA_Diag_GetDid_0E24_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E25_Tire_Press_FR(uint8* respData);
uint16 DEA_Diag_GetDid_0E26_Tire_Press_FL(uint8* respData);
uint16 DEA_Diag_GetDid_0E27_Tire_Press_RR(uint8* respData);
uint16 DEA_Diag_GetDid_0E28_Tire_Press_RL(uint8* respData);
uint16 DEA_Diag_GetDid_0E29_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E2A_RAM_error_data(uint8* respData);
uint16 DEA_Diag_GetDid_0E2B_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E2C_Info(uint8* respData);
uint16 DEA_Diag_GetDid_0E80_Info(uint8* respData);

extern DIAG_DEA_RET_ENUM DEA_Diag_RequestSocData(uint16_t did, uint8_t *data, uint16_t len);/*yangxl*/
extern void DEA_Diag_DidL351_Handler(void);/*yangxl*/
void SetBuzzerStatus(uint16_t buzzID,uint8_t buzzstatus); //yangxl
uint16_t str_to_hex(uint8_t *buf);



extern boolean diagSetWarningLampLightingStatus(tInnerWlId innerID, tWlStatus wlStatus);




#endif

