/*
 * audio_ipc_protocol.h
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_AUDIO_CONTROL_AUDIO_IPC_PROTOCOL_H_
#define TASK_AUDIO_AUDIO_CONTROL_AUDIO_IPC_PROTOCOL_H_
#include <stdint.h>
#include "AudioSpiIpcProtocol.h"

extern void audio_ipc_setCmdId(uint32_t cmd, uint8_t *address);
extern uint32_t audio_ipc_getCmdId(uint8_t *address);

#endif /* TASK_AUDIO_AUDIO_CONTROL_AUDIO_IPC_PROTOCOL_H_ */
