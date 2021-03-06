#ifndef _SID_0X2F_CFG_H
#define _SID_0X2F_CFG_H

#include"diagCommon_cfg.h"
#include"sid0x2f_if.h"



typedef enum{
	IOCTRL_ID_DUMMY_TEST = 0x0100,
	IOCTRL_ID_AUTO_TEST_INIT = 0x0101,
	IOCTRL_ID_AUTO_TEST_FINISH = 0x0102,
	IOCTRL_ID_ENTER_AGING_MODE = 0x0103,
	IOCTRL_ID_EXIT_AGING_MODE = 0x0105,
	IOCTRL_ID_SWITCH_SOC_MFG_MODE = 0xF11F,
	IOCTRL_ID_SWITCH_MEDIA_SOURCE = 0x0A02,
	IOCTRL_ID_SET_SOUND_EFFECT = 0x0A06,
	IOCTRL_ID_PLAY_SONG = 0x0A08,
	IOCTRL_ID_SET_LOOP_MODE = 0x0A0A,
	IOCTRL_ID_OPEN_BT = 0x0401,
	IOCTRL_ID_CLOSE_BT = 0x0402,
	IOCTRL_ID_CLOSE_BT_PHONE = 0x0407,
	IOCTRL_ID_SWITCH_TO_BT_MUSIC = 0x0408,
	IOCTRL_ID_SWITCH_TO_BT = 0x0409,
	IOCTRL_ID_BT_MUSIC_CONTROL = 0x040A,
	IOCTRL_ID_BT_CALLIN_ACCEPT = 0x040B,
	IOCTRL_ID_SET_BT_AUDIO_VOLUME = 0x040D,
	IOCTRL_ID_CONNECT_BT = 0x040E,
	IOCTRL_ID_OPEN_WIFI = 0x0301,
	IOCTRL_ID_CLOSE_WIFI = 0x0302,
	IOCTRL_ID_CONNECT_WIFI = 0x0305,
	IOCTRL_ID_PING_WIFI_ADDR = 0x0306,
	IOCTRL_ID_OPEN_WIFI_HOTSPOT = 0x0309,
	IOCTRL_ID_CLOSE_WIFI_HOTSPOT = 0x030A,
	IOCTRL_ID_SET_WIFI_HOTSPOT = 0x030B,
	IOCTRL_ID_OPEN_GPS = 0x0601,
	IOCTRL_ID_CLOSE_GPS = 0x0602,
	IOCTRL_ID_SET_GPS_VOLUME = 0x0604,
	IOCTRL_ID_ENTER_ROUTE_PAGE = 0x0606,
	IOCTRL_ID_SET_DESTNATION = 0x0607,
	IOCTRL_ID_SIMU_NAVI = 0x0608,
	IOCTRL_ID_ENTER_NAVI_PAGE = 0x0609,
	IOCTRL_ID_SET_NAVI_VOLUME = 0x060A,
	IOCTRL_ID_ACTIVATE_NAVI = 0x060C,
	IOCTRL_ID_GPS_DETAIL = 0x060D,
	IOCTRL_ID_SET_SYS_VOLUME = 0x0702,
	IOCTRL_ID_SET_SYS_MUTE = 0x0703,
	IOCTRL_ID_SET_UUID = 0x0706,
	IOCTRL_ID_CLEAR_UUID = 0x0707,
	IOCTRL_ID_FAC_RESET = 0x070C,
	IOCTRL_ID_USB_VIDEO_PAGE = 0x070D,
	IOCTRL_ID_TAIL_DOOR_PAGE = 0x070E,
	IOCTRL_ID_SET_FAN_SPEED = 0x070F,
	IOCTRL_ID_ENTER_AC_PAGE = 0x0710,
	IOCTRL_ID_AVM_PAGE = 0x0901,
	IOCTRL_ID_PICTURE_PAGE = 0x0902,
	IOCTRL_ID_CHANGE_CAM = 0x0903,
	IOCTRL_ID_SWITCH_DVR = 0x0904,
	IOCTRL_ID_SWITCH_MAIN_SEAT_PAGE = 0x0905,
	IOCTRL_ID_SWITCH_CO_SEAT_PAGE = 0x0906,
	IOCTRL_ID_SWITCH_CAR_FUNC_PAGE = 0x0907,
	IOCTRL_ID_SWITCH_LIGHT_SET_PAGE = 0x0908,
	IOCTRL_ID_SWITCH_AUTO_GO_PAGE = 0x0909,
	IOCTRL_ID_CALIBRATE_AMP = 0x0801,
}tIOCtrlID;


extern const tIOCtrlBuffer* sid0x2fDid0X0101DummyStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len);
extern const tIOCtrlBuffer* sid0x2fDid0X0101DummyStopTest(const tUdsReqInfor *reqInfor,tIOControlParameterType req);
extern boolean sid0x2fDid0X0101DummyVerifyPara(const uint8*data, uint16 len);
extern tUdsNRCType sid0x2fDid0X0101DummyCheckExtraDataOrState(const uint8*data, uint16 len);

extern const tIOCtrlBuffer* sid0x2fDidAutoTestStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len);

extern boolean sid0x2fDid0XF11FVerifyPara(const uint8*data, uint16 len);
extern const tIOCtrlBuffer* sid0x2fDid0XF11FStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len);
extern const tIOCtrlBuffer* sid0x2fDid0XF11FStopTest(const tUdsReqInfor *reqInfor,tIOControlParameterType req);

#endif
