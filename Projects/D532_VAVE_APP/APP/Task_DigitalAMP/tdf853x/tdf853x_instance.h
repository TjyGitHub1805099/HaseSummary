/*
 * tdf853x_instance.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: gaojing
 */

#ifndef TASK_AUDIO_TDF853X_TDF853X_INSTANCE_H_
#define TASK_AUDIO_TDF853X_TDF853X_INSTANCE_H_

struct tdf853x_instance;

typedef int (*i2c_write_func)(struct tdf853x_instance* instance, uint8_t* buffer, uint8_t length);
typedef int (*i2c_read_func)(struct tdf853x_instance* instance, uint8_t* buffer, uint8_t length);

typedef struct tdf853x_instance{
	uint32_t instance;
	uint8_t cfgNum;
	i2c_read_func i2c_read;
	i2c_write_func i2c_write;
}tdf853x_instance;

#endif /* TASK_AUDIO_TDF853X_TDF853X_INSTANCE_H_ */
