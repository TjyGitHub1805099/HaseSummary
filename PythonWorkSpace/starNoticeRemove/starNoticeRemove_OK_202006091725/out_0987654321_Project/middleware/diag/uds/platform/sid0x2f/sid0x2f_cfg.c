

#include"sid0x2f_cfg.h"
#include"sid0x27_if.h"

const tSid0x2fCfgInfor gSid0x2fCfgInfor = {
	
};




const tIOCtrlInfo gIOControlInfo_ivi[] = {

	{IOCTRL_ID_DUMMY_TEST,TRUE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,sid0x2fDid0X0101DummyCheckExtraDataOrState,sid0x2fDid0X0101DummyStartTest,sid0x2fDid0X0101DummyStopTest},
	{IOCTRL_ID_SWITCH_SOC_MFG_MODE,TRUE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0XF11FVerifyPara,NULL,sid0x2fDid0XF11FStartTest,sid0x2fDid0XF11FStopTest},
};

tIOCtrlStatus gIOCtrlStatus_ivi[] = {
	{IOCTRL_ID_DUMMY_TEST,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_SOC_MFG_MODE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
};






const tIOCtrlInfo gIOControlInfo_ic[] = {
#if 0
	{IOCTRL_ID_DUMMY_TEST,TRUE,UDS_INNER_SECURITY_LEVEL_1 | UDS_INNER_SECURITY_LEVEL_2,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDid0X0101DummyStartTest,NULL},
	{IOCTRL_ID_SWITCH_MEDIA_SOURCE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_SOUND_EFFECT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_PLAY_SONG,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_LOOP_MODE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_OPEN_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLOSE_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLOSE_BT_PHONE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SWITCH_TO_BT_MUSIC,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SWITCH_TO_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_BT_MUSIC_CONTROL,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_BT_CALLIN_ACCEPT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_BT_AUDIO_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_OPEN_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLOSE_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CONNECT_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_OPEN_WIFI_HOTSPOT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLOSE_WIFI_HOTSPOT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_OPEN_GPS,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLOSE_GPS,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_GPS_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_ENTER_ROUTE_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SIMU_NAVI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_ENTER_NAVI_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_NAVI_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_ACTIVATE_NAVI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_GPS_DETAIL,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_SYS_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SET_SYS_MUTE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CLEAR_UUID,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_FAC_RESET,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_REVERSE_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_PICTURE_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_CHANGE_CAM,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
	{IOCTRL_ID_SWITCH_DVR,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
#endif

		{IOCTRL_ID_DUMMY_TEST,TRUE,UDS_INNER_SECURITY_LEVEL_1 | UDS_INNER_SECURITY_LEVEL_2,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDid0X0101DummyStartTest,NULL},
		{IOCTRL_ID_AUTO_TEST_INIT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_AUTO_TEST_FINISH,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_ENTER_AGING_MODE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_EXIT_AGING_MODE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_MEDIA_SOURCE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_SOUND_EFFECT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_PLAY_SONG,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_LOOP_MODE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_OPEN_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLOSE_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLOSE_BT_PHONE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_TO_BT_MUSIC,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_TO_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_BT_MUSIC_CONTROL,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_BT_CALLIN_ACCEPT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_BT_AUDIO_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CONNECT_BT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_OPEN_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLOSE_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CONNECT_WIFI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_PING_WIFI_ADDR,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_OPEN_WIFI_HOTSPOT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLOSE_WIFI_HOTSPOT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_WIFI_HOTSPOT,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_OPEN_GPS,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLOSE_GPS,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_GPS_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_ENTER_ROUTE_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_DESTNATION,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SIMU_NAVI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_ENTER_NAVI_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_NAVI_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_ACTIVATE_NAVI,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_GPS_DETAIL,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_SYS_VOLUME,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_SYS_MUTE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_UUID,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CLEAR_UUID,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_FAC_RESET,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_USB_VIDEO_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_TAIL_DOOR_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SET_FAN_SPEED,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_ENTER_AC_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_AVM_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_PICTURE_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CHANGE_CAM,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_DVR,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_MAIN_SEAT_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_CO_SEAT_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_CAR_FUNC_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_LIGHT_SET_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_SWITCH_AUTO_GO_PAGE,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
		{IOCTRL_ID_CALIBRATE_AMP,FALSE,UDS_INNER_SECURITY_LEVEL_0,sid0x2fDid0X0101DummyVerifyPara,NULL,sid0x2fDidAutoTestStartTest,NULL},
};


tIOCtrlStatus gIOCtrlStatus_ic[] = {
	{IOCTRL_ID_DUMMY_TEST,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_AUTO_TEST_INIT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_AUTO_TEST_FINISH,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_ENTER_AGING_MODE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_EXIT_AGING_MODE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},	
	{IOCTRL_ID_SWITCH_MEDIA_SOURCE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_SOUND_EFFECT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_PLAY_SONG,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_LOOP_MODE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_OPEN_BT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLOSE_BT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLOSE_BT_PHONE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_TO_BT_MUSIC,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_TO_BT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_BT_MUSIC_CONTROL,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_BT_CALLIN_ACCEPT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_BT_AUDIO_VOLUME,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CONNECT_BT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_OPEN_WIFI,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLOSE_WIFI,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CONNECT_WIFI,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_PING_WIFI_ADDR,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_OPEN_WIFI_HOTSPOT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLOSE_WIFI_HOTSPOT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_WIFI_HOTSPOT,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_OPEN_GPS,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLOSE_GPS,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_GPS_VOLUME,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_ENTER_ROUTE_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_DESTNATION,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SIMU_NAVI,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_ENTER_NAVI_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_NAVI_VOLUME,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_ACTIVATE_NAVI,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_GPS_DETAIL,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_SYS_VOLUME,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_SYS_MUTE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_UUID,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CLEAR_UUID,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_FAC_RESET,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_USB_VIDEO_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_TAIL_DOOR_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SET_FAN_SPEED,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_ENTER_AC_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_AVM_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_PICTURE_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CHANGE_CAM,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_DVR,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_MAIN_SEAT_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_CO_SEAT_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_CAR_FUNC_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_LIGHT_SET_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_SWITCH_AUTO_GO_PAGE,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
	{IOCTRL_ID_CALIBRATE_AMP,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
};




const tIOCtrlInfo *gIOControlInfoArray[UDS_CH_MAX_NUM] = {
	gIOControlInfo_ivi,
	gIOControlInfo_ic,
	NULL,
};


tIOCtrlStatus *gIOCtrlStatusArray[UDS_CH_MAX_NUM] = {
	gIOCtrlStatus_ivi,
	gIOCtrlStatus_ic,
	NULL,
};


uint8 gIOCtrlDidNum[UDS_CH_MAX_NUM];


void sid0x2fInit()
{
#if 0
	uint8 ch = 0;
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
		gIOCtrlDidNum[ch] = sizeof(gIOControlInfoArray[ch])/sizeof(tIOCtrlInfo);
#else
	gIOCtrlDidNum[0] = sizeof(gIOControlInfo_ivi)/sizeof(tIOCtrlInfo);
	gIOCtrlDidNum[1] = sizeof(gIOControlInfo_ic)/sizeof(tIOCtrlInfo);
	gIOCtrlDidNum[2] = 0;
#endif
}
