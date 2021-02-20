#ifndef _OAT_H_
#define _OAT_H_
#include "Std_Types.h"

#define OAT_TIME1		(20U)		/*! 2S */
#define OAT_TIME2		(20U)		/*! 2S */
#define OAT_TIME_E		(20U)		/*! sensor value out of range  error judge time interval 20S */

#define OAT_SENSOR_MAX	(28616)		/*! 28k ohm */
#define OAT_SENSOR_MIN	(661)		/*! 661 ohm */

/** define bool 1 */
#define OAT_True    1

/** define bool 0 */
#define OAT_False   0

#define SettingTimeForTs	(2*60*60)		//unit:S


#define MIN_OAT_TEMP	1
#define MAX_OAT_TEMP	1270


typedef enum 
{
   OAT_Initial = 0,	/**< Initial State */
   OAT_Normal,		/**< Normal State */
   OAT_Exception,	/**< Exception State */		
} OAT_Mode;


/**591 project require display state**/
typedef enum
{
	DIS_Fixstate1 = 1,
	DIS_Fixstate2,
	DIS_state1,
	DIS_state2,	
	DIS_state3,
	DIS_state4,
	DIS_state5,
	DIS_state6,

}DISPLAY_STATE;


/**OAT information**/
typedef struct  
{
	  signed short  s16OatValue;      	/**< OAT signal value  温度有正负符号之分*/ 
	unsigned char   u8OatValid; 		/**< OAT signal validity : True valid , False not valid */ 
	unsigned char   u8OatAlive;			/**< OAT signal alive : True alive , False not alive */
	unsigned char   u8OatTimeoutFlag;	/**< OAT signal timeout : False not timeout , True timeout */
}OAT_InputInfor;




void Oat_Init();

OAT_InputInfor Get_OatInputIUnfor();

void OAT_DisplayProc(int16_t new_oat_value);

void OAT_Main(void);

#endif
