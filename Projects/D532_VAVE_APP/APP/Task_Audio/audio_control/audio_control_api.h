/*
 * audio_control_api.h
 *
 *  Created on: 2018Äê12ÔÂ2ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_API_H_
#define TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_API_H_


/* Beep id */
#define BEEP_FCW 0
#define BEEP_FEB 1
#define BEEP_LKA_STANDBY 2
#define BEEP_LKA_CONTROLLING 3
#define BEEP_LKA_HAND_LEAVE 4
#define BEEP_LKA_ASK_CONTROL 5
#define BEEP_LDW 6
#define BEEP_UPA_500MS 7
#define BEEP_UPA_3000MS 8
#define BEEP_METER_EAPM 9
#define BEEP_FAP_1 10
#define BEEP_FAP_2 11
#define BEEP_METER_MOD 12
#define BEEP_METER_Door_Open_Buzzer 13
#define BEEP_METER_KEY_LEFT 14
#define BEEP_METER_OFF_POSITION 15
#define BEEP_METER_E_PKB_LOWER_VOICE 16
#define BEEP_METER_SEAT_BEALT_WARNING 17
#define BEEP_METER_USER_MODE 18
#define BEEP_METER_ILLUMINATION 19
#define BEEP_METER_AT_GEAR 20

#define BEEP_ACC BEEP_FCW
#define BEEP_METER_BackDoor_Open_Buzzer BEEP_METER_Door_Open_Buzzer
#define BEEP_METER_KEY_UNPLUG BEEP_METER_KEY_LEFT
#define BEEP_METER_E_PKB BEEP_METER_OFF_POSITION
#define BEEP_METER_NOT_P BEEP_METER_Door_Open_Buzzer
#define BEEP_METER_IDLE_STOP BEEP_METER_KEY_LEFT
#define BEEP_METER_PKB_ALARM BEEP_METER_OFF_POSITION
#define BEEP_METER_MULTIDISPLAY BEEP_METER_ILLUMINATION


#define BEEP_SONAR_LV1 101
#define BEEP_SONAR_LV2 102
#define BEEP_SONAR_LV3 103
#define BEEP_SONAR_LV4 104
#define BEEP_METER_LIGHT_LEFTON 105

#define BEEP_METER_TIRE_AIR_PRESSURE_ALARM BEEP_METER_LIGHT_LEFTON


/* click clack */
/* TODO: to be the same with audio dsp lib */
#define AUDIO_CLICK_ACTION 0
#define AUDIO_CLACK_ACTION 1

int api_audio_beep_on_by_id(int id);
int api_audio_beep_off_by_id(int id);

int api_audio_clickclack(int action);

int api_audio_mute_beep(int mute);

int api_audio_enter_ecall();
int api_audio_leave_ecall();

#endif /* TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_API_H_ */
