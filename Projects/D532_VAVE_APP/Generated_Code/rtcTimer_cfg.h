/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : rtcTimer_cfg.h
**     Project     : D532_VAVE_APP
**     Processor   : S32K148_176
**     Component   : rtc
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-11-26, 14:42, # CodeGen: 106
**     Contents    :
**         RTC_DRV_Init                      - status_t RTC_DRV_Init(uint32_t instance,const rtc_init_config_t *const...
**         RTC_DRV_Deinit                    - status_t RTC_DRV_Deinit(uint32_t instance);
**         RTC_DRV_GetDefaultConfig          - void RTC_DRV_GetDefaultConfig(rtc_init_config_t * const config);
**         RTC_DRV_StartCounter              - status_t RTC_DRV_StartCounter(uint32_t instance);
**         RTC_DRV_StopCounter               - status_t RTC_DRV_StopCounter(uint32_t instance);
**         RTC_DRV_GetCurrentTimeDate        - status_t RTC_DRV_GetCurrentTimeDate(uint32_t instance,rtc_timedate_t *const...
**         RTC_DRV_SetTimeDate               - status_t RTC_DRV_SetTimeDate(uint32_t instance,const rtc_timedate_t *const...
**         RTC_DRV_ConfigureRegisterLock     - status_t RTC_DRV_ConfigureRegisterLock(uint32_t instance,const...
**         RTC_DRV_GetRegisterLock           - void RTC_DRV_GetRegisterLock(uint32_t instance,rtc_register_lock_config_t...
**         RTC_DRV_ConfigureTimeCompensation - status_t RTC_DRV_ConfigureTimeCompensation(uint32_t instance,uint8_t...
**         RTC_DRV_GetTimeCompensation       - void RTC_DRV_GetTimeCompensation(uint32_t instance,uint8_t *...
**         RTC_DRV_ConfigureFaultInt         - void RTC_DRV_ConfigureFaultInt(uint32_t instance,rtc_interrupt_config_t...
**         RTC_DRV_ConfigureSecondsInt       - void RTC_DRV_ConfigureSecondsInt(uint32_t instance,rtc_seconds_int_config_t...
**         RTC_DRV_ConfigureAlarm            - status_t RTC_DRV_ConfigureAlarm(uint32_t instance,rtc_alarm_config_t *const...
**         RTC_DRV_GetAlarmConfig            - void RTC_DRV_GetAlarmConfig(uint32_t instance,rtc_alarm_config_t * alarmConfig);
**         RTC_DRV_IsAlarmPending            - bool RTC_DRV_IsAlarmPending(uint32_t instance);
**         RTC_DRV_ConvertSecondsToTimeDate  - void RTC_DRV_ConvertSecondsToTimeDate(const uint32_t * seconds,rtc_timedate_t...
**         RTC_DRV_ConvertTimeDateToSeconds  - void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t *const...
**         RTC_DRV_IsYearLeap                - bool RTC_DRV_IsYearLeap(uint16_t year);
**         RTC_DRV_IsTimeDateCorrectFormat   - bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t *const timeDate);
**         RTC_DRV_GetNextAlarmTime          - status_t RTC_DRV_GetNextAlarmTime(uint32_t instance,rtc_timedate_t *const...
**         RTC_DRV_IRQHandler                - void RTC_DRV_IRQHandler(uint32_t instance);
**         RTC_DRV_SecondsIRQHandler         - void RTC_DRV_SecondsIRQHandler(uint32_t instance);
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file rtcTimer_cfg.h
** @version 01.00
*/
/*!
**  @addtogroup rtcTimer_cfg_module rtcTimer_cfg module documentation
**  @{
*/
#ifndef rtcTimer_cfg_H
#define rtcTimer_cfg_H
/*! MODULE rtcTimer_cfg. */
/*! Include inherited beans */
#include "clockManager_cfg.h"
#include "Cpu.h"
#include "rtc_driver.h"

/*! @brief Device instance number */
#define RTCTIMER_CFG 0UL



/*! RTC instance init configuration */
extern       rtc_init_config_t                  rtcTimer_cfg_Config0;
/*! RTC start time and date */
extern       rtc_timedate_t                     rtcTimer_cfg_StartTime0;

/*! RTC Register lock configuration */
extern       rtc_register_lock_config_t         rtcTimer_cfg_LockConfig0;

/*! RTC Fault Interrupt configuration (Time Overflow Interrupt & Time Invalid Interrupt) */
extern       rtc_interrupt_config_t             rtcTimer_cfg_FaultIntConfig0;

/*! RTC Time Seconds Interrupt configuration */
extern       rtc_seconds_int_config_t            rtcTimer_cfg_SecIntConfig0;


/*! RTC Alarm configuration 0 */
extern rtc_alarm_config_t   rtcTimer_cfg_AlarmConfig0;

#endif
/*! ifndef rtcTimer_cfg_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
