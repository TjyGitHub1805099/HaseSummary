/*
 * dsp_spi.h
 *
 *  Created on: 2018Äê9ÔÂ17ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_SPI_DSP_SPI_H_
#define TASK_AUDIO_SPI_DSP_SPI_H_
#include"lpspi_shared_function.h"

typedef struct{
	uint32_t instance_id;
	lpspi_state_t lpspiState;
} dsp_instance;

#endif /* TASK_AUDIO_SPI_DSP_SPI_H_ */
