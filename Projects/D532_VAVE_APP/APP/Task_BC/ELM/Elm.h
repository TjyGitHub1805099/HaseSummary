
#ifndef _ELM_H_
#define _ELM_H_

#include "Std_Types.h"


/** define bool 1 */
#define ELM_True    1

/** define bool 0 */
#define ELM_False   0

/**SOC������ֵ**/
#define BMS_SocMax    100  /*���SOC 100%*/





//�����ģ��������н���
typedef enum 
{
   ELM_Initial = 0,	/**< Initial State */
   ELM_Normal,		/**< Normal State */
   ELM_Exception,	/**< Exception State */		
} ELM_Mode;


//�������ص�����������Ϣ
typedef struct  
{
	unsigned char  u8SocValue;      	/**< soc signal value */ 
	unsigned char  u8SocValid; 		    /**< soc signal validity : True valid , False not valid */ 
	unsigned char  u8SocAlive;			/**< soc signal alive : True alive , False not alive */
	unsigned char  u8SocTimeoutFlag;	/**< soc signal timeout : False not timeout , True timeout */
}BmsSoc_InputInfor;


void Soc_Init();

BmsSoc_InputInfor Bms_SocInputInfor(void);

uint8_t SocMaxJudge(uint8_t soc_value);

void SOC_vExceptionProcess(void);

void Elm_SocMain(void);

#endif
