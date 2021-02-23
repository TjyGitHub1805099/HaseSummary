#ifndef EXTERN_RTC_MANAGEMENT_H
#define EXTERN_RTC_MANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif


/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "Task_M_IpcApp_Cfg.h"
#include "MemMap.h"
/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/
#define RTCCT_IPC_HW_CHANNEL		1//SPI1
#define RTCCT_IPC_SW_CHANNEL		IPC_GW_CHANNEL_SYSTEM/* IPC_GW_CHANNEL_SYSTEM = 8 ,include time info, sys info, version info, back light*/
#define RTCCT_IPC_SW_FUNCTION_ID_READ	IPC_SW_SYSTEM_FUCTION_RTCCT_READ
#define RTCCT_IPC_SW_FUNCTION_ID_WRITE	IPC_SW_SYSTEM_FUCTION_RTCCT_WRITE
#define RTCCT_RECV_IPC_SW_HMICMD_LEN	0X07//year month day week hour minute second

/*============================[T Y P E  D E F I N E]============================================*/
/************************************************************************************************/
typedef struct {
/* [RA8900CE Register table */
	uint8 second;	/**< second Register, offset: 0x0 */
	uint8 minutes;	/**< minutes Register, offset: 0x1 */
	uint8 hour;		/**< hour Register, offset: 0x2 */
	
	uint8 week;		/**< week Register, offset: 0x3 */
	
	uint8 day;		/**< day Register, offset: 0x4 */
	uint8 month;	/**< month Register, offset: 0x5 */
	uint8 year;		/**< year Register, offset: 0x6 */

	uint8 ramReserve;	/**< ramReserve Register, offset: 0x7 */

	uint8 min_alram;	/**< min_alram Register, offset: 0x8 */
	uint8 hour_alram;	/**< hour_alram Register, offset: 0x9 */
	uint8 day_alram;	/**< day_alram Register, offset: 0xA */

	uint8 timerCounter0;	/**< timerCounter0 Register, offset: 0xB */
	uint8 timerCounter1;	/**< timerCounter1 Register, offset: 0xC */

	uint8 externRegister;	/**< extern Register, offset: 0xD */
	uint8 flagRegister;		/**< flag Register, offset: 0xE */
	uint8 controlRegister;	/**< control Register, offset: 0xF */

/* define used by sys */
	uint8 initFlag;
	uint8 votageLowFlag;
}ExternRTCType, *ExternRTCPtr;

typedef struct {

	uint8 second;	/**< second Register, offset: 0x0 */
	uint8 minutes;	/**< minutes Register, offset: 0x1 */
	uint8 hour;		/**< hour Register, offset: 0x2 */

	uint8 week;		/**< week Register, offset: 0x3 */

	uint8 day;		/**< day Register, offset: 0x4 */
	uint8 month;	/**< month Register, offset: 0x5 */
	uint8 year;		/**< year Register, offset: 0x6 */

	uint8 ramReserve;	/**< ramReserve Register, offset: 0x7 */

	uint8 min_alram;	/**< min_alram Register, offset: 0x8 */
	uint8 hour_alram;	/**< hour_alram Register, offset: 0x9 */
	uint8 day_alram;	/**< day_alram Register, offset: 0xA */

	uint8 timerCounter0;	/**< timerCounter0 Register, offset: 0xB */
	uint8 timerCounter1;	/**< timerCounter1 Register, offset: 0xC */

	uint8 externRegister;	/**< extern Register, offset: 0xD */
	uint8 flagRegister;		/**< flag Register, offset: 0xE */
	uint8 controlRegister;	/**< control Register, offset: 0xF */

	/* define used buy sys */
	uint8 hmiWriteFlag;
	uint8 hmiReadFlag;

}ExternSTDType, *ExternSTDPtr;

/*============================[RA8900CE Register table]=========================================*/
/*
	Address Function	bit7	bit6	bit5	bit4	bit3	bit2	bit1	bit0	Read	Write
	00 SEC						40		20		10		8		4		2		1		P		P
	01 MIN						40		20		10		8		4		2		1		P		P
	02 HOUR								20		10		8		4		2		1		P		P
	03 WEEK						6		5		4		3		2		1		0		P		P
	04 DAY								20		10		8		4		2		1		P		P
	05 MONTH									10		8		4		2		1		P		P
	06 YEAR				80		40		20		10		8		4		2		1		P		P
	07 RAM																				P		P
	08 MIN Alarm		AE		40		20		10		8		4		2		1		P		P
	09 HOUR Alarm		AE				20		10		8		4		2		1		P		P
	0A WEEK Alarm		AE		6		5		4		3		2		1		0		P		P
	   DAY Alarm						20		10		8		4		2		1		P		P
	0B Timer Counter 0 128		64		32		16		8		4		2		1		P		P
	0C Timer Counter 1									2048	1024	512 	256 	P		P
	0D Extension Register TEST WADA 	USEL	TE		FSEL1	FSEL0	TSEL1	TSEL0	P		P
	0E Flag Register					UF		TF		AF				VLF 	VDET	P		P
	0F Control Register CSEL1	CSEL0	UIE 	TIE 	AIE						RESET	P		P
*/
/* @RA8900CE datasheet page 38*/
#define EXTERN_RTC_DATA_MAX_LEN	0x20

/*@RA8900CE page15 : Alarm registers*/
#define EXTERN_RTC_MIN_ALARM_ADDRESS	0x08
#define EXTERN_RTC_MIN_ALARM_DATA		0x00

#define EXTERN_RTC_HOUR_ALARM_ADDRESS	0x09
#define EXTERN_RTC_HOUR_ALARM_DATA		0x00

#define EXTERN_RTC_DAY_ALARM_ADDRESS	0x0A
#define EXTERN_RTC_DAY_ALARM_DATA		0x00

/*@RA8900CE page15 : Fixed-cycle timer control registers*/
#define EXTERN_RTC_TimerCounter0_ADDRESS		0x0B
#define EXTERN_RTC_TimerCounter0_ALARM_DATA		0x00

#define EXTERN_RTC_TimerCounter1_ADDRESS	0x0C
#define EXTERN_RTC_TimerCounter1_DATA		0x00

/*@RA8900CE page15 : Extension register*/
#define EXTERN_RTC_ExtensionRegister_ADDRESS	0x0D
#define EXTERN_RTC_ExtensionRegister_DATA		0x0A

/*@RA8900CE page15 : Flag registerr*/
#define EXTERN_RTC_FlagRegister_ADDRESS		0x0E
#define EXTERN_RTC_FlagRegister_DATA		0x00

/*@RA8900CE page15 : Control register*/
#define EXTERN_RTC_ControlRegister_ADDRESS	0x0F
#define EXTERN_RTC_ControlRegister_DATA		0x40


/* Timer Enable */
#define EXTERN_RTC_TE_ADDRESS	0x0D
#define EXTERN_RTC_TE_BITPOS	0x04
#define EXTERN_RTC_TE_BITLEN	0x01
/* FOUT frequency Select '10'->1HZ*/
#define EXTERN_RTC_FSEL_ADDRESS	0x0D
#define EXTERN_RTC_FSEL_BITPOS	0x03
#define EXTERN_RTC_FSEL_BITLEN	0x02
/* Timer Select 10'->once per second*/
#define EXTERN_RTC_TEST_ADDRESS	0x0D
#define EXTERN_RTC_TEST_BITPOS	0x01
#define EXTERN_RTC_TEST_BITLEN	0x02
/* Voltage Detection Flag */
#define FLEXIO_SHIFTCFG_SSTART_MASK              0x3u
#define FLEXIO_SHIFTCFG_SSTART_SHIFT             0u
#define FLEXIO_SHIFTCFG_SSTART_WIDTH             2u

#define EXTERN_RTC_VLF_MASK		0x2u

#define EXTERN_RTC_VLF_ADDRESS	0x0E
#define EXTERN_RTC_VLF_BITPOS	0x01
#define EXTERN_RTC_VLF_SHIFT	1u
#define EXTERN_RTC_VLF_WIDTH	0x01


/* Alarm Interrupt Enable flag */
#define EXTERN_RTC_AIE_ADDRESS	0x0F
#define EXTERN_RTC_AIE_BITPOS	0x03
#define EXTERN_RTC_AIE_BITLEN	0x01
/* votage low flag */
#define EXTERN_RTC_TIE_ADDRESS	0x0F
#define EXTERN_RTC_TIE_BITPOS	0x04
#define EXTERN_RTC_TIE_BITLEN	0x01
/* Update Interrupt Enable flag */
#define EXTERN_RTC_UIE_ADDRESS	0x0F
#define EXTERN_RTC_UIE_BITPOS	0x05
#define EXTERN_RTC_UIE_BITLEN	0x01


extern ExternSTDType sStdDefaultData,gSetStdData;

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/
/* extern RTC i2c initial */
Std_ReturnType ExtrRtc_Init();

/* standard time get from extern rtc*/
Std_ReturnType ExtrRtc_StdTimeGet(ExternSTDType *stdData);

/* standard time set to extern rtc*/
Std_ReturnType ExtrRtc_StdTimeSet(ExternSTDType *stdData);

/* external RTC main deal */
void ExtrRtc_MainFunction();

/* Second Panel need current time */
uint8 GetRtcTime(ExternSTDType *stData);

extern NVM_SEC_VAR_UNSPECIFIED uint8 rtcStoredData[];


#ifdef __cplusplus
}
#endif

#endif /* EXTERN_RTC_MANAGEMENT_H */
