/*
 * audio_control.h
 *
 *  Created on: 2018Äê10ÔÂ19ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_H_
#define TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_H_

void audio_control_variable_init();
void initialize_ssv_table(int rate);
void audio_handle_ipc_msg(uint8 *msg, uint32 len);

extern uint8_t audio_debug_flag;
extern uint8_t audio_car_model;

#endif /* TASK_AUDIO_AUDIO_CONTROL_AUDIO_CONTROL_H_ */
