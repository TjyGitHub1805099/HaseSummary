#ifndef _PRM_H_
#define _PRM_H_

#include "Std_Types.h"


/** define bool 1 */
#define VEL_True    1

/** define bool 0 */
#define VEL_False   0


/**ƽ�������ۼ�ֵ�����ֵ**/
#define AvPpackSumMax	4200000000







//�����ģ��������н���
typedef enum 
{
   PRM_Initial = 0,	/**< Initial State */
   PRM_Normal,		/**< Normal State */
   PRM_Exception,	/**< Exception State */		
} PRM_Mode;




//���幦�ʱ��������Ϣ
typedef struct  
{
	signed short     s16PpackValue;      	/**< Regen signal value */ 
	unsigned char  	 u8PrmValid; 		    /**< Powermeter signal validity : True valid , False not valid */ 
	unsigned char  	 u8PrmAlive;			/**< Powermeter signal alive : True alive , False not alive */	
	unsigned char    u8PrmTimeoutFlag;	    /**< Powermeter signal timeout : False not timeout , True timeout */
}Prm_InputInfor;





void Prm_Init();

Prm_InputInfor PrmInputInfor(void);

void PRM_vExceptionProcess(void);

void PRM_Main(void);










#endif
