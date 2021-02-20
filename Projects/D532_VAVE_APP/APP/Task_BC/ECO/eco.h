#ifndef _ECO_H_
#define _ECO_H_

#include "AccelGuide.h"

#define  PARMETER_V_5   	1200	//120km/h expand 10times

#define  PARMETER_V_3		100	//10km/h	expand 10times

#define  PARMETER_X1	2

#define  PARMETER_T1	600	//second(10min)

#define  Timecnt_Max	64800

typedef enum
{
	Condition_one = 1,
	Condition_two,
	Condition_three,
	Condition_four,
	Invalid_value,

}CONDITION_MODE;


typedef struct
{
	unsigned char ECO_rank;
	unsigned char ECOrankValid;

}ECO_Infor;



void ECO_VMAIN(void);

void ECO_Init(void);


#endif