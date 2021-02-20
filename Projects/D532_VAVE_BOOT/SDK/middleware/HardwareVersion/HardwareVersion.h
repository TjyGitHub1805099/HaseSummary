#ifndef   HARDWARE_VERSION_H
#define   HARDWARE_VERSION_H

#include "Std_Types.h"

typedef enum
{
	VERSION_0 = 0,
	VERSION_1,
	VERSION_2,
	VERSION_3,
	VERSION_4,
	VERSION_5,
	VERSION_6,
	VERSION_7,
	MAX_CIRCUIT_VERSION
}CircuitVersion;

typedef enum
{
	N1210 = 0,    //internal Amp
	N1210A,       //external Amp 
	MAX_VAR_CTRL_VERSION
}VariantCtrlVersion;

#define VERSION_STEP_LEVEL       20 //25 //100

//CircuitVersion AD Val
#define VERSION_0_LEVEL  	       0       //0Ω
#define VERSION_1_LEVEL  	       (930-15)       //1KΩ
#define VERSION_2_LEVEL  	       (769-15)       //3.3KΩ
#define VERSION_3_LEVEL  	       (695-15)       //4.7KΩ
#define VERSION_4_LEVEL  	       (608-15)      //6.8KΩ
#define VERSION_5_LEVEL  	       (561-15)       //8.2KΩ
#define VERSION_6_LEVEL  	       (512-15)       //10KΩ
#define VERSION_7_LEVEL  	       (465-15)       //12KΩ

//VariantCtrl AD Val
#define VARIANTCTRL_STEP_LEVEL       100 //25 //100

#define N1210_LEVEL  	           0         //0Ω
#define N1210A_LEVEL  	           769      //3.3KΩ


extern uint8 getCircuitVerion(void);
extern uint8 getVariantCtrlVerion(void);


#endif
