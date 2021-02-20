#ifndef _ACCELGUIDE_H_
#define _ACCELGUIDE_H_

#define THRESHOLD_A		10
#define THRESHOLD_B		7

#define PARMETER_V5		120		//km/h

#define N	30	//waitting for make sure the finall value

typedef struct
{
	unsigned char Parameter_Y;
	unsigned char AccelValue;
	unsigned char AccelValueValid;

}AccelGuide_Infor;


void AccelGuide_Init(void);
void AccelGuide_VMAIN(void);


#endif