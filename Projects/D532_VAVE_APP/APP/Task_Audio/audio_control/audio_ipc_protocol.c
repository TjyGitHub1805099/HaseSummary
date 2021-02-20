/*
 * audio_ipc_protocol.c
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: gaojing
 */
#include <stdint.h>
#include "audio_ipc_protocol.h"

/* cmd takes 4 bytes */
void audio_ipc_setCmdId(uint32_t cmd, uint8_t *address)
{
	*address = cmd & 0xFF;
	address++;
	*address = (cmd >> 8) & 0xFF;
	address++;
	*address = (cmd >> 16) & 0xFF;
	address++;
	*address = (cmd >> 24) & 0xFF;
}

uint32_t audio_ipc_getCmdId(uint8_t *address)
{
	uint32_t cmd;
	cmd = *address;
	address++;
	cmd += (*address << 8);
	address++;
	cmd += (*address << 16);
	address++;
	cmd += (*address << 24);
	return cmd;
}
