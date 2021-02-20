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

//CircuitVersion AD Val
#define VERSION_STEP_LEVEL      	23//< 0.15/3.3*1024/2
#define VERSION_0_LEVEL  	       	0//=0/3.3*1024
#define VERSION_1_LEVEL  	       	(930-8)//=3/3.3*1024
#define VERSION_2_LEVEL  	       	(769-8)//=2.48/3.3*1024
#define VERSION_3_LEVEL  	       	(695-8)//=2.24/3.3*1024
#define VERSION_4_LEVEL  	       	(608-8)//=1.96/3.3*1024
#define VERSION_5_LEVEL  	       	(561-8)//=1.81/3.3*1024
#define VERSION_6_LEVEL  	       	(512-8)//=1.65/3.3*1024
#define VERSION_7_LEVEL  	       	(465-8)//=1.5/3.3*1024
//0oum(0V) 		1.0koum(3.0V)	3.3koum(2.48V) 	4.7koum(2.24V) 	6.8koum(1.96V)	8.2koum(1.81V)	10koum(1.65V)	12koum(1.5V)
//s00(N532)		s01(N532)		s02(N532)		s03(N532)		s04(N532)		s05(N532)		s06(N532)		s07(N532)


//VariantCtrl AD Val
#define VARIANTCTRL_STEP_LEVEL     	25// < 0.15/3.3*1024/2
#define N1210_LEVEL  	           	(608-8)//=1.96/3.3*1024
#define N1210A_LEVEL  	           	(695-8)//=2.24/3.3*1024
//0oum(0V) 		3.3koum(2.48V) 	4.7koum(2.24V) 	6.8koum(1.96V)
//N1210(N591)	N1210A(N591)	N1210A(N532)	N1210(N532)


extern uint8 getCircuitVerion(void);
extern uint8 getVariantCtrlVerion(void);
extern uint8 getITMasterCtrlVerion(void);
extern uint8 gITMasterCtrlVersion ;

#endif
